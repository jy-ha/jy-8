/*
 * SensorBase.cpp
 *
 * Created: 2016-02-06 오후 2:11:24
 *  Author: Blonix
 */ 

#include "../Common/Vector.h"
#include "SensorBase.h"

CSensorBase::CSensorBase()
{
	bUpdate = false;
	bFilter = false;
}

CSensorBase::~CSensorBase()
{
	
}


void CSensorBase::KalmanPredictUpdate1D(SKalman1D* Kalman, double NewData)
{
	double K;	// Kalman gain
	
	// Predict
	// Kalman->X 는 1차원에서 그냥 예전값과 동일할거라 예측됨(?)
	Kalman->P = Kalman->P + Kalman->Q;
	
	// Update
	K = Kalman->P / (Kalman->P + Kalman->R);
	Kalman->X = Kalman->X + K * (NewData - Kalman->X);
	Kalman->P = (1 - K) * Kalman->P;
}

void CSensorBase::KalmanUpdate2D(SKalman2D* Kalman, double NewX_0)
{
	double y = NewX_0 - Kalman->X_0;
	double S = Kalman->P_00 + Kalman->R;
	double K_0 = Kalman->P_00 / S;
	double K_1 = Kalman->P_10 / S;
	Kalman->X_0 += K_0 * y;
	Kalman->X_1 += K_1 * y;
	Kalman->P_00 -= K_0 * Kalman->P_00;
	Kalman->P_01 -= K_0 * Kalman->P_01;
	Kalman->P_10 -= K_1 * Kalman->P_00;
	Kalman->P_11 -= K_1 * Kalman->P_01;
}

void CSensorBase::LowPassFilter(SLowHigh *lowhigh, double NewData, double Dt)
{
	lowhigh->FilterData = (lowhigh->tau * lowhigh->FilterData + Dt * NewData) / (lowhigh->tau + Dt);
}

void CSensorBase::HighPassFilter(SLowHigh *lowhigh, double NewData, double Dt)
{
	lowhigh->FilterData = (lowhigh->tau / (lowhigh->tau + Dt) * lowhigh->FilterData)
						+ (lowhigh->tau / (lowhigh->tau + Dt) * (NewData - lowhigh->PreRawData));
	
	lowhigh->PreRawData = NewData;
}