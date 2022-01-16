/*
 * Perception.cpp
 *
 * Created: 2016-02-07 오전 12:22:03
 *  Author: Blonix
 */ 
#include "define.h"


CPerception::CPerception()
{
	MPU6050.bUpdate = false;
	MPU6050.InitKalmanAcc(0, 0, 0, 0);
	MPU6050.InitKalmanRot(0, 0, 0, 0);
	MPU6050.EnableFilter(true);
	
	HCSR04_Front.bUpdate = false;
	HCSR04_Front.InitKalman(0, 0);
	HCSR04_Front.EnableFilter(true);
	HCSR04_Down.bUpdate = false;
	HCSR04_Down.InitKalman(0, 0);
	HCSR04_Down.EnableFilter(true);
	
	bNockUpdate = true;
	Nock_Front.bUpdate = true;
	Nock_Left.bUpdate = true;
	Nock_Right.bUpdate = true;
	Nock_Back.bUpdate = true;
	
	Light_Infrared.bUpdate = false;
	Light_Optical.bUpdate = false;
	Light_Optical.InitKalman(0, 0.01, 0.3, 0);
	Light_Optical.EnableFilter(true);
}

CPerception::~CPerception()
{
	
}

void CPerception::Init()
{
	MPU6050.init();
	HCSR04_Down.Init('A', PA4, 4);
	HCSR04_Front.Init('A', PA5, 5);
	Nock_Front.Init(0);
	Nock_Left.Init(1);
	Nock_Right.Init(2);
	Nock_Back.Init(3);
	Light_Infrared.Init(4);
	Light_Optical.Init(5);
}

void CPerception::Update(double dt)
{
	char	TempC[20];
	if(MPU6050.bUpdate){
		Vector	TempAcc;
		TempAcc = MPU6050.getConvRotateData(dt/1000);
		dtostrf(TempAcc.x, 10, 5, TempC);
		uart_puts(TempC);
		uart_puts("  ");
		dtostrf(TempAcc.y, 10, 5, TempC);
		uart_puts(TempC);
		uart_puts("  ");
		dtostrf(TempAcc.z, 10, 5, TempC);
		uart_puts(TempC);
		uart_puts("\r\n");
	}
	if(HCSR04_Down.bUpdate){
		double TempDist;
		TempDist = HCSR04_Down.GetDistance(false);
		dtostrf(TempDist, 10, 5, TempC);
		uart_puts(TempC);
		uart_puts("\r\n");
	}
	if(bNockUpdate){
		double Tempd;
		Tempd = Nock_Front.GetData();
		if(Tempd>=3){
			dtostrf(Tempd, 10, 5, TempC);
			uart_puts(TempC);
			uart_puts("\r\n");
		}
	}
}

void CPerception::UltraSonic()
{
	if(HCSR04_Front.Trigered)	HCSR04_Front.Duration++;
	if(HCSR04_Down.Trigered)	HCSR04_Down.Duration++;
}

void CPerception::IntON(uint8_t IntNum)
{
	if(HCSR04_Front.IntNum == IntNum)	HCSR04_Front.ReceiveEcho();
	if(HCSR04_Down.IntNum == IntNum)	HCSR04_Down.ReceiveEcho();
}
