#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ, NUM,HEXNUM,REG,NEQ,AND,OR,NOT,DEREF,NEG

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				// spaces
	{"\\+", '+'},					// plus
	{"-", '-'},					// sub
	{"\\*", '*'},					// multi
	{"/", '/'},					// devide
	{"\\(", '('},					// left
  {"\\$[a-zA-Z]{2,3}",REG}, //register
  {"0[xX][0-9a-fA-F]+",HEXNUM},
  {"[1-9][0-9]*",NUM}, //decimal number
	{"\\)", ')'},					// right 
  {"!=",NEQ},
  {"&&",AND},
  {"\\|\\|",OR},
	{"==", EQ},						// equal
  {"!",NOT}
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;
int tokenCount=0;
static bool make_token(char *e) {
tokenCount=0;
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 &&
          pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i,
            rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        if(tokenCount>31)
            assert(0);
        switch (rules[i].token_type) {
        case '+':
        case '(':
        case ')':
        case '-':
        case '*':
        case AND:
        case OR:
        case NOT:
        case NEQ:
        case '/':
        case EQ:
            tokens[tokenCount++].type=rules[i].token_type;
            break;
        case NUM:
        case HEXNUM:
        case REG:
            tokens[tokenCount].type=rules[i].token_type;
            strncpy(tokens[tokenCount++].str, substr_start, substr_len);
            break;
        }
        break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}
	return true;
}

bool checkParentheses(int p, int q,bool *success) {
  int stk = 0, i = 0;
  for (i = p; i <= q; i++) {
    if (tokens[i].type == '(') {
      stk++;
    } else if (tokens[i].type == ')') {
      stk--;
      if (stk == -1) {
          *success=false;
          return 0;
      }
      if (stk == 0) {
        if (tokens[p].type=='(' && i== q) {
          return 1;
        }else{
            return 0;
        }
      }
    }
  }
  return false;
}

