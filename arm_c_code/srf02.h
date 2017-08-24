// We will use I2C2 which is called 1 here (silly)
// SCL on P9_19 (3.3 V tolerant)  I2C-2 (SCL)
// SDA on P9_20 (3.3 V tolerant)  I2C-2 (SDA)

// i2c_scan 1 to test to see if device is there

#define 	I2CBUS 		1

// Base address for the SRF02 sonar module

#define 	ADDR 		0x70

// Write buffer size

#define		BUF_SIZE    	12

// Function declaration
// Gets range from the srf02

unsigned int get_srf02_range(void) ;
