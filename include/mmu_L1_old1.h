#ifndef __MMU_H__
#define __MMU_H__

#define PAGE_TABLE_L1_BASE_ADDR_MASK      (0xffffc000)
#define PHYSICAL_MEM_ADDR                 0x00000000
#define VIRTUAL_MEM_ADDR                  0x00000000
#define MEM_MAP_SIZE                      0x800000
#define PHYSICAL_IO_ADDR                  0x20200000
#define VIRTUAL_IO_ADDR                   0x7E200000
#define IO_MAP_SIZE                       0x800000

void mmu_section(unsigned int mmu_base, unsigned int vadd, unsigned int padd, unsigned int flags);
void init_sys_mmu(void);
void start_mmu(void);

#endif
