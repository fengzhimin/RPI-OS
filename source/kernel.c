#include "../include/mmu.h"
#include "../include/uart.h"
#include "../include/timer.h"

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif

extern unsigned int __end;
extern void start_MMU(unsigned int, unsigned int);
extern void invalidate_tlbs();
extern void _enable_interrupts();
extern void _disable_interrupts();
extern arm_timer_t *ArmTimer;

void kernel_main()
{
	int i = 0;
	init_sys_mmu();
	start_mmu();  /* 开启MMU */

	/* 重定位内核 */
	asm volatile(
		"mov r0, pc\n\t"
		"add r0, r0, #0xc0000000\n\t"
		"add lr, lr, #0xC0000000\n\t"
		"add sp, sp, #0xC0000000\n\t"
		"add pc, r0, #0xc\n\t"
	);

	/*清空恒等隐射*/
	unsigned int mmu_base = (unsigned int)(&__end);

	uart_init();
	char *_temp = "0000000000\r\n";
	unsigned int argc1 = 0;
	HexToString(&argc1, _temp);
	uart_puts(_temp);
	//char *_ch = "Hello, PRI kernel World!!";
	init_arm_timer(Kernel_1Hz);
	_enable_interrupts();
//      uart_puts(_ch);
//	uart_puts(_ch);



	while(1)
	{
		char *_ch = "0000\r\n";
		IntToString(ArmTimer->Value, _ch);
		uart_puts(_ch);
		sleep(500);
	}


	char *_ch1 = "init mmu success!";
	for(i = 0; i < strlen(_ch1); i++)
		uart_putc(_ch1[i]);
	while(1)
		uart_putc(uart_getc());
}
