#include "cpu/exec/template-start.h"

#define instr lidt

static void do_execute()
{
  cpu.idtr.baseAddr = lnaddr_read(op_src->addr, 2);
  if (ops_decoded.is_operand_size_16)
  {
	cpu.idtr.baseAddr = lnaddr_read(op_src->addr+ 2, 3);
  }
  else
  {
	cpu.idtr.baseAddr = lnaddr_read(op_src->addr + 2, 4);
  }

  print_asm_template1();
}

make_instr_helper(rm);

#include "cpu/exec/template-end.h"
