/*
 * SimpleADC.h
 *
 * Created: 2016-02-21 오후 10:26:50
 *  Author: Blonix
 */ 


#ifndef SIMPLEADC_H_
#define SIMPLEADC_H_

#if (F_CPU == 16000000UL)
	#define SIMPLEADC_ADCSRA	0b11000111
#else
	#error "ADCSRA freescaler need change"
#endif

//======================================================================================//
//================================  SimpleADC Class  ===================================//
//======================================================================================//

class CSimpleADC : public CSensorBase
{
private:
	SKalman1D	KalmanData;
	uint8_t		MyADMUX;
	
public:
	CSimpleADC();
	
	void		Init(uint8_t NumADC);
	void		InitKalman(double InitData, double Q, double R, double P);
	uint16_t	GetData();		// delay 65 ~ 260 us
	
};


#endif /* SIMPLEADC_H_ */