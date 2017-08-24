//
// Macros that make doing fixed point operations easy (fix.h)
//

#define     PI        3.14159

#define     Q         6
#define     twoQ      12

#define     Q24       24
#define     Q12       12
#define     Q0        0

// Fixed point operationss

#define  FADD(op1,op2)      ( (op1) + (op2) )
#define  FSUB(op1,op2)      ( (op1) - (op2) )
#define  FMUL(op1,op2,q)    ((int32_t) (((int64_t) ((int64_t) (op1) * (int64_t) (op2))) >> q))

// #define  FDIV(op1,op2,q)    ( (int32_t) (((int64_t)(op1) << q)/ ((int64_t) op2 )) )

// Convert from a q1 format to q2 format

#define  FCONV(op1,q1,q2)     (((q2) > (q1)) ? ((op1) << ((q2)-(q1))) : ((op1) >> ((q1)-(q2))))

// Convert a float to a fixed-point representation in q format

#define  TOFIX(op1, q)       ((int32_t) ((op1) * ((float) (1 << (q)))))

// Convert a fixed-point number back to a float

#define  TOFLT(op1, q)       ( ((float) (op1)) / ((float) (1 << (q))) )

