#include "cpu/exec/template-start.h"

#define instr test

static void do_execute(){
    update_eflags_pf_zf_sf(op_src->val&op_src->val);
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"
