// Assembly language code to run on pru 1
// PWM generation and reading of wheel encoders

#include    "./pru1.h"
.origin		0
.entrypoint	START

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

START:		      
            clear_REGS	
            movi32   sharedMem, PRU_SHARED_MEM_ADDR
			ocp_port_ENABLE
            read_pwm_res
            read_clk_cnt
       
// Keep reading state from shared_memory
// Waiting for PRU 0 to tell us to run
  
WAIT_FOR_RUN_FLAG:  
            hard_brake
            get_state
            qbbc    WAIT_FOR_RUN_FLAG, run_flag
            zero_encoder_regs

MAIN:		        
            send_pru0_interrupt
            get_state 

// See if run flag is still set
      
            qbbc    STOP, run_flag 

// clkCntReg tells us how many PWM clocks we
// should generate before we interrupt PRU0

            mov     i, clkCntReg
I_LOOP:
		    read_pwm_values	
            pwm_start 

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@   

// Will do the following either 256, 1024, or 4096 times
// depending on user selected PWM resolution
// Want to do this as fast as we possible can.
// Completing j-loop constitutes one PWM clock cycle.
                  
			mov	     j, pwmResReg	 
J_LOOP:     
            check_encoder_edges

ENC_1_CHK:  enc_cnt      enc1, enc1_bit
ENC_2_CHK:  enc_cnt      enc2, enc2_bit		
ENC_3_CHK:  enc_cnt      enc3, enc3_bit
ENC_4_CHK:  enc_cnt      enc4, enc4_bit 

PWM_1_CHK:  pwm_timer    M1_ctrl, pwm1, M1_0, M1_1
PWM_2_CHK:  pwm_timer    M2_ctrl, pwm2, M2_0, M2_1
PWM_3_CHK:  pwm_timer    M3_ctrl, pwm3, M3_0, M3_1
PWM_4_CHK:  pwm_timer    M4_ctrl, pwm4, M4_0, M4_1

            dec     j			
			qbne	J_LOOP, j, 0
          
            dec     i
            qbne    I_LOOP, i, 0

// End of sample period
// Write encoder values into shared memory and go back to MAIN

			sbbo	&enc1, sharedMem, ENC_OS, 16 
            qba     MAIN
//
// We come here when run flag has been de-asserted
// Turn LED off!
//

STOP:
            hard_brake
			qbbs    HALT_PRU1, halt_flag
            qba     WAIT_FOR_RUN_FLAG

// HALT flag set .. we are quitting

HALT_PRU1:  
            hard_brake                   	
            send_ARM_interrupt    
            halt	


