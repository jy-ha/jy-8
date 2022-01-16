/*
 * SensorBase.h
 *
 * Created: 2016-02-06 오후 2:12:31
 *  Author: Blonix
 */ 


#ifndef SENSORBASE_H_
#define SENSORBASE_H_


struct SKalman1D{
	double	X;							// 상태 행렬
	double	Q;							// 센서 노이즈 공분산 상수
	double	P;							// 상태 공분산 행렬
	double	R;							// 측정 공분산 행렬
};

struct SKalman2D{
	double	X_0, X_1;					// 상태 행렬
	double	Q_0, Q_1;					// 센서 노이즈 공분산 상수
	double	P_00, P_01, P_10, P_11;		// 상태 공분산 행렬
	double	R;							// 측정 공분산 행렬
};

struct SLowHigh{
	double tau;
	double PreRawData;
	double FilterData;
};



//======================================================================================//
//==============================  Sensor Base Class  ===================================//
//======================================================================================//


class CSensorBase
{
private:
	
protected:
	bool	bFilter;
	
	void KalmanPredictUpdate1D(SKalman1D *kalman, double NewData);
	//void KalmanPredict2D(SKalman2D *kalman, double NewX_1, double Dt);		// 시스템마다 달라지므로 해당 클래스 안에서 구현. 가상함수 사용시 속도 저하
	void KalmanUpdate2D(SKalman2D*, double NewX_0);
	
	void LowPassFilter(SLowHigh *lowhigh, double NewData, double Dt);
	void HighPassFilter(SLowHigh *lowhigh, double NewData, double Dt);
	
	
public:
	bool	bUpdate;
	
	CSensorBase();
	~CSensorBase();

	void EnableFilter(bool Newbool){ bFilter = Newbool; };		// 칼만필터 사용 여부
};



#endif /* SENSORBASE_H_ */