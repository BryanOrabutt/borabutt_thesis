//
// This is the motor control code that will run on PRU 0
// 

#include <stdint.h>
#include "pru_cfg.h"
#include "pru_intc.h"
#include "mem.h"
#include "pru0Lib.h"
#include "motorLib.h"
#include "pru0.h"

// Define input and output registers

volatile register uint32_t __R30;
volatile register uint32_t __R31;

/* Mapping Constant table register to variable */

volatile pruCfg CT_CFG __attribute__((cregister("PRU_CFG", near), peripheral));
volatile far pruIntc CT_INTC __attribute__((cregister("PRU_INTC", far), peripheral));

// Global pointer to memory stucture

shared_memory_t   *mem ;

// Global variables that allow us to handle GPIO

int  *clrGPIO1_reg ;
int  *setGPIO1_reg ;
int  *readGPIO1_reg ;

int  *clrGPIO3_reg ;
int  *setGPIO3_reg ;
int  *readGPIO3_reg ;

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Subroutine to perform PRU initialization
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

void initPRU(void) {	
	CT_INTC.SICR = PRU1_PRU0_EVT ;
	initGPIO();
	return ;
}

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Subroutine to wait for an interrupt.
// It also clears the interrupt
// Toggles the PRU LED at interrupt rate
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

void waitForInterrupt(void) {
   while (!(__R31 & HOST0_MASK)) { } ;  // wait for interrupt  
   CT_INTC.SICR = PRU1_PRU0_EVT ;       // clear interrupt
   TOGGLE_PRU_LED ;
   return ;
}
 
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Routine to kill some time
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
void killTime(int32_t delay) {
   int i ;   
   for (i=0; i<delay; i++) ;
   return ;
} // end killTime()


// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// MAIN PROGRAM STARTS HERE
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void main() {

// Point to 12 kB of shared memory

   mem = (shared_memory_t *) PRU_SHARED_MEM_ADDR ;

// Perform some PRU initialization tasks

   initPRU() ;

// Enable the motor driver signals

   enableBuffers() ;

// Keep implementing commands until we are told to exit
// Put in some delay.
// Don't want to check the status too rapidly so we
// kill some time ...

   while (!mem->exitFlag) {
       killTime(KILL_TIME) ;              
       switch (mem->command.status) {
           case  IDLE:      break ;

           case  START:     doCommand(mem->command.code) ;
                            break ;

           case  ACTIVE:    break ;

           case  COMPLETED: break ;

           case  ABORTED:   break ;  
                          
       } // end switch
    } // end while
 
   doCommand(HALT_PRU) ;

// Disable the motor driver signals

   disableBuffers() ;

// Turn the PRU LED off

   OFF_PRU_LED;

//   GPIO1pin(LED_PIN, OFF) ;

   __R31 = 35;    // PRU 0 to ARM interrupt
   __halt();      // halt the PRU

} // end main













