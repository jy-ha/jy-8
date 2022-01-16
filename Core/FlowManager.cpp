/*
 * CFlowManager.cpp
 *
 * Created: 2016-01-12 오후 7:44:36
 *  Author: Blonix
 */ 

#include "define.h"


CFlowManager::CFlowManager()
{
	for(uint8_t i = 0; i<MAX_DELAYEVENT; i++){	timer1_ms[i] = 0;}
	DelayEventCounter = 0;
	MilliTick = 0;
}
CFlowManager::~CFlowManager()
{
}

void CFlowManager::ErrorOutput(uint16_t ErrCode)
{
	DDRB	|= (1<<ERRORPIN);
	PORTB	|= (1<<ERRORPIN);
	pwmMaskPortb |= (1<<ERRORPIN);
	
	char	TempMsg[20];
	sprintf(TempMsg, "Error! Code :: %d", ErrCode);
	uart_puts(TempMsg);
}

void CFlowManager::Delay(uint16_t millisecond, SGolbalEvent MyEvent_)
{
	for(uint8_t i = 0; i<MAX_DELAYEVENT; i++)
	{
		if(DelayEventTime[i] == 0){
			timer1_ms[i] = 0;
			DelayEventTime[i] = millisecond;
			DelayEventArray[i] = MyEvent_;
			DelayEventCounter++;
			return;
		}
	}
	ErrorOutput(ERRORCODE_0001);	// MAX_DELAYEVENT 이상의 delay 사용 시도
}
void CFlowManager::Delay(uint16_t millisecond, uint16_t EventID)
{
	for(uint8_t i = 0; i<MAX_DELAYEVENT; i++)
	{
		if(DelayEventTime[i] == 0){
			timer1_ms[i] = 0;
			DelayEventTime[i] = millisecond;
			DelayEventArray[i].nID = EventID;
			DelayEventCounter++;
			return;
		}
	}
	ErrorOutput(ERRORCODE_0001);	// MAX_DELAYEVENT 이상의 delay 사용 시도
}

void CFlowManager::ExecDelayEvent()
{
	for(uint8_t i = 0; i<DelayEventCounter; i++){
		if(DelayEventTime[i] != 0){
			if(timer1_ms[i] > DelayEventTime[i]){
				DelayEventTime[i] = 0;
				EventAdd(DelayEventArray[i]);
				DelayEventCounter--;
				
				for(uint8_t a = i; a<DelayEventCounter; a++){
					DelayEventTime[a] = DelayEventTime[a+1];
					DelayEventTime[a+1] = 0;
					timer1_ms[a] = timer1_ms[a+1];
					DelayEventArray[a] = DelayEventArray[a+1];
				}
			}
		}
	}
}

void CFlowManager::ExecUartEvent()
{
	switch(uart_getc())
	{
	case 0x0001:					break;			// Start Read
	case UART_NO_DATA:				return;
	case UART_BUFFER_OVERFLOW:		ErrorOutput(ERRORCODE_0007);	return;
	case UART_OVERRUN_ERROR:		ErrorOutput(ERRORCODE_0008);	return;
	case UART_FRAME_ERROR:			ErrorOutput(ERRORCODE_0009);	return;
	default:						ErrorOutput(ERRORCODE_0010);	return;
	}
	
	SGolbalEvent	NewEvent;
	uint8_t			DataCheckByte;
	uint8_t			TempBuff[3];
	
	DataCheckByte = (uint8_t)uart_getc();							// 0b00000001 if no param
	TempBuff[0] = CTOI(uart_getc());
	TempBuff[1] = CTOI(uart_getc());
	TempBuff[2] = CTOI(uart_getc());
	NewEvent.nID = (TempBuff[0]*100) + (TempBuff[1]*10) + TempBuff[2];
	
	if(DataCheckByte & 0b00000010){									// nParam1
		TempBuff[0] = CTOI(uart_getc());
		TempBuff[1] = CTOI(uart_getc());
		TempBuff[2] = CTOI(uart_getc());
		NewEvent.nID = (TempBuff[0]*100) + (TempBuff[1]*10) + TempBuff[2];
	}
	if(DataCheckByte & 0b00000100){									// nParam2
		TempBuff[0] = CTOI(uart_getc());
		TempBuff[1] = CTOI(uart_getc());
		TempBuff[2] = CTOI(uart_getc());
		NewEvent.nID = (TempBuff[0]*100) + (TempBuff[1]*10) + TempBuff[2];
	}
	if(DataCheckByte & 0b00001000){									// pData
		uint16_t	TempDataReader;
		for(uint16_t i=0; i<MAX_EVENTDATALEN; i++){
			TempDataReader = uart_getc();
			if(TempDataReader == UART_NO_DATA || 0x0000)	break;	// End Tag
			NewEvent.Data[i] = (uint8_t)TempDataReader;
		}
	}
	EventAdd(NewEvent);
}


////////////////////////////////////////////////////////////////////////////////////////////////

void	EventAdd(SGolbalEvent MyEvent)
{
	for(uint8_t a = 0; a<MAX_EVENTQUEUE; a++){
		if(GlobalEventQueue[a].nID == ENOTHING){
			GlobalEventQueue[a] = MyEvent;
			GlobalEventNum++;
			return;
		}
	}
}
void	EventAdd(uint16_t EventID)
{
	for(uint8_t a = 0; a<MAX_EVENTQUEUE; a++){
		if(GlobalEventQueue[a].nID == ENOTHING){
			GlobalEventQueue[a].nID = EventID;
			GlobalEventNum++;
			return;
		}
	}
}
void	EventDel()
{
	if(GlobalEventNum == 0)	return;
	GlobalEventQueue[0].nID = ENOTHING;
	GlobalEventNum--;
	for(uint8_t a = 0; a<GlobalEventNum; a++){
		GlobalEventQueue[a] = GlobalEventQueue[a+1];
		GlobalEventQueue[a+1].nID = ENOTHING;
	}
}