//uint32_t strNum(char *str) {
//  int num = 0;
//  bool neg=false;
//  if (*str=='-'){
//      str++;
//      neg=true;
//  }
//  while (*str) {
//    num = num * 10 + (*str - '0');
//    str++;
//  }
//  if(neg){
//      num=-num;
//  }
//  return num;
//
uint32_t strNum(char * str,int type){
    int ret;
    if(type==HEXNUM)
        sscanf(str,"%x",&ret);
    if(type==NUM)
        sscanf(str,"%d",&ret);
    return ret;
}
void replaceToken() {
  int i = 0;
  for (; i < tokenCount; i++) {
      if(tokens[i].type=='*'&&(i==0||(tokens[i-1].type!=NUM&&tokens[i-1].type!=HEXNUM))){
        uint32_t data=swaddr_read(strNum(tokens[i+1].str, tokens[i+1].type), 4);
        tokens[i].type=DEREF;
        tokens[i+1].type=HEXNUM;
        sprintf(tokens[i+1].str,"%x",data);
      }else if(tokens[i].type=='-'&&(tokens[i-1].type=='('||tokens[i-1].type=='+'||tokens[i-1].type=='-'||tokens[i-1].type=='*'||tokens[i-1].type=='/')){
          tokens[i].type=NEG;
      }else if (tokens[i].type == REG) {
      if (strlen(tokens[i].str) == 4) {
        switch (tokens[i].str[2]) {
        case 'A':
        case 'a':
          sprintf(tokens[i].str, "%d", cpu.gpr[0]._32);
          break;
        case 'c':
        case 'C':
          sprintf(tokens[i].str, "%d", cpu.gpr[1]._32);
          break;
        case 'd':
        case 'D':
          if (tokens[i].str[3] == 'x' || tokens[i].str[3] == 'X')
            sprintf(tokens[i].str, "%d", cpu.gpr[2]._32);
          if (tokens[i].str[3] == 'i' || tokens[i].str[3] == 'I')
            sprintf(tokens[i].str, "%d", cpu.gpr[7]._32);
          break;
        case 'B':
        case 'b':
          if (tokens[i].str[3] == 'x' || tokens[i].str[3] == 'X')
            sprintf(tokens[i].str, "%d", cpu.gpr[3]._32);
          if (tokens[i].str[3] == 'p' || tokens[i].str[3] == 'P')
            sprintf(tokens[i].str, "%d", cpu.gpr[5]._32);
          break;
        case 'S':
        case 's':
          if (tokens[i].str[3] == 'p' || tokens[i].str[3] == 'P')
            sprintf(tokens[i].str, "%d", cpu.gpr[4]._32);
          if (tokens[i].str[3] == 'i' || tokens[i].str[3] == 'I')
            sprintf(tokens[i].str, "%d", cpu.gpr[6]._32);
          break;
        }
      } else if (strlen(tokens[i].str) == 3){
        switch (tokens[i].str[1]) {
        case 'A':
        case 'a':
          if (tokens[i].str[2] == 'x' || tokens[i].str[2] == 'X')
            sprintf(tokens[i].str, "%d", cpu.gpr[0]._16);
           if (tokens[i].str[2] == 'l' || tokens[i].str[2] == 'L')
            sprintf(tokens[i].str, "%d", cpu.gpr[0]._8[0]);
            if (tokens[i].str[2] == 'h' || tokens[i].str[2] == 'H')
            sprintf(tokens[i].str, "%d", cpu.gpr[0]._8[1]);
          break;
        case 'c':
        case 'C':
           if (tokens[i].str[2] == 'x' || tokens[i].str[2] == 'X')
            sprintf(tokens[i].str, "%d", cpu.gpr[1]._16);
           if (tokens[i].str[2] == 'l' || tokens[i].str[2] == 'L')
            sprintf(tokens[i].str, "%d", cpu.gpr[1]._8[0]);
            if (tokens[i].str[2] == 'h' || tokens[i].str[2] == 'H')
            sprintf(tokens[i].str, "%d", cpu.gpr[1]._8[1]);
          break;
        case 'd':
        case 'D':
          if (tokens[i].str[2] == 'x' || tokens[i].str[2] == 'X')
            sprintf(tokens[i].str, "%d", cpu.gpr[2]._16);
          if (tokens[i].str[2] == 'i' || tokens[i].str[2] == 'I')
            sprintf(tokens[i].str, "%d", cpu.gpr[7]._16);
            if (tokens[i].str[2] == 'l' || tokens[i].str[2] == 'L')
            sprintf(tokens[i].str, "%d", cpu.gpr[2]._8[0]);
            if (tokens[i].str[2] == 'h' || tokens[i].str[2] == 'H')
            sprintf(tokens[i].str, "%d", cpu.gpr[2]._8[1]);
         break;
        case 'B':
        case 'b':
          if (tokens[i].str[2] == 'x' || tokens[i].str[2] == 'X')
            sprintf(tokens[i].str, "%d", cpu.gpr[3]._16);
          if (tokens[i].str[2] == 'p' || tokens[i].str[2] == 'P')
            sprintf(tokens[i].str, "%d", cpu.gpr[5]._16);
             if (tokens[i].str[2] == 'l' || tokens[i].str[2] == 'L')
            sprintf(tokens[i].str, "%d", cpu.gpr[3]._8[0]);
            if (tokens[i].str[2] == 'h' || tokens[i].str[2] == 'H')
            sprintf(tokens[i].str, "%d", cpu.gpr[3]._8[1]);
          break;
        case 'S':
        case 's':
          if (tokens[i].str[2] == 'p' || tokens[i].str[2] == 'P')
            sprintf(tokens[i].str, "%d", cpu.gpr[4]._16);
          if (tokens[i].str[2] == 'i' || tokens[i].str[2] == 'I')
            sprintf(tokens[i].str, "%d", cpu.gpr[6]._16);
          break;
        }
      }
    tokens[i].type = NUM;
    }
  }
}
uint32_t eval(int p, int q, bool *success) {
    replaceToken();
  if (p > q) {
    *success = false;
    return 0;
  } else if (p == q) {
    return strNum(tokens[p].str,tokens[p].type);
  }else if (checkParentheses(p, q, success) == true) {
    return eval(p + 1, q - 1, success);
  } else {
    if (*success == false) {
      return 0;
      Log("Invalid operation");
    }
    int i = p;
    char op = 0;
    for (; i < q; i++) {
        if(tokens[i].type=='('){
            int count=0;
            do{
                if(tokens[i].type=='(')
                    count++;
                else if(tokens[i].type==')')
                    count--;
                i++;
            }while(count);
        }
        if(i>=q)
            break;
        if((op==0 || tokens[(int)op].type=='*' || tokens[(int)op].type=='/')&& (tokens[i].type=='*'||tokens[i].type =='/'))
            op=i;
        if(tokens[i].type=='+'||tokens[i].type=='-')
            op=i;
        if(tokens[i].type==NOT||tokens[i].type==EQ||tokens[i].type==NEQ||tokens[i].type==AND||tokens[i].type==OR||tokens[i].type==DEREF)
            op=i;
       }
    if(p+1==q&&tokens[p].type==NEG)
        op=p;
    int val1=0;
    if(p!=op)
        val1=eval(p, op - 1, success);
    int val2 = eval(op + 1, q, success);
    switch (tokens[(int)op].type) {
    case '+':
      return val1 + val2;
      break;
    case '-':
      return val1 - val2;
      break;
    case '*':
      return val1 * val2;
      break;
    case '/':
      return val1 / val2;
      break;
    case EQ:
      return val1==val2;
      break;
    case NEQ:
      return val1!=val2;
      break;
    case AND:
      return val1&&val2;
      break;
    case OR:
      return val1||val2;
      break;
    case NOT:
      return !val2;
      break;
    case NEG:
      return -val2;
      break;
    case DEREF:
    default:
      return val2;
    }
  }
}

uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  
  int ans=eval(0, tokenCount-1, success);
  int i=0;
  for (; i < 32; ++i) {
      tokens[i].type=0;
      memset(tokens[i].str,0,32);
  }
  return ans;
  /* TODO: Insert codes to evaluate the expression. */
  panic("please implement me");
  return 0;
}
