//
// Created by Eric Zhao on 11/9/2021.
//

#include "cpu/exec/helper.h"

make_helper(hlt){
  if(cpu.INTR){
	return 1;
  }
  return 0;
}