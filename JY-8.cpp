/*
 * JY_8.cpp
 *
 * Created: 2016-01-11 오후 5:01:51
 *  Author: Blonix
 */ 


#include "define.h"
#include "CarMovement.h"
#include "ArmMovement.h"
#include "EtcMovement.h"

SGolbalEvent		GlobalEventQueue[MAX_EVENTQUEUE];
uint8_t				GlobalEventNum = 0;
uint8_t				pwmMaskPortb = 0x00;
uint8_t				pwmMaskPortc = 0x00;
CFlowManager		FlowManager;
CDCController		DCController;
CSoundController	SoundController;
CServoController	ServoController;
CPerception			Perception;
void InitClasses(CCarMovement*, CArmMovement*, CEtcMovement*);

int main(void)
{
	//==================================  TIMER SETTING  ==================================//
	TCCR0	|= (1<<CS01);						//prescaler 8,	pwm, UltraSonic counter
	TCNT0	= 0xD9;
	TIMSK	|= (1<<TOIE0);
	
    TCCR1B	|= (1<<CS01);						//prescaler 8,	Servo Controller
	TCNT1	= 0x63C0;
    TIMSK	|= (1<<TOIE1);
	
	TCCR2	|= (1<<CS01) | (1<<CS00);			//prescaler 64,	delay, millis, Sound Controller
	TCNT2	= 0x06;
	TIMSK	|= (1<<TOIE2);
	
	TCCR3B	|= (1<<CS01);						//prescaler 8,	Sound Controller
	TCNT3	= 0x0000;
	ETIMSK	|= (1<<TOIE3);
	
	//==================================  COMMON SETTING ==================================//
	for(uint8_t i=0; i<MAX_EVENTQUEUE; i++)		GlobalEventQueue[i].nID = ENOTHING;
	
	uart_init(UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU));
    sei();
	
	i2c_init();
	_delay_ms(50);
	
	SoundController.Init();
	Perception.Init();
	
	//==================================  CUSTOM SETTING ==================================//
	CCarMovement	CarMovement;
	CArmMovement	ArmMovement;
	CEtcMovement	EtcMovement;
	InitClasses(&CarMovement, &ArmMovement, &EtcMovement);
	
	_delay_ms(50);
	uart_puts("JY-8 Autonomic Nerve Initialized\r\n");
	//FlowManager.Delay(3000, ETEST);
	//FlowManager.Delay(500, ETEST2);
	
	//====================================== MAIN LOOP ====================================//
	
	uint32_t	LastMilliTick = 0;
	uint32_t	LoopDeltaTime;
	//Vector		TempTest = {10, 0, 10};						// Test
	
    while(1){
		LoopDeltaTime = FlowManager.MilliTick - LastMilliTick;
		LastMilliTick = FlowManager.MilliTick;
		
		//FlowManager.ExecUartEvent();
		
		CarMovement.CheckEvent();
		ArmMovement.CheckEvent();
		EtcMovement.CheckEvent();
		SoundController.CheckEvent();
		
		FlowManager.ExecDelayEvent();
		Perception.Update(LoopDeltaTime);
		
		//ArmMovement.SetTransition(TempTest, TempTest);		// Test
		//TempTest.y++;										// Test
		
		_delay_ms(10);				// Nock sensor?
	}
    return 0;
}

void InitClasses(CCarMovement* Car, CArmMovement* Arm, CEtcMovement* Etc)
{
	SDCMotor		WheelL = {"WheelL", 'A', PB0, PA0, PA1, false};
	SDCMotor		WheelR = {"WheelR", 'A', PB1, PA3, PA2, false};
	Car->Init(WheelL, WheelR);
	
	SServo			ArmRot = {"ArmRot", PC0};
	SServo			ArmLow = {"ArmLow", PC1};
	SServo			ArmHigh = {"ArmHigh", PC2};
	SServo			ArmHand = {"ArmHand", PC4};
	Arm->Init(ArmRot, ArmLow, ArmHigh, ArmHand);
	
	SServo			Head = {"Head", PC3};
	Etc->Init(Head);
}


//====================================== INTERRUPT ====================================//

ISR(TIMER0_OVF_vect) // Timer0 : (513 * 100)Hz		19.5us
{
	TCNT0 = 0xD9;
	
	DCController.PWM_DCMotor();
	Perception.UltraSonic();
}

ISR(TIMER1_OVF_vect) // Timer1 : 20ms or 600us or 10us
{
	ServoController.PWM_ServoMotor();

	TCNT1 = ServoController.ServoTCNT;
}

ISR(TIMER2_OVF_vect) // Timer2 : 1ms
{
	static volatile uint8_t fastcount;
	
	TCNT2 = 0x06;
	for(fastcount = 0; fastcount<FlowManager.DelayEventCounter; fastcount++){
		FlowManager.timer1_ms[fastcount]++;
	}
	FlowManager.MilliTick++;		// reset to 0 after 49.7 days
	SoundController.TimeCheck();
}

ISR(TIMER3_OVF_vect) // Timer3 : (KEY)*2 Hz
{	
	TCNT3 = SoundController.SNDTCNT;
	
	SoundController.MakeFrequency();
}


ISR(INT0_vect) { Perception.IntON(0); }
ISR(INT1_vect) { Perception.IntON(1); }
ISR(INT2_vect) { Perception.IntON(2); }
ISR(INT3_vect) { Perception.IntON(3); }
ISR(INT4_vect) { Perception.IntON(4); }
ISR(INT5_vect) { Perception.IntON(5); }
ISR(INT6_vect) { Perception.IntON(6); }
ISR(INT7_vect) { Perception.IntON(7); }

