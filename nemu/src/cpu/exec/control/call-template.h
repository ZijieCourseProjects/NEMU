#include "cpu/exec/template-start.h"

#define instr call

static void do_execute(){
    if(ops_decoded.is_operand_size_16){
        cpu.esp-=DATA_BYTE;
        MEM_W(cpu.esp,cpu.eip+DATA_BYTE+1);
    }else{
        cpu.esp-=DATA_BYTE;
        MEM_W(cpu.esp, cpu.eip+DATA_BYTE+1);
    }
        cpu.eip+=op_src->val;
    print_asm_template1();
}

make_instr_helper(i)

make_helper(concat(call_rm_,SUFFIX)){
    int len=concat(decode_rm_,SUFFIX)(eip+1);
    if(ops_decoded.is_operand_size_16){
        cpu.esp-=DATA_BYTE;
        MEM_W(cpu.esp,cpu.eip+len+1);
    }else{
        cpu.esp-=DATA_BYTE;
        MEM_W(cpu.esp, cpu.eip+len+1);
    }
        cpu.eip=op_src->val;
        return 0;
}

#include "cpu/exec/template-end.h"
