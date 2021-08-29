#include "cpu/exec/template-start.h"

#define instr jbe

static void do_execute(){
    if(cpu.eflags.ZF||cpu.eflags.CF){
        cpu.eip+=op_src->val;
        if(ops_decoded.is_operand_size_16)
            cpu.eip=cpu.eip&0x0000FFFF;
    }
    print_asm_template1();
}
#if DATA_BYTE == 1 || DATA_BYTE ==4
make_instr_helper(si)
#endif

make_instr_helper(r)
make_instr_helper(i)

#include "cpu/exec/template-end.h"
