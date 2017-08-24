/****************************************
 * Quick test of I2C routines
 * Using real-time clock
 * Also using the acclerometer
 ****************************************/

#include <stdio.h>
#include "bbbLib.h"
#include "rtc.h"
#include "accel.h"
#include "servo_driver.h"

// Global debug variable

int    debug = 1 ;

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Main program
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

int main(void) {
   rtc_t           time ;
   int             i2c_accel_handle ;
   int8_t          accelStatus ;
   accel_t         accel ;

// Set the time

   time.hr = 5 ;
   time.min = 20 ;
   time.sec = 0 ;

//   set_time(&time) ;

   time.hr = 0 ;
   time.min = 0 ;
   time.sec = 0 ;

// Get time

   time = get_time() ;

// Print the time

   if (debug) printf("\nCurrent time is %d:%d:%d\n", time.hr, time.min, time.sec);

//  Get a i2c handle for the accelerometer
//  Reads the WHO_AMI_I register to make sure
//  it contains a 0x2a

   i2c_accel_handle = initAccel() ;

// Read the status

   accelStatus = getAccelStatus(i2c_accel_handle) ;
   if (debug) printf("Accelerometer status: %x\n",  accelStatus) ;

// Read accelerometer data

   readAccelData(i2c_accel_handle, &accel) ;

// Print the x, y,z coordinates

   printf("status is %x\n", accel.status) ;
   printf("x is %d\n", accel.x) ;
   printf("y is %d\n", accel.y) ;
   printf("z is %d\n", accel.z) ;

// Close accelerometer handle gracefully.
 
   cleanupAccel(i2c_accel_handle) ;

   printf("Testing servo driver.\n") ;

// Reset the servo driver

   resetServoDriver() ;

// Set rep rate to 50 Hz

   setServoFREQ(50.0) ;

// Turn servo one direction and then back the other

   int i ;
   for (i=150; i<=350; i+=50) {
       setServoPW(0, i) ;
       delay_ms(900) ;
       delay_ms(900) ;
    } // end i for-loop

//
// Exit

  resetServoDriver() ;

// Say goodbye!

   printf("\nGoodbye! ...\n\n") ;

// Exit

  return(0);
}


