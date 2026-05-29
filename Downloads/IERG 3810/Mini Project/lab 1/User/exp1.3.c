#include "stm32f10x.h"
//reset = on/low
//set= off/high
//key1 e3 ds1 e5 key up a0 buzz b8
void Delay(u32 count){
	u32 i;
	for(i=0;i<count; i++);
}

int main(void){
	RCC ->APB2ENR |= 1 << 3;//1000(set IOPB to 1)
	RCC ->APB2ENR |= 1 << 6;//\(set IOPE to 1)
	RCC ->APB2ENR |= 1<<2;//(set IOPA to 1)
	
	//key 2
	GPIOB->CRL &= 0xFF0FFFFF;//for bits 0-7, pin 5 is third(CNF7,6,5,...) clear third pos
	GPIOB->CRL |= 0x00300000;//assign 0x3 to third pos,0x3=0b0011,00 CNF 11 MODE
	
	GPIOE->CRL &= 0xFFFFF0FF;//for bits 0-7, pin 2 is sixth(CNF7,6,5,...) clear sixth pos
	GPIOE->CRL |= 0x00000800;//assign 0x8 to sixth pos,0x3=0b1000,10 CNF 00 MODE
	GPIOE->BSRR|=1<<2;
	
	GPIOB->BSRR = 1 << 5;//set
	
	//key1
	
	GPIOE->CRL &= 0xFF0F0FFF;//pin 5 ds1 pin 3 key1
	GPIOE->CRL |= 0x00308000;//ds1 0b0011 key1 0b1000
	GPIOE->BSRR|=1<<3;
	
	GPIOE->BSRR = 1 << 5;//ds1 set
	
	//key up
	GPIOA->CRL &= 0xFFFFFFF0;
	GPIOA->CRL |= 0x00000008;
	GPIOA->BRR |= 1<<0;//reset(low)->ODR=0
	
	GPIOB->CRH &= 0xFFFFFFF0;
	GPIOB->CRH |= 0x00000003;
	GPIOB->BRR = 1 << 8;//buzz set
	
	while(1){
		//key 2
		if(GPIOE->IDR&(1<<2))
			GPIOB->BSRR = 1 << 5;//bit 5 so shift 5,(set)
		else
			GPIOB->BRR = 1 << 5;//reset
		
		//key1
		if(!(GPIOE->IDR&(1<<3))){// if pressed
			Delay(500000);//wait around 0.1s
			if(!(GPIOE->IDR&(1<<3))){
				while(!(GPIOE->IDR&(1<<3)));//wait til state of button changed(released)
				
				if(GPIOE->ODR&1<<5){//if ds1 is set(off)
					GPIOE->BSRR &= ~(1<<5);
					GPIOE->BRR = 1 << 5;//bit 5 so shift 5,(reset)
					}
				else{
					GPIOE->BRR &= ~(1<<5);
					GPIOE->BSRR = 1<<5;
				}
			}
		}
		//key up
		if(GPIOA->IDR&1<<0){
			Delay(500000);
			if(GPIOA->IDR&1<<0){
				while(GPIOA->IDR&1<<0);
				
				if(GPIOB->ODR&1<<8){//if buzz on
					GPIOB->BSRR &= ~(1<<8);
					GPIOB->BRR = 1 << 8;//off buzz(reset)
				}
				else{
					GPIOB->BRR &= ~(1<<8);
					GPIOB->BSRR = 1 << 8;//on buzz(set)
				}
			}
		}
	}
}

