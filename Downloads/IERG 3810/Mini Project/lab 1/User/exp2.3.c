#include <stm32f10x.h>
void clocktree_init(void);
void usart_print_txe(u8 USARTport, char* st);
void usart2_init(u32 pclkl,u32 baud);
void usart1_init(u32 pclkl,u32 baud);

void delay(u32 count){
	u32 i;
	for(i=0;i<count; i++);
}

void usart_print_txe(u8 USARTport, char* st){
	u8 i=0;
	while(st[i] != 0x00){
		if (USARTport ==1){
			while(!(USART1->SR & (1<<7)));
			USART1->DR = st[i];
		}
		if (USARTport ==2){
			while(!(USART2->SR & (1<<7)));
			USART2->DR = st[i];
		}
		if(i==255) break;
		i++;
	}
}

void clocktree_init(void){
	
	u8 PLL=7;
	unsigned char temp=0;
	//clear bit 26-24 MCO,15-0
	RCC->CFGR &= 0xF8FF0000; // RCC->CFGR(RM0008 v21 P101)
	//E 1110 clear bit 24 PLLON
	//6 0110 clear bit 19CSSON Clock detector,16 HSEON
	RCC->CR &= 0xFEF6FFFF; //RCC->CR(RM0008 v21 P99)
	//bit 16 HSEON 1 HSE oscillator ON
	RCC->CR |= 0x00010000;
	//wait until bit 31-17 to be 1(1=true)
	//bit 17 HSERDY 
	//0: HSE oscillator not ready 
	//1: HSE oscillator ready 
	while(!(RCC->CR>>17));
	//reset all bits to 0 and assign bits 11-8 0100
	//bit 11 PPRE2 xx0
	//bits 10-8 PPRE1 100,HCLK divided by 2
	//2.2 set APB2 72MHz so bits 13-11 PPRE2 0xx
	//2.2 change RCC->CFGR = 0x00000400 -> RCC->CFGR = 0x00001400 PPRE2 divided by 1
	RCC->CFGR &=0xFFFF00FF;
	RCC->CFGR |= 0x00001400;
	//assign bit 21-18 be 7(0111)
	//PLLMUL:PLL input clock x 9
	RCC->CFGR |= PLL<<18;
	//set bit 16 PLLSRC =1 HSE oscillator clock selected as PLL input clock
	RCC->CFGR |= 1<<16;
	RCC->CFGR |= 1<<16;
	//set ACR = 00110010
	//bit 5 PRFTBS read-only so no change
	//bit 4 PRFTBE 1 Prefetch is enabled
	//2.2 bit 2-0 LATENCY 010 Two wait state
	FLASH->ACR |= 0x32; //FLASH->ACR (RM0008 v21 P60)
	//assign bit 24 PLLON 1 PLL ON
	RCC->CR |= 0x01000000;
	//wait until bit 31-25to be 1(1=true)
	//25 PLLRDY 
	//0:PLL unlocked
	//1:PLL locked
	while(!(RCC->CR>>25));
	//assign bit 3:0 0010
	//bits 3-2 SWS 00 HSI oscillator used as system clock
	//bits 1-0 SW 10 PLL selected as system clock
	RCC->CFGR |= 0x00000002;
	
	while(temp != 0x02){
		temp=RCC->CFGR>>2;
		temp &= 0x03;
	}
}
void usart2_init(u32 pclkl, u32 baud){
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
void usart1_init(u32 pclkl, u32 baud){
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

int main(void){
	clocktree_init();
	usart2_init(36,9600);
	usart1_init(72,9600);
	volatile u32 tmp=USART1->SR; tmp=USART1->DR; (void)tmp;//reset status register to ensure full frame transfer
	
	while(1){
		usart_print_txe(1,"1155213238");
	}
}
