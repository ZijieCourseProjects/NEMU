#include "cpu/exec/template-start.h"

#define instr cmps

make_helper(concat(cmps_, SUFFIX)) {
	DATA_TYPE src = MEM_R(cpu.esi,S_DS);
	DATA_TYPE dest = MEM_R(cpu.edi,S_ES);
	DATA_TYPE result = src - dest;

	update_eflags_pf_zf_sf((DATA_TYPE_S)result);
	cpu.eflags.CF = result > src;
	cpu.eflags.OF = MSB((src ^ dest) & (src ^ result));

	cpu.esi += (cpu.eflags.DF ? -DATA_BYTE : DATA_BYTE);
	cpu.edi += (cpu.eflags.DF ? -DATA_BYTE : DATA_BYTE);

	print_asm("cmps" str(SUFFIX) " %%es:(%%edi),%%ds:(%%esi)");
	return 1;
}

#include "cpu/exec/template-end.h"
