/*
 * CarMovement.cpp
 *
 * Created: 2016-01-23 오후 4:05:06
 *  Author: Blonix
 */ 

#include "define.h"
#include "CarMovement.h"


CCarMovement::CCarMovement()
{
	bFail = true;
}

void CCarMovement::Init(SDCMotor LMotor, SDCMotor RMotor)
{
	LMotorIndex = DCController.AddDCMotor(LMotor);
	RMotorIndex = DCController.AddDCMotor(RMotor);
	
	if( (LMotorIndex==255) || (RMotorIndex==255) )		bFail = true;
	else												bFail = false;
}

void CCarMovement::CheckEvent()
{
	switch (GlobalEventQueue[0].nID)
	{
	case ETEST:				MoveAhead(100, 3000);															break;
	case ERUNFOWARD:		MoveAhead(GlobalEventQueue[0].nParam1, GlobalEventQueue[0].nParam2);			break;
	case ESTOPLMOTOR:		DCController.StopDCMotor(LMotorIndex);											break;
	case ESTOPRMOTOR:		DCController.StopDCMotor(RMotorIndex);											break;
	case ESTOPLRMOTOR:		DCController.StopDCMotor(LMotorIndex); DCController.StopDCMotor(RMotorIndex);	break;
	default:	return;
	}
	EventDel();
}

void CCarMovement::MoveAhead(int SpeedRatio, uint16_t MilliSec)
{
	if (bFail)	return;
	
	DCController.RunDCMotor(LMotorIndex, SpeedRatio);
	DCController.RunDCMotor(RMotorIndex, SpeedRatio);
	
	FlowManager.Delay(MilliSec, ESTOPLRMOTOR);
}

void CCarMovement::MoveLeft(int SpeedRatio, uint8_t TurnRadius)
{
	if (bFail)	return;
}

void CCarMovement::MoveRight(int SpeedRatio, uint8_t TurnRadius)
{
	if (bFail)	return;
}

void CCarMovement::MoveTurnCW(int SpeedRatio, uint8_t TurnRadius, uint16_t MilliSec)
{
	if (bFail)	return;
}

void CCarMovement::MoveTurnCCW(int SpeedRatio, uint8_t TurnRadius, uint16_t MilliSec)
{
	if (bFail)	return;
}
