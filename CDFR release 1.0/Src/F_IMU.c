#include "F_IMU.h"

Dof6AccGyro g_dynamicState;
int g_firstSample=1;
VectorFloat g_RwEst;

int F_IMU_Init(void){
	F_I2C1_WriteRegister(ADXL345_ADD/2,ADXL345_POWER_CTL,0x08);

	F_I2C1_WriteRegister(ITG3200_ADD/2,SMPLRT_DIV,0x07);
	F_I2C1_WriteRegister(ITG3200_ADD/2,DLPF_FS,0x1E );
	F_I2C1_WriteRegister(ITG3200_ADD/2,INT_CFG,0x00);

	F_IMU_CalcAverageOffsetAccelerometer();
	F_IMU_CalcAverageOffsetGyro();

	return 0;
}
uint8_t F_IMU_Test(void){
	uint8_t readValue;
	F_I2C1_ReadRegister(ADXL345_ADD, ADXL345_DEVID, &readValue);

	return readValue;
}
int F_IMU_GetRawData(){
	uint8_t buff[6];

	F_I2C1_ReadMultipleRegister(ADXL345_ADD, ADXL345_DATAX0, 6, buff);
	//each axis reading coms in 10 bit resolution, ie 2 bytes. Least Significant Byte first!!
	//thus we are converting both bytes in to one int
	g_dynamicState.acc_rawData.x = (((int)buff[1]) << 8) | buff[0] ;
	g_dynamicState.acc_rawData.y = (((int)buff[3]) << 8) | buff[2] ;
	g_dynamicState.acc_rawData.z = (((int)buff[5]) << 8) | buff[4] ;

	F_I2C1_ReadMultipleRegister(ITG3200_ADD, 0x1D, 6, buff);
	g_dynamicState.gyro_rawData.x = ((((int)buff[0]) << 8) | buff[1]) ;
	g_dynamicState.gyro_rawData.y  = ((((int)buff[2]) << 8) | buff[3]) ;
	g_dynamicState.gyro_rawData.z  = ((((int)buff[4]) << 8) | buff[5]) ;

	return 0;
}

int F_IMU_GetOffsetAccelerometer(void){

//	int status = I2C_STATUS_OK ;
//	uint8_t buff[3];
//
//	F_I2C1_ReadMultipleRegister(ADDR_MPU6050, ADXL345_OFSX, 3, buff);
//	//each axis reading coms in 10 bit resolution, ie 2 bytes. Least Significant Byte first!!
//	//thus we are converting both bytes in to one int
//	ptrAccel->offset->x = (int)buff[0] ;
//	ptrAccel->offset->y = (int) buff[1] ;
//	ptrAccel->offset->z = (int) buff[2] ;

	return 0;
}
int F_IMU_CalcAverageOffsetAccelerometer(void){
	int status = I2C_STATUS_OK;
	int X=0,Y=0,Z=0;
	uint8_t buff[6];
	// average loop
	for (int i=0;i<128;i++){
		// retrieve raw data
		F_I2C1_ReadMultipleRegister(ADXL345_ADD, ADXL345_DATAX0, 6, buff);
		X += (((int)buff[1]) << 8) | buff[0] ;
		Y += (((int)buff[3]) << 8) | buff[2] ;
		Z += ( (((int)buff[5]) << 8) | buff[4] ) - 256 ;// -256 is because we have +1g on Z axe
	}
	g_dynamicState.acc_offset.x = X>>7; // >>7 equivalent /128
	g_dynamicState.acc_offset.y = Y>>7;
	g_dynamicState.acc_offset.z = Z>>7;

	return status;
}
int F_IMU_GetAccelerometerData(void){
	int status = I2C_STATUS_OK;
	uint8_t buff[6];

	status = F_I2C1_ReadMultipleRegister(ADXL345_ADD, ADXL345_DATAX0, 6, buff);
	//each axis reading coms in 10 bit resolution, ie 2 bytes. Least Significant Byte first!!
	//thus we are converting both bytes in to one int
	g_dynamicState.acc_data.x = (( ((int)buff[1]) << 8) | buff[0] ) - g_dynamicState.acc_offset.x ;
	g_dynamicState.acc_data.y = (( ((int)buff[3]) << 8) | buff[2] ) - g_dynamicState.acc_offset.y ;
	g_dynamicState.acc_data.z = (( ((int)buff[5]) << 8) | buff[4] ) - g_dynamicState.acc_offset.z ;

	return status;
}

int F_IMU_GetTemp(void){
	uint8_t buff[2];
	F_I2C1_ReadMultipleRegister(ITG3200_ADD, TEMP_OUT, 2, buff);

	int temp = buff[0] | ( (buff[1]<<8) & 0xFF00 );
	temp = (temp/280)-150 ;

	return temp;
}

int F_IMU_CalcAverageOffsetGyro(void){

	int status = I2C_STATUS_KO;
	long int X=0,Y=0,Z=0,i;
	uint8_t buff[6];
	// average loop
	for (i=0;i < 128;i++){
		// retrieve raw data
		status &= F_I2C1_ReadMultipleRegister(ITG3200_ADD, GYRO_XOUT, 6, buff);
		X += (((int)buff[0]) << 8) | buff[1] ;
		Y += (((int)buff[2]) << 8) | buff[3] ;
		Z += (((int)buff[4]) << 8) | buff[5] ;
	}
	g_dynamicState.gyro_offset.x = X/128;
	g_dynamicState.gyro_offset.y = Y/128;
	g_dynamicState.gyro_offset.z = Z/128;

	return status;
}

