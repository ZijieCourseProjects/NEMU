#include "common.h"
#include <stdio.h>
#include <stdlib.h>



#define CACHENAME cacheL1
#define CACHE_BLOCK_SIZE 64
#define CACHE_SIZE 64*1024
#define CACHE_WAY_BIT 3
#define CACHE_GROUP_BIT 7
#define CACHE_BLOCK_BIT 6

#include "memory/cache-template.h"

Cache cacheL1;


#undef CACHENAME
#undef CACHE_BLOCK_SIZE
#undef CACHE_SIZE
#undef CACHE_WAY_BIT
#undef CACHE_GROUP_BIT
#undef CACHE_BLOCK_BIT



void initCache(){
    cacheL1.init=init_cacheL1;
    cacheL1.init(&cacheL1);
}

int read_cache_hit(hwaddr_t addr,Cache *this){
    uint32_t groupIndex = (addr >> this->blockBit) & (this->groupSize -1);
    uint32_t tag = (addr >> (this->groupSize+this->blockBit));

    int i,group=groupIndex*this->waySize;
    for(i = group;i<group+this->waySize;i++){
        if(this->lines[i].valid==true && this->lines[i].tag==tag){
            return i;
        }
    }
    return -1;
}

int read_cacheL1(hwaddr_t addr){
    int ans = read_cache_hit(addr,&cacheL1);
    if(ans!=-1){
        return ans; 
    }
    return 0;
}