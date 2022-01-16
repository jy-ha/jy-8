/*
 * MPU6050.h
 *
 * Created: 2016-02-07 오전 10:52:42
 *  Author: Blonix
 */ 


#ifndef MPU6050_H_
#define MPU6050_H_


//======================================================================================//
//================================  MPU6050 Class  =====================================//
// 참고 코드
// http://davidegironi.blogspot.kr/2013/02/avr-atmega-mpu6050-gyroscope-and.html#.VrX-1vmLSUk
// http://blog.naver.com/PostView.nhn?blogId=hyehwan2&logNo=194200550
// 마호니 필터는 SensorBase Class 에서 참고
// uart 라이브러리와 i2cmaster 라이브러리는 ../Common 폴더에 추가
//======================================================================================//

#include "mpu6050registers.h"

//definitions
#define MPU6050_ADDR (0x68 <<1) //device address - 0x68 pin low (GND), 0x69 pin high (VCC)

//definitions for raw data
//gyro and acc scale
#define MPU6050_GYRO_FS MPU6050_GYRO_FS_2000
#define MPU6050_ACCEL_FS MPU6050_ACCEL_FS_2

#define MPU6050_GYRO_LSB_250 131.0
#define MPU6050_GYRO_LSB_500 65.5
#define MPU6050_GYRO_LSB_1000 32.8
#define MPU6050_GYRO_LSB_2000 16.4
#if MPU6050_GYRO_FS == MPU6050_GYRO_FS_250
#define MPU6050_GGAIN MPU6050_GYRO_LSB_250
#elif MPU6050_GYRO_FS == MPU6050_GYRO_FS_500
#define MPU6050_GGAIN MPU6050_GYRO_LSB_500
#elif MPU6050_GYRO_FS == MPU6050_GYRO_FS_1000
#define MPU6050_GGAIN MPU6050_GYRO_LSB_1000
#elif MPU6050_GYRO_FS == MPU6050_GYRO_FS_2000
#define MPU6050_GGAIN MPU6050_GYRO_LSB_2000
#endif

#define MPU6050_ACCEL_LSB_2 16384.0
#define MPU6050_ACCEL_LSB_4 8192.0
#define MPU6050_ACCEL_LSB_8 4096.0
#define MPU6050_ACCEL_LSB_16 2048.0
#if MPU6050_ACCEL_FS == MPU6050_ACCEL_FS_2
#define MPU6050_AGAIN MPU6050_ACCEL_LSB_2
#elif MPU6050_ACCEL_FS == MPU6050_ACCEL_FS_4
#define MPU6050_AGAIN MPU6050_ACCEL_LSB_4
#elif MPU6050_ACCEL_FS == MPU6050_ACCEL_FS_8
#define MPU6050_AGAIN MPU6050_ACCEL_LSB_8
#elif MPU6050_ACCEL_FS == MPU6050_ACCEL_FS_16
#define MPU6050_AGAIN MPU6050_ACCEL_LSB_16
#endif

#define MPU6050_CALIBRATEDACCGYRO 1 //set to 1 if is calibrated
#if MPU6050_CALIBRATEDACCGYRO == 1
#define MPU6050_AXOFFSET 0
#define MPU6050_AYOFFSET 0
#define MPU6050_AZOFFSET 0
#define MPU6050_AXGAIN 16384.0
#define MPU6050_AYGAIN 16384.0
#define MPU6050_AZGAIN 16384.0
#define MPU6050_GXOFFSET -21
#define MPU6050_GYOFFSET -10
#define MPU6050_GZOFFSET -24
#define MPU6050_GXGAIN 131
#define MPU6050_GYGAIN 131
#define MPU6050_GZGAIN 16.4
#endif

#define MPU6050_KALMAN_Q_ACC 0.01
#define MPU6050_KALMAN_Q_GYR 0.04
#define MPU6050_KALMAN_R_ACC 0.3
#define MPU6050_KALMAN_R_ROT 0.3
#define MPU6050_LOWHIGH_TAU 0.1

class CMPU6050	: public CSensorBase
{
private:
	SKalman1D	KalmanAccX;
	SKalman1D	KalmanAccY;
	SKalman1D	KalmanAccZ;
	SKalman2D	KalmanAngX;
	SKalman2D	KalmanAngY;
	SLowHigh	YawHF;
	double		YawAngle;

	uint8_t buffer[14];
	
	void	KalmanPredict2D(SKalman2D *kalman, double NewX_1, double Dt);
	
	int8_t	mpu6050_readBytes	(uint8_t regAddr, uint8_t length, uint8_t *data);
	int8_t	mpu6050_readByte	(uint8_t regAddr, uint8_t *data);
	void	mpu6050_writeBytes	(uint8_t regAddr, uint8_t length, uint8_t* data);
	void	mpu6050_writeByte	(uint8_t regAddr, uint8_t data);
	int8_t	mpu6050_readBits	(uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data);
	int8_t	mpu6050_readBit		(uint8_t regAddr, uint8_t bitNum, uint8_t *data);
	void	mpu6050_writeBits	(uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data);
	void	mpu6050_writeBit	(uint8_t regAddr, uint8_t bitNum, uint8_t data);
	
public:
	CMPU6050();
	~CMPU6050();
	
	void	init();
	void	setSleepDisabled();
	void	setSleepEnabled();
	
	void	getRawData	(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz);
	Vector	getConvRotateData(double DeltaTime);	// DeltaTime as Second
	Vector	getConvAccelData();
	Vector	getConvGyroData();
	
	// X 는 초기 상태, Q는 노이즈 공분산 상수(물리적 시스템에 따라 결정), R은 측정 공분산 행렬, P는 정지상태의 초기값이면 0, 불확실할수록 큰 값 필요
	
	void	InitKalmanAcc(double ax, double ay, double az, double P);
	void	InitKalmanRot(double Roll, double Pitch, double Yaw, double P);
};


#endif /* MPU6050_H_ */