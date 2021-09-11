#include "cpu/exec/helper.h"

#define setcc_helper(prefix, condition) \
	make_helper(concat3(set, prefix, _rm_b)) { \
		int len = decode_rm_b(eip + 1); \
		int res = 0; \
		if (condition){ \
			res = 1; \
		} \
		write_operand_b(op_src, res); \
		return len + 1; \
	}


setcc_helper(a0nbe, (!cpu.eflags.CF && !cpu.eflags.ZF))
setcc_helper(ae0nb0nc, (!cpu.eflags.CF))
setcc_helper(b0c0nae, (cpu.eflags.CF))
setcc_helper(be0na, (cpu.eflags.CF || cpu.eflags.ZF))
// setcc_helper(c, (cpu.eflags.CF)))
setcc_helper(e0z, (cpu.eflags.ZF))
// ? 
setcc_helper(g0nle, (!cpu.eflags.ZF && cpu.eflags.SF == cpu.eflags.OF))
setcc_helper(ge0nl, (cpu.eflags.SF == cpu.eflags.OF))
setcc_helper(l0nge, (cpu.eflags.SF != cpu.eflags.OF))
setcc_helper(le0ng, (cpu.eflags.ZF || cpu.eflags.SF != cpu.eflags.OF))
// setcc_helper(na, (cpu.eflags.CF)))
// setcc_helper(nae, (cpu.eflags.CF)))
// setcc_helper(nb, (!cpu.eflags.CF)))
// setcc_helper(nbe, (!cpu.eflags.CF) && !cpu.eflags.ZF)))
// setcc_helper(nc, (!cpu.eflags.CF)))
setcc_helper(ne0nz, (!cpu.eflags.ZF))
// setcc_helper(ng, (cpu.eflags.ZF) || cpu.eflags.SF) != cpu.eflags.OF)))
// setcc_helper(nge, (cpu.eflags.SF) != cpu.eflags.OF)))
// setcc_helper(nl, (cpu.eflags.SF) == cpu.eflags.OF)))
// setcc_helper(nle, (cpu.eflags.ZF) && cpu.eflags.SF) != cpu.eflags.OF)))
setcc_helper(no, (!cpu.eflags.OF))
setcc_helper(np0po, (!cpu.eflags.PF))
setcc_helper(ns, (!cpu.eflags.SF))
// setcc_helper(nz, (!cpu.eflags.ZF)))
setcc_helper(o, (cpu.eflags.OF))
setcc_helper(p0pe, (cpu.eflags.PF))
// setcc_helper(pe, (cpu.eflags.PF)))
// setcc_helper(po, (!cpu.eflags.PF)))
setcc_helper(s, (cpu.eflags.SF))
// setcc_helper(z, (cpu.eflags.ZF)))

#undef re