// We will use I2C2 which is called 1 here (silly)
// SCL on P9_19 (3.3 V tolerant)  I2C-2 (SCL)
// SDA on P9_20 (3.3 V tolerant)  I2C-2 (SDA)

// When 1 prints some info for debugging

#define     COLOR_SENSOR_DEBUG          1
// #define     COLOR_SENSOR_DEBUG          0

// i2c_scan 1 to test to see if device is there

#define 	COLOR_SENSOR_I2CBUS 		1

// Base address for the TCS34725 Color Sensor

#define 	COLOR_SENSOR_ADDR 		    0x29

// Write buffer size

#define		BUF_SIZE    	80

// Command bit

#define     CMD_BIT         0x80

// Color sensor registers

#define     ENABLE          0x00
#define     ATIME           0x01
#define     WTIME           0x03
#define     AILTL           0x04
#define     AILTH           0x05
#define     AIHTL           0x06
#define     AIHTH           0x07
#define     PERS            0x0c
#define     CONFIG          0x0d
#define     CONTROL         0x0f
#define     ID              0x12
#define     STATUS          0x13
#define     CDATA           0x14
#define     CDATAH          0x15
#define     RDATA           0x16
#define     RDATAH          0x17
#define     GDATA           0x18
#define     GDATAH          0x19
#define     BDATA           0x1a
#define     BDATAH          0x1b

// Gain settings

#define     GAIN_1X         0x00
#define     GAIN_4X         0x01
#define     GAIN_16X        0x02
#define     GAIN_60X        0x03

// Integration time (154 ms)

#define    INTEG_TIME       0xc0   

// Function declaration
// Dumps raw data from the sensor 

int    init_color_sensor(void) ;
void   cleanup_color_sensor(int i2c_color_handle) ;
void   read_color_sensor(int i2c_color_handle, unsigned *c, 
                        unsigned int *r, unsigned int *g, unsigned int *b) ;


