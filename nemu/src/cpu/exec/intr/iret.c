//
// Created by Eric Zhao on 10/9/2021.
//

#include "common.h"
#include "cpu/exec/helper.h"
#include "cpu/reg.h"

void seg_load(uint8_t);

make_helper(iret){
	cpu.eip= swaddr_read(cpu.esp,4,S_SS);
	cpu.esp+=4;
	cpu.segReg[S_CS].visiblePart= swaddr_read(cpu.esp,2,S_SS);
	cpu.esp+=2;
	seg_load(S_CS);
	cpu.eflags.val= swaddr_read(cpu.esp,4,S_SS);
	cpu.esp+=4;
	return 0;
}
