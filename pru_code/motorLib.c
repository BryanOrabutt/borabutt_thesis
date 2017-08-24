//
// Library of routines which run on PRU0
// for handling the motors
//

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "mem.h"
#include "fix.h"
#include "motorLib.h"

// Pointer to shared memory is a global variable

  extern   shared_memory_t   *mem ;

// Wait for interrupt

  void    waitForInterrupt(void) ;

// *********************************************************
// Routine to halt PRU #1
// Sets halt flag bit in state variable
// We also have to clear the run flag.
// *********************************************************

void  haltPRU(void) {
   mem->command.status = ACTIVE ;
   mem->state = M_HALT ;
   return ;
}

// *********************************************************
// Routine to apply hard brake
// *********************************************************

void hardBrake(void) {
   mem->command.status = ACTIVE ;
   mem->state = M_HARD_BRAKE ;
   return ;
}

// **********************************************************
// Routine to coast to a stop
// **********************************************************

void coast(void) {
   mem->command.status = ACTIVE ;
   mem->state = M_COAST ;
   return ;
}
	
// ************************************************************
// Routine to implement PID loop on a single DC motor
// Using the velocity or differential PID
//
// delta_p = Kp * error
// delta_i = Ki * (error - past_error)
// delta_d = Kd * (error - past_error + 2 * past_past_error)
//
// output = previous_output + (delta_p + delta_i + delta_d)
//
// Errors in Q6 format.
// enc in Q0 format so convert it to Q6 format
// Kp, Ki, Kd are in Q6 format,
// delta_p, delta_i, and delta_d are in Q6 format
// Output in Q0 format.
//
// **************************************************************

int32_t PID(DCmotor_t * motor, int32_t enc) {
   int32_t   delta_p, delta_i, delta_d ;
   int32_t   scr, out ;
   int32_t   max_delta ;
    
   max_delta = motor->max_delta ;

// Update past_error and past_past_error
  
   motor->e2 = motor->e1 ;
   motor->e1 = motor->e0 ;

// Compute new error term
// First convert enc value to Q6 format
// Motor setpoint stored in Q6 format

   enc = FCONV(enc, 0, Q) ;
   motor->e0 = FSUB(motor->setpoint, enc) ;

// Compute delta_p, delta_i, and delta_d
// Deltas will be in Q12 format

   delta_p = FMUL(motor->Kp, motor->e0, 0) ;
   scr = FSUB(motor->e0, motor->e1) ;
   delta_i = FMUL(motor->Ki, scr, 0) ;
   scr = FADD(scr, motor->e2 << 1) ;
   delta_d = FMUL(motor->Kd, scr, 0) ;
   scr = FADD(delta_i, delta_d) ;
   scr = FADD(scr, delta_p) ;

// Convert the delta from Q12 to Q0 format

   scr = FCONV(scr, twoQ, 0) ;

//
// Limit the delta
// This helps ensure stability and keeps the
// robot from "lurching" forward
//

   if (scr > max_delta) scr = max_delta ;
   if (scr < -max_delta) scr = -max_delta ;

// Make sure "out" is in range

   out = FADD(motor->PWMout, scr)  ;
   if (out > motor->PWMmax) {
       out = motor->PWMmax ;
   } else {
       if (out < motor->PWMmin) {
           out = motor->PWMmin ;
       } // end if
   } // end if-then-else

// Save the output and also return it

   motor->PWMout = out ;  
 
   return   out ;
}

// ******************************************************************
// Routine to create state variable
// ******************************************************************

