//
// Defines for PRU #1 assembly code
//

#define         PRU_R31_VEC_VALID         32         // allows notification of program completion
#define         PRU_EVTOUT_1              4          // event number that is sent back for PRU 1 to ARM interrupt
#define         PRU0_PRU1_INTERRUPT       17         // PRU0->PRU1 interrupt number
#define         PRU1_PRU0_INTERRUPT       18         // PRU1->PRU0 interrupt number
#define         ARM_PRU1_INTERRUPT        37		 //  ARM->PRU1 interrupt number

// Shared memory base addres AND
// Byte offsets for shared memory accesses

#define         PRU_SHARED_MEM_ADDR    0x00010000
#define         PWM_OS          0
#define         ENC_OS          16
#define         DELAY_OS        32
#define         STATE_OS        36
#define         CLK_CNT_OS      40
#define         PWM_RES_OS      44

// Define linux space GPIO access

#define         GPIO0                0x44e07000
#define         GPIO1                0x4804C000
#define         GPIO2                0x481ac000
#define         GPIO3                0x481ae000 
   
#define         GPIO_CLEARDATAOUT    0x190           //Clearing GPIO
#define         GPIO_SETDATAOUT      0x194           //Setting GPIO
#define         GPIO_DATAOUT         0x138           //reading GPIO

/* gle
#define        GPIO_DATAOUT         0x13C   
#define        GPIO_DATAIN          0x138   
#define        GPIO_CLEARDATAOUT    0x190    
#define        GPIO_SETDATAOUT      0x194    
*/

#define        GPIO1_15_MASK     0x80       //SWITCH
#define        GPIO1_12_MASK     0x10       //LED

// Motor control signals

#define		M1_0	 r30.t2
#define		M1_1	 r30.t3

#define		M2_0	 r30.t4
#define		M2_1	 r30.t5

#define		M3_0	 r30.t0
#define		M3_1	 r30.t1

#define		M4_0	 r30.t6
#define		M4_1	 r30.t7

// Define register aliases

#define		nopReg				r0.b0

#define		enc1				r1
#define		enc2				r2
#define		enc3				r3	
#define		enc4				r4

#define		pwm1				r5
#define		pwm2				r6
#define		pwm3				r7
#define		pwm4				r8

#define		encNEW				r9
#define		encOLD				r10
#define		encEDGE				r11

#define     GPIO_LED_STATE      r12
#define     GPIO_BUTTON         r13
#define     GPIO_LED            r14

#define     read_gpio1          r15
#define     set_gpio1           r16
#define     clr_gpio1           r17

#define     pwmResReg           r18
#define 	stateReg			r19
#define 	clkCntReg			r20

#define		i				    r21
#define		j				    r22

//Can be used to temporally hold values if needed
// scratchpad register

#define		scr				    r23	   

// Currently not using the dela value
    
#define		delayValue			r24

// Shared memory base address

#define		sharedMem			r25

// R29 is used for subroutine calls

#define		M1_ctrl				stateReg.b0.t2
#define		M2_ctrl				stateReg.b0.t4
#define		M3_ctrl				stateReg.b0.t0
#define		M4_ctrl				stateReg.b0.t6

#define     run_flag            stateReg.b1.t0
#define     brake_flag          stateReg.b1.t1
#define     update_flag         stateReg.b1.t2
#define     halt_flag           stateReg.b1.t3

#define		enc1_bit			encEDGE.b1.t0
#define		enc2_bit			encEDGE.b1.t1
#define		enc3_bit			encEDGE.b1.t2
#define		enc4_bit			encEDGE.b1.t3

// Use r29 for subroutine calls
// Since r30.w0 is our output port!!!!!
// Else we get very odd behavior!

.setcallreg   r29.w0

// Define Macros

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// read_delay_value
//
// Description:
//	read in the delay loop value from sharedMemory (written by host)
// 
// At a byte offset of 32
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.macro		read_delay_value
			lbbo	delayValue, sharedMem, DELAY_OS, 4	
