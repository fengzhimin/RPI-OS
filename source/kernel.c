#include "../include/mmu.h"
#include "../include/uart.h"
#include "../include/timer.h"
#include "../include/task.h"

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif

extern unsigned int __end;
extern void _enable_interrupts();
extern void _disable_interrupts();
extern unsigned int *PTD;
extern unsigned int *PT;
extern arm_timer_t *ArmTimer;

void task_idle0(void)
{
  while(1)
  {
    int i;
    for(i = 0; i < 10; i++)
    {
      uart_puts("Hello first task create success!");
      uart_putc(i+'0');
      uart_puts("\r\n");
    }
  }
}

void task_idle1(void)
{
  int i;
  while(1)
  {
    for(i = 0; i < 10; i++)
    {
      uart_puts("Hello second task create success!");
      uart_putc(i+'0');
      uart_puts("\r\n");
    }
  }
}

void task_idle2(void)
{
  int i;
  while(1)
  {
    for(i = 0; i < 10; i++)
    {
      uart_puts("Hello third task create success!");
      uart_putc(i+'0');
      uart_puts("\r\n");
    }
  }
}

void task_idle3(void)
{
  int i;
  while(1)
  {
    for(i = 0; i < 10; i++)
    {
      uart_puts("Hello fourth task create success!");
      uart_putc(i+'0');
      uart_puts("\r\n");
    }
  }
}

void kernel_main()
{

	int i = 0;
	//init_sys_mmu();
	//start_mmu();  /* 开启MMU */
	unsigned int end = (unsigned int)(&__end+PGDR_MASK)&0xffffc000;
  unsigned int physfree =	init_paging(end-0xC0000000);
	init_ram(physfree);

	/* 重定位内核*/
	asm volatile(
		"add sp, sp, #0xC0000000\n\t"
	);

	/*清空恒等隐射*/
	for(i = 1; i < NR_KERN_PAGETABLE; i++)
		PTD[i] = 0;

	uart_init();
	char *_temp = "0000000000\r\n";
	unsigned int temp;
	HexToString((unsigned int)&temp, _temp);
	uart_puts(_temp);
	task_init();
	unsigned char rank = MAX_rank ;
	unsigned int task_func = (unsigned int)task_idle0;
	unsigned char TID = task_create( rank , task_func);
	task_run(TID);
	task_func = (unsigned int)task_idle1;
	TID = task_create( rank , task_func);
	task_run(TID);
	task_func = (unsigned int)task_idle2;
	TID = task_create( rank , task_func);
	task_run(TID);
  task_func = (unsigned int)task_idle3;
	TID = task_create( rank , task_func);
	task_run(TID);
	init_arm_timer(Kernel_1Hz);
	_enable_interrupts();

	while(1)
	{
		char *_ch = "0000000000\r\n";
		for(i = 0; i < 20; i++)
		{
			HexToString(PTD[i], _ch);
			uart_puts(_ch);
		}
		for(i = 0; i < 20; i++)
		{
			HexToString(PTD[i+0xC00], _ch);
			uart_puts(_ch);
		}
		sleep(500);
	}
}
