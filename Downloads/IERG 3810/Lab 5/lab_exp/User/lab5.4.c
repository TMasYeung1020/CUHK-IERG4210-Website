#include "stm32f10x.h"
#include "IERG3810_LED.h"
#include "IERG3810_KEY.h"
#include "IERG3810_Clock.h"
#include "IERG3810_Buzzer.h"
#include "IERG3810_USART.h"

u32 heartbeat[10];

void ds0_turnOff(void)
{
	GPIOB->BSRR = 1 << 5;
}

void ds0_turnOff2(void)
{
	ds0_turnOff();
}

void tim3_init(u16 arr, u16 psc)
{
	RCC->APB1ENR |= 1 << 1;
	TIM3->ARR = arr;
	TIM3->PSC = psc;
	TIM3 -> DIER |= 1 << 0;
	TIM3 -> CR1 |= 0x01;
	NVIC -> IP[29] = 0x45;
	NVIC -> ISER[0] |= (1<<29);
}

void TIM3_IRQHandler(void)
{
	DS0_on;
	DS0_off;
	DS0_on;
	ds0_turnOff();
	DS0_on;
	ds0_turnOff2();
	DS0_on;
	TIM3->SR &= ~(1<<0);
	TIM3->SR &= ~(1<<0);
}

void tim4_init(u16 arr, u16 psc)
{
	RCC->APB1ENR |= 1 << 2;
	TIM4->ARR = arr;
	TIM4->PSC = psc;
	TIM4 -> DIER |= 1 << 0;
	TIM4 -> CR1 |= 0x01;
	NVIC -> IP[30] = 0x45;
	NVIC -> ISER[0] |= (1<<30);
}

void TIM4_IRQHandler(void)
{
	if(TIM4->SR & 1 << 0)
	{
		GPIOE -> ODR ^= 1 << 5;
	}
	TIM4 -> SR &= ~(1 << 0);
}

/* Set NVIC priority group configuration */
void nvic_setPriorityGroup(u8 priGroup){
	/* set PRIGROUP AIRCR[10:8] */
	u32 tmp1, tmp2;
	tmp2 = priGroup & 0x00000007 <<8; /* only concern 3 bits, 0101&0111= 0101 */
	tmp1 = SCB->AIRCR &= 0x0000F8FF; /* ARMDI0337 page 8-22 */
	tmp1 |= 0x05FA0000; /* must read ARMDI0337 page 8-22 05FA allow writing */
	SCB->AIRCR = tmp1 |= tmp2; /* 0x05FA0500,2.6 indicates two bits of pre-emption priority, six bits of subpriority */
}

int main(void)
{
	IERG3810_LED_Init(); /* for LEDs, Keys and buzzer */
	IERG3810_KEY_Init(); /* for LEDs, Keys and buzzer */
	IERG3810_Buzzer_Init(); /* for LEDs, Keys and buzzer */
	
	IERG3810_clocktree_init();
	nvic_setPriorityGroup(5); /* set PRIGROUP */
	tim3_init(4999, 7199);
	//tim4_init(4999,1799);
	DS0_off;
	DS1_off;
	
	while(1)
	{
		;
	}
}
