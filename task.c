#include<stdint.h>
#include "task.h"
#include "syscall.h"
#include "mem.h"
#include "stm32f4xx.h"

Tblock TCB[MAXTHREAD];
Tblock DefaultTCB;
volatile Tblock *curTCB;
uint8_t curtid=0;
uint32_t fir=0;
uint32_t curStackHead=USER_STACK_START;
uint32_t ticks;

extern void PRINTFC(char* tx, uint8_t len);
extern void PRINTFI(uint32_t tx);
extern uint32_t _etext , _sdata, _edata, _sbss, _ebss;

void Systick_Handler(void){
	__disable_irq();
	__set_CONTROL(__get_CONTROL()&(~(0X1)));
	ticks++;
	if(ticks%1==0){
		SCB->ICSR |= (1<<28);
	}
	__asm volatile("CLREX");
	__set_CONTROL(__get_CONTROL()|(0X1));
	__enable_irq();
}


void set_curTCB(Tblock *task){
	curTCB = task;
	set_user_region(task->sp_start, task->size);
}

int add_task(void (*task)(void)){
	uint32_t AllignStack=0, StackTop=0;
    AllignStack=base_roundup(curStackHead, DEFAULT_TASK_SIZE);
	StackTop=AllignStack-DEFAULT_TASK_SIZE;

	TCB[curtid].tid = curtid;
	TCB[curtid].sp = AllignStack-4;
	TCB[curtid].sp_start = StackTop;
	TCB[curtid].size = DEFAULT_TASK_SIZE;
	TCB[curtid].status = create;
	TCB[curtid].wait = 0;

	PRINTFC("tid: ", 5);
	PRINTFI(curtid);
	PRINTFC(" sp: ", 5);
	PRINTFI((uint32_t)TCB[curtid].sp);
	PRINTFC("\r\n", 2);

	*((uint32_t*)TCB[curtid].sp) = (uint32_t)(0x01000000);
	TCB[curtid].sp = (TCB[curtid].sp-4);
	*((uint32_t*)TCB[curtid].sp) = ((uint32_t)task);//&0XFFFFFFFE);
	TCB[curtid].sp = (TCB[curtid].sp-4);
	*((uint32_t*)TCB[curtid].sp) = 0;
	TCB[curtid].sp = (TCB[curtid].sp-20);
	//TCB[curtid].sp = (TCB[curtid].sp-4);
//	*((uint32_t*)TCB[curtid].sp) = 0;
//	TCB[curtid].sp = (TCB[curtid].sp-36);
	curTCB = &TCB[curtid];
	uint32_t tem=0;
	tem = 0XFFFFFFFD;
	__asm volatile(
			"LDR R3, =curTCB\n"
			"LDR R2, [R3]\n"
		    "LDR R0, [R2]\n"
			"DSB\n"
			"MOV R4, #10\n"
			"MOV R1, #0\n"
			"MOV R5, #11\n"
			"MOV R6, #12\n"
			"LDR R8, %[EXEC]\n"
			"STMDB R0!, {R8}\n"
			"STMDB R0!, {R1,R8-R11}\n"
			"STMDB R0!, {R4-R6}\n"
			"STR R0,[R2]\n"
			"DSB\n"
			"ISB\n"
			:
			:[EXEC]"m"(tem)
			:"r0","r1","r2","r3","r4","r5","r6","r8","r9","r10","r11");
	curtid++;
	curStackHead = StackTop;
}

void start_schedule(void){
	uint32_t AllignStack=0, StackTop=0;

    AllignStack=base_roundup(curStackHead, DEFAULT_TASK_SIZE);
	StackTop=AllignStack-DEFAULT_TASK_SIZE;

	DefaultTCB.tid = 0;
	DefaultTCB.sp = (AllignStack-4);
	DefaultTCB.sp_start = StackTop;
	DefaultTCB.size = DEFAULT_TASK_SIZE; 
	DefaultTCB.status = ready;
	DefaultTCB.wait = 0;

	set_curTCB(&DefaultTCB);
	
	__disable_irq();
	__set_PSP((uint32_t)curTCB->sp);
	__set_CONTROL(__get_CONTROL()|(0X1<<1));
	__DSB();
	__ISB();
	__enable_irq();
	SysTick_Config(1000000);
	sys_yield();
	int i=0;
	while(1){
		i++;
		if(i%100000==0) PRINTFC("T0\r\n",4);
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
//	task->status = ready;
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

