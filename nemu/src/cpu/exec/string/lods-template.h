#include "cpu/exec/template-start.h"

#define instr lods


make_helper(concat(lods_m_, SUFFIX)){
    cpu.eax = MEM_R(cpu.esi);
    int incDec = cpu.eflags.DF ? -DATA_BYTE : DATA_BYTE;
    cpu.esi = cpu.esi + incDec;
    print_asm_template2();

    return 1;
}

#include "cpu/exec/template-end.h"
