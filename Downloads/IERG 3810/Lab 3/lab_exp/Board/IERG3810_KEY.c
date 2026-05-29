#include "stm32f10x.h"
//#include "main.h"
void IERG3810_KEY_Init(void){
	RCC ->APB2ENR |= 1<<2;//(set IOPA to 1)
	RCC ->APB2ENR |= 1 << 6;//\(set IOPE to 1)
	
	//key up
	GPIOA->CRL &= 0xFFFFFFF0;
	GPIOA->CRL |= 0x00000008;
	GPIOA->BRR |= 1<<0;//reset(low)->ODR=0
	
	//key 0
	GPIOE->CRL &= 0xFFF0FFFF;//for bits 0-7, pin 2 is sixth(CNF7,6,5,...) clear sixth pos
	GPIOE->CRL |= 0x00080000;//assign 0x8 to sixth pos,0x3=0b1000,10 CNF 00 MODE
	GPIOE->BSRR|=1<<4;
	
	//key 1
	GPIOE->CRL &= 0xFFFF0FFF;//pin 5 ds1 pin 3 key1
	GPIOE->CRL |= 0x00008000;//ds1 0b0011 key1 0b1000
	GPIOE->BSRR|=1<<3;//set(high)->ODR=1
	
	//key 2
	GPIOE->CRL &= 0xFFFFF0FF;//for bits 0-7, pin 2 is sixth(CNF7,6,5,...) clear sixth pos
	GPIOE->CRL |= 0x00000800;//assign 0x8 to sixth pos,0x3=0b1000,10 CNF 00 MODE
	GPIOE->BSRR|=1<<2;
}
