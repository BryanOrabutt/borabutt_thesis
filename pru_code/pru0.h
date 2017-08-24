//
// Defines used by PRU 0
//

// 70 ms for 1,000,000

#define   KILL_TIME   1000000

// For convenience

#define   TRUE       1
#define   FALSE      0

//#define PRU addresses

/*
#define PRU0
#define HOST1_MASK		(0x80000000)
#define HOST0_MASK		(0x40000000)
#define PRU0_PRU1_EVT		(16)
#define PRU1_PRU0_EVT		(18)
#define PRU0_ARM_EVT		(34)
#define SHARE_MEM		(0x00010000)
*/



// Bit 15 is P8-11
// Bit 14 is p8-16
// Bit 07 is p9-25
// Bit 05 is p9-27

#define TOGGLE_PRU_LED		(__R30 ^= (1 << 15))      //Bit 15
#define OFF_PRU_LED 		(__R30 &= (0xFFFF7FFF))
#define ON_PRU_LED			(__R30 |= (0x00008000))
#define DISABLE_DRV			(__R30 |= (1 << 5))       //Bit 5 Neg logic
#define ENABLE_DRV			(__R30 &= (0xFFFFFFDF))   //Bit 5 Neg logic
#define TOGGLE_DRV			(__R30 &= (1 << 5))       //Bit 5 Neg logic
#define PRU_SW_VALUE		(__R31  & (1 << 14))	  //Bit 14
#define	ACC_IN1_VAL			(__R31  & (1 << 7))	      //Bit 7






