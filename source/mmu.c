#include "../include/mmu.h"

extern void PUT32(unsigned int , unsigned int);
extern unsigned int GET32(unsigned int);
extern unsigned int __end;

void mmu_small(unsigned int vadd, unsigned int padd, unsigned int flags, unsigned int mmubase)
{
	unsigned int ra;
	unsigned int rb;
	unsigned int rc;
	unsigned int mmu_base = MMU_BASE((unsigned int)(&__end));

	ra = vadd >> PGDR_SHIFT;
	rb = (mmu_base&PAGE_TABLE_L1_BASE_ADDR_MASK) | (ra << 2);
	rc = (mmubase&0xFFFFFC00)|PTE_V;   /*总共有2^8项 每项占4K*/
	PUT32(rb, rc);   //first level descriptor
	ra = (vadd>>PAGE_SHIFT)&0xFF;
	rb = (mmubase&0xFFFFFC00)|(ra << 2);
	rc = (padd&0xFFFFF000)|0xFF|flags|PTE_W;
	PUT32(rb, rc);   //second level descriptor
}

void init_sys_mmu(void)
{
	int i;

	unsigned int mmu_L2_base = MMU_BASE((unsigned int)(&__end))+PGDR_SIZE;

	for(i = 0; i < (MEM_MAP_SIZE>>PAGE_SHIFT); i++)
		mmu_small(VIRTUAL_MEM_ADDR+(i<<PAGE_SHIFT), PHYSICAL_MEM_ADDR+(i<<PAGE_SHIFT), 0, mmu_L2_base+i*4);
	for(i = 0; i < (MEM_MAP_SIZE>>PAGE_SHIFT); i++)
		mmu_small(VIRTUAL_KERNEL_ADDR+(i<<PAGE_SHIFT), PHYSICAL_KERNEL_ADDR+(i<<PAGE_SHIFT), 0, mmu_L2_base+((MEM_MAP_SIZE>>PAGE_SHIFT)+i)*4);
	for(i = 0; i < (IO_MAP_SIZE>>PAGE_SHIFT); i++)
		mmu_small(VIRTUAL_IO_ADDR+(i<<PAGE_SHIFT), PHYSICAL_IO_ADDR+(i<<PAGE_SHIFT), 0, mmu_L2_base+(2*(MEM_MAP_SIZE>>PAGE_SHIFT)+i)*4);
}

void start_mmu(void)
{
	unsigned int ttb = MMU_BASE((unsigned int)(&__end));
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

unsigned int init_paging(unsigned int physfree)
{
    unsigned int i;
    unsigned int *pgdir, *pte;

    /*
     * 分配页目录
     */
    pgdir=(unsigned int *)physfree;
    physfree += PGDR_SIZE;
    pte = (unsigned int *)physfree;

    /*分配20张小页表 并且将填充页目录*/
    for(i = 0; i < NR_KERN_PAGETABLE; i++)
    {
        pgdir[i] = pgdir[i+(KERNBASE>>PGDR_SHIFT)] = (physfree&0xFFFFFC00)|PTE_V;
        physfree += (1<<10);
    }

    /*设置恒等隐射，填充小页表 隐射物理地址为[0, &__end] 虚拟地址[0, &__end] [KERNBASE, R(&__end)]*/
    for(i = 0; i < (PAGE_ROUNDUP(physfree)>>PAGE_SHIFT); i++)
    {
        pte[i] = ((i<<PAGE_SHIFT)&0xFFFFF000)|0xFF|PTE_W;
    }

    /*设置IO端口隐射，物理地址为[20000000, 20220000] 虚拟地址[0xC1000000 0xC1220000]
    从第17张小页表开始隐射IO设备地址 第17张小页表的第一项index是0x1000
    */
    for(i = 0; i < 0x220; i++)
        pte[0x1000+i] = ((0x20000000+(i<<PAGE_SHIFT))&0xFFFFF000)|0xFF|PTE_W;

    /*
     * 打开分页
     */
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
             : "r" (pgdir)//(pgdir+0xC0000000)
             : "r0"
     );

    return physfree;
}
