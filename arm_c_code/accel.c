// Need access to std i/o routines 

#include <stdio.h>

// Need access to i2c library funcitions

#include "bbbLib.h"
#include "accel.h"

extern  int   debug ;

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Routine to initialize the accelerometer
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

int initAccel(void) {

// Set up a read buffer

   unsigned char rd_buf[ACCEL_BUF_SIZE] ;

// Set up a write buffer

   unsigned char wr_buf[ACCEL_BUF_SIZE] ;

// Get a handle for the accelerometer
 
   int  i2c_accel_handle ;
   i2c_accel_handle = i2c_open(ACCEL_I2C_BUS, ACCEL_I2C_ADDR) ;

// Let's read the ID register 
// Print it to the screen 
// Check it to make sure it reads 0x2a

   wr_buf[0] = CMD_BIT | WHO_AM_I ;
   i2c_write_read(i2c_accel_handle, ACCEL_I2C_ADDR, wr_buf, 1, ACCEL_I2C_ADDR, rd_buf, 1) ;
   if (debug) printf("The accelerometer returned the ID: %x\n", (int8_t) rd_buf[0]) ;

// Take accelerometer out of standby mode and into wake mode

   wr_buf[0] = CMD_BIT | CTRL_REG1 ;
   wr_buf[1] = ACTIVE ;
   i2c_write(i2c_accel_handle, wr_buf, 2) ;

// Return the acceleromter handle

   return i2c_accel_handle ;

} // end initAccel() ;

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Routine to cleanup the accelerometer
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

void  cleanupAccel(int i2c_accel_handle) {

// Set up a write buffer

   unsigned char wr_buf[ACCEL_BUF_SIZE] ;

   if (debug) printf("Cleaning up after shutting down accelerometer\n") ;

// Take accelerometer out of wake mode and putinto standby mode

   wr_buf[0] = CMD_BIT | CTRL_REG1 ;
   wr_buf[1] = STANDBY ;

// Close the i2c handle 

   i2c_write(i2c_accel_handle, wr_buf, 2) ;
   i2c_close(i2c_accel_handle) ;

   return ;
} // end cleanupAccel()

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Routine to get accelerometer status
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

int8_t  getAccelStatus(int i2c_accel_handle) {

   int      status ;

// Set up a read buffer

   unsigned char rd_buf[ACCEL_BUF_SIZE] ;

// Set up a write buffer

   unsigned char wr_buf[ACCEL_BUF_SIZE] ;

// Selecting the status register to read from

   wr_buf[0] = CMD_BIT | STATUS ;
   i2c_write_read(i2c_accel_handle, ACCEL_I2C_ADDR, wr_buf, 1, ACCEL_I2C_ADDR, rd_buf, 1) ;

   status = (int8_t) rd_buf[0] ;

   return status ;

} // end getAccelStatus() 


// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Routine to read values from accelerometer
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

void readAccelData(int i2c_accel_handle, accel_t * accel ) {

// Set up a read buffer

   unsigned char rd_buf[ACCEL_BUF_SIZE] ;

// Set up a write buffer

   unsigned char wr_buf[ACCEL_BUF_SIZE] ;

// Get x, y, z data (read 7 bytes -> 6 for x, y, z and 1 for status)

   wr_buf[0] = CMD_BIT | STATUS ;
   i2c_write_read(i2c_accel_handle, ACCEL_I2C_ADDR, wr_buf, 1, ACCEL_I2C_ADDR, rd_buf, 7) ;
   
   accel->status = (int32_t) rd_buf[0] ;

   accel->x = ((int32_t) rd_buf[1]) << 4 ;
   accel->x |= ((int32_t) rd_buf[2]) >> 4 ;
   if (accel->x > 2047) accel->x += -4096 ;

   accel->y = ((int32_t) rd_buf[3]) << 4 ;
   accel->y |= ((int32_t) rd_buf[4]) >> 4 ;
   if (accel->y > 2047) accel->y += -4096 ;

   accel->z = ((int32_t) rd_buf[5]) << 4 ;
   accel->z |= ((int32_t) rd_buf[6] ) >> 4;
   if (accel->z > 2047) accel->z += -4096 ;

   return ;
} // end readAccelData()


