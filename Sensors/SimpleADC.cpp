/*
 * SimpleADC.cpp
 *
 * Created: 2016-02-21 오후 10:26:43
 *  Author: Blonix
 */ 

#include "../define.h"


CSimpleADC::CSimpleADC()
{
}

void CSimpleADC::Init(uint8_t NumADC)
{
	MyADMUX = NumADC;
}

void CSimpleADC::InitKalman(double InitData, double Q, double R, double P)
{
	KalmanData.X = InitData;
	KalmanData.Q = Q;
	KalmanData.R = R;
	KalmanData.P = P;
}

uint16_t CSimpleADC::GetData()
{
	ADMUX = MyADMUX;
	ADCSRA = SIMPLEADC_ADCSRA;
	
	while((ADCSRA & 0x10) == 0);									// wait until ADIF = 1
	uint16_t TempData = (uint16_t)ADCL + ((uint16_t)ADCH << 8);		// read A/D
	
	if(bFilter){
		KalmanPredictUpdate1D(&KalmanData, TempData);
		return KalmanData.X;
	}
	
	return TempData;
}
