/*
 * ServoController.cpp
 *
 * Created: 2016-02-21 오전 11:59:14
 *  Author: Blonix
 */ 

#include "../define.h"


CServoController::CServoController()
{
	ServoTCNT = 0x63C0;
	NumServo = 0;
	bRun = false;
	pwmWaitMode = SERVOPWM_WAIT;
	
	for(uint8_t i=0; i<MAX_SERVOMOTOR; i++){
		pwmDutyRatio[i] = 90;
	}
	pwmCounter = 0xFF;
}

uint8_t CServoController::AddServoMotor(SServo NewServo, uint8_t angle)
{
	if(NumServo == MAX_SERVOMOTOR){
		FlowManager.ErrorOutput(ERRORCODE_0006);
		return 255;
	}
	strcpy(ServoMotor[NumServo].Name, NewServo.Name);
	ServoMotor[NumServo].PWMPin = NewServo.PWMPin;
	pwmDutyRatio[NumServo] = angle;
	
	SERVO_MASK		BITSET(NewServo.PWMPin);
	SERVO_DDR		BITSET(NewServo.PWMPin);
	
	NumServo++;
	
	return NumServo-1;
}

uint8_t CServoController::FindServoMotor(char Name[])
{
	for(uint8_t i = 0; i<MAX_SERVOMOTOR; i++){
		if(strcmp(ServoMotor[i].Name, Name) == 0){
			return (unsigned char)i;
		}
	}
	return 0;		// exception ?
}

void CServoController::RunServoMotor(uint8_t ServoIndex, uint8_t angle)
{
	pwmDutyRatio[ServoIndex] = angle;
	bRun = true;
}

void CServoController::StopAllServoMotor()
{
	ServoTCNT = 0x63C0;
	pwmWaitMode = SERVOPWM_WAIT;
	bRun = false;
}

void CServoController::PWM_ServoMotor()
{
	static volatile uint8_t pwmPinlevelServo = 0x00;
	static volatile uint8_t pwmFastcountServo = 0x00;
	
	if(bRun)
	{
		if(pwmWaitMode == SERVOPWM_WAIT){
			SERVO_PORT = SERVO_MASK;			// all pins high
			ServoTCNT = 0xFB50;					// 600us
			pwmWaitMode = SERVOPWM_HIGH;
			return;
		}
		if(pwmWaitMode == SERVOPWM_HIGH){
			ServoTCNT = 0xFFEC;					// 10us
			pwmWaitMode = SERVOPWM_PULSE;
		}
		
		if(++pwmCounter == 0){
			pwmPinlevelServo = SERVO_MASK;												// all pins high
		}
		
		SERVO_PORT = pwmPinlevelServo;
		
		for(pwmFastcountServo = 0; pwmFastcountServo < NumServo; pwmFastcountServo++){
			if(pwmDutyRatio[pwmFastcountServo] == pwmCounter){
				pwmPinlevelServo	BITCLR(ServoMotor[pwmFastcountServo].PWMPin);		// clear specific pin
			}
		}
		
		if(pwmCounter == 179){
			ServoTCNT = 0x63C0;					// 20ms
			pwmWaitMode = SERVOPWM_WAIT;
			pwmCounter = 0xFF;
		}
	}
}
