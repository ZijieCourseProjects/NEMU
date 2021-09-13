#include "cpu/exec/template-start.h"

#define instr bt

#if DATA_BYTE == 2 || DATA_BYTE == 4
static void do_execute () {
	int bit_position = op_src->val & 0x1f;
	cpu.eflags.CF = (op_dest->val >> bit_position) & 0x1;

	print_asm_template2();
}

make_instr_helper(r2rm)
#endif

#include "cpu/exec/template-end.h"
