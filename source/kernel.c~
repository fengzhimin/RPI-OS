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

void kernel_main(void)
{
	init_sys_mmu();
	start_mmu();
	//start_MMU((unsigned int)(&__end), 0x00000001|0x1000|0x0004);
	//start_MMU(L1_PTR_BASE_ADDR, 0x00000001|0x1000|0x0004);
	//invalidate_tlbs();

	uart_init();
	//char *_ch = "Hello, PRI kernel World!!";
	init_arm_timer(Kernel_1Hz);
	_enable_interrupts();
//      uart_puts(_ch);
//	uart_puts(_ch);
	int i = 0;

	
	while(1)
	{
		char *_ch = "0000\n";
		IntToString(ArmTimer->Value, _ch);
		for(i = 0; i < strlen(_ch); i++)
			uart_putc(_ch[i]);
		sleep(500);
	}

	
	char *_ch1 = "init mmu success!";
	for(i = 0; i < strlen(_ch1); i++)
		uart_putc(_ch1[i]);
	while(1)
		uart_putc(uart_getc());
}
