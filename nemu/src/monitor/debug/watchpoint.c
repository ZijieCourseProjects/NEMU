#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *free_;

void init_wp_pool() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_pool[i].NO = i;
		wp_pool[i].next = &wp_pool[i + 1];
	}
	wp_pool[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_pool;
}

WP* new_wp(char * exp){
    if(free_==NULL){
        Log("Too much watchpoints");
        assert(0);
    }
    WP * ret=free_;
    memcpy(ret->exp, exp, strlen(exp)+1);
    ret->next=head;
    if(head)
        head->prev=ret;
    head =ret;
    free_=free_->next;
    return ret;
}

void free_wp(WP *wp){
    wp->prev->next=wp->next;
    wp->next->prev=wp->prev;
    wp->next=free_;
    memset(wp->exp, 0, 64);
    wp->prev=NULL;
    free_=wp;
    return;
}
/* TODO: Implement the functionality of watchpoint */


