
// ********************************
// PRUlib.c
// ********************************

#include "prussdrv.h"
#include "pruss_intc_mapping.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

#include "mem.h"
#include "PRUlib.h"

// Global variable that points to shared memory

extern  shared_memory_t  *shared_memory ;

// Debug variable

extern  int   debug ;

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// PRU initialization routine
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
void PRUinit(void) {
// Initialize structure used by prussdrv_pruintc_intc   
// PRUSS_INTC_INITDATA is found in pruss_intc_mapping.h 

   tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;

/* Allocate and initialize memory */

   prussdrv_init ();

// For PRU 0

   prussdrv_open (PRU_EVTOUT_0);

// For PRU 1

   prussdrv_open (PRU_EVTOUT_1);

// Map PRU's INTC 

   prussdrv_pruintc_init(&pruss_intc_initdata);

// Set up pointer to shared memory

   static void *pruSharedDataMemory;
   prussdrv_map_prumem(PRUSS0_SHARED_DATARAM, &pruSharedDataMemory);
   shared_memory = (shared_memory_t *) pruSharedDataMemory;

   return ;
}

// *****************************************
// Routine to clean up after the PRUs
// *****************************************
void PRUstop(void) {

/* Disable PRU and close memory mappings */

   if (debug) printf("Disabling the PRUs\n") ;
   prussdrv_pru_disable(PRU0);
   prussdrv_pru_disable(PRU1);
   prussdrv_exit ();
   return ;
}

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Here is a subroutine to interact with the PRUs
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
void PRUstart(void) {

// Load and execute binary on PRU0 
// Since using C-code for PRU, we need to give START_ADDR 

   if (debug) printf("Starting PRU0 program\n") ;
   prussdrv_exec_program_at(PRU0, "./text.bin", START_ADDR);

/* Load and execute binary on PRU1 */

   if (debug) printf("Starting PRU1 program\n") ;
   prussdrv_exec_program(PRU1, "./pru1.bin");  
 
   return ;
}
