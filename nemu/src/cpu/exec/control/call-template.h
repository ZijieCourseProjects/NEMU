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
        concat(decode_rm_,SUFFIX)(eip);
        cpu.esp-=DATA_BYTE;
        MEM_W(cpu.esp, cpu.eip+DATA_BYTE+1);
        Log("%x",op_src->val);
        cpu.eip=op_src->val;
        return 0;
}

#include "cpu/exec/template-end.h"
