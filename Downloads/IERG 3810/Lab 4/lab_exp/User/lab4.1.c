#include "stm32f10x.h"
#include "IERG3810_LED.h"
#include "IERG3810_KEY.h"
#include "IERG3810_Clock.h"
#include "IERG3810_Buzzer.h"

/* Function to create a delay loop */
void Delay(u32 count){
    u32 i;
    for(i=0; i<count; i++);
}

/* Initialize KEY2 at PE2 for EXTI-2 interrupt, IRQ#8 */
void key2_exitInit(void){
    RCC->APB2ENR |= 1<<6; /* RM0008 v21 P146, enable port E clock */
    GPIOE->CRL &= 0xFFFFF0FF; /* P171 modify PE2 */
    GPIOE->CRL |= 0x00000800; /* Pull high/low mode 10, input00 */
    GPIOE->ODR |= 1<<2; /* P173 pull high */
    RCC->APB2ENR |= 0x01; /* ARIOEN P146 alternate function IO clock enabled */
    AFIO->EXTICR[0] &= 0xFFFFF0FF; /* AFIO_EXTICR1 P191 */
    AFIO->EXTICR[0] |= 0x00000400; /* Set EXTI2 0100= PE[2] pin */
    EXTI->IMR |= 1<<2; /* MR2 P211 edge trigger interrupt request from line 2 is not masked */
    EXTI->FTSR |= 1<<2; /* TR2 P212 falling edge enabled - triggered when key2 is pressed */
    /* EXTI->RTSR |= 1<<2; */ /* TR2 rising edge enabled (commented out) */
    
    NVIC->IP[8] = 0x65; /* Set priority of this interrupt */
    /* DDI0337E page-8-3 */
    NVIC->ISER[0] |= 1<<8; /* IRQ8, set NVIC SET ENABLE REGISTER */
}

/* Set NVIC priority group configuration */
void nvic_setPriorityGroup(u8 priGroup){
    /* Set PRIGROUP AIRCR[10:8] */
    u32 tmp1, tmp2;
    tmp2 = (priGroup & 0x00000007) << 8; /* Only concern 3 bits, 0101&0111= 0101 */
    tmp1 = SCB->AIRCR &= 0x0000F8FF; /* ARMDI0337 page 8-22 */
    tmp1 |= 0x05FA0000; /* Must read ARMDI0337 page 8-22, 05FA allows writing */
    SCB->AIRCR = tmp1 |= tmp2; /* 0x05FA0500, 2.6 indicates two bits of pre-emption priority, six bits of subpriority */
}

/* Interrupt handler for EXTI2 (KEY2 press) */
void EXTI2_IRQHandler(void){
    u8 i;
    for(i=0; i<10; i++){ /* Blink DS0 10 times */
        DS0_on;
        Delay(1000000);
        DS0_off;
        Delay(1000000);
    }
    EXTI->PR = 1<<2; /* Clear this exception pending bit */
}

/* Main function */
int main(void){
    IERG3810_LED_Init(); /* Initialize LEDs, Keys, and Buzzer */
    IERG3810_KEY_Init(); /* Initialize LEDs, Keys, and Buzzer */
    IERG3810_Buzzer_Init(); /* Initialize LEDs, Keys, and Buzzer */
    
    IERG3810_clocktree_init(); /* Initialize clock tree */
    nvic_setPriorityGroup(5); /* Set PRIGROUP */
    key2_exitInit(); /* Initialize KEY2 interrupt */
    DS0_off; /* Turn off DS0 */
    
    while(1){ /* DS1 blinks constantly */
        DS1_on;
        Delay(500000);
        DS1_off;
        Delay(500000);
    }
}
/* When KEY2 is pressed, interrupt DS1 blinking and blink DS0 10 times */