int32_t  createState(void) {
   int32_t  state ;

// Look at direction field so we can set the state correctly
// Also look at the breaking mode so when we stop.
// We do so either by braking hard or by coasting.

   state = 0 ;

   if (mem->motorENA[M1]) {
        if (mem->motor[M1].wheelDirection == CW) {
           state |= M1_CW ;
        } else {
           state |= M1_CCW ;
        } // end if-then-else
        if (mem->motor[M1].brakeType == HARD) {
           state |= M_HARD_BRAKE ;
        } // end if
   } // end if

   if (mem->motorENA[M2]) {
        if (mem->motor[M2].wheelDirection == CW) {
           state |= M2_CW ;
        } else {
           state |= M2_CCW ;
        } // end if-then-else
        if (mem->motor[M2].brakeType == HARD) {
           state |= M_HARD_BRAKE ;
        } // end if
   } // end if

   if (mem->motorENA[M3]) {
        if (mem->motor[M3].wheelDirection == CW) {
           state |= M3_CW ;
        } else {
           state |= M3_CCW ;
        } // end if-then-else
        if (mem->motor[M3].brakeType == HARD) {
           state |= M_HARD_BRAKE ;
        } // end if
   } // end if
   if (mem->motorENA[M4]) {
        if (mem->motor[M4].wheelDirection == CW) {
           state |= M4_CW ;
        } else {
           state |= M4_CCW ;
        } // end if-then-else
        if (mem->motor[M4].brakeType == HARD) {
           state |= M_HARD_BRAKE ;
        } // end if
   } // end if

// Set the run bit
       
   state |= M_RUN ;

   return state ;

} // end createState() 

// ******************************************************************
// Routine to move.
// *********************************************************************

void move(void) { 
   int        i ;
   DCmotor_t  *motor ;

// Set status to ACTIVE

   mem->command.status = ACTIVE ;

// Set the errors to zero
// Also set the distance traveled to 0
// The setpoint, brake mode, wheel direction
// and target distance all get set by the routines
// in robotLib. Also clear out pwm array.
// enc array is cleared in PRU 1 asm code

   for (i=0; i<NUM_MOTORS; i++) {
      mem->motor[i].e0 = 0 ;
      mem->motor[i].e1 = 0 ;
      mem->motor[i].e2 = 0 ;
      mem->motor[i].distance = 0 ;
      mem->motor[i].PWMout = 0 ;
      mem->pwm[i] = 0 ;
      mem->enc[i] = 0 ;
   } // end for

//
// Initialize the setpoint PID loop also
//

  mem->setpointPID.e0 = 0 ;
  mem->setpointPID.e1 = 0 ;
  mem->setpointPID.e2 = 0 ;

// Main loop
// Keep looping until distance traveled on a single
// motor exceeds the target distance.
// PRU 1 will interrupt us at the desired sample rate.
// The waitForInterrupt routine toggles the PRU LED
// at the sample rate to provide user with visual feedback.
//
 
   int32_t    velInTics[NUM_MOTORS] ;
   int        loop = TRUE ;

// Write state out to shared memory
// PRU1 should start generating PWM outputs

   int32_t  state ;
   state = createState() ;
   mem->state = state ;

   while (loop) {
     waitForInterrupt() ; 
     for (i=0; i < NUM_MOTORS; i++) {
         if (mem->motorENA[i]) { 
            motor = &mem->motor[i] ;
            velInTics[i] = mem->enc[i] - motor->distance ;
            motor->distance = mem->enc[i] ;
            if ((motor->distance) <= (motor->targetDistance)) {
                mem->pwm[i] = PID(motor, velInTics[i]) ;
            } else {
                loop = FALSE ;
                mem->state = M_STOP & mem->state ; // clear run bit  
            } // end if-then-else      
         } // end if
      } // end for

// This is for two wheels only.
// Implement a PID loop to adjust setpoints on two wheels so
// that the two wheels will travel at same velocity.

      adjustSetpoint(velInTics[M2], velInTics[M1]) ;   
  
   } // end while
        
   return  ;

} // end move()

// *******************************************************************
//  Executes the command
//  The move() routine will look at the state and call appropriate
//  subroutine.  Upon return from the called routine, the status
//  will be updated to reflect that the commmand has COMPLETED.
//  The move() routine looks at the mem->motor struct to figure
//  out exactly what is must do.
// ******************************************************************	
	
