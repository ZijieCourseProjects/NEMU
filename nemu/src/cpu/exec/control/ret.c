#include "cpu/exec/helper.h"
#define DATA_BYTE 1
#include "cpu/exec/template-start.h"
make_helper(ret){
    if(ops_decoded.is_operand_size_16){
        cpu.eip=MEM_R(cpu.esp)&0x0000FFFF;
        cpu.esp+=2;
    }else{
        cpu.eip=MEM_R(cpu.esp);
        cpu.esp+=4;
    }
    return 1;
}
