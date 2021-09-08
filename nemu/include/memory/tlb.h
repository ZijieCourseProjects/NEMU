//
// Created by Eric Zhao on 8/9/2021.
//

#ifndef NEMU2021_NEMU_INCLUDE_MEMORY_TLB_H_
#define NEMU2021_NEMU_INCLUDE_MEMORY_TLB_H_
#define TLB_SIZE 64
#include "common.h"

typedef struct {
  uint32_t virtualPageNumber:20;
  uint32_t physicalPageNumber:20;
  bool valid;
}tlbentry;

tlbentry tlb[TLB_SIZE];

void init_tlb();
uint32_t read_tlb(uint32_t tag);
void update_tlb(uint32_t,uint32_t);
#endif //NEMU2021_NEMU_INCLUDE_MEMORY_TLB_H_
