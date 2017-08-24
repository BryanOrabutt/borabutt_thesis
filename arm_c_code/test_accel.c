//
// This is a program to test out a series of submodules
// that might be useful for robotics
//

#define	  ON       1
#define   OFF      0

// We need stdio support


#include  <stdio.h>

// And our robotic library

// #include "robotLib.h"

// And the BBB library

#include "bbbLib.h"

// And the accelerometer include file

#include "accelerometer.h"


int main(void) {

   char  str[80] ;

// Initialize the accelerometer

    int   i2c_color_handle ; 
    i2c_color_handle = init_color_sensor() ;

// Read the color sensor

   unsigned int  c, r, g, b ;

   int   do_it = 1 ;
   while (do_it) {
       printf("\nRead the color sensor? (y/n)  ") ;      
       if (fgets(str, 80, stdin) > 0) {
           if (str[0] == 'y') {
               read_color_sensor(i2c_color_handle, &c, &r, &g, &b) ;
           } 
           else {
                do_it = 0 ;
           } // end if-then-else
       } // end if
   } // end while
    
// Closing up the color sensor

    cleanup_color_sensor(i2c_color_handle) ; 

// Print a closing message

    if (debugPrint) printf("Robot test complete ... exiting!\n") ;
    return ;
}
