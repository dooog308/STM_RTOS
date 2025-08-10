#include<stdint.h>
#include "mem.h"

#define SRAM_START             (0x20000000)
#define SRAM_SIZE              (192*1024)
#define SRAM_END               (SRAM_START+SRAM_SIZE)
#define STACK_ADDRESS          (SRAM_END)
#define ISR_VECTOR_TABLE_SIZE  (106)
 
void _default(void);
void RESET_Handler(void);
void NMI_Handler(void)__attribute__((weak, alias("_default")));
void HardFault_Handler(void)__attribute__((weak, alias("_default")));
void MemoryMAFault_Handler(void)__attribute__((weak, alias("_default")));
void BusFault_Handler(void)__attribute__((weak, alias("_default")));
void UsageFault_Handler(void)__attribute__((weak, alias("_default")));
void SVCall_Handler(void)__attribute__((weak, alias("_default")));
void Debug_Handler(void)__attribute__((weak, alias("_default")));
void PendSV_Handler(void)__attribute__((weak, alias("_default")));
void Systick_Handler(void)__attribute__((weak, alias("_default")));

uint32_t isr_vector[ISR_VECTOR_TABLE_SIZE]__attribute__
((section(".isr_vector"))) =
{
	STACK_ADDRESS,
	(uint32_t)&RESET_Handler,
	(uint32_t)&NMI_Handler,
	(uint32_t)&HardFault_Handler,
	(uint32_t)&MemoryMAFault_Handler,
	(uint32_t)&BusFault_Handler,
	(uint32_t)&UsageFault_Handler,
	0,
	0,
	0,
	0,
	(uint32_t)&SVCall_Handler,
	(uint32_t)&Debug_Handler,
	0,
	(uint32_t)&PendSV_Handler,
	(uint32_t)&Systick_Handler
};

extern uint32_t _etext , _sdata, _edata, _sbss, _ebss;
extern void mpu_init(void);
void main(void);

void _default(void)
{
	while(1);
}
void RESET_Handler(void)
{
	uint32_t DataSize=(uint32_t)&_edata-(uint32_t)&_sdata;
	uint8_t *FLASHdata=(uint8_t*)&_etext;	
	uint8_t *RAMdata=(uint8_t*)&_sdata;
	
	for(uint32_t i=0;i<DataSize;i++)
		RAMdata[i] = FLASHdata[i];	

	uint32_t BssSize=(uint32_t)&_ebss-(uint32_t)&_sbss;
	uint8_t *RAMbss=(uint8_t*)&_sbss;

	for(uint32_t i=0;i<BssSize;i++)
		RAMbss[i] = 0;	
	//mpu_init();
	main();
}

