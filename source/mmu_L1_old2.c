#include "../include/mmu.h"

extern unsigned int __end;
extern void PUT32(unsigned int , unsigned int);
extern unsigned int GET32(unsigned int);

unsigned int gen_ll_pte(unsigned int paddr)
{
	return ((paddr&PTE_L1_SECTION_PADDR_BASE_MASK)|PTE_BITS_L1_SECTION);
}

unsigned int gen_ll_pte_addr(unsigned int baddr, unsigned int vaddr)
{
	return ((baddr&PAGE_TABLE_L1_BASE_ADDR_MASK)|VIRT_TO_PTE_L1_INDEX(vaddr));
}

void init_sys_mmu(void)
{
	unsigned int pte;
	unsigned int pte_addr;
	
	int j;
	unsigned int mmu_base = (unsigned int)(&__end);

	for(j = 0; j < (MEM_MAP_SIZE>>20); j++)
	{
		pte = gen_ll_pte(PHYSICAL_MEM_ADDR + (j<<20));
		pte |= PTE_ALL_AP_L1_SECTION_DEFAULT;
		pte |= PTE_L1_SECTION_NO_CACHE_AND_WB;
		pte |= PTE_L1_SECTION_DOMAIN_DEFAULT;
		pte_addr = gen_ll_pte_addr(mmu_base, VIRTUAL_MEM_ADDR+(j<<20));
		//PUT32(pte_addr, pte);
		*(volatile unsigned int *)pte_addr = pte;
	}


	for(j = 0; j < (IO_MAP_SIZE>>20); j++)
	{
		pte = gen_ll_pte(PHYSICAL_IO_ADDR + (j<<20));
		pte |= PTE_ALL_AP_L1_SECTION_DEFAULT;
		pte |= PTE_L1_SECTION_NO_CACHE_AND_WB;
		pte |= PTE_L1_SECTION_DOMAIN_DEFAULT;
		pte_addr = gen_ll_pte_addr(mmu_base, VIRTUAL_IO_ADDR+(j<<20));
		//PUT32(pte_addr, pte);
		*(volatile unsigned int *)pte_addr = pte;
	}

}

void start_mmu(void)
{
	unsigned int ttb = (unsigned int)(&__end);
	asm(
		"mcr p15,0,%0,c2,c0,0\n"
		"mvn r0,#0\n"
		"mcr p15,0,r0,c3,c0,0\n"
		"mov r0,#0x1\n"
		"mcr p15,0,r0,c1,c0,0\n"
		"mov r0,r0\n"
		"mov r0,r0\n"
		"mov r0,r0\n"
		:
		: "r" (ttb)
		: "r0"
	);
}
