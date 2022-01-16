/*
 * HC_SR04.cpp
 *
 * Created: 2016-02-13 오전 9:51:40
 *  Author: Blonix
 */ 

#include "../define.h"

/* delete this if you have to */
#if !(defined(__AVR_ATmega128__) || defined(__AVR_ATmega128A__))
 #error "HC_SR04.cpp need INT port change"
#endif




CHCSR04::CHCSR04()
{
	IntToggle = false;
	Trigered = false;
	IntNum = 10;		// Unused pin
}

void CHCSR04::Init(char TrigPort_, uint8_t TrigPin_, uint8_t IntNum_)
{
	TrigPort = TrigPort_;
	TrigPin = TrigPin_;
	IntNum = IntNum_;
	
	switch (TrigPort)
	{
	case 'A':	DDRA BITSET(TrigPin);	break;
	case 'B':	DDRB BITSET(TrigPin);	break;
	case 'C':	DDRC BITSET(TrigPin);	break;
	case 'D':	DDRD BITSET(TrigPin);	break;
	case 'E':	DDRE BITSET(TrigPin);	break;
	case 'F':	DDRF BITSET(TrigPin);	break;
	default:	FlowManager.ErrorOutput(ERRORCODE_0004);	break;
	}
	
	switch (IntNum)		// input, external interrupt as Rising Edge
	{
	case 0:	DDRD BITCLR(PD0); EIMSK BITSET(INT0);	EICRA BITSET(ISC00);	EICRA BITSET(ISC01);	break;
	case 1:	DDRD BITCLR(PD1); EIMSK BITSET(INT1);	EICRA BITSET(ISC10);	EICRA BITSET(ISC11);	break;
	case 2:	DDRD BITCLR(PD2); EIMSK BITSET(INT2);	EICRA BITSET(ISC20);	EICRA BITSET(ISC21);	break;
	case 3:	DDRD BITCLR(PD3); EIMSK BITSET(INT3);	EICRA BITSET(ISC30);	EICRA BITSET(ISC31);	break;
	case 4:	DDRE BITCLR(PE4); EIMSK BITSET(INT4);	EICRB BITSET(ISC40);	EICRB BITSET(ISC41);	break;
	case 5:	DDRE BITCLR(PE5); EIMSK BITSET(INT5);	EICRB BITSET(ISC50);	EICRB BITSET(ISC51);	break;
	case 6:	DDRE BITCLR(PE6); EIMSK BITSET(INT6);	EICRB BITSET(ISC60);	EICRB BITSET(ISC61);	break;
	case 7:	DDRE BITCLR(PE7); EIMSK BITSET(INT7);	EICRB BITSET(ISC70);	EICRB BITSET(ISC71);	break;
	default:	FlowManager.ErrorOutput(ERRORCODE_0005);	break;
	}
}


void CHCSR04::Pulse()
{
	switch (TrigPort)
	{
	case 'A':	PORTA BITSET(TrigPin);	_delay_us(10);	PORTA BITCLR(TrigPin);	break;
	case 'B':	PORTB BITSET(TrigPin);	_delay_us(10);	PORTB BITCLR(TrigPin);	break;
	case 'C':	PORTC BITSET(TrigPin);	_delay_us(10);	PORTC BITCLR(TrigPin);	break;
	case 'D':	PORTD BITSET(TrigPin);	_delay_us(10);	PORTD BITCLR(TrigPin);	break;
	case 'E':	PORTE BITSET(TrigPin);	_delay_us(10);	PORTE BITCLR(TrigPin);	break;
	case 'F':	PORTF BITSET(TrigPin);	_delay_us(10);	PORTF BITCLR(TrigPin);	break;
	}
}

double CHCSR04::GetDistance(bool bSync)
{
	Pulse();
	
	if(bSync)	while(!bGetDist){ _delay_us(20); }	// wait
	
	bGetDist = false;
	
	if(bFilter){
		KalmanPredictUpdate1D(&KalmanDistance, Distance);
		return KalmanDistance.X;
	}
	return Distance;
}

void CHCSR04::ReceiveEcho()
{
	if(!IntToggle){
		// Rising Edge
		
		Trigered = true;
		Duration = 0;
		
		switch (IntNum)
		{
		case 0:	EICRA BITCLR(ISC00);	break;
		case 1:	EICRA BITCLR(ISC10);	break;
		case 2:	EICRA BITCLR(ISC20);	break;
		case 3:	EICRA BITCLR(ISC30);	break;
		case 4:	EICRB BITCLR(ISC40);	break;
		case 5:	EICRB BITCLR(ISC50);	break;
		case 6:	EICRB BITCLR(ISC60);	break;
		case 7:	EICRB BITCLR(ISC70);	break;
		}
		IntToggle = true;
	}
	else{
		// Falling Edge
		
		Trigered = false;
		Distance = ((double)Duration) / 3;
		bGetDist = true;
		
		switch (IntNum)
		{
		case 0:	EICRA BITSET(ISC00);	break;
		case 1:	EICRA BITSET(ISC10);	break;
		case 2:	EICRA BITSET(ISC20);	break;
		case 3:	EICRA BITSET(ISC30);	break;
		case 4:	EICRB BITSET(ISC40);	break;
		case 5:	EICRB BITSET(ISC50);	break;
		case 6:	EICRB BITSET(ISC60);	break;
		case 7:	EICRB BITSET(ISC70);	break;
		}
		IntToggle = false;
	}
}

void CHCSR04::InitKalman(double Dist, double P)
{
	KalmanDistance.X = Dist;
	KalmanDistance.Q = HCSR04_KALMAN_Q;
	KalmanDistance.R = HCSR04_KALMAN_R;
	KalmanDistance.P = P;
}
