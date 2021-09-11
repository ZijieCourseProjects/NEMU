//
// Created by Eric Zhao on 11/9/2021.
//

#include "cpu/exec/helper.h"

make_helper(sti){
	cpu.eflags.IF=1;
	print_asm("sti");
	return 1;
}
