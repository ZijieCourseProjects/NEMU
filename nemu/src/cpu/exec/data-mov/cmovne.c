#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "cmovne-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "cmovne-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmovne-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper_v(cmovne_i2r)
make_helper_v(cmovne_i2rm)
make_helper_v(cmovne_r2rm)
make_helper_v(cmovne_rm2r)
