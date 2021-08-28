#include "cpu/exec/template-start.h"

#define instr jle

static void do_execute(){
    if(cpu.eflags.ZF || cpu.eflags.SF!=cpu.eflags.OF){
        cpu.eip+=op_src->val;
        if(ops_decoded.is_operand_size_16)
            cpu.eip=cpu.eip&0x0000FFFF;
    }
}
make_instr_helper(i)

#include "cpu/exec/template-end.h"
