#include <stdio.h>
#include <stdint.h>
#include "FLOAT.h"
#include <sys/mman.h>

extern char _vfprintf_internal;
extern char _fpmaxtostr;
extern char _ppfs_setargs;
extern int __stdio_fwrite(char *buf, int len, FILE *stream);

__attribute__((used)) static int format_FLOAT(FILE *stream, FLOAT f)
{
	/* TODO: Format a FLOAT argument `f' and write the formating
	 * result to `stream'. Keep the precision of the formating
	 * result with 6 by truncating. For example:
	 *              f          result
	 *         0x00010000    "1.000000"
	 *         0x00013333    "1.199996"
	 */
	char si = (f >> 31) & 0x1 ? '-' : '\0';
	if (si)
		f *= -1;
	int intNum = f >> 16;
	int helper[] = {
		0,
		500000000,
		250000000,
		125000000,
		62500000,
		31250000,
		15625000,
		7812500,
		3906250,
		1953125,
		976562,
		488281,
		244140,
		122070,
		61035,
		30517,
		15258,
	};
	int i, fruc;
	for (i = 1; i <= 16; i++)
	{
		if ((f & 0xffff) & (1 << (16 - i)))
		{
			fruc += helper[i];
		}
	}
	fruc /= 1000;
	char buf[80];
	int len = sprintf(buf, "%c%d.%06u", si, intNum, fruc);
	return __stdio_fwrite(buf, len, stream);
}

static void modify_vfprintf()
{
	const int offset = 0x306;
	void *ptCall = (void *)((uint32_t)&_vfprintf_internal + offset);
	*(int32_t *)(ptCall + 1) += ((int32_t)&format_FLOAT - (int32_t)&_fpmaxtostr);
	void *ptArg = ptCall - 0xa;
	*(uint8_t *)(ptArg - 1) = 0x8;
	*(uint8_t *)ptArg = 'R';
	*(uint16_t *)(ptArg + 1) = 0x9090;
	*(uint16_t *)(ptArg-0x14)=0x9090;
	*(uint16_t *)(ptArg-0x18)=0x9090;
}

static void modify_ppfs_setargs()
{
	/* TODO: Implement this function to modify the action of preparing
	 * "%f" arguments for _vfprintf_internal() in _ppfs_setargs().
	 * Below is the code section in _vfprintf_internal() relative to
	 * the modification.
	 */
	void *ptrfunc=&_ppfs_setargs;
	void *ptrDouble = ptrfunc + 0x71;
	const int offset=0x2f;
	*(int8_t *)ptrDouble=0xEB;
	*(int8_t *)(ptrDouble+1)=offset;

#if 0
	enum {                          /* C type: */
		PA_INT,                       /* int */
		PA_CHAR,                      /* int, cast to char */
		PA_WCHAR,                     /* wide char */
		PA_STRING,                    /* const char *, a '\0'-terminated string */
		PA_WSTRING,                   /* const wchar_t *, wide character string */
		PA_POINTER,                   /* void * */
		PA_FLOAT,                     /* float */
		PA_DOUBLE,                    /* double */
		__PA_NOARG,                   /* non-glibc -- signals non-arg width or prec */
		PA_LAST
	};

	/* Flag bits that can be set in a type returned by `parse_printf_format'.  */
	/* WARNING -- These differ in value from what glibc uses. */
#define PA_FLAG_MASK (0xff00)
#define __PA_FLAG_CHAR (0x0100) /* non-gnu -- to deal with hh */
#define PA_FLAG_SHORT (0x0200)
#define PA_FLAG_LONG (0x0400)
#define PA_FLAG_LONG_LONG (0x0800)
#define PA_FLAG_LONG_DOUBLE PA_FLAG_LONG_LONG
#define PA_FLAG_PTR (0x1000) /* TODO -- make dynamic??? */

	while (i < ppfs->num_data_args) {
		switch(ppfs->argtype[i++]) {
			case (PA_INT|PA_FLAG_LONG_LONG):
				GET_VA_ARG(p,ull,unsigned long long,ppfs->arg);
				break;
			case (PA_INT|PA_FLAG_LONG):
				GET_VA_ARG(p,ul,unsigned long,ppfs->arg);
				break;
			case PA_CHAR:	/* TODO - be careful */
				/* ... users could use above and really want below!! */
			case (PA_INT|__PA_FLAG_CHAR):/* TODO -- translate this!!! */
			case (PA_INT|PA_FLAG_SHORT):
			case PA_INT:
				GET_VA_ARG(p,u,unsigned int,ppfs->arg);
				break;
			case PA_WCHAR:	/* TODO -- assume int? */
				/* we're assuming wchar_t is at least an int */
				GET_VA_ARG(p,wc,wchar_t,ppfs->arg);
				break;
				/* PA_FLOAT */
			case PA_DOUBLE:
				GET_VA_ARG(p,d,double,ppfs->arg);
				break;
			case (PA_DOUBLE|PA_FLAG_LONG_DOUBLE):
				GET_VA_ARG(p,ld,long double,ppfs->arg);
				break;
			default:
				/* TODO -- really need to ensure this can't happen */
				assert(ppfs->argtype[i-1] & PA_FLAG_PTR);
			case PA_POINTER:
			case PA_STRING:
			case PA_WSTRING:
				GET_VA_ARG(p,p,void *,ppfs->arg);
				break;
			case __PA_NOARG:
				continue;
		}
		++p;
	}
#endif

	/* You should modify the run-time binary to let the `PA_DOUBLE'
	 * branch execute the code in the `(PA_INT|PA_FLAG_LONG_LONG)'
	 * branch. Comparing to the original `PA_DOUBLE' branch, the
	 * target branch will also prepare a 64-bit argument, without
	 * introducing floating point instructions. When this function
	 * returns, the action of the code above should do the following:
	 */

#if 0
	while (i < ppfs->num_data_args) {
		switch(ppfs->argtype[i++]) {
			case (PA_INT|PA_FLAG_LONG_LONG):
			here:
				GET_VA_ARG(p,ull,unsigned long long,ppfs->arg);
				break;
			// ......
				/* PA_FLOAT */
			case PA_DOUBLE:
				goto here;
				GET_VA_ARG(p,d,double,ppfs->arg);
				break;
			// ......
		}
		++p;
	}
#endif
}

void init_FLOAT_vfprintf()
{
	modify_vfprintf();
	modify_ppfs_setargs();
}
