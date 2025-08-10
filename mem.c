#include "mem.h"
#include "task.h"
#include "stm32f4xx.h"

extern void PRINTFC(char* tx, uint8_t len);
extern Tblock *curTCB; 

void HardFault_Handler(void){
	PRINTFC("HARD FAULT\r\n", 12);
	while(1);
}
void MemoryMAFault_Handler(void){
	__disable_irq();
	__set_CONTROL(__get_CONTROL()&(~(0X1)));
	PRINTFC("MEM FAULT\r\n", 11);
	curTCB->status = kill;
	context_switch();
	__set_CONTROL(__get_CONTROL()|(0X1));
	__enable_irq();
}

void mpu_init(void){
	__disable_irq();
	ARM_MPU_Disable();

	ARM_MPU_SetRegionEx(TEXT_REGION, FLASH_START, 					TEXT_attribute);
    ARM_MPU_SetRegionEx(SRAM_REGION, SRAM_START,
		  SRAM_attribute);	
	ARM_MPU_SetRegionEx(STMPERH_REGION, STMPERH_START,
		  STMPERH_attribute);	

	ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk);
	__enable_irq();
}
