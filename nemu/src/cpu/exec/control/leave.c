#include "cpu/exec/helper.h"
#define DATA_BYTE 4
#include "cpu/exec/template-start.h"
make_helper(leave){
    cpu.esp=cpu.ebp;
    /*only 32*/
    cpu.ebp=MEM_R(cpu.esp,S_SS);
    cpu.esp+=4;
    print_asm_template1();
    return 1;
}
