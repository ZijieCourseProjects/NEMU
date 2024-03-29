#include "cpu/exec/template-start.h"

#define instr lods


make_helper(concat(lods_m_, SUFFIX)){
    REG(R_EAX) = MEM_R(cpu.esi,S_DS);
    int incDec = cpu.eflags.DF ? -DATA_BYTE : DATA_BYTE;
    cpu.esi = cpu.esi + incDec;
    print_asm_template2();

    return 1;
}

#include "cpu/exec/template-end.h"
