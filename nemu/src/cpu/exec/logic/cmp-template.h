#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute(){
    DATA_TYPE result=op_dest->val-op_src->val;
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

make_helper(concat(cmp_eax_,SUFFIX)){

  DATA_TYPE b=instr_fetch(eip+1,DATA_BYTE);
	DATA_TYPE result = REG(R_EAX) - b ;
  update_eflags_pf_zf_sf((DATA_TYPE_S)result);
  cpu.eflags.CF = result > REG(R_EAX);
  cpu.eflags.OF = MSB((REG(R_EAX) ^ b) & (REG(R_EAX) ^ result));
	print_asm_template1();
	return 1+DATA_BYTE;
}
#include "cpu/exec/template-end.h"