void doCommand(int32_t  command_code) {

   switch (command_code) {
       case  NOP:        mem->command.status = IDLE ;                                 
                         break ;

       case  FWD:        move() ;
                         mem->command.status = COMPLETED ; 
                         break ;

       case  BWD:        move() ;
                         mem->command.status = COMPLETED ; 
                         break ;

       case  ROT:        move() ;
                         mem->command.status = COMPLETED ; 
                         break ;

       case BRAKE_HARD:  hardBrake() ;
                         mem->command.status = COMPLETED ; 
                         break ;

       case BRAKE_COAST: coast() ;
                         mem->command.status = COMPLETED ; 
                         break ;

       case HALT_PRU:    haltPRU() ;
                         mem->command.status = COMPLETED ; 
                         break ;
       
       default:          mem->command.status = IDLE ;          
                         break ;

   } // end switch

  return ;
} // end do_command() 

// *******************************************************************
// Implements the setpoint PID loop
//
// delta_p = Kp * error
// delta_i = Ki * (error - past_error)
// delta_p = Kp * error
// delta_i = Ki * (error - past_error)
// delta_d = Kd * (error - past_error + 2 * past_past_error)
//
// output = previous_output + (delta_p + delta_i + delta_d)
//
// Errors in Q6 format.
// Kp, Ki, Kd are in Q6 format,
// delta_p, delta_i, and delta_d are in Q6 format
// Output in Q0 format.
//
// output = previous_output + (delta_p + delta_i + delta_d)
//
// Errors in Q6 format.
// Kp, Ki, Kd are in Q6 format,
// delta_p, delta_i, and delta_d are in Q6 format
// Output in Q6 format.
// ******************************************************************	

void adjustSetpoint(int32_t rightVel, int32_t  leftVel)  {

// Compute difference in wheel velocities
// Convert to Q format

   int32_t   velDiff, scr ;
   int32_t   delta_p, delta_i, delta_d, delta_sp ;
   
// Compute the error 
// M2 is the right motor
// M1 is the left motor
// If velDiff is positive then we will need to slow
// down M2 and speed up M1!!!!

   velDiff = rightVel - leftVel ;  

// Update past_error (e1) and past_past_error (e2)
  
   mem->setpointPID.e2 = mem->setpointPID.e1 ;
   mem->setpointPID.e1 = mem->setpointPID.e0 ;

// Compute new error term
// We want it in Q format where currently Q is 6 

   mem->setpointPID.e0 = FCONV(velDiff, 0, Q) ;

// Compute delta_p, delta_i, and delta_d

   delta_p = FMUL(mem->setpointPID.Kp, mem->setpointPID.e0, 0) ;
   scr = FSUB(mem->setpointPID.e0, mem->setpointPID.e1) ;
   delta_i = FMUL(mem->setpointPID.Ki, scr, 0) ;
   scr = FADD(scr, mem->setpointPID.e2 << 1) ;
   delta_d = FMUL(mem->setpointPID.Kd, scr, 0) ;
   scr = FADD(delta_i, delta_d) ;
   scr = FADD(scr, delta_p) ;

// Convert the delta from 2Q to Q format

   scr = FCONV(scr, twoQ, Q) ;
   delta_sp = FADD(mem->setpointPID.output, scr)  ;

// Make sure "out" is in range

   if (delta_sp > mem->setpointPID.maxVal) {
       delta_sp = mem->setpointPID.maxVal ;
   } else {
       if (delta_sp < mem->setpointPID.minVal) {
           delta_sp = mem->setpointPID.minVal ;
       } // end if
   } // end if-then-else

// Save the output 

   mem->setpointPID.output = delta_sp ;  

// Adjust the setpoint for the two wheel PID loops

   mem->motor[M1].deltaSetpoint = delta_sp ;
   mem->motor[M2].deltaSetpoint = delta_sp ;

// We adjust in a differential manner!

   mem->motor[M2].setpoint = mem->motor[M2].targetSetpoint - delta_sp ;
   mem->motor[M1].setpoint = mem->motor[M1].targetSetpoint + delta_sp ; 

   return ;

}  // end adjustSetpoint() 

	
