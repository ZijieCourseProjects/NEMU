#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute(){
    Log("%x,%x",REG(op_src->val),cpu.ebp);
    if(ops_decoded.is_operand_size_16){
        MEM_W(REG(op_src->val), MEM_R(cpu.esp));
        cpu.esp+=2;
    }else{
        MEM_W(REG(op_src->val), MEM_R(cpu.esp));
        cpu.esp+=4;
    }
}
make_instr_helper(r)

#include "cpu/exec/template-end.h"
