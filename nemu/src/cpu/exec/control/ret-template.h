#include "cpu/exec/template-start.h"

#define instr ret

static void do_execute(){
    if(ops_decoded.is_operand_size_16){
        cpu.eip=MEM_R(cpu.esp)&0x0000FFFF;
        cpu.esp+=2;
    }else{
        cpu.eip=MEM_R(cpu.esp);
        cpu.esp+=4;
    }
}

#include "cpu/exec/template-end.h"
