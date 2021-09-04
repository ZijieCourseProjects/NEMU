typedef struct CacheLine
{
    uint32_t tag;
    bool valid;
    bool dirty;
    uint8_t *data;
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
    CacheLine *lines;

} Cache;

void initCache();
int read_cacheL1(hwaddr_t addr);
int read_cacheL2(hwaddr_t addr);