#ifndef __PROG
#define __PROG

#include"mem.h"

#define DEFAULT_TASK_SIZE (200)
#define MAXTHREAD         10

#define ready             0
#define running           1
#define block             2
#define kill              3
#define sleep_up          4
#define sleep_down        5

typedef struct{
	uint32_t *sp;
	uint8_t tid;
	uint8_t status;
	uint32_t wait;
}Tblock; 

void Systick_Handler(void);
void context_switch(void);
int add_task(uint32_t task);
void start_schedule(void);
uint8_t che_runnable(Tblock *task);
void schedule(void);

#endif
