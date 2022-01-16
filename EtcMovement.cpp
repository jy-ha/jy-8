/*
 * EtcMovement.cpp
 *
 * Created: 2016-03-07 오후 4:28:38
 *  Author: Blonix
 */ 

#include "define.h"
#include "EtcMovement.h"


CEtcMovement::CEtcMovement()
{
	bFail = true;
}

void CEtcMovement::Init(SServo Head)
{
	HeadServoIndex = ServoController.AddServoMotor(Head, 0);
	
	if( HeadServoIndex==255 )	bFail = true;
	else						bFail = false;
}

void CEtcMovement::CheckEvent()
{
	switch (GlobalEventQueue[0].nID)
	{
		case ETURNHEAD:	TurnHead(GlobalEventQueue[0].nParam1);	break;
		default:		return;
	}
	EventDel();
}

void CEtcMovement::TurnHead(uint8_t angle)
{
	ServoController.RunServoMotor(HeadServoIndex, angle);
}
