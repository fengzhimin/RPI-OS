#ifndef __MMU_H__
#define __MMU_H__

#define PAGE_TABLE_L1_BASE_ADDR_MASK      (0xffffc000)
#define PHYSICAL_MEM_ADDR                 0x00000000
#define VIRTUAL_MEM_ADDR                  0x00000000
#define MEM_MAP_SIZE                      0x8000000
#define PHYSICAL_KERNEL_ADDR              0x00000000
#define VIRTUAL_KERNEL_ADDR               0xC0000000
#define PHYSICAL_IO_ADDR                  0x20000000
#define VIRTUAL_IO_ADDR                   0x7E000000
#define IO_MAP_SIZE                       0x8000000

void mmu_small(unsigned int vadd, unsigned int padd, unsigned int flags, unsigned int mmubase);
void init_sys_mmu(void);
void start_mmu(void);

#endif
