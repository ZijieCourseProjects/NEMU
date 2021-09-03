#include "cpu/exec/template-start.h"

#define instr test

static void do_execute(){
    update_eflags_pf_zf_sf(op_src->val&op_dest->val);
    cpu.eflags.CF=0;
    cpu.eflags.OF=0;
    print_asm_template2();
}

make_instr_helper(r2rm)
make_instr_helper(i2rm)

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif

#if DATA_BYTE == 1
make_helper(test_al){
    update_eflags_pf_zf_sf(reg_b(R_EAX)&instr_fetch(eip+1,1));
    cpu.eflags.CF=0;
    cpu.eflags.OF=0;
    print_asm_template1();
    return 2;
}
#endif

#include "cpu/exec/template-end.h"
