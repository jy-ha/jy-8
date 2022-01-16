/*
 * ArmMovement.cpp
 *
 * Created: 2016-02-22 오전 10:43:03
 *  Author: Blonix
 */ 

#include "define.h"
#include "ArmMovement.h"


CArmMovement::CArmMovement()
{
	bFail = true;
}

void CArmMovement::Init(SServo Rot, SServo Low, SServo High, SServo Hand)
{
	RotServoIndex = ServoController.AddServoMotor(Rot, 0);
	LowServoIndex = ServoController.AddServoMotor(Low, 0);
	HighServoIndex = ServoController.AddServoMotor(High, 0);
	HandServoIndex = ServoController.AddServoMotor(Hand, 0);
	
	if( (RotServoIndex==255) || (LowServoIndex==255) || (HighServoIndex==255) || (HandServoIndex==255) )	bFail = true;
	else	bFail = false;
}

void CArmMovement::CheckEvent()
{
	switch (GlobalEventQueue[0].nID)
	{
		case ETEST3:	break;
		default:		return;
	}
	EventDel();
}

bool CArmMovement::SetTransition(Vector Position, Vector Rotation)
{
	if (bFail)	return false;
	
	double		AngleYaw = 0;
	double		AngleLow;
	double		AngleHigh;
	uint16_t	distance = sqrt(Position.x*Position.x + Position.y*Position.y + Position.z*Position.z);
	if(distance == 0)	return false;
	//////////////////////////////////////////////////////////////////////////
	
	if(Position.x != 0){
		if(Position.y < 0)		AngleYaw = RADTOANG(atan( (-Position.y) / (-Position.x) ));
		else					AngleYaw = RADTOANG(atan(Position.y / Position.x));
	}
	//////////////////////////////////////////////////////////////////////////
	
	if(Position.z == 0){
		AngleLow = RADTOANG(acos( (ARMLEN_HIGH*ARMLEN_HIGH - ARMLEN_LOW*ARMLEN_LOW - distance*distance) / (2*distance*ARMLEN_LOW) ));
	}
	else{
		AngleLow =	RADTOANG(asin(distance/Position.z))
				+ RADTOANG(acos( (ARMLEN_HIGH*ARMLEN_HIGH - ARMLEN_LOW*ARMLEN_LOW - distance*distance) / (2*distance*ARMLEN_LOW) ));
	}
	AngleLow = 180 - AngleLow;
	if( (AngleLow > 180) || (AngleLow < 0) )	return false;
	//////////////////////////////////////////////////////////////////////////
	
	AngleHigh = RADTOANG(acos( (distance*distance - ARMLEN_LOW*ARMLEN_LOW - ARMLEN_HIGH*ARMLEN_HIGH) / (2*ARMLEN_HIGH*ARMLEN_LOW) ));
	//////////////////////////////////////////////////////////////////////////
	
	ServoController.RunServoMotor(RotServoIndex, (uint8_t)AngleYaw);
	ServoController.RunServoMotor(LowServoIndex, (uint8_t)AngleLow);
	ServoController.RunServoMotor(HighServoIndex, (uint8_t)AngleHigh);
	return true;
}

void CArmMovement::ReleaseArm()
{
	ServoController.StopAllServoMotor();
}
