#include "macro.h"
#include <stdlib.h>

void concat(init_,CACHENAME)(struct Cache *this){
    int i=0;
    this->blockSize=CACHE_BLOCK_SIZE;
    this->size=CACHE_SIZE;
    this->wayBit=CACHE_WAY_BIT;
    this->groupBit=CACHE_GROUP_BIT;
    this->blockBit=CACHE_BLOCK_BIT;
    this->groupSize=(1 << CACHE_GROUP_BIT);
    this->waySize=(1<<CACHE_WAY_BIT);
    this->lines=malloc(sizeof(CacheLine)*(CACHE_SIZE/CACHE_BLOCK_SIZE));

    for(;i<CACHE_SIZE/CACHE_BLOCK_SIZE;i++){
        (this->lines[i]).valid=false;
        (this->lines[i]).data=malloc(sizeof(uint8_t)*CACHE_BLOCK_SIZE);
        #ifdef DIRTY
            this->lines[i].dirty=false; 
        #endif
    }
}