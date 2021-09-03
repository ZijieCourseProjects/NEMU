typedef struct CacheLine
{
    uint8_t data[CACHE_BLOCK_SIZE];
    uint32_t tag;
    bool valid;
} CacheLine;

typedef struct Cache
{
    CacheLine lines[CACHE_SIZE/CACHE_BLOCK_SIZE];

    void (* init)(struct Cache *this,int numOfLines);
} Cache;

