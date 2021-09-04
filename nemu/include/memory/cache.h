#include "macro.h"
typedef struct CacheLine
{
    uint8_t data[CACHE_BLOCK_SIZE];
    uint32_t tag;
    bool valid;
    #ifdef DIRTY
        bool dirty;
    #endif

        
} CacheLine;

typedef struct Cache
{
    CacheLine lines[CACHE_SIZE/CACHE_BLOCK_SIZE];

    void (* init)(struct Cache *this);
} Cache;



void concat(init_,CACHENAME)(struct Cache *this){
    int i=0;
    for(;i<CACHE_SIZE/CACHE_BLOCK_SIZE;i++){
        (this->lines[i]).valid=false;
        #ifdef DIRTY
            this->lines[i].ditry=false; 
        #endif
    }
}