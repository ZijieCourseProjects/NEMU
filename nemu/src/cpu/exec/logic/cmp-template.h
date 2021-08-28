#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute(){
    int32_t result=op_dest->val-op_src->val;
    update_eflags_pf_zf_sf(result);
    cpu.eflags.CF=(unsigned)(op_dest->val)>(unsigned)(result);
    cpu.eflags.OF=((int32_t)(op_dest->val<(int32_t)op_src->val)&&(result>0))||(((int32_t)op_dest->val>(int32_t)op_src->val)&&result<0);
    Log("ZF:%d,SF:%d,CF:%d,OF:%d",cpu.eflags.ZF, cpu.eflags.SF, cpu.eflags.CF, cpu.eflags.OF);
}

make_instr_helper(i2rm)
make_instr_helper(r2rm)

#include "cpu/exec/template-end.h"
