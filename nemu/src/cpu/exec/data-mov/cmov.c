#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "cmov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "cmov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmov-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper_v(cmov_i2r)
make_helper_v(cmov_i2rm)
make_helper_v(cmov_r2rm)
make_helper_v(cmov_rm2r)
