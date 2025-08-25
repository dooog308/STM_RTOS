#include "mem.h" 
#include "task.h"
#include "stm32f4xx.h"

extern void PRINTFC(char* tx, uint8_t len);
extern void PRINTFI(uint32_t tx);
extern Tblock *curTCB; 

void HardFault_Handler(void){
	PRINTFC("HARD FAULT\r\n", 12);
	while(1);
}
void MemoryMAFault_Handler(void){
	__disable_irq();
	__set_CONTROL(__get_CONTROL()&(~(0X1)));

	PRINTFC("MEM FAULT id: ", 14);
	PRINTFI(curTCB->tid);
	PRINTFC(" address: ", 10);
	PRINTFI(SCB->MMFAR);
	PRINTFC("\r\n",2);
	curTCB->status = kill;
	SCB->ICSR |= (1<<28);
	__set_CONTROL(__get_CONTROL()|(0X1));
	__enable_irq();
	__ISB();
}

uint8_t memsize(uint32_t size){
	uint8_t i=0;
	while(size){
		i++;
		size = (size>>1);
	}
	return i-2;
}

void mpu_init(void){
	__disable_irq();
	ARM_MPU_Disable();

	ARM_MPU_SetRegionEx(TEXT_REGION, FLASH_START, 					TEXT_attribute);
    ARM_MPU_SetRegionEx(SRAM_REGION, SRAM_START,
		  SRAM_attribute);	
	ARM_MPU_SetRegionEx(STMPERH_REGION, STMPERH_START,
		  STMPERH_attribute);

	ARM_MPU_ClrRegion(6);
	ARM_MPU_ClrRegion(7);

	ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk);
	__enable_irq();
}

void set_user_region(uint32_t base, uint32_t size){
	__disable_irq();
	
	ARM_MPU_ClrRegion(USRSTACK_REGION);
	ARM_MPU_ClrRegion(FREE_REGION2);
	ARM_MPU_ClrRegion(FREE_REGION1);	
	
	ARM_MPU_SetRegionEx(USRSTACK_REGION, base, ARM_MPU_RASR_EX(1UL, ARM_MPU_AP_FULL, ARM_MPU_ACCESS_NORMAL(2,2,1), 0X00UL, memsize(size)));

	__enable_irq();
	__DSB();
	__ISB();
}
