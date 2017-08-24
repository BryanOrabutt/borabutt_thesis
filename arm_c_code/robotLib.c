//
// robotLib
//

#include    <stdio.h>
#include    <stdlib.h>
#include    <stdint.h>
#include    <math.h>
#include    <string.h>

#include   "bbbLib.h"
#include   "fix.h"
#include   "mem.h"
#include   "srf02.h"
#include   "servo_driver.h"
#include   "robotLib.h"

// Global variable
// Pointer to shared memory

  extern shared_memory_t  *shared_memory ;

// GUI variables

  extern  GUIvars_t       GUIvars ;

// Debug variable

  extern  int             debug ;

// *********************************************
// Routine to initialize the GPIO we need
// *********************************************
void GPIOinit(void) {

   initPin(ACCEL_PIN) ;                     // GPIO0[2] Accel GPIO interrupt
   setPinDirection(ACCEL_PIN, IN) ;

   initPin(GPIO_LED_PIN) ;                  // GPIO1[12] GPIO LED
   setPinDirection(GPIO_LED_PIN, OUT) ;

   initPin(GPIO_SW_PIN) ;                   // GPIO1[15] GPIO SWITCH
   setPinDirection(GPIO_SW_PIN, IN) ;

   initPin(DRV_PIN) ;                       // GPIO3[19] buffer enable
   setPinDirection(DRV_PIN, OUT) ;
   setPinValue(DRV_PIN, ON) ;

   return ;
} // end GPIOinit()

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Rotuine to read the GPIO switch
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
int buttonPress(void) {
   int  value ;
   value = getPinValue(GPIO_SW_PIN) ;
   return value ;
} // 

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Routine to turn GPIO LED (GPIO1[12]) board on or OFF
// 0 is OFF and 1 is ON
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
void turnLED(int state) {
    setPinValue(GPIO_LED_PIN, state) ;
    return ;
} // end turnLED()

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Routine to get GUI variables 
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
void getGUIvars(char *str) {
   FILE   *fid ;

// Save the str sent back from tcl script to a file

   fid = fopen("./robot.config", "w") ;
   fprintf(fid, "%s\n", str) ;
   fclose(fid) ;

// Parse the string sent back from the gui

   sscanf(str, "%d:%d:%d:%d:%d:%d:%f:%f:%f:%f:%f:%f:%f:%d:%d:%d:%d:%d:%f:%f:%f:%f:%f:%f", 
          &GUIvars.exitFlag,
          &GUIvars.sonarEna,
          &GUIvars.lineEna,
          &GUIvars.rtcEna,
          &GUIvars.accelEna,
          &GUIvars.motorType,
          &GUIvars.Kp,
          &GUIvars.Ki,
          &GUIvars.Kd,
          &GUIvars.samplePeriod,
          &GUIvars.wheelDiam,
          &GUIvars.turnRad,
          &GUIvars.ticsPerRev,
          &GUIvars.M1Ena,
          &GUIvars.M2Ena,
          &GUIvars.M3Ena,
          &GUIvars.M4Ena,
		  &GUIvars.PWMresMode,
		  &GUIvars.Kp_sp,
		  &GUIvars.Ki_sp,
		  &GUIvars.Kd_sp,
          &GUIvars.max_delta,
          &GUIvars.velPIDscale,
          &GUIvars.spPIDscale  
          ) ;

// Dump to the screen if we are in debug mode

   if (debug) {
          printf("exit flag is %d\n", GUIvars.exitFlag) ;
          printf("sonarEna is %d\n", GUIvars.sonarEna) ;
          printf("lineEna is %d\n", GUIvars.lineEna) ;
          printf("rtcEna is %d\n", GUIvars.rtcEna) ;
          printf("accelEna is %d\n", GUIvars.accelEna) ;
          printf("motorType is %d\n", GUIvars.motorType) ;
          printf("Kp is %f\n", GUIvars.Kp) ;
          printf("Ki is %f\n", GUIvars.Ki) ;
          printf("Kd is %f\n", GUIvars.Kd) ;
          printf("samplePeriod is %f\n", GUIvars.samplePeriod) ;
          printf("wheelDiam is %f\n", GUIvars.wheelDiam) ;
          printf("turnRad is %f\n", GUIvars.turnRad) ;
          printf("ticsPerRev is %f\n", GUIvars.ticsPerRev) ;
          printf("M1Ena is %d\n", GUIvars.M1Ena) ;
          printf("M2Ena is %d\n", GUIvars.M2Ena) ;
          printf("M3Ena is %d\n", GUIvars.M3Ena) ;
          printf("M4Ena is %d\n", GUIvars.M4Ena) ;
          printf("PWMresMode is %d\n", GUIvars.PWMresMode) ;
          printf("Kp (setpoint) is %f\n", GUIvars.Kp_sp) ;
          printf("Ki (setpoint) is %f\n", GUIvars.Ki_sp) ;
          printf("Kd (setpoint) is %f\n", GUIvars.Kd_sp) ;
          printf("max_delta is %f\n", GUIvars.max_delta) ;
          printf("velPIDscale is %f\n", GUIvars.velPIDscale) ;
          printf("spPIDscale is %f\n", GUIvars.spPIDscale) ;
   } // end if

   return ;

} // getGUIvars()


// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Routine to load robot paramters from a file
// rather than from the GUI
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
void loadGuiVarsFromFile(char * str) {
   FILE  *fid ;

   fid = fopen("./robot.config", "r") ;
   fscanf(fid, "%s", str) ;
   if (debug) printf("robot.config string read -> %s\n", str) ;
   fclose(fid) ; 

   return  ;
} // end loadGuiVarsFromFile()


// ######################################################
// Routine to configure PRUs
// ######################################################
void configPRU(void) {
   float  scr = 0.0 ;  // scratchpad

   if (debug) printf("In configPRU() \n") ;

// Tells us when to exit program from GUI mode
   
   shared_memory->exitFlag = GUIvars.exitFlag ;

// Not currently using delay

   shared_memory->delay = 0 ;

// PWM resolution
// Sample period is in ms

   switch (GUIvars.PWMresMode) {
      case BITS_IS_8:    shared_memory->PWMres = 255 ;
                         scr = (GUIvars.samplePeriod / PWM_CLK_PERIOD_8BIT) + 0.5 ;
                         break ;
      case BITS_IS_10:   shared_memory->PWMres = 1023 ;
                         scr = (GUIvars.samplePeriod / PWM_CLK_PERIOD_10BIT) + 0.5 ;
                         break ; 
      case BITS_IS_12:   shared_memory->PWMres = 4095 ;
                         scr = (GUIvars.samplePeriod / PWM_CLK_PERIOD_12BIT) + 0.5 ;
                         break ;
   } // end switch

// Number of PWM clock cycles making up a PID sample period

   shared_memory->PWMclkCnt = (uint32_t) (scr) ;

// Either DC or Servo

   shared_memory->motorType = GUIvars.motorType ;

// Motor enables

   shared_memory->motorENA[M1] = GUIvars.M1Ena ;
   shared_memory->motorENA[M2] = GUIvars.M2Ena ;
   shared_memory->motorENA[M3] = GUIvars.M3Ena ;
   shared_memory->motorENA[M4] = GUIvars.M4Ena ;

// Wheel diameter and tics per inch

   shared_memory->wheelDiam = TOFIX(GUIvars.wheelDiam, Q) ; 
   scr = GUIvars.ticsPerRev / (PI * GUIvars.wheelDiam) ;  
   shared_memory->ticsPerInch = TOFIX(scr, Q) ; 

// Initialize DC motor structures
// Multiply the Kp, Ki, Kd values by constant
// We do this so we can use sliders in the GUI
// which span to 0 to 100 range.

   float k ;
   k = GUIvars.velPIDscale ;

   int i ;
   for (i = 0; i < NUM_MOTORS; i++) {
       shared_memory->motor[i].Kp = TOFIX((k * GUIvars.Kp), Q) ;
       shared_memory->motor[i].Ki = TOFIX((k * GUIvars.Ki), Q) ;
       shared_memory->motor[i].Kd = TOFIX((k * GUIvars.Kd), Q) ;
       shared_memory->motor[i].PWMmax = shared_memory->PWMres ;
       shared_memory->motor[i].PWMmin = 0 ;  
       shared_memory->motor[i].max_delta = (int32_t) GUIvars.max_delta ;    
   } // end i loop

// Freeze the PRUs implementing motor control

   shared_memory->command.code = NOP ;
   shared_memory->command.status = IDLE ;
   shared_memory->state = 0 ;

// Initialize the PID loop which adjusts the setpoint
// to make sure both wheels traveling at the same velocity

   initSetpointPID() ;

   return ;
} // end configPRU()


// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Routine to initialize the setpoint PID loop.
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
void initSetpointPID(void) {

// PID error signals

   shared_memory->setpointPID.e0 = 0 ;
   shared_memory->setpointPID.e1 = 0 ;
   shared_memory->setpointPID.e2 = 0 ;

// PID parameters

   float     k ;
   k = GUIvars.spPIDscale ;

   shared_memory->setpointPID.Kp = TOFIX(k * GUIvars.Kp_sp, Q) ;
   shared_memory->setpointPID.Ki = TOFIX(k * GUIvars.Ki_sp, Q) ;
   shared_memory->setpointPID.Kd = TOFIX(k * GUIvars.Kd_sp, Q) ;

// Limits

   shared_memory->setpointPID.minVal = 0 ;
   shared_memory->setpointPID.maxVal = 0 ;
   shared_memory->setpointPID.output = 0 ;

   return ;
} // end initSetpointPID()


// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Dump of entire memory structure to a file
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void memoryDump(void) {
   FILE  *fid ;
   int   i ;

   if (debug) printf("Dumping contents of shared memory to file.\n") ;

   fid = fopen("memory_dump.txt", "w") ;

// Motor enable values

   for (i = 0; i < NUM_MOTORS; i++) {
        fprintf(fid, "mem->motorENA[%d] = %d\n", i+1, shared_memory->motorENA[i]) ;     
   } // end i loop

// PWM and encoder values

   for (i = 0; i < NUM_MOTORS; i++) {
        fprintf(fid, "mem->pwm[%d] = %d\n", i+1, shared_memory->pwm[i]) ;    
        fprintf(fid, "mem->enc[%d] = %d\n", i+1, shared_memory->enc[i]) ; 
   } // end i loop

// Motor parameters

   for (i = 0; i < NUM_MOTORS; i++) {
        fprintf(fid, "mem->motor[%d].setpoint = %d\n", i+1, shared_memory->motor[i].setpoint) ;  
        fprintf(fid, "mem->motor[%d].distance = %d\n", i+1, shared_memory->motor[i].distance) ;  
        fprintf(fid, "mem->motor[%d].targetDistance = %d\n", i+1, shared_memory->motor[i].targetDistance) ;  
        fprintf(fid, "mem->motor[%d].wheelDirection = %d\n", i+1, shared_memory->motor[i].wheelDirection) ;  
        fprintf(fid, "mem->motor[%d].brakeType = %d\n", i+1, shared_memory->motor[i].brakeType) ;  
        fprintf(fid, "mem->motor[%d].e0 = %d\n", i+1, shared_memory->motor[i].e0) ;  
        fprintf(fid, "mem->motor[%d].e1 = %d\n", i+1, shared_memory->motor[i].e1) ;  
        fprintf(fid, "mem->motor[%d].e2 = %d\n", i+1, shared_memory->motor[i].e2) ;  
        fprintf(fid, "mem->motor[%d].Kp = %d\n", i+1, shared_memory->motor[i].Kp) ;  
        fprintf(fid, "mem->motor[%d].Ki = %d\n", i+1, shared_memory->motor[i].Ki) ;    
        fprintf(fid, "mem->motor[%d].Kd = %d\n", i+1, shared_memory->motor[i].Kd) ; 
        fprintf(fid, "mem->motor[%d].PWMmin = %d\n", i+1, shared_memory->motor[i].PWMmin) ; 
        fprintf(fid, "mem->motor[%d].PWMmax = %d\n", i+1, shared_memory->motor[i].PWMmax) ; 
        fprintf(fid, "mem->motor[%d].PWMout = %d\n", i+1, shared_memory->motor[i].PWMout) ;  
        fprintf(fid, "mem->motor[%d].PWMout = %d\n", i+1, shared_memory->motor[i].max_delta) ;          
   } // end i loop

// Other parameters

   fprintf(fid, "mem->wheelDiam = %d\n", shared_memory->wheelDiam) ;  
   fprintf(fid, "mem->ticsperInch = %d\n", shared_memory->ticsPerInch) ;  
   fprintf(fid, "mem->delay = %d\n", shared_memory->delay) ;
   fprintf(fid, "mem->state = %x\n", shared_memory->state) ;
   fprintf(fid, "mem->PWMclkCnt = %d\n", shared_memory->PWMclkCnt) ;
   fprintf(fid, "mem->PWMres = %d\n", shared_memory->PWMres) ;
   fprintf(fid, "mem->exitFlag = %d\n", shared_memory->exitFlag) ;
   fprintf(fid, "mem->motorType = %u\n", shared_memory->motorType) ;
   fprintf(fid, "mem->scr = %d\n", shared_memory->scr) ;
   fprintf(fid, "mem->interruptCounter = %u\n", shared_memory->interruptCounter) ;
   fprintf(fid, "mem->command.code = %u\n", shared_memory->command.code) ;
   fprintf(fid, "mem->command.status = %u\n", shared_memory->command.status) ;

// Data buffer

/*
   for (i=0; i<BUF_LEN; i++) {
      fprintf(fid, "mem->enc_data[%d] = %u\n", i, shared_memory->enc_data[i]) ;
   }
*/

// Close the file and exit

   fclose(fid) ;
   return ;
} // end memoryDump()


// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Routine to convert a distance in inches to an
// equivalent number of encoder tics.
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
int32_t inches2tics(float inches) {
   int32_t   tics ;
   int32_t   inches_Q ;

   inches_Q = TOFIX(inches, Q) ;
   tics = FMUL(shared_memory->ticsPerInch, inches_Q, 0) ;
   tics = FCONV(tics, twoQ, 0) ; // integer
   return tics ;
} // end inches2tics()

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Routine to convert a distance in tics to an
// equivalent number of inches
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
float tics2inches(int32_t tics) {
   float   inches ;
   inches = ((float) tics) / shared_memory->ticsPerInch ;   
   return inches ;
} // end tics2inches()

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Routine to update a motor structure.
// Values are stored into motor structures. 
// Target distance, setpoint, wheel direction and braking
// mode get written out to shared memory.
// The final setpoint is actually stored in targetSetpoint.
// The setpoint is always set to 0.  The move routine in
// PRU 0 will setpoint up to the target.
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

void updateMotor(int motor_num, int dir, int brakeType, float distance, float velocity) {
   int32_t   distInTics, velInTics ;
   float     delX ; 

// delX is distance we would move in one sample period
// samplePeriod is in ms

   delX = velocity * GUIvars.samplePeriod * 0.001 ;

// Need the distance and the velocity that we desire
// converted to tics

   distInTics = inches2tics(distance) ;
   velInTics =  inches2tics(delX) ; 
   shared_memory->motor[motor_num].targetDistance = distInTics ;

// Setpoints should be in Q6 format

   shared_memory->motor[motor_num].targetSetpoint = FCONV(velInTics, 0, Q) ;
   shared_memory->motor[motor_num].setpoint = FCONV(velInTics, 0, Q)  ;
   shared_memory->motor[motor_num].deltaSetpoint = 0 ;

// Wheel direction and braking mode

   shared_memory->motor[motor_num].wheelDirection = dir ;
   shared_memory->motor[motor_num].brakeType = brakeType ; 

// Differential setpoint limits

   shared_memory->setpointPID.maxVal = FCONV(velInTics, 0, Q) ;
   shared_memory->setpointPID.minVal = -FCONV(velInTics, 0, Q) ;  

   return ;

} // end updateMotor()

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Routine to query a motor structure for important values.
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
int32_t queryMotor(int motor_num, int item) {
   int32_t  value ;

   switch(item) {
      case  SETPOINT:    value = shared_memory->motor[motor_num].setpoint ;
                         break ;
      case  DISTANCE  :  value = shared_memory->motor[motor_num].distance ;
                         break ;
      case  TARGET_DIST: value = shared_memory->motor[motor_num].targetDistance ;
                         break ;
      case  WHEEL_DIR:   value = shared_memory->motor[motor_num].wheelDirection ;
                         break ;
      case  BRAKE_TYPE:  value = shared_memory->motor[motor_num].brakeType ;
                         break ;
      default:           value = CRASH;  
                         break ;
   } // end switch

   return value ;

} // end updateMotor()

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Routine to query the command structure
// Makes it easy to determine status of command
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
int32_t query(int item) {
   int32_t  value ;

   switch(item) {
      case  CMD:         value = shared_memory->command.code ;
                         break ;
      case  STATUS:      value = shared_memory->command.status ;
                         break ;
      default:           value = CRASH ; 
                         break ; 
   } // end switch

   return value ;

} // end updateMotor()

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Wait for IDLE to be true
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
void waitForIdle(void) {
   while (query(STATUS) != IDLE) { delay_ms(STATUS_DELAY) ; }
   return ;
} // end for waitForIdle() 

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Wait for Complete to be true
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
void waitForComplete(void) {
   while (query(STATUS) != COMPLETED) { delay_ms(STATUS_DELAY) ; }
   return ;
} // waitForComplete()

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Reset the PRUs
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

