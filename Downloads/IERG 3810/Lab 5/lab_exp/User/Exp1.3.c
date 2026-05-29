#include "stm32f10x.h"

void Delay(u32 count)
{
	u32 i;
	for (i=0; i<count; i++);
}

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC -> APB2ENR |= 1 << 3;
	GPIOB -> CRL &= 0xFF0FFFFF;
	GPIOB -> CRL |= 0x00300000;
	
	while(1)
	{
		GPIOB -> BRR = 1 << 5;
		Delay(1000000);
		GPIOB -> BSRR = 1 << 5;
		Delay(1000000);
	}
}
