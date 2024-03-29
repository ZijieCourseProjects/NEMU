#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/myelf.h"
#include "monitor/watchpoint.h"
#include "nemu.h"
#include "memory/memory.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
char *rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
	free(line_read);
	line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
	add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}
static int cmd_si(char *args) {
  if (args == NULL) {
	cpu_exec(1);
	return 0;
  }
  int steps = 0;
  int i;
  for (i = 0; i < strlen(args); i++) {
	if (args[i] > '9' || args[i] < '0') {
	  printf("valid steps!!\n");
	  return 0;
	}
	steps = steps * 10 + (args[i] - 48);
  }
  cpu_exec(steps);
  return 0;
}

static int cmd_info(char *args) {
  if (*args == 'r') {
	printf("eax:0x%08x in HEX %d in DEC\n", cpu.eax, cpu.eax);
	printf("ecx:0x%08x in HEX %d in DEC\n", cpu.ecx, cpu.ecx);
	printf("edx:0x%08x in HEX %d in DEC\n", cpu.edx, cpu.edx);
	printf("ebx:0x%08x in HEX %d in DEC\n", cpu.ebx, cpu.ebx);
	printf("esp:0x%08x in HEX %d in DEC\n", cpu.esp, cpu.esp);
	printf("ebp:0x%08x in HEX %d in DEC\n", cpu.ebp, cpu.ebp);
	printf("esi:0x%08x in HEX %d in DEC\n", cpu.esi, cpu.esi);
	printf("edi:0x%08x in HEX %d in DEC\n", cpu.edi, cpu.edi);
	printf("eip:0x%08x in HEX %d in DEC\n", cpu.eip, cpu.eip);
  } else if (*args == 'w') {
	WP *ptr = head;
	while (ptr && *ptr->exp) {
	  printf("Watchpoint %d : %s which value is %d in DEC and %x in HEX\n", ptr->NO, ptr->exp, ptr->data, ptr->data);
	  ptr = ptr->next;
	}
  } else {
	printf("invalid operation!!\n");
	return 0;
  }
  return 0;
}

static int cmd_scan(char *args) {
  bool success = true;
  swaddr_t addrToScan = expr(args, &success);
  uint32_t length;
  uint32_t step;
  sscanf(args, "%d 0x%x", &length, &addrToScan);
  if (length <= 0)
	printf("invalid length\n");
  for (step = 0; step < length; step++)
	printf("%08x:[%08x]\n", addrToScan + 4 * step, swaddr_read(addrToScan + 4 * step, 4, S_DS));
  return 0;
}

static int cmd_p(char *args) {
  bool success = true;
  uint32_t expression = expr(args, &success);
  printf("DEC:%d HEX:%x\n", expression, expression);
  return 0;
}

static int cmd_wp(char *args) {
  bool success = true;
  uint32_t data = expr(args, &success);
  if (!success) {
	printf("expr invalid!!!\n");
	return 0;
  }
  WP *newWp = new_wp(args);
  newWp->data = data;
  printf("New watchpoint %d added.The value of %s is:%d in DEC %x in HEX\n", newWp->NO, args, newWp->data, newWp->data);
  return 0;
}
static int cmd_d(char *args) {
  uint32_t no;
  sscanf(args, "%d", &no);
  free_wp(no);
  return 0;
}
static int cmd_page(char *args) {
  lnaddr_t lnaddr;
  sscanf(args,"%x",&lnaddr);
  hwaddr_t hwaddr=page_translate(lnaddr,4);
  printf("physical address: %x\n",hwaddr);
  return 0;
}
static int cmd_bt(char *args) {
  swaddr_t ptrEIP = cpu.eip, ptrEBP = cpu.ebp;
  int no = 0;
  while (ptrEBP) {
	printf("#%d %s with ", no, findfunc(ptrEIP));
	ptrEIP = swaddr_read(ptrEBP + 4, 4, S_SS);
	int i = 0;
	for (; i < 4; i++) {
	  printf("args%d: %d in DEC and %x in HEX ||",
			 i,
			 swaddr_read(ptrEBP + 8 + 4 * i, 4, S_SS),
			 swaddr_read(ptrEBP + 8 + 4 * i, 4, S_SS));
	}
	printf("\n");
	no++;
	ptrEBP = swaddr_read(ptrEBP, 4, S_SS);
  }
  return 0;
}
static int cmd_help(char *args);

static struct {
  char *name;
  char *description;
  int (*handler)(char *);
} cmd_table[] = {
	{"help", "Display informations about all supported commands", cmd_help},
	{"c", "Continue the execution of the program", cmd_c},
	{"q", "Exit NEMU", cmd_q},
	{"si", "Step continue the program", cmd_si},
	{"info", "Print some value", cmd_info},
	{"x", "scan the memory", cmd_scan},
	{"d", "delete a watchpoint", cmd_d},
	{"w", "add new watchpoint", cmd_wp},
	{"bt", "backtrace", cmd_bt},
	{"page","show page translate result",cmd_page},
	{"p", "print something", cmd_p}
	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
	/* no argument given */
	for (i = 0; i < NR_CMD; i++) {
	  printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
	}
  } else {
	for (i = 0; i < NR_CMD; i++) {
	  if (strcmp(arg, cmd_table[i].name) == 0) {
		printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		return 0;
	  }
	}
	printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void ui_mainloop() {
  while (1) {
	char *str = rl_gets();
	char *str_end = str + strlen(str);

	/* extract the first token as the command */
	char *cmd = strtok(str, " ");
	if (cmd == NULL) { continue; }

	/* treat the remaining string as the arguments,
	 * which may need further parsing
	 */
	char *args = cmd + strlen(cmd) + 1;
	if (args >= str_end) {
	  args = NULL;
	}

#ifdef HAS_DEVICE
	extern void sdl_clear_event_queue(void);
	sdl_clear_event_queue();
#endif

	int i;
	for (i = 0; i < NR_CMD; i++) {
	  if (strcmp(cmd, cmd_table[i].name) == 0) {
		if (cmd_table[i].handler(args) < 0) { return; }
		break;
	  }
	}

	if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
