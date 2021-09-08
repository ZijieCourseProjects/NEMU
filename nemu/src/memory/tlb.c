//
// Created by Eric Zhao on 8/9/2021.
//

#include "memory/tlb.h"
#include "stdlib.h"
#include "time.h"

void init_tlb(){
  int i;
  for (i = 0; i < TLB_SIZE; ++i) {
	tlb[i].valid=false;
  }
}

uint32_t read_tlb(uint32_t tag){
  int i;
  for(i=0;i<TLB_SIZE;i++){
	if(tlb[i].valid && tlb[i].virtualPageNumber == tag){
	  return tlb[i].physicalPageNumber;
	}
  }
  return -1;
}

void update_tlb(uint32_t tag,uint32_t physicalPageNumber){
  int i;
  for(i=0;i<TLB_SIZE;i++){
	if (!tlb[i].valid){
	  tlb[i].virtualPageNumber=tag;
	  tlb[i].physicalPageNumber=physicalPageNumber;
	  tlb[i].valid=true;
	  return;
	}
  }
  srand(time(0)) ;
  i=rand()%TLB_SIZE;
  tlb[i].virtualPageNumber=tag;
  tlb[i].physicalPageNumber=physicalPageNumber;
}