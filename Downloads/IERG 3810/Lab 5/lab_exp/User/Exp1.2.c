#include "stm32f10x.h"

void Delay(u32 count)
{
	u32 i;
	for (i=0; i<count; i++);
}

int main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, & GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_5);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, & GPIO_InitStructure);
	GPIO_SetBits(GPIOE, GPIO_Pin_2);
	
	
	while(1)
	{
		if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == 0) {  // Active low
		GPIO_ResetBits(GPIOB, GPIO_Pin_5);
    GPIO_SetBits(GPIOB, GPIO_Pin_5);  // Turn on LED
		}
	}
}
