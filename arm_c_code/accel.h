//
// Accelerometer
//

#define 	ACCEL_I2C_BUS 		 1

// Base address for the I2C RTC

#define     ACCEL_I2C_ADDR       0x1d

// Write buffer size

#define		ACCEL_BUF_SIZE      80

// Command bit

#define     CMD_BIT             0x80

// Accelerometer registers

#define     STATUS             0x00

#define     OUT_X_MSB          0x01
#define     OUT_X_LSB          0x02
#define     OUT_Y_MSB          0x03
#define     OUT_Y_LSB          0x04
#define     OUT_Z_MSB          0x05
#define     OUT_Z_LSB          0x06

#define     SYSMOD             0x0b
#define     INT_SOURCE         0x0c

#define     WHO_AM_I           0x0d
#define     XYZ_DATA_CFG       0x0e
#define     HP_FILTER_CUTOFF   0x0f

#define     CTRL_REG1         0x2a
#define     CTRL_REG2         0x2b
#define     CTRL_REG3         0x2c
#define     CTRL_REG4         0x2d
#define     CTRL_REG5         0x2e

// Modes

#define     STANDBY      0x00
#define     ACTIVE       0x01

// A structure acceleration i.e. a x,y,z triplet

typedef struct  {
  int32_t  status ;
  int32_t  x ;
  int32_t  y ;
  int32_t  z ;
} accel_t ;

// Function declaration

int     initAccel(void) ;
void    cleanupAccel(int i2c_accel_handle) ;
int8_t  getAccelStatus(int i2c_accel_handle) ;
void    readAccelData(int i2c_accel_handle, accel_t  * accel) ;

