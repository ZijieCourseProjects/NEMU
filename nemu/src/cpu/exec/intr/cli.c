//
// Created by Eric Zhao on 10/9/2021.
//

#include "cpu/exec/helper.h"

make_helper(cli){
  cpu.eflags.IF=0;
  print_asm("cli");
  return 1;
}
