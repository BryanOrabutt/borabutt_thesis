// Need access to std i/o routines 

#include <stdio.h>

// Need access to i2c library funcitions

#include "bbbLib.h"

// Need access to servo_driver.h

#include "servo_driver.h"

// Routine to set the PWM freqeuncy
// Routine accepts a freqeuncy between 40 Hz and 1000 Hz
// and sets the pre-scaler value in the PCA9685 servo controller

#define   DEBUG    0

unsigned int resetServoDriver(void) {

   int 	   		 i2c_handle ;

// Set up a write buffer

   unsigned char wr_buf[BUF_SIZE] ;

// Get a handle for the I2C servo controller

   i2c_handle = i2c_open(I2C_BUS, I2C_ADDR) ;

// Write to MODE1 register

   wr_buf[0] = PCA9685_MODE1 ;
   wr_buf[1] = 0x00 ;
   i2c_write(i2c_handle, wr_buf, 2) ;

// Close the I2C channel and return

   i2c_close(i2c_handle) ;

   return(TRUE);
}

// ******************************************************************************

unsigned int setServoFREQ(float freq) {

   int 	   		         i2c_handle ;
   float                 tmp ;
   unsigned char         pre_scale_value ;
   unsigned char         old_mode ;
   unsigned char         new_mode ;

// Set up a write buffer

   unsigned char wr_buf[BUF_SIZE] ;

// Set up a read buffer

   unsigned char rd_buf[BUF_SIZE] ;

// Make sure frequency is in the allowed range (40 Hz to 1 kHz)
// If not, then return an error

   if ((freq < 40.0) || (freq > 1000)) return(FALSE) ;
 
// Convert freqeuncy to a pre-scaler value

   tmp = (25.0e6 / (4096 * freq)) + 0.5 ;
   pre_scale_value = ((unsigned char) tmp)  - 1 ;
   if (DEBUG) printf("Pre-scaler value: %d\n", (int) pre_scale_value) ;

// Get a handle for the I2C servo controller

   i2c_handle = i2c_open(I2C_BUS, I2C_ADDR);

// Need to read current mode

   wr_buf[0] = PCA9685_MODE1 ;
   i2c_write_read(i2c_handle, I2C_ADDR, wr_buf, 1, I2C_ADDR, rd_buf, 1) ;
   old_mode = rd_buf[0] ;
   new_mode = (old_mode & 0x7f) | 0x10 ; // sleep mode

// Write out new mode and put PCA9685 to sleep

   wr_buf[1] = new_mode ;
   i2c_write(i2c_handle, wr_buf, 2) ;

//  Write out the prescaler value

   wr_buf[0] = PCA9685_PRESCALE ;
   wr_buf[1] = pre_scale_value ;
   i2c_write(i2c_handle, wr_buf, 2) ;

// Restore old mode setting

   wr_buf[0] = PCA9685_MODE1 ;
   wr_buf[1] = old_mode ;
   i2c_write(i2c_handle, wr_buf, 2) ;

// Wait 5 ms 

   delay_ms(5) ;

// Turn on auto increment

   wr_buf[1] = old_mode | 0xa1 ;
   i2c_write(i2c_handle, wr_buf, 2) ;

// Close the I2C channel and return

   i2c_close(i2c_handle) ;

   return(TRUE);
}

// ******************************************************************
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// ******************************************************************

// Routine to set the pulse width in ticks
// 0.5 ms to 2ms is 100 to 400

unsigned int setServoPW(int chan, int pw) {

   int 	           i2c_handle ;
   unsigned char   ms_byte, ls_byte ;
   unsigned char   cr_addr ;

// Set up a write buffer

   unsigned char wr_buf[BUF_SIZE] ;

// Make sure we have a valid channel number

   if ((chan < 0) || (chan > 15)) return(FALSE) ;

   if (DEBUG) printf("Channel #:  %d\n", chan) ;

// Get a handle for the I2C servo controller

   i2c_handle = i2c_open(I2C_BUS, I2C_ADDR);

// Control register base address for a particular servo
// can be computed by taking 4 * channel number + 6

   cr_addr = (unsigned char) (4 * chan + SERVO_0_ON_L);
   if (DEBUG) printf("Control register address is %d\n", (int) cr_addr) ;

// Need to break the pulse with up into two bytes

   ms_byte = (unsigned char) (pw / 256) ;
   ls_byte = (unsigned char) (pw % 256 );
   if (DEBUG) printf("ms_byte = %d, ls_byte = %d\n", (int) ms_byte, (int) ls_byte) ;

// Base address for control register

   wr_buf[0] = cr_addr ;

// Pulse turned on when counter equals $000

   wr_buf[1] = 0x00 ;
   wr_buf[2] = 0x00 ;

// Pulse turned off when counter equals ...

   wr_buf[3] = ls_byte ;
   wr_buf[4] = ms_byte ;

   i2c_write(i2c_handle, wr_buf, 5) ;

// Close the i2c channel and exit

   i2c_close(i2c_handle) ;

   return(TRUE);
}
