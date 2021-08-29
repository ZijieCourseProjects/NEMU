#include "cpu/exec/template-start.h"

#if DATA_BYTE == 4
make_helper(ret){
    if(ops_decoded.is_operand_size_16){
        cpu.eip=MEM_R(cpu.esp)&0x0000FFFF;
        cpu.esp+=2;
    }else{
        cpu.eip=MEM_R(cpu.esp);
        cpu.esp+=4;
    }
    print_asm("ret");
    return 0;
}
#endif

#if DATA_BYTE == 2
make_helper(ret_imm){
    int16_t imm=instr_fetch(eip+1, 2);
    if(ops_decoded.is_operand_size_16){
        cpu.eip=MEM_R(cpu.esp)&0x0000FFFF;
        cpu.esp+=2;
    }else{
        cpu.eip=MEM_R(cpu.esp);
        cpu.esp+=4;
    }
    cpu.esp+=imm;
    print_asm("ret");
    return 0;
}
#endif

#include "cpu/exec/template-end.h"
