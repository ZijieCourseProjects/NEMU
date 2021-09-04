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
    int blockSize;
    int size;
    int blockBit;
    int groupBit;
    int wayBit;
    int groupSize;
    int waySize;
    CacheLine lines[CACHE_SIZE/CACHE_BLOCK_SIZE];

    void (* init)(struct Cache *this);
} Cache;



void concat(init_,CACHENAME)(struct Cache *this){
    int i=0;
    this->blockSize=CACHE_BLOCK_SIZE;
    this->size=CACHE_SIZE;
    this->wayBit=CACHE_WAY_BIT;
    this->groupBit=CACHE_GROUP_BIT;
    this->blockBit=CACHE_BLOCK_BIT;
    this->groupSize=(1 << CACHE_GROUP_BIT);
    this->waySize=(1<<CACHE_WAY_BIT);

    for(;i<CACHE_SIZE/CACHE_BLOCK_SIZE;i++){
        (this->lines[i]).valid=false;
        #ifdef DIRTY
            this->lines[i].ditry=false; 
        #endif
    }
}