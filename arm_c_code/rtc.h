// we will use I2C2 which is enumerated as 1 on the BBB
// I2CBUS 1
// SCL on P9_17 (3.3 V tolerant)
// SDA on P9_18 (3.3 V tolerant)

#define 	RTC_I2C_BUS 		1

// Base address for the I2C RTC

#define 	RTC_I2C_ADDR 		0x68

// Write buffer size

#define		RTC_BUF_SIZE    	12

// Base address

#define    BASE_ADDR            0x00

// CLOCK structue consists of a sec, min, and hour field

typedef struct  {
  unsigned short int sec ;
  unsigned short int min ;
  unsigned short int hr ;
} rtc_t ;

// Function declaration

rtc_t   get_time(void) ;
void    set_time(rtc_t * time) ;
