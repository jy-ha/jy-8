/*
 * DefinedEvents.h
 *
 * Created: 2016-01-28 오후 5:15:15
 *  Author: Blonix
 */ 


#ifndef DEFINEDEVENTS_H_
#define DEFINEDEVENTS_H_
//=====================================================================================//


enum
{
	ENOTHING,
	ETEST,
	ETEST2,
	ETEST3,
	
	// SoundController
	ESND_STOP,
	
	// CarMovement
	ERUNFOWARD,
	ESTOPLMOTOR,
	ESTOPRMOTOR,
	ESTOPLRMOTOR,
	
	// EtcMovement
	ETURNHEAD
};


//=====================================================================================//



enum
{
	ERRORCODE_0001,			// delay event overflow
	ERRORCODE_0002,			// add DCmotor overflow
	ERRORCODE_0003,			// Invalid PinPort (DCMotor)
	ERRORCODE_0004,			// Invalid PinPort (HC_SR04)
	ERRORCODE_0005,			// Invalid IntPin  (HC_SR04)
	ERRORCODE_0006,			// add Servomotor overflow
	ERRORCODE_0007,			// UART_BUFFER_OVERFLOW
	ERRORCODE_0008,			// UART_OVERRUN_ERROR
	ERRORCODE_0009,			// UART_FRAME_ERROR
	ERRORCODE_0010			// FlowManager uart error
};



//=====================================================================================//

struct SGolbalEvent{
	uint16_t	nID;
	uint16_t	nParam1;
	uint16_t	nParam2;
	uint8_t		Data[MAX_EVENTDATALEN];
};

extern	SGolbalEvent	GlobalEventQueue[MAX_EVENTQUEUE];
extern	uint8_t			GlobalEventNum;
extern	void			EventAdd(SGolbalEvent MyEvent);		// FlowManager.cpp
extern	void			EventAdd(uint16_t EventID);			// FlowManager.cpp
extern	void			EventDel();							// FlowManager.cpp


#endif /* DEFINEDEVENTS_H_ */