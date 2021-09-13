#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "jmp-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jmp-template.h"
#undef DATA_BYTE

void seg_load(uint8_t segNo);

make_helper(jmp_intersegment) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	uint16_t cs = instr_fetch(eip + 5, 2);

	cpu.eip = addr - (1 + 4 + 2);
	cpu.segReg[S_CS].visiblePart=cs;
	seg_load(S_CS);

	print_asm("ljmp $%#x,$%#x", cs, addr);
	return 1 + 4 + 2;
}
