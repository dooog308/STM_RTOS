#ifndef __SYSCALL
#define __SYSCALL

#define SYS_yield  0
#define SYS_sleep  1

#include<stdint.h>

void SVCall_CHandler(unsigned int *svc_arg);
void sys_yield(void);
void sys_sleep(uint16_t time);

#endif
