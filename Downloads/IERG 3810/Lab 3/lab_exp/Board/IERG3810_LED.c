#include "stm32f10x.h"
//#include "main.h"
#define DS0_on GPIOB->BRR = 1<<5
#define DS0_off GPIOB->BSRR = 1<<5
#define DS1_on GPIOE->BRR = 1<<5
#define DS1_off GPIOE->BSRR = 1<<5

void IERG3810_LED_Init(void){
	RCC ->APB2ENR |= 1 << 3;//1000(set IOPB to 1)
	RCC ->APB2ENR |= 1 << 6;//\(set IOPE to 1)
	//led0
	GPIOB->CRL &= 0xFF0FFFFF;//for bits 0-7, pin 5 is third(CNF7,6,5,...) clear third pos
	GPIOB->CRL |= 0x00300000;//assign 0x3 to third pos,0x3=0b0011,00 CNF 11 MODE
	GPIOB->BSRR = 1 << 5;//set
	//led1
	GPIOE->CRL &= 0xFF0FFFFF;//pin 5 ds1 pin 3 key1
	GPIOE->CRL |= 0x00300000;//ds1 0b0011 key1 0b1000
	
	GPIOE->BSRR = 1 << 5;//ds1 set
}
