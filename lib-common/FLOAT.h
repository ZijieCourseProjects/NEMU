#ifndef __FLOAT_H__
#define __FLOAT_H__

#include "trap.h"

typedef int FLOAT;

static inline signed short sign(FLOAT a){
    return (a>>31 & 0x1)?1:0;
}

static inline FLOAT toUnsign(FLOAT x){
    return x&0x7FFFFFFF;
}

static inline int toUnsign_i(int x){
    return x<0?-x:x;
}

static inline FLOAT toNeg(FLOAT x){
    return x|0x80000000;
}

static inline int F2int(FLOAT a) {
    return sign(a)? -1 * (toUnsign(a) >> 16):(toUnsign(a)>>16);
}

static inline FLOAT int2F(int a) {
    char Sa = (a<0)?1:0;
    if(Sa){
        a=-a;
    }
    FLOAT x = a << 16;
    if(Sa){
        x=toNeg(x);
    }
	return x;
}

static inline FLOAT F_mul_int(FLOAT a, int b) {
    return a*b;
}

static inline FLOAT F_div_int(FLOAT a, int b) {
    return a/b;
}



FLOAT f2F(float);
FLOAT F_mul_F(FLOAT, FLOAT);
FLOAT F_div_F(FLOAT, FLOAT);
FLOAT Fabs(FLOAT);
FLOAT sqrt(FLOAT);
FLOAT pow(FLOAT, FLOAT);

// used when calling printf/sprintf to format a FLOAT argument
#define FLOAT_ARG(f) (long long)f

void init_FLOAT_vfprintf(void);

#endif
