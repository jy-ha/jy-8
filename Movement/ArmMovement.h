/*
 * ArmMovement.h
 *
 * Created: 2016-02-22 오전 10:43:15
 *  Author: Blonix
 */ 


#ifndef ARMMOVEMENT_H_
#define ARMMOVEMENT_H_

#define	ARMLEN_LOW		30
#define ARMLEN_HIGH		30

class CArmMovement
{
private:
	uint8_t			RotServoIndex;
	uint8_t			LowServoIndex;
	uint8_t			HighServoIndex;
	uint8_t			HandServoIndex;
	bool			bFail;
	
public:
	CArmMovement();
	void Init(SServo Rot, SServo Low, SServo High, SServo Hand);
	
	void CheckEvent();
	
	bool SetTransition(Vector Position, Vector Rotation);
	void ReleaseArm();
};


#endif /* ARMMOVEMENT_H_ */