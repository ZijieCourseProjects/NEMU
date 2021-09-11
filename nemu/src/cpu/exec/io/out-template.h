#include "cpu/exec/template-start.h"
#include "device/port-io.h"
#define instr out

make_helper (concat(out_si2a_, SUFFIX)) {
  uint8_t imm8 = instr_fetch(cpu.eip + 1, 1);
  pio_write(imm8, DATA_BYTE, REG(R_EAX));
  print_asm("out\t%x", imm8);
  return 2;
}

make_helper (concat(out_r2rm_, SUFFIX)) {
  pio_write(reg_w(R_EDX), DATA_BYTE, REG(R_EAX));
  print_asm("out");
  return 1;
}

#include "cpu/exec/template-end.h"