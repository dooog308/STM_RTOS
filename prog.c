#include<stdint.h>
#include "prog.h"
#include "stm32f4xx.h"

Tblock TCB[MAXTHREAD];
Tblock *curTCB;
uint8_t curtid=1;
uint32_t curStackHead=USER_STACK_START;
uint32_t ticks;

extern void PRINTFC(char* tx, uint8_t len);
extern uint32_t _etext , _sdata, _edata, _sbss, _ebss;

void Systick_Handler(void){
	__disable_irq();
	__set_CONTROL(__get_CONTROL()&(~(0X1)));
	ticks++;
	if(ticks%100==0) context_switch();
	__set_CONTROL(__get_CONTROL()&(~(0X1<<1)));
	__set_CONTROL(__get_CONTROL()|(0X1));
	__enable_irq();
}

void context_switch(void){
	curTCB->sp = (uint32_t*)__get_PSP();
	schedule();
	__set_PSP((uint32_t)curTCB->sp);
}

int add_task(uint32_t task){
	curStackHead -= DEFAULT_TASK_SIZE;
	if(curStackHead<=_ebss) return -1;

	TCB[curtid].tid = curtid;
	TCB[curtid].sp = (uint32_t*)curStackHead;
	TCB[curtid].status = ready;
	TCB[curtid].wait = 0;
	
	*(TCB[curtid].sp) = (uint32_t)(0x01000000);
	TCB[curtid].sp =(uint32_t*)((uint32_t)TCB[curtid].sp-4);
	*(TCB[curtid].sp) = task;
	TCB[curtid].sp =(uint32_t*)((uint32_t)TCB[curtid].sp-24);
	curtid++;
}

void start_schedule(void){
	TCB[0].tid = 0;
	TCB[0].sp = (uint32_t*)USER_STACK_START;
	TCB[0].status = ready;
	TCB[0].wait = 0;
	curTCB = &TCB[0];
	
	__set_PSP((uint32_t)curTCB->sp);
	__set_CONTROL(__get_CONTROL()|(0X1<<1));
	SysTick_Config(1000000);
	int i=0;
	while(1){
		i++;
		if(i%100000==0)PRINTFC("T0\r\n",4);
		//__set_CONTROL(__get_CONTROL()&(~(0X1<<1)));
	};
}

void schedule(void){
	uint8_t i=(curTCB->tid+1)%curtid;
	if(i==0)i=1;
	while(TCB[i].wait!=0){
		if(TCB[i].status==sleep_up&&TCB[i].wait>=ticks
	||TCB[i].status==sleep_down&&TCB[i].wait<=ticks){
			TCB[i].status = running;
			TCB[i].wait = 0;
		}
		i++;
	}
	curTCB = &TCB[i];
}

