// Need access to std i/o routines 

#include <stdio.h>

// Need access to i2c library funcitions

#include "bbbLib.h"
#include "color_sensor.h"

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Routine to initialize the TCS3475 color sensor
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

int init_color_sensor(void){

// Set up a read buffer

   unsigned char rd_buf[BUF_SIZE] ;

// Set up a write buffer

   unsigned char wr_buf[BUF_SIZE] ;

// Get a handle for the TCS3475 color sensor
 
   int   i2c_color_handle ;
   i2c_color_handle = i2c_open(COLOR_SENSOR_I2CBUS, COLOR_SENSOR_ADDR);

// Need to enable the sensor by writing value to the ENABLE register

   wr_buf[0] = CMD_BIT | ENABLE ;

// Setting the lower to bits should enable the sensor

   wr_buf[1] = 0x03 ;
   i2c_write(i2c_color_handle, wr_buf, 2) ;

// Let's read the ID register 
// Print it to the screen when debugging
// Check it to make sure it reads 0x44

   wr_buf[0] = CMD_BIT | ID ;
   i2c_write_read(i2c_color_handle, COLOR_SENSOR_ADDR, wr_buf, 1, COLOR_SENSOR_ADDR, rd_buf, 1) ;
   if (COLOR_SENSOR_DEBUG) {
         printf("The color sensor returned the ID: %x\n", (int) rd_buf[0]) ;
   }

// Let's set the gain of the sensor

   wr_buf[0] = CMD_BIT | CONTROL ;
   wr_buf[1] = GAIN_16X ;
   i2c_write(i2c_color_handle, wr_buf, 2) ;

   if (COLOR_SENSOR_DEBUG) {
         printf("Gain setting is: %x\n", (int) wr_buf[1]) ;
   }

// Let's set the integration time of the sensor

   wr_buf[0] = CMD_BIT | ATIME ;
   wr_buf[1] = INTEG_TIME ;
   i2c_write(i2c_color_handle, wr_buf, 2) ;

   if (COLOR_SENSOR_DEBUG) {
         printf("Integration time is: %x\n", (int) wr_buf[1]) ;
   }

// Return the i2c color sensor handle

   return i2c_color_handle ;

}

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Routine to cleanup the TCS3475 color sensor
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

void  cleanup_color_sensor(int i2c_color_handle) {

//   unsigned char wr_buf[BUF_SIZE] ;

// Need to disable the sensor by writing value to the ENABLE register

// Clearing the lower 2 bits should disable the sensor

/*
   wr_buf[0] = CMD_BIT | ENABLE ;
   wr_buf[1] = 0x00 ;
   i2c_write(i2c_color_handle, wr_buf, 2) ;
*/

// Close the i2c channel

   i2c_close(i2c_color_handle) ;

   return ;
}

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Routine to read the TCS3475 color sensor
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

void read_color_sensor(int i2c_color_handle, unsigned int *c, 
                       unsigned int *r, unsigned int *g, unsigned int *b) {

// Set up a read buffer

   unsigned char rd_buf[BUF_SIZE] ;

// Set up a write buffer

   unsigned char wr_buf[BUF_SIZE] ;

// Let's get the "clear" data from the sensor 

   wr_buf[0] = CMD_BIT | CDATA ;
   i2c_write_read(i2c_color_handle, COLOR_SENSOR_ADDR, wr_buf, 1, COLOR_SENSOR_ADDR, rd_buf, 2) ;
   if (COLOR_SENSOR_DEBUG) {
       *c = (unsigned int) rd_buf[0] ;
       *c |= ((unsigned int) rd_buf[1]) << 8 ;
       printf("\nClear data value: %u => %u %u \n", *c, 
              (unsigned int) rd_buf[1],  (unsigned int) rd_buf[0]) ;
   }

// Let's get the "red" data from the sensor 

   wr_buf[0] = CMD_BIT | RDATA ;
   i2c_write_read(i2c_color_handle, COLOR_SENSOR_ADDR, wr_buf, 1, COLOR_SENSOR_ADDR, rd_buf, 2) ;
   if (COLOR_SENSOR_DEBUG) {
       *r = (unsigned int) rd_buf[0] ;
       *r |= ((unsigned int) rd_buf[1]) << 8 ;
       printf("Red data value: %u\n", *r) ;
   }

// Let's get the "green" data from the sensor 

   wr_buf[0] = CMD_BIT | GDATA ;
   i2c_write_read(i2c_color_handle, COLOR_SENSOR_ADDR, wr_buf, 1, COLOR_SENSOR_ADDR, rd_buf, 2) ;
   if (COLOR_SENSOR_DEBUG) {
       *g = (unsigned int) rd_buf[0] ;
       *g |= ((unsigned int) rd_buf[1]) << 8 ;
       printf("Green data value: %u\n", *g) ;
   }
// Let's get the "blue" data from the sensor 

   wr_buf[0] = CMD_BIT | BDATA ;
   i2c_write_read(i2c_color_handle, COLOR_SENSOR_ADDR, wr_buf, 1, COLOR_SENSOR_ADDR, rd_buf, 2) ;
   if (COLOR_SENSOR_DEBUG) {
       *b = (unsigned int) rd_buf[0] ;
       *b |= ((unsigned int) rd_buf[1]) << 8 ;
       printf("Blue data value: %u\n", *b) ;
   }

// Exit

   return ;
}


