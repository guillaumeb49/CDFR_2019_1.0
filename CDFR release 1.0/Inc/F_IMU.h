#ifndef F_IMU_H_
#define F_IMU_H_

#include "main.h"
#include "i2c.h"

/*################### Adresses I2C ###################*/

#define ADXL345_ADD	0xA6
#define ITG3200_ADD 0xD0

#define ADXL345_ID 	0xE5

/*################### Registres ###################*/

#define ADXL345_DEVID 			0x00
#define ADXL345_RESERVED1 		0x01
#define ADXL345_THRESH_TAP 		0x1d
#define ADXL345_OFSX 			0x1e
#define ADXL345_OFSY 			0x1f
#define ADXL345_OFSZ 			0x20
#define ADXL345_DUR 			0x21
#define ADXL345_LATENT 			0x22
#define ADXL345_WINDOW 			0x23
#define ADXL345_THRESH_ACT 		0x24
#define ADXL345_THRESH_INACT 	0x25
#define ADXL345_TIME_INACT 		0x26
#define ADXL345_ACT_INACT_CTL 	0x27
#define ADXL345_THRESH_FF 		0x28
#define ADXL345_TIME_FF 		0x29
#define ADXL345_TAP_AXES 		0x2A
#define ADXL345_ACT_TAP_STATUS 	0x2B
#define ADXL345_BW_RATE 		0x2c
#define ADXL345_POWER_CTL 		0x2d
#define ADXL345_INT_ENABLE 		0x2e
#define ADXL345_INT_MAP 		0x2f
#define ADXL345_INT_SOURCE 		0x30
#define ADXL345_DATA_FORMAT 	0x31
#define ADXL345_DATAX0 			0x32
#define ADXL345_DATAX1 			0x33
#define ADXL345_DATAY0 			0x34
#define ADXL345_DATAY1 			0x35
#define ADXL345_DATAZ0 			0x36
#define ADXL345_DATAZ1 			0x37
#define ADXL345_FIFO_CTL 		0x38
#define ADXL345_FIFO_STATUS 	0x39

#define ADXL345_BW_1600 0xF // 1111
#define ADXL345_BW_800  0xE // 1110
#define ADXL345_BW_400  0xD // 1101
#define ADXL345_BW_200  0xC // 1100
#define ADXL345_BW_100  0xB // 1011
#define ADXL345_BW_50   0xA // 1010
#define ADXL345_BW_25   0x9 // 1001
#define ADXL345_BW_12   0x8 // 1000
#define ADXL345_BW_6    0x7 // 0111
#define ADXL345_BW_3    0x6 // 0110

#define ITG3200_ADDR_AD0_HIGH  0x69   //AD0=1 0x69 I2C address when AD0 is connected to HIGH (VCC) - default for sparkfun breakout
#define ITG3200_ADDR_AD0_LOW   0x68   //AD0=0 0x68 I2C address when AD0 is connected to LOW (GND)
// "The LSB bit of the 7 bit address is determined by the logic level on pin 9.
// This allows two ITG-3200 devices to be connected to the same I2C bus.
// One device should have pin9 (or bit0) LOW and the other should be HIGH." source: ITG3200 datasheet
// Note that pin9 (AD0 - I2C Slave Address LSB) may not be available on some breakout boards so check
// the schematics of your breakout board for the correct address to use.

#define GYROSTART_UP_DELAY  70    // 50ms from gyro startup + 20ms register r/w startup

/* ---- Registers ---- */
#define WHO_AM_I           0x00  // RW   SETUP: I2C address
#define SMPLRT_DIV         0x15  // RW   SETUP: Sample Rate Divider
#define DLPF_FS            0x16  // RW   SETUP: Digital Low Pass Filter/ Full Scale range
#define INT_CFG            0x17  // RW   Interrupt: Configuration
#define INT_STATUS         0x1A  // R	Interrupt: Status
#define TEMP_OUT           0x1B  // R	SENSOR: Temperature 2bytes
#define GYRO_XOUT          0x1D  // R	SENSOR: Gyro X 2bytes
#define GYRO_YOUT          0x1F  // R	SENSOR: Gyro Y 2bytes
#define GYRO_ZOUT          0x21  // R	SENSOR: Gyro Z 2bytes
#define PWR_MGM            0x3E  // RW	Power Management

