#include<stdint.h>
#include "syscall.h"
#include "prog.h"
#include "stm32f4xx.h"


extern Tblock *curTCB;
extern uint64_t ticks;
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
			sys_sleep((uint32_t)svc_arg[0]);
			break;
	}
}

void sys_yield(void){
	__disable_irq();
	context_switch();
	__enable_irq();
}

void sys_sleep(uint16_t time){
	__disable_irq();
	time *= 1000;
	if(time+ticks<ticks) curTCB->status=sleep_down;
	else curTCB->status=sleep_up;
	curTCB->wait = time+ticks;
	context_switch();
	__enable_irq();	
}
