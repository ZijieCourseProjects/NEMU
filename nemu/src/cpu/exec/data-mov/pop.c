#include "cpu/exec/helper.h"

#define DATA_BYTE 2
#include "pop-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "pop-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper_v(pop_r)

make_helper(popal) {
	cpu.eax = swaddr_read(cpu.esp + 28, 4, S_SS);
	cpu.ecx = swaddr_read(cpu.esp + 24, 4, S_SS);
	cpu.edx = swaddr_read(cpu.esp + 20, 4, S_SS);
	cpu.ebx = swaddr_read(cpu.esp + 16, 4, S_SS);
	cpu.ebp = swaddr_read(cpu.esp +  8, 4, S_SS);
	cpu.esi = swaddr_read(cpu.esp +  4, 4, S_SS);
	cpu.edi = swaddr_read(cpu.esp +  0, 4, S_SS);
	cpu.esp += 32;

	print_asm("popal");

	return 1;
}
