#include <stm32f10x.h>

void IERG3810_clocktree_init(void);

void IERG3810_clocktree_init(void){
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
