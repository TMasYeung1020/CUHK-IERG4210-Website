#include "stm32f10x.h"
//#include "main.h"

void IERG3810_LED_Init(void){
	RCC->APB2ENR |= 1<<3; // GPIOB
    RCC->APB2ENR |= 1<<6; // GPIOE
	// DS0 (PB5)
    GPIOB->CRL &= 0xFF0FFFFF;
    GPIOB->CRL |= 0x00300000;
    GPIOB->BSRR = 1<<5; // set (off)

    // DS1 (PE5)
    GPIOE->CRL &= 0xFF0FFFFF;
    GPIOE->CRL |= 0x00300000;
    GPIOE->BSRR = 1<<5; // set (off)

}
