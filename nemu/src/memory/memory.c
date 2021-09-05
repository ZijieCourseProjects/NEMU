#include "burst.h"
#include "common.h"
#include "cpu/reg.h"
#include "memory/cache.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

lnaddr_t seg_translate(swaddr_t addr, size_t len, uint8_t sreg) {
  return addr + cpu.segReg[sreg].invisiblePart.base_15_0 +
         (cpu.segReg[sreg].invisiblePart.base_23_16 << 16) +
         (cpu.segReg[sreg].invisiblePart.base_31_24 << 24);
}

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
  // return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
  int l1_1st_line = read_cacheL1(addr);
  uint32_t offset = addr & (cacheL1.blockSize - 1);
  uint8_t ret[BURST_LEN << 1];
  if (offset + len > cacheL1.blockSize) {
    int l1_2nd_line = read_cacheL1(addr + cacheL1.blockSize - offset);
    memcpy(ret, cacheL1.lines[l1_1st_line].data + offset,
           cacheL1.blockSize - offset);
    memcpy(ret + cacheL1.blockSize - offset, cacheL1.lines[l1_2nd_line].data,
           len - (cacheL1.blockSize - offset));
  } else {
    memcpy(ret, cacheL1.lines[l1_1st_line].data + offset, len);
  }

  int tmp = 0;
  uint32_t ans = unalign_rw(ret + tmp, 4) & (~0u >> ((4 - len) << 3));
  return ans;
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
  write_cacheL1(addr, len, data);
  // dram_write(addr, len, data);
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
  return hwaddr_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
  hwaddr_write(addr, len, data);
}

uint32_t swaddr_read(swaddr_t addr, size_t len, uint8_t sreg) {
#ifdef DEBUG
  assert(len == 1 || len == 2 || len == 4);
#endif
  if (cpu.cr0.protect_enable) {
    lnaddr_t lnaddr = seg_translate(addr, len, sreg);
    return lnaddr_read(lnaddr, len);
  }
  return lnaddr_read(addr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data, uint8_t sreg) {
#ifdef DEBUG
  assert(len == 1 || len == 2 || len == 4);
#endif
  if (cpu.cr0.protect_enable) {
    lnaddr_t lnaddr = seg_translate(addr, len, sreg);
    return lnaddr_write(lnaddr, len, data);
  }
  lnaddr_write(addr, len, data);
}
