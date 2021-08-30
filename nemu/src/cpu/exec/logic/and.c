#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#define REGI AL
#include "and-template.h"
#undef DATA_BYTE
#undef REGI

#define DATA_BYTE 2
#define REGI AX
#include "and-template.h"
#undef DATA_BYTE
#undef REGI

#define DATA_BYTE 4
#define REGI EAX
#include "and-template.h"
#undef DATA_BYTE
#undef REGI

/* for instruction encoding overloading */

make_helper_v(and_i2a)
make_helper_v(and_i2rm)
make_helper_v(and_si2rm)
make_helper_v(and_r2rm)
make_helper_v(and_rm2r)
