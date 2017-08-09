//
// Main program for demonstration robot (beaglebot.c)
//

#include   <stdio.h>
#include   <stdlib.h>
#include   <assert.h>
#include   <stdint.h>
#include   <math.h>
#include   "prussdrv.h"
#include   "pruss_intc_mapping.h"
#include   "mio.h"
#include   "child.h"
#include   "bbbLib.h"
#include   "mem.h"
#include   "PRUlib.h"
#include   "robotLib.h"

// TRUE and FALSE

#define   TRUE   1
#define   FALSE  0

// GUI mode or not

int         GUImode = TRUE ;

// We need a global variable that wil point to the shared memory

shared_memory_t  *shared_memory ;

// Need a variable for debugging

int		    debug = TRUE ;

// Need global structure to hold GUI variables

GUIvars_t   GUIvars ;

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Routine to execute PRU programs 
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

void execPRUprograms() {

// Initialize the PRUs

   if (debug) printf("Initializing the PRUs.\n") ;
   PRUinit() ;

// Configure PRU 0 based on GUI settings

   if (debug) printf("Configuring PRU 0 with GUI data\n") ;
   configPRU() ;

// Start the PRUs

   if (debug) printf("Start the PRUs ...\n") ;
   PRUstart() ;

   return ;
}

// **********************************
// Main program
// **********************************
int main (void) {  

   FILE    *read_from, *write_to;
   char    str[STR_LEN] ;
   int     exitFlag = FALSE ;
   int     runFlag = FALSE ;    

// Print a welcome statement

   if (debug) printf("\nSIUE Beaglebot Project\n") ;
   if (debug) printf("24-Jul-2016\n\n") ;

// GPIO initialization

   if (debug) printf("Initializing the GPIOs which we will use ...\n") ;
   GPIOinit() ;
   turnLED(OFF) ;

// Look to see if user wants to use the GUI

   if (GUImode) {

// Start the gui
// Two-way pipe 

      start_child("tclsh", &read_from, &write_to);
      fprintf(write_to, "source ./tcl/gui.tcl \n") ;

// Get data from GUI

      while (!exitFlag) {
          if  (fgets(str, STR_LEN, read_from) != NULL) {
             getGUIvars(str) ;
             exitFlag = GUIvars.exitFlag ;
             if (!exitFlag) {
                if (!runFlag) {
                   execPRUprograms() ;
                   testRobot() ;
                   runFlag = TRUE ;
                } else {;
                   PRUstop() ;
                   execPRUprograms() ;
                   testRobot() ;
                } // end if then elseif
             } // end if
          } //end if
      } // end while 

   } else {          // NO GUI!!!!

// Get the GUI string from the robot.config file
// and parse it as usual

        loadGuiVarsFromFile(str) ;
        getGUIvars(str) ;

// The string we load may have the exit flag set
// We don' want this ...

        GUIvars.exitFlag = FALSE ;

// Load, configure, and start the PRUs 

        execPRUprograms() ;

// Run the the test robot code

        testRobot() ;

   } // end if-the-else       

// User wants to exit so let PRU0 know

   exitFlag = TRUE ;
   shared_memory->exitFlag = TRUE ;

// Delay for 1 sec before disabling PRUs 

   if (debug) memoryDump() ;
   pauseSec(1) ;        
   PRUstop() ;

   return 0;
} // end main

