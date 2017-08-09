//
// robotLib.h
//
#define   FALSE   0
#define   TRUE    1

#define   PASS    1
#define   FAIL    0

#define   M_RUN			 (1 << 8)
#define   M_HARD_BRAKE   (1 << 9)
#define   M_UPDATE		 (1 << 10)
#define   M_HALT		 (1 << 11)


// Motor control for state register

#define   M1_CW		(0x00000004)
#define   M1_CCW	(0x00000008)
#define   M2_CW		(0x00000010)
#define   M2_CCW	(0x00000020)
#define   M3_CW		(0x00000001)
#define   M3_CCW	(0x00000002)
#define   M4_CW		(0x00000040)
#define   M4_CCW	(0x00000080)

// Period of PWM clock in ms
// Measure on the scope and enter
// correct value here

#define     PWM_CLK_PERIOD_12BIT     0.8
#define     PWM_CLK_PERIOD_10BIT     0.2
#define     PWM_CLK_PERIOD_8BIT      0.05

// PWM resolution modes

#define     BITS_IS_8          1
#define     BITS_IS_10         2
#define     BITS_IS_12         3

#define     GPIO_LED_PIN      44
#define     GPIO_SW_PIN       47
#define     ACCEL_PIN          2
#define     DRV_PIN          115

// Delay (in ms) to wait between status checks

#define     STATUS_DELAY      50

// Function prototype declarations

void    getGUIvars(char *str) ;
void    loadGuiVarsFromFile(char *str) ;
void    GPIOinit(void) ;
void    turnLED(int state) ;
int     buttonPress(void) ;
void    configPRU(void) ;
void    memoryDump(void) ;

int32_t inches2tics(float inches) ;
float   tics2inches(int32_t tics) ;
void    updateMotor(int motor_num, int dir, int brakeType, float distance, float velocity) ;
int32_t queryMotor(int motor_num, int item) ;
int32_t query(int item) ;

void    resetPRU(void) ;
void    waitForIdle(void) ;
void    waitForComplete(void) ;

int     fwd(float distance, float velocity) ;
int     bwd(float distance, float velocity) ;
int     rotate(float degrees, float velocity, int direction) ;
int     right(void) ;
int     left(void) ;
int     releaseBrake (void) ;
int     applyBrake (void) ;

void    testDrive(void) ;
void    testSonar(void) ;
void    testServo(void) ;
void    testRobot(void) ;

void    initSetpointPID(void) ;




