#include <stdio.h>
#include "bbbLib.h"
#include "rtc.h"

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Routine to get time from real time clock
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

rtc_t  get_time(void) {

  int 	   		        i2c_rtc_handle ;
  unsigned short int    lower_nybble, upper_nybble ;
  rtc_t    		        time ;

// Set up a read buffer

   unsigned char rd_buf[RTC_BUF_SIZE] ;

// Set up a write buffer

   unsigned char wr_buf[RTC_BUF_SIZE] ;

// Get a handle for the I2C RTC

   i2c_rtc_handle = i2c_open(RTC_I2C_BUS, RTC_I2C_ADDR);

// We need to point to the first register in I2C RTC
// that we want to read from

   wr_buf[0] = 0x00 ;

// We will write the pointer (1 byte) to the RTC and then read 3 bytes
// from register location 00, 01, 02 (sec, min, hours)

   i2c_write_read(i2c_rtc_handle, RTC_I2C_ADDR, wr_buf, 1, RTC_I2C_ADDR, rd_buf, 3) ;

// Convert the 3 bytes from RTC to hr, min, and sec

   lower_nybble = (unsigned short int) (rd_buf[0] & 0x0f) ;
   upper_nybble = (unsigned short int) (rd_buf[0] & 0x70) ;
   upper_nybble = (upper_nybble >> 4) ;
   time.sec = lower_nybble + (10 * upper_nybble) ;

   lower_nybble = (unsigned short int) (rd_buf[1] & 0x0f) ;
   upper_nybble = (unsigned short int) (rd_buf[1] & 0x70) ;
   upper_nybble = (upper_nybble >> 4) ;
   time.min = lower_nybble + (10 * upper_nybble) ;

   lower_nybble = (unsigned short int) (rd_buf[2] & 0x0f) ;
   upper_nybble = (unsigned short int) (rd_buf[2] & 0x30) ;
   upper_nybble = (upper_nybble >> 4) ;
   time.hr = lower_nybble + (10 * upper_nybble) ;

// Close the i2c channel

   i2c_close(i2c_rtc_handle) ;

// Exit

  return(time);
}

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Routine to set time on real time clock
/// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

void  set_time(rtc_t * time) {
  int 	   		        i2c_rtc_handle ;
  int                   ones, tens, byte ;

// Set up a write buffer

   unsigned char wr_buf[RTC_BUF_SIZE] ;

// Get a handle for the I2C RTC

   i2c_rtc_handle = i2c_open(RTC_I2C_BUS, RTC_I2C_ADDR);

// We need to point to the first register in I2C RTC
// that we want to read from

   wr_buf[0] = BASE_ADDR ;

// Seconds

   tens = time->sec / 10 ;
   ones = time->sec - (10 * tens) ;
   byte =  (tens << 4) | ones ;
   wr_buf[1] = (unsigned char) byte ;

// Minutes

   tens = time->min / 10 ;
   ones = time->min - (10 * tens) ;
   byte =  (tens << 4) | ones ;
   wr_buf[2] = (unsigned char) byte ;

// Hours

   byte = time->hr ;
   wr_buf[3] = (unsigned char) byte ;

   i2c_write(i2c_rtc_handle, wr_buf, 4) ;

// Close the i2c channel

   i2c_close(i2c_rtc_handle) ;

// Exit

  return ;
}

