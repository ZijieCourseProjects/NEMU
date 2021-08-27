#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute(){
    update_eflags_pf_zf_sf(op_dest->val-op_src->val);
}

make_instr_helper(i2rm)

#include "cpu/exec/template-end.h"
