//
// Created by Eric Zhao on 10/9/2021.
//

#include "cpu/exec/helper.h"

void raise_intr(uint8_t);

make_helper(intr){
  int NO=instr_fetch(eip+1,1);
  cpu.eip+=2;
  print_asm("int %x",NO);
  raise_intr(NO);
  return 0;
}
