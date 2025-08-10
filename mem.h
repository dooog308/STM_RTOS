#ifndef __MEM
#define __MEM

#include<stdint.h>

#define SRAM_START        (0x20000000)
#define FLASH_START       (0x08000000)
#define STMPERH_START     (0x40000000)
#define STACK_START       (SRAM_START+192*1024)
#define KERNEL_SIZE       (32*1024)
#define USER_STACK_START  (STACK_START-KERNEL_SIZE)

#define SRAM_REGION       0
#define TEXT_REGION       1
#define STMPERH_REGION    2
#define MAINSTACK_REGION  3
#define USRSTACK_REGION   4

#define SRAM_attribute  ARM_MPU_RASR_EX(0UL, ARM_MPU_AP_FULL, ARM_MPU_ACCESS_NORMAL(2,2,1), 0X00UL, ARM_MPU_REGION_SIZE_256KB)
#define TEXT_attribute  ARM_MPU_RASR_EX(0UL, ARM_MPU_AP_RO, ARM_MPU_ACCESS_NORMAL(2,2,0), 0X00UL, ARM_MPU_REGION_SIZE_2MB)
#define STMPERH_attribute  ARM_MPU_RASR_EX(1UL, ARM_MPU_AP_FULL, ARM_MPU_ACCESS_DEVICE(1), 0X00UL, ARM_MPU_REGION_SIZE_512MB)
#define MAINSTACK_attribute  ARM_MPU_RASR(0, ARM_MPU_AP_PRIV, 0, 0, 1, 1, 0X00, ARM_MPU_REGION_SIZE_32KB)

extern uint32_t _etext , _sdata, _edata, _sbss, _ebss;

void MemoryMAFault_Handler(void);
void HardFault_Handler(void);
void mpu_init(void);

#endif
