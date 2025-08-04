#include<stdint.h>
#include "syscall.h"
#include "stm32f4xx.h"


extern void PRINTFC(char* tx, uint8_t len);
extern void PRINTFI(uint32_t tx);
extern void context_switch();

void SVCall_CHandler(unsigned int *svc_arg){
	unsigned int sysid=((char*)svc_arg[6])[-2];
	switch(sysid){
		case 0:
			sys_yield();
			break;
		case 1:
			break;
		case 54:
			break;
	}
}

void sys_yield(void){
	__disable_irq();
	context_switch();
	__enable_irq();
}
