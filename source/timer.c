#include "../include/timer.h"
#include "../include/interrupt.h"

arm_timer_t *ArmTimer;

int sleep(int ms)
{
	volatile unsigned int *TIMER_CL0_P = (unsigned int *)TIMER_CL0;
	unsigned long current_time = 0;
	unsigned long us = 0;
	if(ms < 0)
	{
		return -1;
	}
	us = ms*1000;
	current_time = *TIMER_CL0_P;
	while((*TIMER_CL0_P - current_time) < us);
	
	return 0;
}

void init_arm_timer(unsigned int Load)
{
	ArmTimer = (arm_timer_t *)ARMTIMER_BASE;
	irq_controller_t *IRQcontroller = (irq_controller_t *)INTERRUPT_CONTROLLER_BASE;
	IRQcontroller->Enable_Basic_IRQs = BASIC_ARM_TIMER_IRQ;
	ArmTimer->Load = Load;
	ArmTimer->Reload = Load;
	ArmTimer->PreDivider = 0xF9;   //timer_clock = 250000000/(PreDivide + 1)
	ArmTimer->Control = ARMTIMER_CTRL_23BIT | 
			    ARMTIMER_CTRL_PRESCALE_1 | 
			    ARMTIMER_CTRL_INT_ENABLE | 
		 	    ARMTIMER_CTRL_ENABLE;
}
