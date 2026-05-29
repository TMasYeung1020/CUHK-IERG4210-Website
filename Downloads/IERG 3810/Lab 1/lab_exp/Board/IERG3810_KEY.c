#include "stm32f10x.h"
//#include "main.h"
void IERG3810_KEY_Init(void){
	// Enable clocks
    RCC->APB2ENR |= 1<<6; // GPIOE
    RCC->APB2ENR |= 1<<2; // GPIOA
	
	//key 0
	GPIOE->CRL &= 0xFFF0FFFF;//for bits 0-7, pin 2 is sixth(CNF7,6,5,...) clear sixth pos
	GPIOE->CRL |= 0x00080000;//assign 0x8 to sixth pos,0x3=0b1000,10 CNF 00 MODE
	GPIOE->BSRR|=1<<4;
	
	// KEY2 (PE2) input with pull-up
    GPIOE->CRL &= 0xFFFFF0FF;
    GPIOE->CRL |= 0x00000800;
    GPIOE->BSRR |= 1<<2;

    // KEY1 (PE3) input with pull-up
    GPIOE->CRL &= 0xFFFF0FFF;
    GPIOE->CRL |= 0x00008000;
    GPIOE->BSRR |= 1<<3;

    // KEY_UP (PA0) input with pull-down
    GPIOA->CRL &= 0xFFFFFFF0;
    GPIOA->CRL |= 0x00000008;
    GPIOA->BRR |= 1<<0;
}
