#include "../include/uart.h"
#include "../include/timer.h"

extern void _enable_interrupts();
extern void _disable_interrupts();
extern arm_timer_t *ArmTimer;

void common_undef_handler(void)
{
	sleep(500);
	char *_ch = "undefine interrupts\r\n";
	uart_puts(_ch);
}

void common_swi_handler(void)
{
	sleep(500);
	char *_ch = "swi interrupts\r\n";
	uart_puts(_ch);
}

void common_pref_handler(void)
{
	char *_ch = "prefetch interrupts\r\n";
	uart_puts(_ch);
	unsigned int register_value[8];
	asm(
					"str r0, %0\n"
					"str r1, %1\n"
					"str r2, %2\n"
					"str r3, %3\n"
					"str r4, %4\n"
					"str r13, %5\n"
					"str r14, %6\n"
					"str r15, %7\n"
					: "=m"(register_value[0]), "=m"(register_value[1]), "=m"(register_value[2]),
					  "=m"(register_value[3]), "=m"(register_value[4]), "=m"(register_value[5]),
						"=m"(register_value[6]), "=m"(register_value[7])
	);

	int i;
	char *_temp = "0000000000\r\n";
	for(i = 0; i < 8; i++)
	{
			HexToString(register_value[i], _temp);
			uart_puts(_temp);
	}

		sleep(5000);
}

void common_Dabt_handler(void)
{
	sleep(500);
	char *_ch = "data abort interrupts\r\n";
	uart_puts(_ch);
}

void common_res_handler(void)
{
	sleep(500);
	char *_ch = "reserved interrupts\r\n";
	uart_puts(_ch);
}

void common_irq_handler(void)
{
	//sleep(500);
	ArmTimer->IRQClear = 0x0;
	char *_ch = "timer interrupts\r\n";
	uart_puts(_ch);
}


void common_fiq_handler(void)
{
	char *_ch = "fiq interrupts\r\n";
	uart_puts(_ch);
}
