#include "common.h"
#include <stdio.h>
#include <stdlib.h>

#define CACHENAME cacheL1
#define CACHE_BLOCK_SIZE 64
#define CACHE_SIZE 64*1024

#include "memory/cache.h"

Cache cacheL1;


#undef CACHENAME
#undef CACHE_BLOCK_SIZE
#undef CACHE_SIZE

void initCache(){
    cacheL1.init=init_cacheL1;
    cacheL1.init(&cacheL1);
}