#include "prefix/prefix.h"

#include "data-mov/mov.h"
#include "data-mov/cmovne.h"
#include "data-mov/cmov.h"
#include "data-mov/xchg.h"
#include "data-mov/movext.h"
#include "data-mov/movsx.h"
#include "data-mov/movzb.h"
#include "data-mov/movzw.h"
#include "data-mov/cltd.h"
#include "data-mov/push.h"
#include "data-mov/pop.h"
#include "data-mov/setne.h"
#include "data-mov/lgdt.h"
#include "data-mov/sete.h"

#include "arith/adc.h"
#include "arith/add.h"
#include "arith/dec.h"
#include "arith/inc.h"
#include "arith/neg.h"
#include "arith/imul.h"
#include "arith/mul.h"
#include "arith/idiv.h"
#include "arith/div.h"
#include "arith/sbb.h"
#include "arith/sub.h"

#include "control/jmp.h"
#include "control/jns.h"
#include "control/call.h"
#include "control/je.h"
#include "control/jbe.h"
#include "control/jg.h"
#include "control/jle.h"
#include "control/jl.h"
#include "control/jne.h"
#include "control/jge.h"
#include "control/js.h"
#include "control/jb.h"
#include "control/jnb.h"
#include "control/ja.h"
#include "control/ret.h"
#include "control/leave.h"


#include "logic/and.h"
#include "logic/or.h"
#include "logic/not.h"
#include "logic/xor.h"
#include "logic/sar.h"
#include "logic/shl.h"
#include "logic/shr.h"
#include "logic/shrd.h"
#include "logic/test.h"
#include "logic/bt.h"
#include "logic/cmp.h"

#include "string/rep.h"
#include "string/scas.h"
#include "string/stos.h"
#include "string/movs.h"
#include "string/lods.h"

#include "misc/misc.h"

#include "special/special.h"
