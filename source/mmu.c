#include "../include/mmu.h"

extern void PUT32(unsigned int , unsigned int);
extern unsigned int GET32(unsigned int);
extern unsigned int __end;

void mmu_small(unsigned int vadd, unsigned int padd, unsigned int flags, unsigned int mmubase)
{
	unsigned int ra;
	unsigned int rb;
	unsigned int rc;
	unsigned int mmu_base = (unsigned int)(&__end);

	ra = vadd >> 20;
	rb = (mmu_base&PAGE_TABLE_L1_BASE_ADDR_MASK) | (ra << 2);
	rc = (mmubase&0xFFFFFC00)|1;   /*总共有2^8项 每项占4K*/
	PUT32(rb, rc);   //first level descriptor
	ra = (vadd>>12)&0xFF;
	rb = (mmubase&0xFFFFFC00)|(ra << 2);
	rc = (padd&0xFFFFF000)|0xFF|flags|2;
	PUT32(rb, rc);   //second level descriptor
}

void init_sys_mmu(void)
{
	int i;

	unsigned int mmu_base = (unsigned int)(&__end)+(0x1<<15);

	for(i = 0; i < (MEM_MAP_SIZE>>12); i++)
		mmu_small(VIRTUAL_MEM_ADDR+(i<<12), PHYSICAL_MEM_ADDR+(i<<12), 0, mmu_base+i*4);
	for(i = 0; i < (MEM_MAP_SIZE>>12); i++)
		mmu_small(VIRTUAL_KERNEL_ADDR+(i<<12), PHYSICAL_KERNEL_ADDR+(i<<12), 0, mmu_base+((MEM_MAP_SIZE>>12)+i)*4);
	for(i = 0; i < (IO_MAP_SIZE>>12); i++)
		mmu_small(VIRTUAL_IO_ADDR+(i<<12), PHYSICAL_IO_ADDR+(i<<12), 0, mmu_base+(2*(MEM_MAP_SIZE>>12)+i)*4);
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
