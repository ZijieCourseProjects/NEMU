#include "cpu/exec/template-start.h"

#define instr lods


make_helper(concat(lods_m_, SUFFIX)){
    Log("esi:%x,mem(res):%x",REG(R_ESI),MEM_R(REG(R_ESI)));
    REG(R_EAX) = MEM_R(REG(R_ESI));
    int incDec = cpu.eflags.DF ? -DATA_BYTE : DATA_BYTE;
    REG(R_ESI) = REG(R_ESI) + incDec;
    print_asm_template2();

    return 1;
}

#include "cpu/exec/template-end.h"
