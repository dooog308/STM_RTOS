#ifndef __SYSCALL
#define __SYSCALL

#define SYS_yield  0

#include<stdint.h>

void SVCall_CHandler(unsigned int *svc_arg);
void sys_yield(void);

#endif
