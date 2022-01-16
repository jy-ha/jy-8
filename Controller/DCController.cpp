/*
 * DCController.cpp
 *
 * Created: 2016-01-13 오전 11:14:48
 *  Author: Blonix
 */ 

#include "../define.h"


CDCController::CDCController()
{
	NumDC = 0x00;
	NumRunDC = 0x00;
	
	for(uint8_t i=0; i<MAX_DCMOTOR; i++){
		pwmDutyRatio[i] = 0x00;
	}
	pwmCounter = 0xFF;
}


unsigned char CDCController::AddDCMotor(SDCMotor NewMotor)
{
	if(NumDC == MAX_DCMOTOR){
		FlowManager.ErrorOutput(ERRORCODE_0002);
		return 255;
	}
	strcpy(DCMotor[NumDC].Name, NewMotor.Name);
	DCMotor[NumDC].InPinPort = NewMotor.InPinPort;
	DCMotor[NumDC].EnablePin = NewMotor.EnablePin;
	DCMotor[NumDC].In1Pin = NewMotor.In1Pin;
	DCMotor[NumDC].In2Pin = NewMotor.In2Pin;
	DCMotor[NumDC].bRun = false;
	
	DC_MASK		|=	(1 << NewMotor.EnablePin);
	DC_DDR		|=	(1 << NewMotor.EnablePin);
	
	switch(NewMotor.InPinPort)
	{
	case 'A':	DDRA |=	(1<<NewMotor.In1Pin) | (1<<NewMotor.In2Pin);	break;
	case 'B':	DDRB |=	(1<<NewMotor.In1Pin) | (1<<NewMotor.In2Pin);	break;
	case 'C':	DDRC |=	(1<<NewMotor.In1Pin) | (1<<NewMotor.In2Pin);	break;
	case 'D':	DDRD |= (1<<NewMotor.In1Pin) | (1<<NewMotor.In2Pin);	break;
	case 'E':	DDRE |= (1<<NewMotor.In1Pin) | (1<<NewMotor.In2Pin);	break;
	case 'F':	DDRF |=	(1<<NewMotor.In1Pin) | (1<<NewMotor.In2Pin);	break;
	default:	FlowManager.ErrorOutput(ERRORCODE_0003);	break;
	}
	NumDC++;
	
	return NumDC-1;
}

unsigned char CDCController::FindDCMotor(char Name[])
{
	for(uint8_t i = 0; i<MAX_DCMOTOR; i++){
		if(strcmp(DCMotor[i].Name, Name) == 0){
			return (unsigned char)i;
		}
	}
	return 0;		// exception ?
}

void CDCController::RunDCMotor(uint8_t MotorIndex, int8_t SpeedRatio)
{	
	if(SpeedRatio > 0){								// Normal turn
		pwmDutyRatio[MotorIndex] = SpeedRatio;
		
		switch(DCMotor[MotorIndex].InPinPort)
		{
		case 'A':	PORTA |= (1<<DCMotor[MotorIndex].In1Pin);	PORTA &= ~(1<<DCMotor[MotorIndex].In2Pin);	break;
		case 'B':	PORTB |= (1<<DCMotor[MotorIndex].In1Pin);	PORTB &= ~(1<<DCMotor[MotorIndex].In2Pin);	break;
		case 'C':	PORTC |= (1<<DCMotor[MotorIndex].In1Pin);	PORTC &= ~(1<<DCMotor[MotorIndex].In2Pin);	break;
		case 'D':	PORTD |= (1<<DCMotor[MotorIndex].In1Pin);	PORTD &= ~(1<<DCMotor[MotorIndex].In2Pin);	break;
		case 'E':	PORTE |= (1<<DCMotor[MotorIndex].In1Pin);	PORTE &= ~(1<<DCMotor[MotorIndex].In2Pin);	break;
		case 'F':	PORTF |= (1<<DCMotor[MotorIndex].In1Pin);	PORTF &= ~(1<<DCMotor[MotorIndex].In2Pin);	break;
		default:	FlowManager.ErrorOutput(ERRORCODE_0003);	break;
		}
	}
	else{											// Reverse turn
		pwmDutyRatio[MotorIndex] = -SpeedRatio;
		
		switch(DCMotor[MotorIndex].InPinPort)
		{
		case 'A':	PORTA |= (1<<DCMotor[MotorIndex].In2Pin);	PORTA &= ~(1<<DCMotor[MotorIndex].In1Pin);	break;
		case 'B':	PORTB |= (1<<DCMotor[MotorIndex].In2Pin);	PORTB &= ~(1<<DCMotor[MotorIndex].In1Pin);	break;
		case 'C':	PORTC |= (1<<DCMotor[MotorIndex].In2Pin);	PORTC &= ~(1<<DCMotor[MotorIndex].In1Pin);	break;
		case 'D':	PORTD |= (1<<DCMotor[MotorIndex].In2Pin);	PORTD &= ~(1<<DCMotor[MotorIndex].In1Pin);	break;
		case 'E':	PORTE |= (1<<DCMotor[MotorIndex].In2Pin);	PORTE &= ~(1<<DCMotor[MotorIndex].In1Pin);	break;
		case 'F':	PORTF |= (1<<DCMotor[MotorIndex].In2Pin);	PORTF &= ~(1<<DCMotor[MotorIndex].In1Pin);	break;
		default:	FlowManager.ErrorOutput(ERRORCODE_0003);	break;
		}
	}
	
	if(DCMotor[MotorIndex].bRun == false){
		DCMotor[MotorIndex].bRun = true;
		NumRunDC++;
	}
}

void CDCController::StopDCMotor(uint8_t MotorIndex)
{
	pwmDutyRatio[MotorIndex] = 0x00;
	
	if(DCMotor[MotorIndex].bRun == true){
		DCMotor[MotorIndex].bRun = false;
		NumRunDC--;
		DC_PORT &= ~(1 << DCMotor[MotorIndex].EnablePin); 
	}
}

void CDCController::ToggleDCMotor(uint8_t MotorIndex, int8_t SpeedRatio)
{
	if(DCMotor[MotorIndex].bRun){
		StopDCMotor(MotorIndex);
	}
	else{
		RunDCMotor(MotorIndex, SpeedRatio);
	}
}

void CDCController::PWM_DCMotor()			// (ISR frequency * 100)
{
	static volatile uint8_t pwmPinlevelDC = 0x00;
	static volatile uint8_t pwmFastcountDC = 0x00;
	
	if(NumRunDC)	// if not 0
	{
		DC_PORT = pwmPinlevelDC;
		
		if(++pwmCounter == 0){
			pwmPinlevelDC = DC_MASK;												// all pins high
		}
	
		for(pwmFastcountDC = 0; pwmFastcountDC < NumDC; pwmFastcountDC++){
			if(pwmDutyRatio[pwmFastcountDC] == pwmCounter){
				pwmPinlevelDC &= ~(1 << DCMotor[pwmFastcountDC].EnablePin);			// clear specific pin
			}
		}
	
		if(pwmCounter == 99)	pwmCounter = 0xFF;
	}
}
