//
// Created by Eric Zhao on 10/9/2021.
//

#include <setjmp.h>
#include <cpu/reg.h>
#include <memory/memory.h>
#include "common.h"

extern jmp_buf jbuf;
void seg_load(int8_t);
lnaddr_t seg_translate(swaddr_t addr, size_t len, uint8_t sreg);

void raise_intr(uint8_t NO){
  /*Save the status*/
  cpu.esp-=4;
  swaddr_write(cpu.esp,4,cpu.eflags.val,S_SS);
  cpu.esp-=2;
  swaddr_write(cpu.esp,2,cpu.segReg[S_SS].visiblePart,S_SS);
  cpu.esp-=4;
  swaddr_write(cpu.esp,4,cpu.eip,S_SS);

  uint32_t high= lnaddr_read(cpu.idtr.baseAddr+sizeof(struct GateDescriptor)*NO,4);
  uint32_t low= lnaddr_read(cpu.idtr.baseAddr+sizeof (struct GateDescriptor)*NO +4,4);
  cpu.segReg[S_CS].visiblePart=(low >> 16)&0xFFFF;
  seg_load(S_CS);
  uint32_t offset=(high&0xFFFF0000)|(low&0xFFFF);
  cpu.eip= seg_translate(offset,4,S_CS);

  longjmp(jbuf,1);
}
