/*
 * DCController.cpp
 *
 * Created: 2016-01-13 오전 11:14:48
 *  Author: Blonix
 */ 
#define MAX_DCMOTOR			10


#include <string.h>
#include "DCController.h"


CDCController::CDCController()
{
	NumDC = 0x00;
	NumRunDC = 0x00;
	
	for(int i=0; i<MAX_DCMOTOR; i++){
		pwmDutyCompare[i] = 0x00;
		pwmDutyRatio[i] = 0x00;
	}
	pwmCounter = 0xFF;
}
CDCController::~CDCController()
{
}


void CDCController::AddDCMotor(char Name[], unsigned char InPinPort, unsigned char EnablePin, unsigned char In1Pin, unsigned char In2Pin)
{
	if(NumDC == MAX_DCMOTOR){
		return;
	}
	
	strcpy(DCMotor[NumDC].Name, Name);
	DCMotor[NumDC].InPinPort = InPinPort;
	DCMotor[NumDC].EnablePin = EnablePin;
	DCMotor[NumDC].In1Pin = In1Pin;
	DCMotor[NumDC].In2Pin = In2Pin;
	DCMotor[NumDC].bRun = false;
	
	pwmMaskPortb	|=	(1 << EnablePin);
	DDRB			|=	(1 << EnablePin);
	
	switch(DCMotor[NumDC].InPinPort)
	{
	case 'A':	DDRA |=	(1<<In1Pin) | (1<<In2Pin);	break;
	case 'B':	DDRB |=	(1<<In1Pin) | (1<<In2Pin);	break;
	case 'C':	DDRC |=	(1<<In1Pin) | (1<<In2Pin);	break;
	case 'D':	DDRD |= (1<<In1Pin) | (1<<In2Pin);	break;
	case 'E':	DDRE |= (1<<In1Pin) | (1<<In2Pin);	break;
	case 'F':	DDRF |=	(1<<In1Pin) | (1<<In2Pin);	break;
	default:	break;
	}
	
	NumDC++;
}

unsigned char CDCController::FindDCMotor(char Name[])
{
	for(int i = 0; i<MAX_DCMOTOR; i++){
		if(strcmp(DCMotor[i].Name, Name) == 0){
			return (unsigned char)i;
		}
	}
	return 0;		// exception ?
}

void CDCController::RunDCMotor(unsigned char MotorIndex, int SpeedRatio)
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
		default:	break;
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
		default:	break;
		}
	}
	
	if(DCMotor[MotorIndex].bRun == false){
		DCMotor[MotorIndex].bRun = true;
		NumRunDC++;
	}
}

void CDCController::StopDCMotor(unsigned char MotorIndex)
{
	pwmDutyRatio[MotorIndex] = 0x00;
	
	if(DCMotor[MotorIndex].bRun == true){
		DCMotor[MotorIndex].bRun = false;
		NumRunDC--;
		PORTB &= ~(1 << DCMotor[MotorIndex].EnablePin); 
	}
}

void CDCController::ToggleDCMotor(unsigned char MotorIndex, int SpeedRatio)
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
	static volatile unsigned char pwmPinlevelDC = 0x00;
	static volatile unsigned char pwmFastcountDC = 0x00;
	
	if(NumRunDC)	// if not 0
	{
		PORTB = pwmPinlevelDC;
		
		if(++pwmCounter == 0)
		{
			for(pwmFastcountDC = 0; pwmFastcountDC < NumDC; pwmFastcountDC++){
				pwmDutyCompare[pwmFastcountDC] = pwmDutyRatio[pwmFastcountDC];
			}
			pwmPinlevelDC = pwmMaskPortb;												// all pins high
		}
	
		for(pwmFastcountDC = 0; pwmFastcountDC < NumDC; pwmFastcountDC++){
			if(pwmDutyCompare[pwmFastcountDC] == pwmCounter){
				pwmPinlevelDC &= ~(1 << DCMotor[pwmFastcountDC].EnablePin);			// clear specific pin
			}
		}
	
		if(pwmCounter == 99)	pwmCounter = 0xFF;
	}
}
