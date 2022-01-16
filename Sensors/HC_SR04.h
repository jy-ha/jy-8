/*
 * HC_SR04.h
 *
 * Created: 2016-02-13 오전 9:51:50
 *  Author: Blonix
 */ 


#ifndef HC_SR04_H_
#define HC_SR04_H_


//======================================================================================//
//=================================  HC SR04 Class  ====================================//
//======================================================================================//

#define HCSR04_KALMAN_Q 0.01
#define HCSR04_KALMAN_R 0.3


class CHCSR04 : public CSensorBase
{
private:
	SKalman1D	KalmanDistance;
	double		Distance;		// cm
	
	bool		bGetDist;		// set true when NewData income
	bool		IntToggle;		// ISR Falling & Rising toggle
	char		TrigPort;
	uint8_t		TrigPin;
	
	void		Pulse();		// include 10us delay
	
public:
	CHCSR04();
	
	bool		Trigered;	// Duration++ at each 19.5us when true
	uint8_t		IntNum;		// interrupt number
	uint16_t	Duration;	// Duration/3 = 1cm
	
	void		Init(char TrigPort, uint8_t TrigPin, uint8_t INT_Num);		// INT pin ONLY (EchoPin)
	void		InitKalman(double Dist, double P);
	double		GetDistance(bool bSync);	// true = wait until receive
	void		ReceiveEcho();				// INT ISR
};



#endif /* HC_SR04_H_ */