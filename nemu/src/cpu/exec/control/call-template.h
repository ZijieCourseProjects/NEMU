#include "cpu/exec/template-start.h"

#define instr call

static void do_execute(){
        cpu.esp-=DATA_BYTE;
        MEM_W(cpu.esp, cpu.eip+DATA_BYTE+1);
        cpu.eip+=op_src->val;
    print_asm_template1();
}

make_instr_helper(i)

make_helper(concat(call_rm_,SUFFIX)){
        cpu.esp-=DATA_BYTE;
        MEM_W(cpu.esp, cpu.eip+DATA_BYTE+1);
        swaddr_t next = instr_fetch(eip+1,DATA_BYTE);
        Log("%x",next);
        cpu.eip=next;
        return 1+DATA_BYTE;
}

#include "cpu/exec/template-end.h"
