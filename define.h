/*
 * define.h
 *
 * Created: 2016-01-13 오전 12:15:12
 *  Author: Blonix
 */ 


#ifndef DEFINE_H_
#define DEFINE_H_

#define F_CPU					16000000UL
#define UART_BAUD_RATE			9600

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>

/* Require heavy memory */
#include <stdio.h>			// FlowManager
#include <stdlib.h>			// Perception
#include <string.h>			// DCController, ServoController
//////////////////////////

#define BITSET(x)		|= (1<<x)
#define BITCLR(x)		&= ~(1<<x)
#define BITTOG(x)		^= (1<<x)
#define PI				3.141592
#define RADTOANG(x)		(x) * 180 / PI
#define CTOI(x)			(uint8_t)((x) - '0')

#define ERRORPIN			PB7
#define SOUNDPIN			PA6

#define MAX_DELAYEVENT		10
#define MAX_DCMOTOR			8
#define MAX_SERVOMOTOR		8
#define MAX_NAMELEN			10
#define MAX_EVENTQUEUE		10
#define MAX_MELODYLEN		24
#define MAX_UARTBUFFLEN		64		// must be power of 2
#define MAX_EVENTDATALEN	24


#include "Common/Vector.h"
#include "Common/uart.h"
#include "Common/i2cmaster.h"

#include "Sensors/SensorBase.h"
#include "Sensors/MPU6050.h"
#include "Sensors/HC_SR04.h"
#include "Sensors/SimpleADC.h"

#include "DefinedEvents.h"
#include "FlowManager.h"
#include "Controller/SoundController.h"
#include "Controller/DCController.h"
#include "Controller/ServoController.h"
#include "Perception.h"

extern uint8_t				pwmMaskPortb;
extern uint8_t				pwmMaskPortc;
extern CFlowManager			FlowManager;
extern CDCController		DCController;
extern CSoundController		SoundController;
extern CServoController		ServoController;
extern CPerception			Perception;


#endif /* DEFINE_H_ */