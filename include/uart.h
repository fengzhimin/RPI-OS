#ifndef __UART_H__
#define __UART_H__

#include "base.h"

#define AUX_ENABLES     0x7E215004
#define AUX_MU_IO_REG   0x7E215040
#define AUX_MU_IER_REG  0x7E215044
#define AUX_MU_IIR_REG  0x7E215048
#define AUX_MU_LCR_REG  0x7E21504C
#define AUX_MU_MCR_REG  0x7E215050
#define AUX_MU_LSR_REG  0x7E215054
#define AUX_MU_MSR_REG  0x7E215058
#define AUX_MU_SCRATCH  0x7E21505C
#define AUX_MU_CNTL_REG 0x7E215060
#define AUX_MU_STAT_REG 0x7E215064
#define AUX_MU_BAUD_REG 0x7E215068
#define GPFSEL1    0x7E200004UL
#define GPPUD      0x7E200094UL
#define GPPUDCLK0  0x7E200098UL
#define IRQ_DISABLE1 0x7E00B21C
#define IRQ_ENABLE1 0x7E00B210

void mmio_write(unsigned int reg, unsigned int data);
unsigned int mmio_read(unsigned int reg);
void uart_init ();
void uart_putc ( unsigned int c );
unsigned char uart_getc();
int strlen(const char* str);
void uart_write(const unsigned char* buffer, int size);
void uart_puts(const char* str);
void IntToString(unsigned int _int, char *_char);
void HexToString(unsigned int _int, char *_char);

#endif
