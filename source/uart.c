#include "../include/uart.h"

void mmio_write(unsigned int reg, unsigned int data)
{
        *(volatile unsigned int *)reg = data;
}

unsigned int mmio_read(unsigned int reg)
{
        return *(volatile unsigned int *)reg;
}

void uart_init ()
{
    mmio_write(IRQ_DISABLE1, 1<<29);
    unsigned int ra;

    mmio_write(AUX_ENABLES,1);
    mmio_write(AUX_MU_IER_REG,0);
    mmio_write(AUX_MU_CNTL_REG,0);
    mmio_write(AUX_MU_LCR_REG,3);
    mmio_write(AUX_MU_MCR_REG,0);
    mmio_write(AUX_MU_IER_REG,0x5); //enable rx interrupts
    mmio_write(AUX_MU_IIR_REG,0xC6);
    mmio_write(AUX_MU_BAUD_REG, 3254);   //(250000000/(8*9600))-1

    ra=mmio_read(GPFSEL1);
    ra&=~(7<<12); //gpio14
    ra|=2<<12;    //alt5
    ra&=~(7<<15); //gpio15
    ra|=2<<15;    //alt5
    mmio_write(GPFSEL1,ra);

    mmio_write(GPPUD,0);
    mmio_write(GPPUDCLK0,(1<<14)|(1<<15));

    mmio_write(GPPUDCLK0,0);

    mmio_write(AUX_MU_CNTL_REG,3);

    mmio_write(IRQ_ENABLE1, 1<<29);
}

void uart_putc ( unsigned int c )
{
    while(1)
    {
        if(mmio_read(AUX_MU_LSR_REG)&0x20) break;
    }
    mmio_write(AUX_MU_IO_REG,c);
}

unsigned char uart_getc()
{
    while(1)
    {
        if(mmio_read(AUX_MU_LSR_REG)&0x1) break;
    }
    return mmio_read(AUX_MU_IO_REG);
}


int strlen(const char* str)
{
        int ret = 0;
        while ( (unsigned int)str[ret] != 0 )
                ret++;
        return ret;
}


void uart_write(const unsigned char* buffer, int size)
{
        for ( int i = 0; i < size; i++ )
                uart_putc(buffer[i]);
}


void uart_puts(const char* str)
{
        uart_write((const unsigned char*) str, strlen(str));
}

void IntToString(unsigned int _int, char * _char)
{
	int i = 0;
	for(i = 0; i < 4; i++)
	{
		_char[3-i] = _int%10 + '0';
		_int /= 10;
	}
}