/* ---- bit maps ---- */
#define DLPFFS_FS_SEL             0x18  // 00011000
#define DLPFFS_DLPF_CFG           0x07  // 00000111
#define INTCFG_ACTL               0x80  // 10000000
#define INTCFG_OPEN               0x40  // 01000000
#define INTCFG_LATCH_INT_EN       0x20  // 00100000
#define INTCFG_INT_ANYRD_2CLEAR   0x10  // 00010000
#define INTCFG_ITG_RDY_EN         0x04  // 00000100
#define INTCFG_RAW_RDY_EN         0x01  // 00000001
#define INTSTATUS_ITG_RDY         0x04  // 00000100
#define INTSTATUS_RAW_DATA_RDY    0x01  // 00000001
#define PWRMGM_HRESET             0x80  // 10000000
#define PWRMGM_SLEEP              0x40  // 01000000
#define PWRMGM_STBY_XG            0x20  // 00100000
#define PWRMGM_STBY_YG            0x10  // 00010000
#define PWRMGM_STBY_ZG            0x08  // 00001000
#define PWRMGM_CLK_SEL            0x07  // 00000111

/************************************/
/*    REGISTERS PARAMETERS    */
/************************************/
// Sample Rate Divider
#define NOSRDIVIDER         0 // default    FsampleHz=SampleRateHz/(divider+1)
// Gyro Full Scale Range
#define RANGE2000           3   // default
// Digital Low Pass Filter BandWidth and SampleRate
#define BW256_SR8           0   // default    256Khz BW and 8Khz SR
#define BW188_SR1           1
#define BW098_SR1           2
#define BW042_SR1           3
#define BW020_SR1           4
#define BW010_SR1           5
#define BW005_SR1           6
// Interrupt Active logic lvl
#define ACTIVE_ONHIGH       0 // default
#define ACTIVE_ONLOW        1
// Interrupt drive type
#define PUSH_PULL           0 // default
#define OPEN_DRAIN          1
// Interrupt Latch mode
#define PULSE_50US          0 // default
#define UNTIL_INT_CLEARED   1
// Interrupt Latch clear method
#define READ_STATUSREG      0 // default
#define READ_ANYREG         1
// Power management
#define NORMAL              0 // default
#define STANDBY             1
// Clock Source - user parameters
#define INTERNALOSC         0   // default
#define PLL_XGYRO_REF       1
#define PLL_YGYRO_REF       2
#define PLL_ZGYRO_REF       3
#define PLL_EXTERNAL32      4   // 32.768 kHz
#define PLL_EXTERNAL19      5   // 19.2 Mhz

/*------------------- Structures de données -------------------*/

struct vector{
   int x;
   int y;
   int z;
};
typedef struct vector Vector;

struct vectorFloat{
   float x;
   float y;
   float z;
};
typedef struct vectorFloat VectorFloat;

struct dof6AccGyro{
	Vector acc_rawData;
	VectorFloat acc_data;
	Vector acc_offset;
	Vector gyro_rawData;
	VectorFloat gyro_data;
	Vector gyro_offset;
};
typedef struct dof6AccGyro Dof6AccGyro;

/*################### Fonctions ###################*/

int F_IMU_Init(void);
uint8_t F_IMU_Test(void);
int F_IMU_GetRawData(void);

int F_IMU_GetOffsetAccelerometer(void);
int F_IMU_CalcAverageOffsetAccelerometer(void);
int F_IMU_GetAccelerometerData(void);
int F_IMU_CalcAverageOffsetGyro(void);
int F_IMU_GetTemp(void);

void F_IMU_RawGyroToDegsec(void);
void F_IMU_Normalize3DVec(VectorFloat vect_f);
void F_IMU_RawAccToG(void);
void F_IMU_GetInclinations(void);
#endif /*__ IMU_H */
