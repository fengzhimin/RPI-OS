#ifndef __TIMER_H__
#define __TIMER_H__

#define TIMER_BASE    0xC1003000
#define TIMER_CS      0xC1003000
#define TIMER_CL0     0xC1003004
#define TIMER_CHI     0xC1003008
#define TIMER_C0      0xC100300c
#define TIMER_C1      0xC1003010
#define TIMER_C2      0xC1003014
#define TIMER_C3      0xC1003018

/****
ARM timer
****/
#define ARMTIMER_BASE  0xC100B400

#define ARMTIMER_CTRL_23BIT            (1 << 1)
#define ARMTIMER_CTRL_PRESCALE_1       (0 << 2)
#define ARMTIMER_CTRL_PRESCALE_16      (1 << 2)
#define ARMTIMER_CTRL_PRESCALE_256     (2 << 2)

#define ARMTIMER_CTRL_INT_ENABLE       (1 << 5)
#define ARMTIMER_CTRL_INT_DISABLE      (0 << 5)

#define ARMTIMER_CTRL_ENABLE           (1 << 7)
#define ARMTIMER_CTRL_DISABLE          (0 << 7)

#define Kernel_1Hz      1000000
#define Kernel_10Hz     100000
#define Kernel_100Hz    10000
#define Kernel_1000Hz   1000

typedef struct {
	volatile unsigned int Load;
	volatile unsigned int Value;
	volatile unsigned int Control;
	volatile unsigned int IRQClear;
	volatile unsigned int RAWIRQ;
	volatile unsigned int MaskedIRQ;
	volatile unsigned int Reload;
	volatile unsigned int PreDivider;
	volatile unsigned int FreeRunningCounter;
} arm_timer_t;

void init_arm_timer(unsigned int Load);

int sleep(int ms);



#endif
