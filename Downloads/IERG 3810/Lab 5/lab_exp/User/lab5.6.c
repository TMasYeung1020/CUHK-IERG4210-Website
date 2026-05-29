#include "stm32f10x.h"
#include "IERG3810_LED.h"
#include "IERG3810_KEY.h"
#include "IERG3810_Clock.h"
#include "IERG3810_Buzzer.h"
#include "IERG3810_USART.h"

#define LEDO_PWM_VAL TIM3->CCR2

u32 heartbeat[10];
u16 led0pwmval = 0;
u8 dir = 1;

void delay(u32 count)
{
	u32 i;
	for (i=0; i<count; i++);
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

void tim3_init_pwm(u16 arr, u16 psc)
{
	RCC->APB2ENR |= 1 << 3;
	GPIOB->CRL &= 0xFF0FFFFF;
	GPIOB->CRL |= 0x00B00000;
	RCC->APB2ENR |= 1<<0;
	AFIO->MAPR &= 0xFFFFF3FF;
	AFIO->MAPR |= 1<<11;
	RCC->APB1ENR |= 1<<1;
	TIM3->ARR = arr;
	TIM3->PSC = psc;
	TIM3->CCMR1 |= 7<<12;
	TIM3->CCMR1 |= 1<<11;
	TIM3->CCER |= 1<<4;
	TIM3->CR1 = 0x0080;
	TIM3->CR1 |= 0x01;
}

void TIM3_IRQHandler(void)
{
	if(TIM3->SR & 1 << 0)
	{
		GPIOB -> ODR ^= 1 << 5;
	}
	TIM3 -> SR &= ~(1 << 0);
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

void Systick_init_10ms(void){
	
	SysTick->CTRL = 0;
	SysTick->LOAD = 719999;
	SysTick->VAL = 0;
	SysTick->CTRL |= 7;
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
	Systick_init_10ms();
	IERG3810_LED_Init(); /* for LEDs, Keys and buzzer */
	IERG3810_KEY_Init(); /* for LEDs, Keys and buzzer */
	IERG3810_Buzzer_Init(); /* for LEDs, Keys and buzzer */
	
	IERG3810_clocktree_init();
	nvic_setPriorityGroup(5); /* set PRIGROUP */
	tim3_init_pwm(6666, 71);
	// tim3_init(4999, 7199);
	tim4_init(4999, 1799);
	DS0_off;
	
	//heartbeat[0]=10;
	//heartbeat[1]=16;

	while(1)
	{
		delay(1500);
		if (dir) led0pwmval++;
		else led0pwmval--;
		if (led0pwmval > 5000) dir = 0;
		if (led0pwmval == 0) dir = 1;
		LEDO_PWM_VAL = led0pwmval;
	}
}
