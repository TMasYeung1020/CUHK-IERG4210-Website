#include "stm32f10x.h"
#include "LCDinit.h"
#include "IERG3810_TFTLCD.h"
#include "IERG3810_LED.h"
#include "IERG3810_Clock.h"
#include "IERG3810_Buzzer.h"
#include "IERG3810_KEY.h"
void Delay(u32 count){
	u32 i;
	for(i=0;i<count; i++);
}

void nvic_setPriorityGroup(u8 priGroup)
{
    //-- set PRIGROUP AIRCR[10:8]
    u32 tmp1, tmp2;
    tmp2 = priGroup & 0x00000007 << 8;     // only concern 3 bits
    tmp1 = SCB->AIRCR &= 0x0000F8FF;       // ARMDI0337 page 8-22
    tmp1 |= 0x05FA0000;                    // *** must read ARMDI0337 page 8-22
    SCB->AIRCR = tmp1 |= tmp2;
}
//EXTI2 handler for KEY2 //given
void EXTI2_IRQHandler(void)
{
    u8 i;
    for (i=0; i<10; i++)
    {
        DS0_on;
        Delay(1000000);
        DS0_off;
        Delay(1000000);
    }
    EXTI->PR = 1<<2; //Clear this exception pending bit
}


void key2_exitInit(void) // Exp-4.1, GIVEN
{
    //KEY2 at PE2, EXIT-2, IRQ#8
    RCC->APB2ENR |= 1<<6;     //RM0008 v21 P146, enable port-E clock
    GPIOE->CRL &= 0xFFFFF0FF; //RM0008 v21 P171, modify PE2
    GPIOE->CRL |= 0x00000800; //pull high/low mode '10', input '00'
    GPIOE->ODR |= 1<<2;       //GPIOx_ODR (RM0008 v21 P173), pull high
    RCC->APB2ENR |= 0x01;     //AFIOEN, RCC_APB2ENR (RM0008 v21 P146)
    AFIO->EXTICR[0] &= 0xFFFFF0FF; //AFIO_EXTICR1 (RM0008 v21 P191)
    AFIO->EXTICR[0] |= 0x00000400;
    EXTI->IMR |= 1<<2;        //MR2 (RM0008 v21 P211) edge trigger
    EXTI->FTSR |= 1<<2; //TR2 (RM0008 v21 P212) falling edge
    //EXIT->RTSR |= 1<<2;     //TR2 (RM0008 v21, page-212) rising edge

    NVIC->IP[8] = 0x65;       //set priority of this interrupt
    //DDI0337E page-8-3
    NVIC->ISER[0] |= (1<<8);  //IRQ8, set NVIC 'SET ENABLE REGISTER'
}


int main(void)
{
    LED_Init();
		KEY_Init();
		Buzzer_Init();// for LEDs, Keys and Buzzer (use your own design)
	
    clocktree_init();
    //usart2_init(3&, 9600); //keep if you want. not important
    nvic_setPriorityGroup(5); //set PRIGROUP
    key2_exitInit();         //Exp4.1, Init KEY2 as an interrupt input
    DS0_off;

    while(1)
    {
        //usart_print_txe(2, "41"); // not important
        //sheep++;                            // not important
        DS1_on;
        Delay(500000);
        DS1_off;
        Delay(500000);
    }
}


