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
#define PAGE_SHIFT         12
#define PGDR_SHIFT         20
#define PAGE_SIZE          (0x1<<PAGE_SHIFT)
#define PGDR_SIZE          (0x1<<14)
#define PAGE_MASK          (PAGE_SIZE-1)
#define PGDR_MASK          (PGDR_SIZE-1)
#define PAGE_ROUNDUP(x)   (((x)+PAGE_MASK)&(~PAGE_MASK))

#define PTE_V   0x001 /* Valid */
#define PTE_W   0x002 /* Read/Write */
#define NR_KERN_PAGETABLE    20
#define VADDR(pdi, pti) ((unsigned int)(((pdi)<<PGDR_SHIFT)|((pti)<<PAGE_SHIFT)))
#define KERNBASE  VADDR(0xC00, 0)
#define R(x) ((x)+KERNBASE)

#define MMU_BASE(x)        ((x|PGDR_MASK)&~PGDR_MASK)

void mmu_small(unsigned int vadd, unsigned int padd, unsigned int flags, unsigned int mmubase);
void init_sys_mmu(void);
void start_mmu(void);
unsigned int init_paging(unsigned int physfree);

#endif
