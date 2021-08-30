#include "cpu/exec/template-start.h"

#define instr call

static void do_execute(){
    if(ops_decoded.is_operand_size_16){
        cpu.esp-=2;
        MEM_W(cpu.esp,cpu.eip+decode_i_w(cpu.eip+1)+1);
    }else{
        cpu.esp-=4;
        MEM_W(cpu.esp, cpu.eip+decode_i_l(cpu.eip+1)+1);
    }
        cpu.eip+=op_src->val;
    print_asm_template1();
}

make_instr_helper(i)
make_instr_helper(rm)


#include "cpu/exec/template-end.h"
