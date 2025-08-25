#ifndef __USRSYS
#define __USRSYS

#include "syscall.h"
#include <stdint.h>

#define yield() __asm("SVC %[sid]"\
					  :           \
					  :[sid]"I"(SYS_yield));

#define sleep(a) __asm("MOV R0, %[p1]\n"\
				       "SVC %[sid]\n"   \
					   :                \
					   :[p1]"r"(a), [sid]"I"(SYS_sleep));
#endif

