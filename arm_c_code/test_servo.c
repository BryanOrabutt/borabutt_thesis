/****************************************
 * Quick test of I2C routines
 * Using real-time clock
 ****************************************/

// Need access to std i/o routines 

#include <stdio.h>

// Need access to routine for SRF02 sonar module

#include "servo_driver.h"

// Simple main program which gets a reading from the SRF02

int main(void)
{
   unsigned int    range ;
   int i ;

// Reset the servo driver

   resetServoDriver() ;

// Set rep rate to 50 Hz

   setServoFREQ(50.0) ;

// Turn servo one direction and then back the other

   for (i=150; i<=350; i+=50) {
       setServoPW(0, i) ;
       delay_ms(900) ;
       delay_ms(900) ;
    }

//
// Exit

  resetServoDriver() ;
  return(0);
}


