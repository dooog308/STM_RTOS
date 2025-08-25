#include<stdint.h>
#include "task.h"
#include "mem.h"
#include "stm32f4xx.h"

Tblock TCB[MAXTHREAD];
Tblock DefaultTCB;
volatile Tblock *curTCB;
uint8_t curtid=0;
uint32_t curStackHead=USER_STACK_START;
uint32_t ticks;

extern void PRINTFC(char* tx, uint8_t len);
extern void PRINTFI(uint32_t tx);
extern uint32_t _etext , _sdata, _edata, _sbss, _ebss;

void Systick_Handler(void){
	__disable_irq();
	__set_CONTROL(__get_CONTROL()&(~(0X1)));
	ticks++;
	if(ticks%100==0){
		SCB->ICSR |= (1<<28);
	}
	__set_CONTROL(__get_CONTROL()|(0X1));
	__enable_irq();
}


void set_curTCB(Tblock *task){
	curTCB = task;
	set_user_region(task->sp_start, task->size);
}

int add_task(uint32_t task){
	uint32_t AllignStack=0, StackTop=0;
    AllignStack=base_roundup(curStackHead, DEFAULT_TASK_SIZE);
	StackTop=AllignStack-DEFAULT_TASK_SIZE;

	TCB[curtid].tid = curtid;
	TCB[curtid].sp = (uint32_t*)(AllignStack-4);
	TCB[curtid].sp_start = StackTop;
	TCB[curtid].size = DEFAULT_TASK_SIZE;
	TCB[curtid].status = create;
	TCB[curtid].wait = 0;

	PRINTFC("tid: ", 5);
	PRINTFI(curtid);
	PRINTFC(" sp: ", 5);
	PRINTFI((uint32_t)TCB[curtid].sp);
	PRINTFC("\r\n", 2);

	*(TCB[curtid].sp) = (uint32_t)(0x01000000);
	TCB[curtid].sp =(uint32_t*)((uint32_t)TCB[curtid].sp-4);
	*(TCB[curtid].sp) = task;
	TCB[curtid].sp =(uint32_t*)((uint32_t)TCB[curtid].sp-24);
	curTCB = &TCB[curtid];
	__asm volatile(
				  "LDR R3, =curTCB\n"
				  "LDR R2, [R3]\n"
				  "STMDB R0!, {R4-R11}\n"
				  "STR R0, [R2]\n"
				  "DSB\n"
				  "ISB\n"
				  :
				  :
				  :"r0","r2","r3","r7","r4","r5","r6","r8","r9","r10","r11");
	curtid++;
	curStackHead = StackTop;
}

void start_schedule(void){
	uint32_t AllignStack=0, StackTop=0;

    AllignStack=base_roundup(curStackHead, DEFAULT_TASK_SIZE);
	StackTop=AllignStack-DEFAULT_TASK_SIZE;

	DefaultTCB.tid = 0;
	DefaultTCB.sp =  (uint32_t*)(AllignStack-4);
	DefaultTCB.sp_start = StackTop;
	DefaultTCB.size = DEFAULT_TASK_SIZE; 
	DefaultTCB.status = ready;
	DefaultTCB.wait = 0;

	set_curTCB(&DefaultTCB);
	
	__set_PSP((uint32_t)curTCB->sp);
	__set_CONTROL(__get_CONTROL()|(0X1<<1));
	SysTick_Config(1000000);
	int i=0;
	while(1){

		if(i%100000==0);//PRINTFC("T0\r\n",4);
	}
}

uint8_t che_runnable(Tblock *task){
	if(task->status==sleep_up&&task->wait<ticks) 
			return 0;
	if(task->status==sleep_down&&task->wait>ticks) 
			return 0;
	if(task->status==kill)
			return 0;
	task->wait=0;
	task->status = ready;
	return 1;
}
void schedule(void){
	__disable_irq();
	uint8_t i=(curTCB->tid+1)%curtid;
	while(!che_runnable(&TCB[i])){
	    i++;
		i %= curtid;
	}
	set_curTCB(&TCB[i]);
	__enable_irq();
}

