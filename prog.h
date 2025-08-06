#ifndef __PROG
#define __PROG

#define STACK_START       (0x20000000+192*1024)
#define KERNEL_SIZE       (50*1024)
#define DEFAULT_TASK_SIZE (200)
#define USER_STACK_START  (STACK_START-KERNEL_SIZE)
#define MAXTHREAD         10

#define ready             0
#define running           1
#define block             2
#define sleep_up          3
#define sleep_down        4

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
void schedule(void);

#endif
