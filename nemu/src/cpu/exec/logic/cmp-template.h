#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute(){
    DATA_TYPE result=op_dest->val-op_src->val;
    Log("dest:%x,src:%x,result:%x",op_dest->val,op_src->val,result);
    update_eflags_pf_zf_sf((DATA_TYPE_S)result);
      cpu.eflags.CF = result > op_dest->val;
      cpu.eflags.OF = MSB((op_dest->val ^ op_src->val) & (op_dest->val ^ result));
    print_asm_template2();
}
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif

make_instr_helper(r2rm)
make_instr_helper(rm2r)
make_instr_helper(i2rm)

#if DATA_BYTE == 1
make_helper(cmp_al_b){
    int8_t simm = instr_fetch(eip+1,1);
    DATA_TYPE result=(cpu.eax&0xFF)-simm;
    update_eflags_pf_zf_sf((int8_t)result);
      cpu.eflags.CF = result > (cpu.eax&0xFF);
      cpu.eflags.OF = MSB(((cpu.eax&0xFF) ^ simm) & (simm ^ result));
      return 2;
}
#endif

#include "cpu/exec/template-end.h"
