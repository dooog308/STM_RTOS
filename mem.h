#ifndef __MEM
#define __MEM

#include<stdint.h>

extern uint32_t _etext , _sdata, _edata, _sbss, _ebss;

#define SRAM_START        (0x20000000U)
#define SRAM_SIZE         (192U*1024U)
#define SRAM_END          (SRAM_START+SRAM_SIZE)
#define KERNEL_START      (SRAM_END-10*1024)
#define STACK_ADDRESS     (SRAM_END)
#define FLASH_START       (0x08000000U)
#define STMPERH_START     (0x40000000U)
#define STACK_START       (SRAM_START+SRAM_SIZE)
#define FREESPACE_START   ((uint32_t)&_ebss)
#define KERNEL_SIZE       (32U*1024U)
#define USER_STACK_START  (KERNEL_START-KERNEL_SIZE)

#define SRAM_REGION       0
#define TEXT_REGION       1
#define STMPERH_REGION    2
#define FREE_REGION1  	  3
#define FREE_REGION2  	  4
#define USRSTACK_REGION   5

#define SRAM_attribute     ARM_MPU_RASR_EX(0UL, ARM_MPU_AP_FULL, ARM_MPU_ACCESS_NORMAL(2,2,1), 0X00UL, ARM_MPU_REGION_SIZE_1KB)
#define TEXT_attribute     ARM_MPU_RASR_EX(0UL, ARM_MPU_AP_RO, ARM_MPU_ACCESS_NORMAL(2,2,0), 0X00UL, ARM_MPU_REGION_SIZE_2MB)
#define STMPERH_attribute  ARM_MPU_RASR_EX(1UL, ARM_MPU_AP_FULL, ARM_MPU_ACCESS_DEVICE(1), 0X00UL, ARM_MPU_REGION_SIZE_64MB)
#define FREE_attribute     ARM_MPU_RASR_EX(0UL, ARM_MPU_AP_FULL, ARM_MPU_ACCESS_NORMAL(2,2,1), 0X00UL, ARM_MPU_REGION_SIZE_256KB)

#define base_roundup(base, size)  ((base)-((base)%size))

void BusFault_Handler(void);
void UsageFault_Handler(void);
void MemoryMAFault_Handler(void);
void HardFault_Handler(void);
uint8_t memsize(uint32_t size);
void mpu_init(void);
void set_user_region(uint32_t base, uint32_t size);

#endif
