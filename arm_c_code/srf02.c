// Need access to std i/o routines 

#include <stdio.h>

// Need access to i2c library funcitions

#include "bbbLib.h"

// Need access to routine for SRF02 sonar module

#include "srf02.h"

// Routine to get time from real time clock

unsigned int get_srf02_range(void) {

   int 	   		 		i2c_srf02_handle ;
   unsigned short int   MSbyte, LSbyte ;
   unsigned int   	 	range ;

// Set up a read buffer

   unsigned char rd_buf[BUF_SIZE] ;

// Set up a write buffer

   unsigned char wr_buf[BUF_SIZE] ;

// Get a handle for the I2C SRF02 sonar sensor

   i2c_srf02_handle = i2c_open(I2CBUS, ADDR);

// We need to send the acquire command
// $51 will get us a range in cm
// The command needs to be registered to register 0x00

   wr_buf[0] = 0x00 ;
   wr_buf[1] = 0x51 ;

// We will write a register location and command (2 bytes) to the SRF02 

   i2c_write(i2c_srf02_handle, wr_buf, 2) ;

// Need to wait 70 ms (maximum time for the echo to return)

   delay_ms(70) ;

// Now read the range (2 bytes)
// Write the register location we want first
// We need to read from registers $02 and $03

   wr_buf[0] = 0x02 ;
   i2c_write_read(i2c_srf02_handle, ADDR, wr_buf, 1, ADDR, rd_buf, 2) ;

// Convert the 2 bytes to range

   LSbyte = (unsigned int) (rd_buf[1]) ;
   MSbyte = (unsigned int) (rd_buf[0]) ;
   range = LSbyte + (MSbyte << 8) ;

// Close the i2c channel

   i2c_close(i2c_srf02_handle) ;

// Exit

   return(range);
}


