#include "cpu/exec/template-start.h"

#define instr test

static void do_execute(){
    update_eflags_pf_zf_sf(op_src->val&op_dest->val);
    cpu.eflags.CF=0;
    cpu.eflags.OF=0;
    print_asm_template2();
}

make_instr_helper(r2rm)

#include "cpu/exec/template-end.h"
