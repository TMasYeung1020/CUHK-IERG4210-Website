#include "stm32f10x.h"
//#include "main.h"

void Buzzer_Init(void){
	RCC ->APB2ENR |= 1 << 3;//1000(set IOPB to 1)
	
	GPIOB->CRH &= 0xFFFFFFF0;
	GPIOB->CRH |= 0x00000003;
	GPIOB->BRR = 1 << 8;//set
}
