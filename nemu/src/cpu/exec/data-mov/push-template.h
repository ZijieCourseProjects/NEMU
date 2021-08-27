#include "cpu/exec/template-start.h"

#define instr push

static void do_execute(){
    cpu.eip-=(ops_decoded.is_operand_size_16?2:4);
    MEM_W(cpu.esi, op_src->val);
    print_asm_template1();
}

make_instr_helper(r)


#include "cpu/exec/template-end.h"