.endm

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// get_state
//
// Description:
//	update the status register value in pru1 Memory
//	The state value can be updated by pru0 and the ARM
//	system if necesary it is used to determine wheel direction
//	braking system, if a update of PWM values need to happen,
//	or if the system should stop
//
//	Status byte structure:	Wheel control:     b0
//				Status Flags: 	   b1-b2
//					b1.t0 - run flag    (1 if in run mode) 
//					b1.t1 - brake flag  (1 if in hard brake)
//					b1.t2 - update flag (1 if an update for pwm)
//					b1.t3 - halt flag  (1 if we want to halt pru)
//				   Nop (stays zero):  b3
//
//	At a byte offset of 36 
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.macro 		get_state
			lbbo	stateReg, sharedMem, STATE_OS, 4
.endm
		
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// read_clk_cnt
//
// Description:
//   Tells us how many pwm clock cycles we should run
//   before interrupting PRU0
// 
// At a byte offset of 40
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.macro		read_clk_cnt
			lbbo	clkCntReg, sharedMem, CLK_CNT_OS, 4	
.endm

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// read_pwm_res
//
// Description:
//	read in the pwm maximum count from sharedMemory (written by host)
// Either 255 (8 bit), 1023 (10 bit), or 4095 (12 bit)
// 
// At a byte offset of 44
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.macro		read_pwm_res
			lbbo	pwmResReg, sharedMem, PWM_RES_OS, 4	
.endm

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// pwm_timer
//
// Description:
//	This decrements the PWM register given the motor 
//	timer register and will stop the pwm signal if 
//	necessary
//
// Usage:
// 	pwm_timer	M1_ctrl, pwm1, M1_1, M1_1, NEXT_LINE 
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.macro		pwm_timer
.mparam		M0_ctrl, pwm0, M0_0, M0_1
			
		   qbbc 	CCW, M0_ctrl 		//Check if we are clockwise or counter clockwise
CW:		   qbne 	PWM_JMP, pwm0, 0	//Check if time to bring low
		   clr	    M0_0			    //Set bit low
		   qba	    NEXT			    //Jump to the next instruction

CCW:	   qbne	    PWM_JMP, pwm0, 0	//counter clockwise case
		   clr	    M0_1			
		   qba	    NEXT

PWM_JMP:   dec     pwm0			        //If it is not time to bring low decrement
		   NO_OP				        //NO_OP to mimic jump
NEXT:
.endm

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// check_encoder_edges
//
// Description:
//      Transfers the old encoder values read the new ones
//	and xor to see if there is an edge
//
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.macro		check_encoder_edges
			mov	encOLD.b1, encNEW.b1
			mov	encNEW.b1, r31.b1
			xor	encEDGE.b1, encNEW.b1, encOLD.b1		
.endm

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// zero_encoder_regs
//
// Description:
//   Clears the enc1, enc2, enc3, enc4 registers
//
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.macro		zero_encoder_regs
            zero &enc1, 16
.endm

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// enc_cnt
//
// Description:
//      Reads the encoder tics and increments if need be
//
// Usage:
//      enc_cnt		enc1, enc1_bit
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.macro		enc_cnt
.mparam		enc0, enc0_bit
            qbbc	ENC_JMP, enc0_bit	//If clear jump to ENC_JMP
            inc	    enc0			    //If there is an edge increment
		    qba	    NEXT			
ENC_JMP:	NO_OP				      
		    NO_OP				
NEXT:
.endm

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// store_encoder_values
//
// Description:
//	Stores the current encoder values to shared sharedMemory
//
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.macro		store_encoder_values
			sbbo	&enc1, sharedMem, ENC_OS, 16
.endm

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// pwm_start
//
// Description:
//	Uses the state register to start the pwm values
//	stored in b0
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.macro		pwm_start
			mov	r30.b0, stateReg.b0
.endm

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// brake
//
// Description:
//	Stops the pwm by setting outputs to zero or to one
//	depending on the brake bit
//
// Usage:	
//	stop_pwm  NEXT_LINE
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.macro		brake	
			qbbs	HARD_BR, brake_flag
			mov	    r30.b0, 0x00
			qba	    NEXT
HARD_BR:	mov	    r30.b0, 0xFF
			NO_OP		
