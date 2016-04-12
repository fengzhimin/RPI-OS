#include "../include/uart.h"
#include "../include/timer.h"

extern void _enable_interrupts();
extern void _disable_interrupts();
extern arm_timer_t *ArmTimer;

void common_irq_handler(void)
{
	//sleep(500);
	ArmTimer->IRQClear = 0x0;
	char *_ch = "timer interrupts\n";
	
	int i;
	for(i = 0; i < strlen(_ch); i++)
		uart_putc(_ch[i]);
}


