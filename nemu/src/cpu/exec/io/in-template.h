#include "cpu/exec/template-start.h"
#include "device/port-io.h"
#define instr in

make_helper (concat(in_si2a_, SUFFIX)) {
  uint8_t imm8 = instr_fetch(cpu.eip + 1, 1);
  REG(R_EAX) = pio_read(imm8, DATA_BYTE);
  print_asm("in\t%x", imm8);
  return 2;
}
make_helper (concat(in_r2rm_, SUFFIX)) {
  REG(R_EAX) = pio_read((ioaddr_t) reg_w(R_EDX), DATA_BYTE);
  print_asm("in");
  return 1;
}

#include "cpu/exec/template-end.h"