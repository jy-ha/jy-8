/*
 * MPU6050.cpp
 *
 * Created: 2016-02-07 오전 10:52:33
 *  Author: Blonix
 */ 


#include "../define.h"

CMPU6050::CMPU6050()
{
}
CMPU6050::~CMPU6050()
{
}

void CMPU6050::KalmanPredict2D(SKalman2D* Kalman, double NewX_1, double DeltaTime)
{
	// 자이로 + 가속도계
	Kalman->X_0 += DeltaTime * (NewX_1 - Kalman->X_1);
	
	Kalman->P_00 += DeltaTime * (DeltaTime*Kalman->P_11 - Kalman->P_01 - Kalman->P_10) + Kalman->Q_0;
	Kalman->P_01 -= DeltaTime * Kalman->P_11;
	Kalman->P_10 -= DeltaTime * Kalman->P_11;
	Kalman->P_11 += Kalman->Q_1; // ?
}

/*
 * read bytes from chip register
 */
int8_t CMPU6050::mpu6050_readBytes(uint8_t regAddr, uint8_t length, uint8_t *data) {
	uint8_t i = 0;
	int8_t count = 0;
	if(length > 0) {
		//request register
		i2c_start(MPU6050_ADDR | I2C_WRITE);
		i2c_write(regAddr);
		_delay_us(10);
		//read data
		i2c_start(MPU6050_ADDR | I2C_READ);
		for(i=0; i<length; i++) {
			count++;
			if(i==length-1)
				data[i] = i2c_readNak();
			else
				data[i] = i2c_readAck();
		}
		i2c_stop();
	}
	return count;
}

/*
 * read 1 byte from chip register
 */
int8_t CMPU6050::mpu6050_readByte(uint8_t regAddr, uint8_t *data) {
    return mpu6050_readBytes(regAddr, 1, data);
}

/*
 * write bytes to chip register
 */
void CMPU6050::mpu6050_writeBytes(uint8_t regAddr, uint8_t length, uint8_t* data) {
	if(length > 0) {
		//write data
		i2c_start(MPU6050_ADDR | I2C_WRITE);
		i2c_write(regAddr); //reg
		for (uint8_t i = 0; i < length; i++) {
			i2c_write((uint8_t) data[i]);
		}
		i2c_stop();
	}
}

/*
 * write 1 byte to chip register
 */
void CMPU6050::mpu6050_writeByte(uint8_t regAddr, uint8_t data) {
    return mpu6050_writeBytes(regAddr, 1, &data);
}

/*
 * read bits from chip register
 */
int8_t CMPU6050::mpu6050_readBits(uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data) {
    // 01101001 read byte
    // 76543210 bit numbers
    //    xxx   args: bitStart=4, length=3
    //    010   masked
    //   -> 010 shifted
    int8_t count = 0;
    if(length > 0) {
		uint8_t b;
		if ((count = mpu6050_readByte(regAddr, &b)) != 0) {
			uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
			b &= mask;
			b >>= (bitStart - length + 1);
			*data = b;
		}
    }
    return count;
}

/*
 * read 1 bit from chip register
 */
int8_t CMPU6050::mpu6050_readBit(uint8_t regAddr, uint8_t bitNum, uint8_t *data) {
    uint8_t b;
    uint8_t count = mpu6050_readByte(regAddr, &b);
    *data = b & (1 << bitNum);
    return count;
}

/*
 * write bit/bits to chip register
 */
void CMPU6050::mpu6050_writeBits(uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data) {
    //      010 value to write
    // 76543210 bit numbers
    //    xxx   args: bitStart=4, length=3
    // 00011100 mask byte
    // 10101111 original value (sample)
    // 10100011 original & ~mask
    // 10101011 masked | value
	if(length > 0) {
		uint8_t b = 0;
		if (mpu6050_readByte(regAddr, &b) != 0) { //get current data
			uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
			data <<= (bitStart - length + 1); // shift data into correct position
			data &= mask; // zero all non-important bits in data
			b &= ~(mask); // zero all important bits in existing byte
			b |= data; // combine data with existing byte
			mpu6050_writeByte(regAddr, b);
		}
	}
}

/*
 * write one bit to chip register
 */
void CMPU6050::mpu6050_writeBit(uint8_t regAddr, uint8_t bitNum, uint8_t data) {
    uint8_t b;
    mpu6050_readByte(regAddr, &b);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    mpu6050_writeByte(regAddr, b);
}

/*
 * set sleep disabled
 */
void CMPU6050::setSleepDisabled() {
	mpu6050_writeBit(MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, 0);
}

/*
 * set sleep enabled
 */
