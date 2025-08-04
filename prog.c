#include<stdint.h>
#include "prog.h"
#include "stm32f4xx.h"

Tblock TCB[MAXTHREAD];
Tblock *curTCB;
uint8_t curtid=1;
uint32_t curStackHead=USER_STACK_START;

extern void PRINTFC(char* tx, uint8_t len);
extern uint32_t _etext , _sdata, _edata, _sbss, _ebss;

void Systick_Handler(void){
	__disable_irq();
	context_switch();
	__enable_irq();
}

void context_switch(void){
	curTCB->sp = (uint32_t*)__get_PSP();
	schedule();
	__set_PSP((uint32_t)curTCB->sp);
	__set_CONTROL(__get_CONTROL()|(0X1<<1));
}

int add_task(uint32_t task){
	curStackHead -= DEFAULT_TASK_SIZE;
	if(curStackHead<=_ebss) return -1;
	TCB[curtid].tid = curtid;
	TCB[curtid].sp = (uint32_t*)curStackHead;
	
	*(TCB[curtid].sp) = (uint32_t)(0x01000000);
	TCB[curtid].sp =(uint32_t*)((uint32_t)TCB[curtid].sp-4);
	*(TCB[curtid].sp) = task;
	TCB[curtid].sp =(uint32_t*)((uint32_t)TCB[curtid].sp-24);
	curtid++;
}

void start_schedule(void){
	TCB[0].tid = 0;
	TCB[0].sp = (uint32_t*)USER_STACK_START;

	curTCB = &TCB[0];
	__set_PSP((uint32_t)curTCB->sp);
	__set_CONTROL(__get_CONTROL()|(0X1<<1));
	SysTick_Config(16000000);
	while(1);
}

void schedule(void){
	if(curTCB==&TCB[1]) curTCB = &TCB[2];
	else curTCB = &TCB[1];
}

