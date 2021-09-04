#include "common.h"
#include "burst.h"
#include "memory/cache.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>


void ddr3_read_replace(hwaddr_t addr, void *data);
void dram_write(hwaddr_t addr, size_t len, uint32_t data);
void ddr3_write_replace(hwaddr_t addr, void *data, uint8_t *mask);



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


#define CACHENAME cacheL2
#define CACHE_BLOCK_SIZE 64
#define CACHE_SIZE 4*1024*1024
#define CACHE_WAY_BIT 4
#define CACHE_GROUP_BIT 12
#define CACHE_BLOCK_BIT 6
#define DIRTY

#include "memory/cache-template.h"

Cache cacheL2;


#undef CACHENAME
#undef CACHE_BLOCK_SIZE
#undef CACHE_SIZE
#undef CACHE_WAY_BIT
#undef CACHE_GROUP_BIT
#undef CACHE_BLOCK_BIT
#undef DIRTY


void initCache(){
    init_cacheL1(&cacheL1);
    init_cacheL2(&cacheL2);
}

static int read_cache_hit(hwaddr_t addr,Cache *this,uint32_t *groupStart,uint32_t *tag){
    uint32_t groupIndex = (addr >> this->blockBit) & (this->groupSize -1);
    *tag = (addr >> (this->groupSize+this->blockBit));

    int i;
    *groupStart=groupIndex*this->waySize;
    for(i = *groupStart;i<*groupStart+this->waySize;i++){
        if(this->lines[i].valid==true && this->lines[i].tag==*tag){
            return i;
        }
    }
    return -1;
}

int read_cacheL1(hwaddr_t addr){
    uint32_t group,tag;
    int ans = read_cache_hit(addr,&cacheL1,&group,&tag);
    if(ans!=-1){
        return ans; 
    }
    int p1=read_cacheL2(addr);
    srand(time(0));
    int i =group+rand()%cacheL1.waySize;
    memcpy(cacheL1.lines[i].data,cacheL2.lines[p1].data,cacheL1.blockSize);
    cacheL1.lines[i].valid=true;
    cacheL1.lines[i].tag=tag;
    return i;
}

int read_cacheL2(hwaddr_t addr){
    uint32_t group,tag;
    int ans = read_cache_hit(addr,&cacheL2,&group,&tag);
    uint32_t blockStart = (addr >> cacheL2.blockBit) << cacheL2.blockBit;
    if(ans!=-1){
        return ans;
    }
    srand(time(0));
    int i=group+rand()%cacheL2.waySize;
    /* Write back*/
    if(cacheL2.lines[i].valid && cacheL2.lines[i].dirty){
        uint8_t ret[BURST_LEN << 1];
        uint32_t addrStart=(cacheL2.lines[i].tag << (cacheL2.groupBit + cacheL2.blockBit)|((group / cacheL2.waySize) << cacheL2.blockBit));
        int w;
        memset(ret,1,sizeof(ret));
        for(w=0;w<cacheL2.blockSize/BURST_LEN;w++){
            ddr3_write_replace(addrStart+BURST_LEN*w,cacheL2.lines[i].data+BURST_LEN*w,ret);
        }
    }
    /* read in */
    int j;
    for(j=0;j<cacheL2.blockSize/BURST_LEN;j++){
        ddr3_read_replace(blockStart+BURST_LEN * j,cacheL2.lines[i].data+BURST_LEN*j);
    }
    cacheL2.lines[i].dirty=false;
    cacheL2.lines[i].valid=true;
    cacheL2.lines[i].tag=tag;
    return i;
}