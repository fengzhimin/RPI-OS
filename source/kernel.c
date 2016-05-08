#include "../include/mmu.h"
#include "../include/uart.h"
#include "../include/timer.h"

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif

extern unsigned int __end;
extern void _enable_interrupts();
extern void _disable_interrupts();
extern arm_timer_t *ArmTimer;

void kernel_main()
{

	int i = 0;
	//init_sys_mmu();
	//start_mmu();  /* 开启MMU */
	unsigned int end = (unsigned int)(&__end+PGDR_MASK)&0xffffc000;
	init_paging(end-0xC0000000);

	uart_init();
	char *_temp = "0000000000\r\n";
	unsigned int temp;
	HexToString((unsigned int)&temp, _temp);
	uart_puts(_temp);
	init_arm_timer(Kernel_1Hz);
	_enable_interrupts();

	/* 重定位内核*/
	asm volatile(
		"add sp, sp, #0xC0000000\n\t"
	);

	/*清空恒等隐射*/
	unsigned int *mmu_base = ((unsigned int *)end)-0xC0000000;
	for(i = 1; i < NR_KERN_PAGETABLE; i++)
		mmu_base[i] = 0;

	while(1)
	{
		char *_ch = "0000000000\r\n";
		for(i = 0; i < 20; i++)
		{
			HexToString(mmu_base[i], _ch);
			uart_puts(_ch);
		}
		for(i = 1; i < 20; i++)
		{
			HexToString(mmu_base[i+0xC00], _ch);
			uart_puts(_ch);
		}
		sleep(500);
	}


	char *_ch1 = "init mmu success!";
	for(i = 0; i < strlen(_ch1); i++)
		uart_putc(_ch1[i]);
	while(1)
		uart_putc(uart_getc());
}
