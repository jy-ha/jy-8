/*
 * FlowManager.h
 *
 * Created: 2016-01-12 오후 7:44:48
 *  Author: Blonix
 */ 


#ifndef FlowManager_H_
#define FlowManager_H_


//======================================================================================//
//==============================  Flow Manager Class  ==================================//
//======================================================================================//


class CFlowManager
{
private:
	uint16_t		DelayEventTime[MAX_DELAYEVENT];
	SGolbalEvent	DelayEventArray[MAX_DELAYEVENT];		// 최대 10개의 delay 동시기능
	
	
public:	
	CFlowManager();
	~CFlowManager();
	
	void ErrorOutput(uint16_t ErrCode);
	void Delay(uint16_t millisecond, SGolbalEvent);			// timer 이용 잠복성 이벤트 함수
	void Delay(uint16_t millisecond, uint16_t EventID);
	void ExecDelayEvent();
	void ExecUartEvent();
	
	uint16_t			DelayEventCounter;
	volatile uint16_t	timer1_ms[MAX_DELAYEVENT];
	volatile uint32_t	MilliTick;
};



#endif /* FlowManager_H_ */