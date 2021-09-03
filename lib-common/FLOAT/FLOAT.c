#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {

	return (FLOAT)((ll)a * (ll)b >> 16);
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
	/* Dividing two 64-bit integers needs the support of another library
	 * `libgcc', other than newlib. It is a dirty work to port `libgcc'
	 * to NEMU. In fact, it is unnecessary to perform a "64/64" division
	 * here. A "64/32" division is enough.
	 *
	 * To perform a "64/32" division, you can use the x86 instruction
	 * `div' or `idiv' by inline assembly. We provide a template for you
	 * to prevent you from uncessary details.
	 *
	 *     asm volatile ("??? %2" : "=a"(???), "=d"(???) : "r"(???), "a"(???), "d"(???));
	 *
	 * If you want to use the template above, you should fill the "???"
	 * correctly. For more information, please read the i386 manual for
	 * division instructions, and search the Internet about "inline assembly".
	 * It is OK not to use the template above, but you should figure
	 * out another way to perform the division.
	 */
	 int sa = sign_bit(a);
	 int sb = sign_bit(b);
	 int s = int_no_sign(sa) * int_no_sign(sb);

	FLOAT asmresup,modasm,asmresdown;	

	asm volatile ("div %2": "=a" (asmresup),"=d"(modasm):"r" (int_no_sign(sb)*b),"a" (int_no_sign(sa)*a),"d" (0));
	asm volatile ("div %2": "=a" (asmresdown),"=d"(modasm):"r" (int_no_sign(sb)*b),"a" (0),"d" (modasm));
	asmresdown = 0x0000FFFF&(asmresdown>>16);
	asmresup = asmresup << 16;
	return (asmresup+asmresdown)*s;
}


FLOAT f2F(float a) {
	/* You should figure out how to convert `a' into FLOAT without
	 * introducing x87 floating point instructions. Else you can
	 * not run this code in NEMU before implementing x87 floating
	 * point instructions, which is contrary to our expectation.
	 *
	 * Hint: The bit representation of `a' is already on the
	 * stack. How do you retrieve it to another variable without
	 * performing arithmetic operations on it directly?
	 */
	int t = *((int*)&a);
	int s = t >> 31;
	int e = (t >> 23) & 0xff;
	int m = t & 0x7fffff;
	FLOAT res = m;
	int exp = e - 0x7f;
	if(!e){
		if(!m) return 0;
		else exp = 1 - e; 
	} else if(!(e ^ 0xff)){ //if denormalized
		return (-1) ^ ((!s) << 31); // positive infinity of negtive infinity
	}else res |= (1 << 23);   // add 1(after shrl) as a normalized float number
	if(exp > 7){
		res <<= exp - 7;  //23-16=7
	} else {
		res >>= -exp + 7;
	}

	return (res * int_sign(s));
}


FLOAT Fabs(FLOAT a) {
	return a * int_no_sign(sign_bit(a));
}

/* Functions below are already implemented */

FLOAT sqrt(FLOAT x) {
	FLOAT dt, t = int2F(2);

	do {
		dt = F_div_int((F_div_F(x, t) - t), 2);
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

FLOAT pow(FLOAT x, FLOAT y) {
	/* we only compute x^0.333 */
	FLOAT t2, dt, t = int2F(2);

	do {
		t2 = F_mul_F(t, t);
		dt = (F_div_F(x, t2) - t) / 3;
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}