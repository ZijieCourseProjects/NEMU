#include "cpu/exec/template-start.h"

#define instr call

static void do_execute(){
    if(ops_decoded.is_operand_size_16){
        cpu.esp-=2;
        MEM_W(cpu.esp,cpu.eip);
    }else{
        cpu.esp-=4;
        MEM_W(cpu.esp, cpu.eip);
    }
        cpu.eip+=op_src->val;
}

make_instr_helper(i)


#include "cpu/exec/template-end.h"
