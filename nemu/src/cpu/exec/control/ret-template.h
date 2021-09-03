#include "cpu/exec/template-start.h"

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
make_helper(ret_imm){
    int16_t imm=instr_fetch(eip+1, 2);
    cpu.eip=MEM_R(cpu.esp);
    cpu.esp+=4;
    Log("%x",cpu.eip);
    Log("%d",imm);
    cpu.esp+=imm;
    print_asm("ret");
    return 0;
}

#include "cpu/exec/template-end.h"
