#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute(){
    if(ops_decoded.is_operand_size_16){
        OPERAND_W(op_dest, MEM_R(cpu.esp));
        cpu.esp+=2;
    }else{
        OPERAND_W(op_dest, MEM_R(cpu.esp));
        cpu.esp+=4;
    }
    print_asm_template1();
}
make_instr_helper(r)

#include "cpu/exec/template-end.h"
