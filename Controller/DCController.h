/*
 * DCController.h
 *
 * Created: 2016-01-13 오전 11:15:02
 *  Author: Blonix
 */ 


#ifndef DCController_H_
#define DCController_H_

#define	DC_PORT		PORTB
#define	DC_DDR		DDRB
#define DC_MASK		pwmMaskPortb

struct SDCMotor{
	char			Name[MAX_NAMELEN];
	char			InPinPort;				// A B C D E F
	uint8_t			EnablePin;				// only port B
	uint8_t			In1Pin;
	uint8_t			In2Pin;
	bool	bRun;
};

//======================================================================================//
//=============================  DC Controller Class  ==================================//
//======================================================================================//


class CDCController
{
private:
	uint8_t					NumDC;								// volatile X
	uint8_t					NumRunDC;							// volatile X
	SDCMotor				DCMotor[MAX_DCMOTOR];				// volatile X
	
	volatile uint8_t		pwmDutyRatio[MAX_DCMOTOR];			// 0 ~ 100
	volatile uint8_t		pwmCounter;							// 0 ~ 100
	
public:	
	CDCController();
	
	uint8_t AddDCMotor(SDCMotor);
	uint8_t FindDCMotor(char Name[]);
	
	void RunDCMotor(uint8_t MotorIndex, int8_t SpeedRatio);		// SpeedRatio : -100 ~ 100
	void StopDCMotor(uint8_t MotorIndex);
	void ToggleDCMotor(uint8_t MotorIndex, int8_t SpeedRatio);	// SpeedRatio : -100 ~ 100
	
	void PWM_DCMotor();												// Run at ISR
};





#endif /* DCController_H_ */