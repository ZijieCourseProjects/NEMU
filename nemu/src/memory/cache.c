#include "common.h"
#include <stdio.h>
#include <stdlib.h>

#define CACHE_BLOCK_SIZE 64
#define CACHE_SIZE 64*1024

#include "memory/cache.h"

void init(Cache *this,int numOfLines){
    int i=0;
    for(;i<numOfLines;i++){
        (this->lines[i]).valid=false;
        #ifdef DIRTY
            *this->lines[i].ditry=false; 
        #endif
    }
}