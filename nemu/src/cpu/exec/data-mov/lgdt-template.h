#include "cpu/exec/template-start.h"

#define instr lgdt

static void do_execute()
{
    cpu.gdtr.limit = lnaddr_read(op_src->val, 2);
    if (ops_decoded.is_operand_size_16)
    {
        cpu.gdtr.baseAddr = lnaddr_read(op_src->val+ 2, 3);
    }
    else
    {
        cpu.gdtr.baseAddr = lnaddr_read(op_src->val + 2, 4);
    }

    print_asm_template1();
}

make_instr_helper(rm);

#include "cpu/exec/template-end.h"