NEXT:
.endm

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// brake
//
// Description:
//	Hard brake
//	
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.macro		hard_brake	
			mov	    r30.b0, 0xff
.endm

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// read_pwm_values
//
// Usage:
//	read in PWM values from sharedMemory
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.macro		read_pwm_values
			lbbo	pwm1, sharedMem, PWM_OS, 16
.endm

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// No operation
// (1 clock cycle)
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.macro		NO_OP
			mov    nopReg, nopReg
.endm

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Clear All registers (R0-R28)
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.macro      clear_REGS
	        zero    &r0, 116	
.endm	

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Turn the GPIO LED on (p8.12, GPIO1[12], GPIO:44)
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.macro		led_ON
            set    GPIO_LED_STATE, 12
            sbbo   GPIO_LED, set_gpio1, 0, 4
.endm

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Turn the GPIO LED off (p8.12, GPIO1[12], GPIO:44)
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.macro		led_OFF
            clr     GPIO_LED_STATE, 12
            sbbo    GPIO_LED, clr_gpio1, 0, 4
.endm

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Toggle the GPIO LED (p8.12, GPIO1[12], GPIO:44)
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.macro		led_TOGGLE
            xor      GPIO_LED_STATE.b1, GPIO_LED_STATE.b1, 1<<4
            qbbc     L0, GPIO_LED_STATE, 12            
            led_ON
            qba      L1
L0:			led_OFF                      
L1:                           
.endm 

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// QBPR : Quick branch if button press
//
// Usage: 
//	qbpr LOCATION
//
// Branches to target location if GPIO button is pressed
//	(Green Button, p8.15, GPIO1[15], GPIO:47)
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.macro		qbpr	
.mparam   	LOCATION 
	        lbbo   GPIO_BUTTON, read_gpio1, 0, 4
            qbbs   LOCATION, GPIO_BUTTON.t15
.endm

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// movi32 : Move a 32bit value to a register
//
// Usage:
//     movi32   dst, src    
//
// Sets dst = src. Src must be a 32 bit immediate value.
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.macro  	movi32               
.mparam 	dst, src
            mov     dst.w0, src & 0xFFFF
            mov     dst.w2, src >> 16
.endm

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// dec: Decrement value/register
// 
// Usage:
//	dec value
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.macro      dec      
.mparam     value           
            sub     value, value, #1
.endm  

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// inc: Increment value/register
// 
// Usage:
//	inc value
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.macro	    inc
.mparam		value
			add	value,value, #1
.endm

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Copy a bit from source register to the destination register
// dreg is the destination register
// dbit is the bit number in the destination register
// sreg is the source register
// sbit is the bit number in the source register
// (4 clock cycles)
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.macro      copy_bit  
.mparam     dreg, dbit, sreg, sbit
            clr     dreg, dbit
            qbbc    END, sreg, sbit
            set     dreg, dbit
END:       
.endm

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Macro to set up the GPIO utils
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.macro		gpio_SETUP
            mov     read_gpio1, GPIO1 | GPIO_DATAOUT
            mov     set_gpio1, GPIO1 | GPIO_SETDATAOUT
            mov     clr_gpio1, GPIO1 | GPIO_CLEARDATAOUT
            set     GPIO_LED, 12
.endm

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// send_ARM_interrupt
//
// Description:
//	Send an interrupt to the arm from pru1
//
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.macro		send_ARM_interrupt
			mov	r31.b0, PRU_R31_VEC_VALID | PRU_EVTOUT_1
.endm

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// send_pru0_interrupt
//
// Description:
//	Send an interrupt to pru0 from pru1
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
.macro		send_pru0_interrupt
			ldi	r31, PRU1_PRU0_INTERRUPT + 16
.endm

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Macro used to enable to OCP port
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^		
.macro		ocp_port_ENABLE
            lbco    r0, C4, 4, 4     // load SYSCFG reg into r0 (use c4 const addr)
	        clr     r0, 4            // clear bit 4 (STANDBY_INIT)
	        sbco    r0, C4, 4, 4     // store the modified r0 back at the load addr
.endm

