/*
 * CarMovement.h
 *
 * Created: 2016-01-23 오후 4:05:22
 *  Author: Blonix
 */ 


#ifndef MOVEMENT_H_
#define MOVEMENT_H_



class CCarMovement
{
private:
	uint8_t	LMotorIndex;
	uint8_t	RMotorIndex;
	bool			bFail;
		
public:
	CCarMovement();
	void Init(SDCMotor LMotor, SDCMotor RMotor);
	
	void CheckEvent();
	
	void MoveAhead(int SpeedRatio, uint16_t MilliSec);
	void MoveLeft(int SpeedRatio, uint8_t TurnRadius);
	void MoveRight(int SpeedRatio, uint8_t TurnRadius);
	void MoveTurnCW(int SpeedRatio, uint8_t TurnRadius, uint16_t MilliSec);
	void MoveTurnCCW(int SpeedRatio, uint8_t TurnRadius, uint16_t MilliSec);
	
	//void StopLRMotor(){ DCController.StopDCMotor(LMotorIndex); DCController.StopDCMotor(RMotorIndex); }
};


#endif /* CARMOVEMENT_H_ */