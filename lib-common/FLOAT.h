#ifndef __FLOAT_H__
#define __FLOAT_H__

#include "trap.h"

typedef int FLOAT;
#define ll long long
#define sign_bit(res) (res >> 31) & 1
#define int_no_sign(sign) (sign ? -1 : 1)
#define int_sign(sign) int_no_sign(sign)
#define test() printf("\n")

static inline int F2int(FLOAT a) {
	return a >> 16;
}

static inline FLOAT int2F(int a) {
	return a << 16;
}

static inline FLOAT F_mul_int(FLOAT a, int b) {
	return (a * b);
}

static inline FLOAT F_div_int(FLOAT a, int b) {
	return (a / b);
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