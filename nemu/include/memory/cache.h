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

    void (* init)(struct Cache *this,int numOfLines);
} Cache;

