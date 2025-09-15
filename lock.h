#ifndef __LOCK
#define __LOCK

#include<stdint.h>

typedef struct{
	uint8_t owe;
	uint8_t tid;
}spinlock;

uint8_t LoadandStore(spinlock *lock);
uint8_t init_spinlock(spinlock *lock);
uint8_t try_spinlock(spinlock *lock);
uint8_t free_spinlock(spinlock *lock);

#endif
