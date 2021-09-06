#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "jmp-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jmp-template.h"
#undef DATA_BYTE

static void seg_load(uint8_t segNo){
    uint16_t  DesIndex = (cpu.segReg[segNo].visiblePart>>3)&0x1FFF;
    uint32_t low= lnaddr_read(cpu.gdtr.baseAddr+8*DesIndex,4);
    uint32_t high= lnaddr_read(cpu.gdtr.baseAddr+8*DesIndex+4,4);
    cpu.segReg[segNo].invisiblePart.base_15_0=low&0xFFFF0000;
    cpu.segReg[segNo].invisiblePart.base_23_16=high&0xFF;
    cpu.segReg[segNo].invisiblePart.base_31_24=high&0xFF000000;
    cpu.segReg[segNo].invisiblePart.limit_15_0=low&0x0000FFFF;
    cpu.segReg[segNo].invisiblePart.limit_19_16=high&0xF0000;
}

make_helper(ljmp){
    uint32_t op0;
    uint16_t op1;
    op0=instr_fetch(eip+1,4);
    op1= instr_fetch(eip+5,2);

    cpu.eip=op0;
    cpu.segReg[S_CS].visiblePart=op1;
    seg_load(S_CS);
    print_asm_template2();
    return 0;
}