#include "cpu/exec/template-start.h"

#define instr setne

static void do_execute(){
    Log("%d",cpu.eflags.ZF);
    if(!cpu.eflags.ZF){
        op_src->val=1;
    }else{
        Log("%x",op_src->val);
        op_src->val=0;
    }
    print_asm_template1();
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"