void CMPU6050::setSleepEnabled() {
	mpu6050_writeBit(MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, 1);
}

/*
 * initialize the accel and gyro
 */
void CMPU6050::init() {

	//allow mpu6050 chip clocks to start up
	_delay_ms(100);

	//set sleep disabled
	setSleepDisabled();
	//wake up delay needed sleep disabled
	_delay_ms(10);

	//set clock source
	//  it is highly recommended that the device be configured to use one of the gyroscopes (or an external clock source)
	//  as the clock reference for improved stability
	mpu6050_writeBits(MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, MPU6050_CLOCK_PLL_XGYRO);
	//set DLPF bandwidth to 42Hz
	mpu6050_writeBits(MPU6050_RA_CONFIG, MPU6050_CFG_DLPF_CFG_BIT, MPU6050_CFG_DLPF_CFG_LENGTH, MPU6050_DLPF_BW_42);
    //set sampe rate
	mpu6050_writeByte(MPU6050_RA_SMPLRT_DIV, 4); //1khz / (1 + 4) = 200Hz
	//set gyro range
	mpu6050_writeBits(MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, MPU6050_GYRO_FS);
	//set accel range
	mpu6050_writeBits(MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, MPU6050_ACCEL_FS);
}

/*
 * get raw data converted to g and deg/sec values
 */
Vector CMPU6050::getConvRotateData(double DeltaTime) {
	Vector AccelVec;
	Vector GyroVec;
	Vector Rotate;
	
	mpu6050_readBytes(MPU6050_RA_ACCEL_XOUT_H, 14, (uint8_t *)buffer);
	AccelVec.x = (((int16_t)buffer[0]) << 8) | buffer[1];
	AccelVec.y = (((int16_t)buffer[2]) << 8) | buffer[3];
	AccelVec.z = (((int16_t)buffer[4]) << 8) | buffer[5];
	GyroVec.x = (((int16_t)buffer[8]) << 8) | buffer[9];
	GyroVec.y = (((int16_t)buffer[10]) << 8) | buffer[11];
	GyroVec.z = (((int16_t)buffer[12]) << 8) | buffer[13];

	#if MPU6050_CALIBRATEDACCGYRO == 1
	AccelVec.x = (double)(AccelVec.x-MPU6050_AXOFFSET);
	AccelVec.y = (double)(AccelVec.y-MPU6050_AYOFFSET);
	AccelVec.z = (double)(AccelVec.z-MPU6050_AZOFFSET);
	GyroVec.x = (double)(GyroVec.x-MPU6050_GXOFFSET)/MPU6050_GXGAIN;
	GyroVec.y = (double)(GyroVec.y-MPU6050_GYOFFSET)/MPU6050_GYGAIN;
	GyroVec.z = (double)(GyroVec.z-MPU6050_GZOFFSET)/MPU6050_GZGAIN;
	#else
	GyroVec.x = (double)(GyroVec.x)/MPU6050_GGAIN;
	GyroVec.y = (double)(GyroVec.y)/MPU6050_GGAIN;
	GyroVec.z = (double)(GyroVec.z)/MPU6050_GGAIN;
	#endif
	
	if(bFilter){
		KalmanPredict2D(&KalmanAngX, GyroVec.x, DeltaTime);
		KalmanPredict2D(&KalmanAngY, GyroVec.y, DeltaTime);
		KalmanUpdate2D(&KalmanAngX, RADTOANG(atan2(AccelVec.x, AccelVec.z)));
		KalmanUpdate2D(&KalmanAngY, RADTOANG(atan2(AccelVec.y, AccelVec.z)));
		HighPassFilter(&YawHF, YawAngle + GyroVec.z  * DeltaTime, DeltaTime);
		YawAngle = YawHF.FilterData;
		Rotate.x = KalmanAngX.X_0;
		Rotate.y = KalmanAngY.X_0;
		Rotate.z = YawAngle;
		return Rotate;
	}
	
	YawAngle = YawAngle + GyroVec.z * DeltaTime;
	Rotate.x = atan2(AccelVec.x, AccelVec.z) * 180 / 3.141592;
	Rotate.y = atan2(AccelVec.y, AccelVec.z) * 180 / 3.141592;
	Rotate.z = YawAngle;
	return Rotate;
}

