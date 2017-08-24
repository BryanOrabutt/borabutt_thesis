//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Defines
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

// BUF_LEN is length of data buffer
// STR_LEN is length of string buffer

#define     BUF_LEN         200
#define     STR_LEN         250

// Motor defines

#define     NUM_MOTORS      4

#define     M1              0
#define     M2              1
#define     M3              2
#define     M4              3

#define     CRASH           -9999

// Wheel directions

#define   CW      0
#define   CCW     1

// Brake types

#define   COAST   0
#define   HARD    1
//
// Commands we can give PRU 0
//

#define   NOP            0
#define   FWD            1
#define   BWD            2
#define   ROT            3
#define   BRAKE_HARD     4
#define   BRAKE_COAST    5
#define   HALT_PRU       6

// 
// Here the codes for status of commands
//

#define   CMD            1
#define   STATUS         2

#define   IDLE           0
#define   START          1
#define   ACTIVE         2
#define   COMPLETED      3
#define   ABORTED        4

// These are used when we query the motor structure

#define   SETPOINT       1
#define   DISTANCE       2
#define   TARGET_DIST    3
#define   WHEEL_DIR      4
#define   BRAKE_TYPE     5

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// A structure that decribes a command from
// the ARM to PRU 0
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

typedef struct {
    int32_t     code ;
    int32_t     status ;
} command_t ;


// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Declare a structure to hold the GUI variables
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

typedef struct {
    int      exitFlag ;
    int      sonarEna ;
    int      lineEna ;
    int      rtcEna ;
    int      accelEna ;
    int      motorType ;
    float    Kp ;
    float    Ki ;
    float    Kd ;
    float    samplePeriod ;
    float    wheelDiam ;
    float    turnRad ;
    float    ticsPerRev ;
    int      M1Ena ;
    int      M2Ena ;
    int      M3Ena ;
    int      M4Ena ;
    int      PWMresMode ;
    float    Kp_sp ;
    float    Ki_sp ;
    float    Kd_sp ;
    float    max_delta ;
    float    velPIDscale ;
    float    spPIDscale ;
} GUIvars_t ;


// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// A DC motor structure
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

typedef struct {
    int32_t     setpoint ;         // desired velocity (in tics)
    int32_t     targetSetpoint ;   // will rammp up intil this is reached
    int32_t     deltaSetpoint ;    // steps we will take in ramping up
    int32_t     distance ;         // dist in tics (actual)
    int32_t     targetDistance ;   // dist in tics (desired)
    int32_t     wheelDirection ;   // CW or CCW
    int32_t     brakeType ;        // COAST or HARD
    int32_t     e0 ;               // current error
    int32_t     e1 ;               // past error
    int32_t     e2 ;               // past "past error"
    int32_t     Kp ;               // proportional gain (Q)
    int32_t     Ki ;               // integral gain (Q)
    int32_t     Kd ;               // deriviative gain (Q)
    int32_t     PWMmin ;           // minumum PWM out allowed
    int32_t     PWMmax ;           // maximum PWM out allowed
    int32_t     PWMout ;           // PWM output  
    int32_t     max_delta ;        // Maximum change in PID output in a single step
}   DCmotor_t;

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// A PID structure
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
typedef  struct {
    int32_t     e0 ;              // current error
    int32_t     e1 ;              // past error
    int32_t     e2 ;              // past past error
    int32_t     Kp ;              // proportional constant
    int32_t     Ki ;              // integral constant
    int32_t     Kd ;              // derivative constant
    int32_t     minVal ;          // minimum output
    int32_t     maxVal ;          // maximum output
    int32_t     output ;          // PID loop output
} PID_t ;

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// Our shared memory structure
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

typedef struct {
    int32_t     pwm[NUM_MOTORS] ;      // shared mem byte os of 0
    int32_t     enc[NUM_MOTORS] ;      // os of 16
    int32_t     delay ;                // os of 32
    int32_t     state ;                // os of 36
    int32_t     PWMclkCnt ;            // os of 40
    int32_t     PWMres ;               // os of 44
    int32_t     exitFlag ;             // exit when true
    int32_t     interruptCounter ;     // sample counter
    int32_t     motorType ;            // DC or stepper
    int32_t     motorENA[NUM_MOTORS] ; // Motor enables
    int32_t     scr ;                  // scratchpad register 
    int32_t     wheelDiam ;            // diameter in inches (Q)
    int32_t     ticsPerInch;           // encoder tics per inch (Q)
    int32_t     enc_data[BUF_LEN] ;    // Buffer of encoder data
    command_t   command ;              // Motor command structure
    DCmotor_t   motor[NUM_MOTORS] ;    // DC motor structure
    PID_t       setpointPID ;        
}   shared_memory_t ;




