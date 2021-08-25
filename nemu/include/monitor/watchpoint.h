#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	struct watchpoint *next;
  struct watchpoint *prev;
  char exp[64];
  uint32_t data;
	/* TODO: Add more members if necessary */
} WP;
WP * head;
WP * new_wp(char *exp);
void free_wp(WP *wp);
#endif
