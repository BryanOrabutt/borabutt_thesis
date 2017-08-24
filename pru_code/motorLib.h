//
// Function prototype declarations
//
//
// Defines used by motorLib 
//
#define   FALSE   0
#define   TRUE    1

#define   M_RUN			 (1 << 8)
#define   M_STOP         (~M_RUN)
#define   M_HARD_BRAKE   (1 << 9)
#define   M_UPDATE		 (1 << 10)
#define   M_HALT		 (1 << 11)
#define   M_COAST		 0

// Wheel directions

#define   CW      0
#define   CCW     1

// Motor control for state register

#define   M1_CW		(0x00000004)
#define   M1_CCW	(0x00000008)
#define   M2_CW		(0x00000010)
#define   M2_CCW	(0x00000020)
#define   M3_CW		(0x00000001)
#define   M3_CCW	(0x00000002)
#define   M4_CW		(0x00000040)
#define   M4_CCW	(0x00000080)

void      haltPRU(void) ;
void      hardBrake(void) ;
void      coast(void) ;
int32_t   PID(DCmotor_t *motor, int32_t enc) ;
void      move(void) ;
void      doCommand(int32_t  command_code) ;
void      adjustSetpoint(int32_t rightVel, int32_t  leftVel) ;
int32_t   createState(void) ;