void resetPRU(void) {
   if (debug) printf("Entering resetPRU()\n") ;
   shared_memory->command.code = BRAKE_HARD ;
   shared_memory->command.status = START ; 
   waitForComplete() ;
   shared_memory->command.code = NOP ;
   shared_memory->command.status = IDLE ; 
   return ;
} // end resetPRU()

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Routine to drive robot forward
// a distance (in inches) with a given velocity
// (in inches per sec)
// 
// Return a 1 if successfully started command
// Return a 0 if not successful
//
// Only implementing for two motors.
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
int fwd(float distance, float velocity) {

// Wait until we are idle

   if (debug) { printf("Waiting for IDLE in fwd().\n") ; }      

   waitForIdle() ;

// Update motor structures with information about 
// how forward command should be carried out

   updateMotor(M1, CW, HARD, distance, velocity) ;
   updateMotor(M2, CW, HARD, distance, velocity) ;

// Update command structure to indicate we desre to drive FWD 

   shared_memory->command.code = FWD ;
   shared_memory->command.status = START ;

   if (debug) { printf("Waiting for COMPLETED in fwd().\n") ; }   
   waitForComplete() ;

// After command is seen to complete then set
// command to a no-op and state as being idle

   shared_memory->command.code = NOP ;
   shared_memory->command.status = IDLE ;
   shared_memory->state = 0 ;

   return PASS ;

} // end fwd()

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Routine to drive robot backward
// Only implementing for two motors.
// M1 is left motor
// M2 motor is right
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
int  bwd(float distance, float velocity) {

// Wait until we are idle

   if (debug) { printf("Waiting for IDLE in bwd();\n") ; }      
   waitForIdle() ;

// Update motor structures with information about 
// how forward command should be carried out

   updateMotor(M1, CCW, HARD, distance, velocity) ;
   updateMotor(M2, CCW, HARD, distance, velocity) ;

// Update command structure to indicate we desre to drive BWD 

   shared_memory->command.code = BWD ;
   shared_memory->command.status = START ;

   if (debug) { printf("Waiting for COMPLETED in bwd() ;\n") ; }   
   waitForComplete() ;

// After command is seen to complete then set
// command to a no-op and state as being idle

   shared_memory->command.code = NOP ;
   shared_memory->command.status = IDLE ;
   shared_memory->state = 0 ;

   return PASS ;

} // end bwd()


// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Routine to spin robot in direction specified 
// # of degrees at particular velocity.
// Only implementing for two motors.
// One motor gets driven CW and the other CCW.
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
int rotate(float degrees, float velocity, int direction) {
   float  distance ;
//
// Need to determin distance we need to travel
//
   distance = (PI / 180.0) * GUIvars.turnRad * degrees ;

// Wait until we are idle

   if (debug) { printf("Waiting for IDLE in rotate().\n") ; }     
   waitForIdle() ;

// Update motor structures with information about 
// how forward command should be carried out

   if (direction == CW) {
      updateMotor(M1, CW, HARD, distance, velocity) ;
      updateMotor(M2, CCW, HARD, distance, velocity) ;

   } else {
      updateMotor(M1, CCW, HARD, distance, velocity) ;
      updateMotor(M2, CW, HARD, distance, velocity) ;
   } // end if-then-else

// Update command structure to indicate we desre to ROT

   shared_memory->command.code = ROT ;
   shared_memory->command.status = START ;

   if (debug) { printf("Waiting for COMPLETED in rotate().\n") ; }   
   waitForComplete() ;

// After command is seen to complete then set
// command to a no-op and state as being idle

   shared_memory->command.code = NOP ;
   shared_memory->command.status = IDLE ;
   shared_memory->state = 0 ;

   return PASS ;
} // end rotate() ;


// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Routine to drive to make a right turn
// Just calling the rotate() routine.
// Only implementing for two motors.
// Turn at 6 in/sec
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

int right(void) {  
   
   if (rotate(90.0, 6.0, CW) == PASS) {
      return PASS ; 
   } else {
      return FAIL ;
   } // end if-then-else


} // end right()

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Routine to drive to make a left turn
// Just calling the rotate() routine.
// Only implementing for two motors.
// Turn at 6 in/sec
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

int left(void) {

   if (rotate(90.0, 6.0, CCW) == PASS) {
      return PASS ; 
   } else {
      return FAIL ;
   } // end if-then-else
   
} // end left()

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Routine to apply the brake
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

int applyBrake (void) {

   shared_memory->command.code = BRAKE_HARD ;
   shared_memory->command.status = START ; 
   waitForComplete() ;
   shared_memory->command.code = NOP ;
   shared_memory->command.status = IDLE ; 

   return PASS ;
} // end brake() 

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Routine to release the brake
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

int releaseBrake (void) {

   return PASS ;

} // end brake() 

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Routine to take a test drive
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

void testDrive(void) {

// Reset PRU .. hard brake

   resetPRU() ;

   fwd(48.0, 12.0) ;  
   left() ;
   fwd(24.0, 12.0) ;
   left() ;
   fwd(48.0, 12.0) ; 
   left() ;
   fwd(24.0, 12.0) ;
   left() ;       
  
   fwd(48.0, 12.0) ;
   rotate(180.0, 6.0, CW) ;
   fwd(48.0, 12.0) ;
   rotate(180.0, 6.0, CW) ;
   fwd(48.0, 12.0) ;
   rotate(180.0, 6.0, CW) ;
   fwd(48.0, 12.0) ;
   rotate(180.0, 6.0, CW) ;

   return  ;
} // end test_drive()

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Routine to test our robot
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

void testRobot(void) {
   if (debug) printf("Entering testRobot()\n") ;
   testDrive() ;
   if (debug) printf("Leaving testRobot()\n") ;
   return ;
}






