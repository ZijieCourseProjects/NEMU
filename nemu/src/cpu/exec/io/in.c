//
// Created by Eric Zhao on 11/9/2021.
//

#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "in-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "in-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "in-template.h"
#undef DATA_BYTE


make_helper_v(in_si2a);
make_helper_v(in_r2rm);