#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute(){
    if(ops_decoded.is_operand_size_16){
        MEM_W(REG(op_src->reg), MEM_R(cpu.esp));
        cpu.esp+=2;
    }else{
      Log("%x",MEM_R(cpu.esp));
        OPERAND_W(op_src, MEM_R(cpu.esp));
        cpu.esp+=4;
    }
    print_asm_template1();
}
make_instr_helper(r)

#include "cpu/exec/template-end.h"
