#include <stm32f10x.h>
void IERG3810_usart2_init(u32 pclkl,u32 baud);
void IERG3810_usart1_init(u32 pclkl,u32 baud);

void IERG3810_usart2_init(u32 pclkl, u32 baud){
	//pclkl 36(MHz) baud 9600(bps)
	//PA2 Tx PA3 Rx
	//-- USART2
	float temp;
	u16 mantissa;
	u16 fraction;
	//temp=36*1000000/(9600*16)=234.375
	temp=(float)(pclkl*1000000)/(baud*16);
	//mantissa=234
	mantissa = temp;
	//fraction=6
	fraction = (temp-mantissa)*16;
	//mantissa=3744
	mantissa <<= 4;
	//mantissa=3750
	mantissa += fraction;
	//init port A
	RCC->APB2ENR |= 1<<2; //RCC_APB2ENR (RM0008 v21 P146)
	//USART2EN enable
	RCC->APB1ENR |= 1<<17; //RCC_APB1ENR (RM0008 v21 P148)
	//clear port A pin 2&3
	GPIOA->CRL &= 0xFFFF00FF; //GPIOx_CRL (RM0008 v21 P171)
	//assign port A pin 2 1011 Alternate function output Push-pull max 50MHz
	//assign port A pin 3 1000 Input pull-up/down
	GPIOA->CRL |= 0x00008B00;
	//bit 17 USART2RST 1 Reset USART2
	RCC->APB1RSTR |= 1<<17;//RCC_APB1RSTR (RM0008 v21 P109)
	//clear bit 17
	RCC->APB1RSTR &= ~(1<<17);
	//BRR =0000 1110 1010 0110
	USART2->BRR = mantissa;//USART_BRR (RM0008 v21 P820)
	//mask CRl with 0010 0000 0000 1000
	//bit 13 UE 1 USART enable
	//bit 3 TE 1 transmitter enable 
	USART2->CR1 |= 0x2008; //USART_CR1 (RM0008 v21 P821)
}
//2.2
void IERG3810_usart1_init(u32 pclkl, u32 baud){
	//pclkl 2 72(MHz) baud 9600(bps)
	//-- USART1
	//PA9 and PA10 are TX and RX of USART1
	float temp;
	u16 mantissa;
	u16 fraction;
	//temp=36*1000000/(9600*16)=234.375
	temp=(float)(pclkl*1000000)/(baud*16);
	//mantissa=234
	mantissa = temp;
	//fraction=6
	fraction = (temp-mantissa)*16;
	//mantissa=3744
	mantissa <<= 4;
	//mantissa=3750
	mantissa += fraction;
	//init port A
	RCC->APB2ENR |= 1<<2; //RCC_APB2ENR (RM0008 v21 P146)
	//USART1EN enable
	RCC->APB2ENR |= 1<<14; //RCC_APB1ENR (RM0008 v21 P146)
	//clear port A pin 9&10
	GPIOA->CRH &= 0xFFFFF00F; //GPIOx_CRL (RM0008 v21 P171)
	//assign port A pin 9 1011 Alternate function output Push-pull max 50MHz
	//assign port A pin 10 1000 Input pull-up/down
	GPIOA->CRH |= 0x000008B0;
	//bit 14 USART1RST 1 Reset USART1
	RCC->APB2RSTR |= 1<<14;//RCC_APB2RSTR (RM0008 v21 P106)
	//clear bit 14
	RCC->APB2RSTR &= ~(1<<14);
	//BRR =0000 1110 1010 0110
	USART1->BRR = (int)mantissa;//USART_BRR (RM0008 v21 P820)
	//mask CRl with 0010 0000 0000 1000
	//bit 13 UE 1 USART enable
	//bit 3 TE 1 transmitter enable 
	USART1->CR1 |= 0x2008; //USART_CR1 (RM0008 v21 P821)
}
