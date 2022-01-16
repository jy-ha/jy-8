/*
 * ServoController.h
 *
 * Created: 2016-02-21 오전 11:59:31
 *  Author: Blonix
 */ 


#ifndef SERVOCONTROLLER_H_
#define SERVOCONTROLLER_H_

#define	SERVO_PORT	PORTC
#define	SERVO_DDR	DDRC
#define	SERVO_MASK	pwmMaskPortc

enum{	SERVOPWM_WAIT,	SERVOPWM_HIGH,	SERVOPWM_PULSE	};

struct SServo{
	char			Name[MAX_NAMELEN];
	uint8_t			PWMPin;				// only port C
};

//======================================================================================//
//============================  Servo Controller Class  ================================//
//======================================================================================//


class CServoController
{
private:
	uint8_t					NumServo;							// volatile X
	bool					bRun;								// volatile X
	SServo					ServoMotor[MAX_SERVOMOTOR];			// volatile X
	
	volatile uint8_t		pwmDutyRatio[MAX_SERVOMOTOR];		// 0 ~ 180
	volatile uint8_t		pwmCounter;							// 0 ~ 180
	volatile uint8_t		pwmWaitMode;
	
public:
	CServoController();
	
	uint8_t AddServoMotor(SServo, uint8_t angle);			// angle : 0 ~ 180
	uint8_t FindServoMotor(char Name[]);
	void RunServoMotor(uint8_t ServoIndex, uint8_t angle);		// angle : 0 ~ 180
	void StopAllServoMotor();
	void PWM_ServoMotor();										// Run at ISR
	
	volatile uint16_t	ServoTCNT;
};



#endif /* SERVOCONTROLLER_H_ */