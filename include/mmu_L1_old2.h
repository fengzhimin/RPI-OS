#ifndef __MMU_H__
#define __MMU_H__

#define PAGE_TABLE_L1_BASE_ADDR_MASK      (0xffffc000)
#define VIRT_TO_PTE_L1_INDEX(addr)        (((addr) & 0xfff00000)>>18)
#define PTE_L1_SECTION_NO_CACHE_AND_WB    (0x0<<2)
#define PTE_L1_SECTION_DOMAIN_DEFAULT     (0x0<<5)
#define PTE_ALL_AP_L1_SECTION_DEFAULT     (0x0<<10)
#define PTE_L1_SECTION_PADDR_BASE_MASK    (0xfff00000)
#define PTE_BITS_L1_SECTION               (0x2)
#define L1_PTR_BASE_ADDR                  0x00200000
#define PHYSICAL_MEM_ADDR                 0x00000000
#define VIRTUAL_MEM_ADDR                  0x00000000
#define MEM_MAP_SIZE                      0x800000
#define PHYSICAL_IO_ADDR                  0x20200000
#define VIRTUAL_IO_ADDR                   0x7E200000
#define IO_MAP_SIZE                       0x800000

unsigned int gen_ll_pte(unsigned int paddr);
unsigned int gen_ll_pte_addr(unsigned int baddr, unsigned int vaddr);
void init_sys_mmu(void);
void start_mmu(void);

#endif
