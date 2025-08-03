#ifndef __PROG
#define __PROG

#define STACK_START       (0x20000000+192*1024)
#define KERNEL_SIZE       (50*1024)
#define DEFAULT_TASK_SIZE (200)
#define USER_STACK_START  (STACK_START-KERNEL_SIZE)
#define MAXTHREAD         10

typedef struct{
	uint8_t tid;
	uint32_t *sp;
}Tblock; 

void Systick_Handler(void);
int add_task(uint32_t task);
void start_schedule(void);
void schedule(void);

#endif