// convert raw readings to degrees/sec
void F_IMU_RawGyroToDegsec(void) {
  g_dynamicState.gyro_data.x = ((float) g_dynamicState.gyro_rawData.x) / 14.375;
  g_dynamicState.gyro_data.y = ((float) g_dynamicState.gyro_rawData.y) / 14.375;
  g_dynamicState.gyro_data.z = ((float) g_dynamicState.gyro_rawData.z) / 14.375;
}

void F_IMU_Normalize3DVec(VectorFloat vect_f) {
  float R;
  R = sqrt(vect_f.x*vect_f.x + vect_f.y*vect_f.y + vect_f.z*vect_f.z);
  vect_f.x /= R;
  vect_f.y /= R;
  vect_f.z /= R;
}
void F_IMU_RawAccToG(void){
  g_dynamicState.acc_data.x = ((float) g_dynamicState.acc_rawData.x) / 256.0;
  g_dynamicState.acc_data.y = ((float) g_dynamicState.acc_rawData.y) / 256.0;
  g_dynamicState.acc_data.z = ((float) g_dynamicState.acc_rawData.z) / 256.0;
}
/**************************** Calcul des angles avec un filtre de Kalman simplifié ********************
 *
 **************************** ATTENTION CETTE FONCTION NE FONCTIONNNE PAS *****************************
 *
 */
void F_IMU_GetInclinations(void) {
//  int w = 0;
//  float tmpf = 0.0;
//  int signRzGyro;
//  float wGyro=0.2;
//  int interval = 20;
//
//  F_IMU_GetRawData();			// Reccuperation des valeurs de l'Accelero et du gyro
//  F_IMU_RawAccToG();			// Conversion
//  F_IMU_Normalize3DVec(g_dynamicState.acc_data);			// Traitement
//  F_IMU_RawGyroToDegsec();		// Conversion
//
//
//  if (g_firstSample==1) { // the NaN check is used to wait for good data from the Arduino
//	g_RwEst.x=g_dynamicState.acc_rawData.x;
//	g_RwEst.y=g_dynamicState.acc_rawData.y;
//	g_RwEst.z=g_dynamicState.acc_rawData.z;
//  }
//  else{
//    //evaluate RwGyro vector
//    if(fabs(g_RwEst.z) < 0.1) {
//      //Rz is too small and because it is used as reference for computing Axz, Ayz it's error fluctuations will amplify leading to bad results
//      //in this case skip the gyro data and just use previous estimate
//      g_dynamicState.gyro_data.x=g_RwEst.x;
//      g_dynamicState.gyro_data.y=g_RwEst.y;
//      g_dynamicState.gyro_data.z=g_RwEst.z;
//    }
//    else {
//    	g_dynamicState.gyro_rawData.y *= -1; //Attention platine Sparkfun inversion rotation X et Y + signe sur un axe
//									 //get angles between projection of R on ZX/ZY plane and Z axis, based on last RwEst
//		tmpf = g_dynamicState.gyro_rawData.y;
//		tmpf *= (interval / 1000.0f);
//		g_estimate.pitch = atan2(g_RwEst.x,g_RwEst.z) * COEF_180_PI;
//		g_estimate.pitch += tmpf;
//
//		tmpf = g_dynamicState.gyro_rawData.x;
//		tmpf *= (interval / 1000.0f);
//		g_estimate.roll = atan2(g_RwEst.y,g_RwEst.z) * COEF_180_PI;
//		g_estimate.roll += tmpf;
//		//estimate sign of RzGyro by looking in what qudrant the angle Axz is,
//		//RzGyro is pozitive if  Axz in range -90 ..90 => cos(Awz) >= 0
//		signRzGyro = ( cos(g_estimate.pitch * COEF_180_PI) >=0 ) ? 1 : -1;
//
//		//reverse calculation of RwGyro from Awz angles, for formulas deductions see  http://starlino.com/imu_guide.html
//
//		g_dynamicState.gyro_rawData.x = sinf(g_estimate.pitch * COEF_PI_180);
//		g_dynamicState.gyro_rawData.x /= sqrtf( 1 + F_Math_Squared(cosf(g_estimate.pitch * COEF_PI_180)) *
//				F_Math_Squared(tanf(g_estimate.roll * COEF_PI_180)) );
//		g_dynamicState.gyro_rawData.y = sinf(g_estimate.roll * COEF_PI_180);
//		g_dynamicState.gyro_rawData.y /= sqrtf( 1 + F_Math_Squared(cosf(g_estimate.roll * COEF_PI_180)) *
//				F_Math_Squared(tanf(g_estimate.pitch * COEF_PI_180)) );
//		g_dynamicState.gyro_rawData.z = signRzGyro * sqrtf(1 - F_Math_Squared(g_dynamicState.gyro_rawData.x) - F_Math_Squared(g_dynamicState.gyro_rawData.y));
//    }
//
//	//combine Accelerometer and gyro readings
//	g_RwEst.x = g_dynamicState.acc_rawData.x + wGyro * g_dynamicState.gyro_rawData.x / (1 + wGyro);
//	g_RwEst.y = g_dynamicState.acc_rawData.y + wGyro * g_dynamicState.gyro_rawData.y / (1 + wGyro);
//	g_RwEst.z = g_dynamicState.acc_rawData.z + wGyro * g_dynamicState.gyro_rawData.z / (1 + wGyro);
//
//	F_IMU_Normalize3DVec(g_RwEst);
//  }
//
//  g_firstSample = 1;
}
