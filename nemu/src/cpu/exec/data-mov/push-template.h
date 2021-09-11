#include "cpu/exec/template-start.h"

#define instr push

static void do_execute(){
    cpu.esp-=(ops_decoded.is_operand_size_16?2:4);
    MEM_W(cpu.esp, op_src->val,S_SS);
    print_asm_template1();
}


make_instr_helper(r)
make_instr_helper(rm)
make_instr_helper(i)
#if DATA_BYTE == 1
make_instr_helper(si)
#endif

#include "cpu/exec/template-end.h"
