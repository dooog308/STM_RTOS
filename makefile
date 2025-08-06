GXX=arm-none-eabi-gcc
CFLAG=-mcpu=cortex-m4 -mthumb -nostdlib
CPPFLAG=-DSTM32F429xx \
		-Ivendor/CMSIS/Device/stm/Include \
		-Ivendor/CMSIS/CMSIS/Core/Include
LINKER_FILE=linker.ld
LDFLAG=-T $(LINKER_FILE)

all: LED.elf

LED.elf: main.o startup.o system_stm32f4xx.o prog.o syscall.o SYSCALL.o
	$(GXX) $(CFLAG) $(CPPFLAG) $(LDFLAG) -g $^ -o LED.elf
main.o: main.c usrsys.h prog.h
	$(GXX) $(CFLAG) $(CPPFLAG) -g -c main.c
prog.o: prog.c prog.h
	$(GXX) $(CFLAG) $(CPPFLAG) -g -c prog.c
syscall.o: syscall.c syscall.h prog.h
	$(GXX) $(CFLAG) $(CPPFLAG) -g -c syscall.c
SYSCALL.o: SYSCALL.s
	$(GXX) $(CFLAG) $(CPPFLAG) -g -c SYSCALL.s
startup.o: startup.c
	$(GXX) $(CFLAG) $(CPPFLAG) -g -c startup.c
system_stm32f4xx.o: vendor/CMSIS/Device/stm/Source/Templates/system_stm32f4xx.c
	$(GXX) $(CFLAG) $(CPPFLAG) -g -c vendor/CMSIS/Device/stm/Source/Templates/system_stm32f4xx.c

.PHONY: clean
clean:
	-rm *.o LED.elf
