#include<stdint.h>
#include "stm32f4xx.h"
#include "task.h"
#include "usrsys.h"

void GPIO_init(void);
void clock_init(void);
void usart_init(void);

uint8_t k=123;

void PRINTFC(char* tx, uint8_t len){
	while(len)
	{
		while(!(USART1->SR&0X40));//等待傳送完成
		USART1->DR = *tx;
		tx++;
		len--;
	}
}
void PRINTFI(uint32_t tx){
	if(tx/10!=0) PRINTFI(tx/10);
	while(!(USART1 -> SR&0X40));//等待傳送完成
	USART1 -> DR = (tx%10)+'0';
}

extern void TEST(void);
extern Tblock TCB[MAXTHREAD];
extern Tblock *curTCB;
extern uint32_t SystemCoreClock;

void task1(void){
	uint32_t i=0;//, num[200];
	while(1){
		if(i%100000==0) PRINTFC("T1\r\n", 4);
		i++;
	}
}
void task2(void){
	uint32_t i=0, k=0, num[70];
	sleep(1250);
		
	while(1){
		i++;
		if(i%100000==0){
			PRINTFC("T2\r\n", 4);
	  
	
	//PRINTFC(" K: ", 4);
	//PRINTFI((uint32_t)&i);
	//PRINTFC("\r\n", 2);
	}
	}
}
void task3(void){
	uint32_t i=0;//,  num[200];
	uint32_t *tem;

	while(1){
		if(i%100000==0)PRINTFC("T3\r\n", 4);
		i++;
	}
}
void task4(void){
	uint32_t i=0;//, num[200];
	while(1){
		i++;
		if(i%100000==0){
				PRINTFC("T4\r\n", 4);
//PRINTFC(" I: ", 4);
//	PRINTFI((uint32_t)&i);
//	PRINTFC("\r\n", 2);

		}
	}
}

extern void mpu_init(void);

int main(){
	GPIO_init();
	clock_init();
	usart_init();
	PRINTFC("HIHI\r\n", 6);
	if(k==123){
		GPIOG->ODR |= (0X1<<13);
		GPIOG->ODR |= (0X1<<14);
	}
	add_task((uint32_t)task1);
	add_task((uint32_t)task2);
	add_task((uint32_t)task3);
	add_task((uint32_t)task4);
	start_schedule();
	while(1);
}
void GPIO_init(void)
{
	RCC -> AHB1ENR |= (1);//GPIOA CLOCK
	//RCC -> AHB1ENR |= (1<<1);//GPIOB CLOCK
	//usart tx
	GPIOA -> MODER |= (0x2<<18);//alternate mode
	GPIOA -> OSPEEDR |= (0x3<<18);//speed high
	GPIOA -> AFR[1] |= (0X7<<4);//alternative function
	//LED 3 4
	RCC->AHB1ENR |= (0x1<<6);
	GPIOG->MODER |= (0X1<<26);
	GPIOG->MODER |= (0X1<<28);
}
void clock_init(void)
{
	RCC -> CR |= (1<<16);
	while(!(RCC->CR & 0X20000));
	FLASH -> ACR |= (1<<8);//prefetch enable
	FLASH -> ACR |= (0x1);
	RCC -> PLLCFGR &= ~(0x7fff);
	RCC -> PLLCFGR |= (1<<22);
	RCC -> PLLCFGR |= (0x5a<<6);
	RCC -> PLLCFGR |= (0x4);
	RCC -> CFGR |= (0x4<<10);
	RCC -> CR |= (1<<24);
	while(!(RCC->CR & 0X2000000));
	RCC -> CFGR |= (0X2);
}
void usart_init(void)
{
	RCC->APB2ENR |= (1<<4);//clock
	USART1 -> BRR = (0x61<<4 | 0);//USART的頻率設置
	//設定,enable
	USART1 -> CR1 |= (1<<15);
	USART1 -> CR1 |= (1<<5);
	USART1 -> CR1 |= (1<<3);
	USART1 -> CR1 |= (1<<2);
	USART1 -> CR1 |= (1<<13);
}
