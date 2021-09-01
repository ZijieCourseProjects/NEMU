#ifndef __MY_ELF__
#define __MY_ELF__

extern swaddr_t findvar(char *varName,bool *success);
extern char* findfunc(swaddr_t ret);

#endif