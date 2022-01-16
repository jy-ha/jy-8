/*
 * Perception.h
 *
 * Created: 2016-02-07 오전 12:22:17
 *  Author: Blonix
 */ 


#ifndef PERCEPTION_H_
#define PERCEPTION_H_


//======================================================================================//
//==============================  Perception Class  ====================================//
//======================================================================================//


class CPerception
{
private:
	CMPU6050	MPU6050;
	CHCSR04		HCSR04_Front;
	CHCSR04		HCSR04_Down;
	CSimpleADC	Nock_Front;
	CSimpleADC	Nock_Left;
	CSimpleADC	Nock_Right;
	CSimpleADC	Nock_Back;
	CSimpleADC	Light_Infrared;
	CSimpleADC	Light_Optical;
	bool		bNockUpdate;
	
	//Vector		Location;
	//Vector		Rotation;
	//Vector		LinearVelocity;
	//Vector		RadialVelocity;
	//Vector		LinearAcceleration;
	//Vector		RadialAcceleration;
	
	
public:
	CPerception();
	~CPerception();
	
	void Init();
	void Update(double dt);
	void UltraSonic();			// 0.33cm per count
	void IntON(uint8_t);		// 0 ~ 7
};



#endif /* PERCEPTION_H_ */