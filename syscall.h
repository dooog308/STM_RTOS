#ifndef __SYSCALL
#define __SYSCALL

#define SYS_yield  0
#define SYS_sleep  1
#define SYS_malloc 2
#define SYS_free   3

#include<stdint.h>

void SVCall_CHandler(unsigned int *svc_arg);
void sys_yield(void);
void sys_sleep(uint16_t time);
void sys_malloc(void **ptr, uint32_t size);
void sys_free(void **ptr);

#endif
