#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "push-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "push-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "push-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper_v(push_r)
make_helper_v(push_i)
make_helper_v(push_rm)

make_helper(pushal) {
	swaddr_write(cpu.esp -  4, 4, cpu.eax, S_SS);
	swaddr_write(cpu.esp -  8, 4, cpu.ecx, S_SS);
	swaddr_write(cpu.esp - 12, 4, cpu.edx, S_SS);
	swaddr_write(cpu.esp - 16, 4, cpu.ebx, S_SS);
	swaddr_write(cpu.esp - 20, 4, cpu.esp, S_SS);
	swaddr_write(cpu.esp - 24, 4, cpu.ebp, S_SS);
	swaddr_write(cpu.esp - 28, 4, cpu.esi, S_SS);
	swaddr_write(cpu.esp - 32, 4, cpu.edi, S_SS);
	cpu.esp -= 32;

	print_asm("pushal");

	return 1;
}
