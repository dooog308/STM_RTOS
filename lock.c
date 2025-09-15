#include"lock.h"

extern void PRINTFC(char* tx, uint8_t len);
extern void PRINTFI(uint32_t tx);

uint8_t LoadandStore(spinlock *lock){
	uint8_t ret = 1;
	if(lock->owe==1) return 1;
	 __asm volatile(
			"MOV R1, #0X1\n"
			"LDREX R0, [%[addr]]\n"
			"CMP R0, #0\n"
			"ITT EQ\n"
			"STREXEQ R0, R1, [%[addr]]\n"
			"MOVEQ %[ret], R0\n"
			:[ret]"+r"(ret), [addr]"+r"(lock)
			:
			:"r0", "r1"
				   );
	return ret;
}

uint8_t init_spinlock(spinlock *lock){
	lock->owe = 0;
	lock->tid = 0;
	return 0;
}

uint8_t try_spinlock(spinlock *lock){
	while(LoadandStore(lock));
	return 0;
}
uint8_t free_spinlock(spinlock *lock){
	lock->owe = 0;
	lock->tid = 0;
}