Vector	CMPU6050::getConvAccelData	()
{
	Vector AccelVec;
	
	mpu6050_readBytes(MPU6050_RA_ACCEL_XOUT_H, 14, (uint8_t *)buffer);
	AccelVec.x = (((int16_t)buffer[0]) << 8) | buffer[1];
	AccelVec.y = (((int16_t)buffer[2]) << 8) | buffer[3];
	AccelVec.z = (((int16_t)buffer[4]) << 8) | buffer[5];
	
	#if MPU6050_CALIBRATEDACCGYRO == 1
	AccelVec.x = (double)(AccelVec.x-MPU6050_AXOFFSET)/MPU6050_AXGAIN;
	AccelVec.y = (double)(AccelVec.y-MPU6050_AYOFFSET)/MPU6050_AYGAIN;
	AccelVec.z = (double)(AccelVec.z-MPU6050_AZOFFSET)/MPU6050_AZGAIN;
	#else
	AccelVec.x = (double)(AccelVec.x)/MPU6050_AGAIN;
	AccelVec.y = (double)(AccelVec.y)/MPU6050_AGAIN;
	AccelVec.z = (double)(AccelVec.z)/MPU6050_AGAIN;
	#endif
	
	if(bFilter){
		KalmanPredictUpdate1D(&KalmanAccX, AccelVec.x);
		KalmanPredictUpdate1D(&KalmanAccY, AccelVec.y);
		KalmanPredictUpdate1D(&KalmanAccZ, AccelVec.z);
		AccelVec.x = KalmanAccX.X;
		AccelVec.y = KalmanAccY.X;
		AccelVec.z = KalmanAccZ.X;
		return AccelVec;
	}
	
	return AccelVec;
}

Vector	CMPU6050::getConvGyroData	()
{
	Vector GyroVec;
	
	mpu6050_readBytes(MPU6050_RA_ACCEL_XOUT_H, 14, (uint8_t *)buffer);
	GyroVec.x = (((int16_t)buffer[8]) << 8) | buffer[9];
	GyroVec.y = (((int16_t)buffer[10]) << 8) | buffer[11];
	GyroVec.z = (((int16_t)buffer[12]) << 8) | buffer[13];
	
	#if MPU6050_CALIBRATEDACCGYRO == 1
	GyroVec.x = (double)(GyroVec.x-MPU6050_GXOFFSET)/MPU6050_GXGAIN;
	GyroVec.y = (double)(GyroVec.y-MPU6050_GYOFFSET)/MPU6050_GYGAIN;
	GyroVec.z = (double)(GyroVec.z-MPU6050_GZOFFSET)/MPU6050_GZGAIN;
	#else
	GyroVec.x = (double)(GyroVec.x)/MPU6050_GGAIN;
	GyroVec.y = (double)(GyroVec.y)/MPU6050_GGAIN;
	GyroVec.z = (double)(GyroVec.z)/MPU6050_GGAIN;
	#endif
	
	return GyroVec;
}

void	CMPU6050::InitKalmanAcc(double ax, double ay, double az, double P)
{
	KalmanAccX.X = ax;
	KalmanAccX.Q = MPU6050_KALMAN_Q_ACC;
	KalmanAccX.R = MPU6050_KALMAN_R_ACC;
	KalmanAccX.P = P;
	
	KalmanAccY.X = ay;
	KalmanAccY.Q = MPU6050_KALMAN_Q_ACC;
	KalmanAccY.R = MPU6050_KALMAN_R_ACC;
	KalmanAccY.P = P;
	
	KalmanAccZ.X = az;
	KalmanAccZ.Q = MPU6050_KALMAN_Q_ACC;
	KalmanAccZ.R = MPU6050_KALMAN_R_ACC;
	KalmanAccZ.P = P;
}

void	CMPU6050::InitKalmanRot(double Roll, double Pitch, double Yaw, double P)
{
	KalmanAngX.X_0 = Roll;
	KalmanAngX.X_1 = 0;
	KalmanAngX.Q_0 = MPU6050_KALMAN_Q_ACC;
	KalmanAngX.Q_1 = MPU6050_KALMAN_Q_GYR;
	KalmanAngX.R = MPU6050_KALMAN_R_ROT;
	KalmanAngX.P_00 = P;
	KalmanAngX.P_01 = 0;
	KalmanAngX.P_10 = 0;
	KalmanAngX.P_11 = P;
	
	KalmanAngY.X_0 = Pitch;
	KalmanAngY.X_1 = 0;
	KalmanAngY.Q_0 = MPU6050_KALMAN_Q_ACC;
	KalmanAngY.Q_1 = MPU6050_KALMAN_Q_GYR;
	KalmanAngY.R = MPU6050_KALMAN_R_ROT;
	KalmanAngY.P_00 = P;
	KalmanAngY.P_01 = 0;
	KalmanAngY.P_10 = 0;
	KalmanAngY.P_11 = P;
	
	YawAngle = Yaw;
	YawHF.tau = MPU6050_LOWHIGH_TAU;
}
