#include "cpu/exec/template-start.h"
#include "memory/tlb.h"

#define instr mov

static void do_execute() {
	OPERAND_W(op_dest, op_src->val);
	print_asm_template2();
}

make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

make_helper(concat(mov_a2moffs_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	MEM_W(addr, REG(R_EAX),S_DS);

	print_asm("mov" str(SUFFIX) " %%%s,0x%x", REG_NAME(R_EAX), addr);
	return 5;
}

make_helper(concat(mov_moffs2a_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	REG(R_EAX) = MEM_R(addr,S_DS);

	print_asm("mov" str(SUFFIX) " 0x%x,%%%s", addr, REG_NAME(R_EAX));
	return 5;
}

#if DATA_BYTE==2
static void seg_load(uint8_t segNo){
    uint16_t  DesIndex = (cpu.segReg[segNo].visiblePart>>3)&0x1FFF;
    uint32_t low= lnaddr_read(cpu.gdtr.baseAddr+8*DesIndex,4);
    uint32_t high= lnaddr_read(cpu.gdtr.baseAddr+8*DesIndex+4,4);
    cpu.segReg[segNo].invisiblePart.base_15_0=(low&0xFFFF0000) >> 16;
    cpu.segReg[segNo].invisiblePart.base_23_16=high&0xFF;
    cpu.segReg[segNo].invisiblePart.base_31_24=(high&0xFF000000) >> 24;
    cpu.segReg[segNo].invisiblePart.limit_15_0=low&0x0000FFFF;
    cpu.segReg[segNo].invisiblePart.limit_19_16=(high&0xF0000) >> 16;
}

make_helper(mov_rm2sreg){
    uint8_t modrm= instr_fetch(eip+1,1);
    uint8_t sreg_no=(modrm>>3)&0x7;
    uint8_t srcreg=(modrm&0x7);
    cpu.segReg[sreg_no].visiblePart= REG(srcreg);
    seg_load(sreg_no);
    return 2;
}
#endif
#if DATA_BYTE == 4
	make_helper(mov_r2cr){
		int len=decode_rm2r_l(eip+1);
		if(op_dest->reg==0){
		  cpu.cr0.val=op_src->val;
		}else if(op_dest->reg==3){
		  cpu.cr3.val=op_src->val;
		  init_tlb();
		}
		print_asm_template2();
		return 1+len;
	}

	make_helper(mov_cr2r){
		int len=decode_rm2r_l(eip+1);
		if(op_dest->reg==0){
		  OPERAND_W(op_src,cpu.cr0.val);
		}else if(op_dest->reg==3){
		  OPERAND_W(op_src,cpu.cr3.val);
		}
		print_asm_template2();
		return 1+len;
	}
#endif
#include "cpu/exec/template-end.h"
