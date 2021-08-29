#include "cpu/exec/template-start.h"

#define instr push

static void do_execute(){
    cpu.esp-=(ops_decoded.is_operand_size_16?2:4);
    MEM_W(cpu.esp, op_src->val);
    print_asm_template1();
}


make_instr_helper(r)

make_helper(concat(push_m_,SUFFIX)){
  cpu.esp-=DATA_BYTE;
  MEM_W(cpu.esp,MEM_R(instr_fetch(eip+1,DATA_BYTE)));
  print_asm_template1();
  return 1+DATA_BYTE;
}

#include "cpu/exec/template-end.h"
