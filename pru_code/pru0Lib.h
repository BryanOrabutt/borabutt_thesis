//
//  Defines 
//

// Misc defines

#define PRU0
#define HOST1_MASK		(0x80000000)
#define HOST0_MASK		(0x40000000)

//#define PRU0_PRU1_EVT	(16)

#define PRU1_PRU0_EVT		(18)

// Shared memory address

#define   PRU_SHARED_MEM_ADDR     0x00010000  

// GPIO bank addresses

#define      GPIO0                0x44e07000
#define      GPIO1                0x4804c000
#define      GPIO2                0x481ac000
#define      GPIO3                0x481ae000          

// These can be OR'ed with the above bank addresses

#define      GPIO_DATAOUT         0x13C   
#define      GPIO_DATAIN          0x138   
#define      GPIO_CLEARDATAOUT    0x190    
#define      GPIO_SETDATAOUT      0x194  

// GPIO LED GPIO1[12]

#define      LED_PIN      12 

// GPIO LED GPIO3[19]

#define      DRV_PIN      19 


#define      OFF          0
#define      ON           1

//
// Library function prototype declarations
//

void       initGPIO(void) ;
void       GPIO1pin(int pin, int value) ;
void       blinkLED(void) ;

void       GPIO3pin(int pin, int value) ;
void       enableBuffers(void) ;
void       disableBuffers(void) ;



