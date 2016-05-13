GNU = arm-none-eabi-
CFLAGS = -mcpu=arm1176jzf-s -fpic -ffreestanding -I ../include
OBJ = ../kernel.img
SOURCE = kernel.o boot.o linker.ld mmu.o uart.o interrupt.o timer.o \
				 task.o

all:
	make ${OBJ}
	rm -f *.o *.elf
	mountU.sh

${OBJ}:kernel.elf
	${GNU}objdump -D kernel.elf > kernel.list
	${GNU}objcopy $^ -O binary $@

boot.o:boot.S
	${GNU}gcc ${CFLAGS} -c $^ -o $@

kernel.o:kernel.c
	${GNU}gcc ${CFLAGS} -std=gnu99 -c $^ -o $@ -O2 -Wall -Wextra

mmu.o:mmu.c
	${GNU}gcc ${CFLAGS} -std=gnu99 -c $^ -o $@ -Wall -Wextra

uart.o:uart.c
	${GNU}gcc ${CFLAGS} -std=gnu99 -c $^ -o $@ -Wall -Wextra

interrupt.o:interrupt.c
	${GNU}gcc ${CFLAGS} -std=gnu99 -c $^ -o $@ -Wall -Wextra

timer.o:timer.c
	${GNU}gcc ${CFLAGS} -std=gnu99 -c $^ -o $@ -Wall -Wextra

task.o:task.c
	${GNU}gcc ${CFLAGS} -std=gnu99 -c $^ -o $@ -Wall -Wextra

kernel.elf:$(SOURCE)
	${GNU}gcc -T linker.ld -o $@ -ffreestanding -O2 -nostdlib $(SOURCE)

clean:
	rm -f ${OBJ}
	rm -f *.o
	rm -f *.elf
	rm -f *.img
	rm -f *.list
