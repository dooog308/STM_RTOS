#include<stdint.h>
#include "syscall.h"
#include "task.h"
#include "mem.h"
#include "stm32f4xx.h"


extern Tblock *curTCB;
extern uint64_t ticks;
extern void PRINTFC(char* tx, uint8_t len);
extern void PRINTFI(uint32_t tx);

void SVCall_CHandler(unsigned int *svc_arg){
	unsigned int sysid=((char*)svc_arg[6])[-2];
	switch(sysid){
		case SYS_yield:
			sys_yield();
			break;
		case SYS_sleep:
			sys_sleep((uint32_t)svc_arg[0]);
			break;
		case SYS_malloc:
			sys_malloc((void*)svc_arg[0], (uint32_t)svc_arg[1]);
			break;
		case SYS_free:
			sys_free((void*)svc_arg[0]);
	}
}

void sys_yield(void){
	__disable_irq();
	SCB->ICSR |= (1<<28);
	__enable_irq();
}

void sys_sleep(uint16_t time){
	__disable_irq();
	time *= 1000;
	if(time+ticks<ticks) curTCB->status=sleep_down;
	else curTCB->status=sleep_up;
	curTCB->wait = time+ticks;
	SCB->ICSR |= (1<<28);
	__enable_irq();	
} 

void sys_malloc(void **ptr, uint32_t size){
	*ptr = (void*)find_space(size);
}
void sys_free(void **ptr){
	free_space((uint32_t)*ptr);
	*ptr = 0;
}
