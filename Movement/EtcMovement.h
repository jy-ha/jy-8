/*
 * EtcMovement.h
 *
 * Created: 2016-03-07 오후 4:29:42
 *  Author: Blonix
 */ 


#ifndef ETCMOVEMENT_H_
#define ETCMOVEMENT_H_



class CEtcMovement
{
private:
	uint8_t			HeadServoIndex;
	bool			bFail;
	
public:
	CEtcMovement();
	void Init(SServo Head);
	
	void CheckEvent();
	
	void TurnHead(uint8_t angle);
};


#endif /* ETCMOVEMENT_H_ */