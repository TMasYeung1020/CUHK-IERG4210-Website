#include "stm32f10x.h"
#include "IERG3810_LED.h"
#include "IERG3810_KEY.h"
#include "IERG3810_Clock.h"
#include "IERG3810_Buzzer.h"

/* Function to create a delay loop */
void Delay(u32 count){
	u32 i;
	for(i=0;i<count; i++);
}

/* Initialize KEY2 at PE2 for EXTI-2 interrupt, IRQ#8 */
void key2_exitInit(void){
	RCC->APB2ENR |= 1<<6; /* RM0008 v21 P146, enable port E clock */
	GPIOE->CRL &= 0xFFFFF0FF; /* P171 modify PE2 */
	GPIOE->CRL |= 0x00000800; /* pull high/low mode 10, input00 */
	GPIOE->ODR |= 1<<2; /* P173 pull high */
	RCC->APB2ENR |=0x01; /* ARIOEN P146 alternate function IO clock enabled */
	AFIO->EXTICR[0] &= 0xFFFFF0FF; /* AFIO_EXTICR1 P191 */
	AFIO->EXTICR[0] |= 0x00000400; /* set EXTI2 0100= PE[2] pin */
	EXTI->IMR |= 1<<2; /* MR2 P211 edge trigger interrupt reqeust from line 2 is not masked */
	EXTI->FTSR |= 1<<2; /* TR2 P212 falling edge enabled== if key2 is pressed */
	/* EXTI->RTSR |= 1<<2; */ /* TR2 rising edge enabled */
	
	NVIC->IP[8]= 0x65; /* set priority of this interrupt */
	/* DDI0337E page-8-3 */
	NVIC->ISER[0] |= 1<<8; /* IRQ8, set NVIC SET ENABLE REGISTER */
}

/* Initialize KEYUP at PA0 for EXTI-0 interrupt, IRQ#6 */
void keyUp_exitInit(void){
	RCC->APB2ENR |= 1<<2; /* RM0008 v21 P146, enable port A clock */
	GPIOA->CRL &= 0xFFFFFFF0; /* P171 modify PA0 */
	GPIOA->CRL |= 0x00000008; /* pull high/low mode 10, input00 */
	GPIOA->ODR &= ~(1<<0); /* P173 pull low */
	RCC->APB2ENR |=0x01; /* ARIOEN P146 alternate function IO clock enabled */
	AFIO->EXTICR[0] &= 0xFFFFFFF0; /* AFIO_EXTICR1 P191 */
	AFIO->EXTICR[0] |= 0x00000000; /* set EXTI0 0000= PA[0] pin */
	EXTI->IMR |= 1<<0; /* MR0 P211 edge trigger interrupt reqeust from line 0 is not masked */
	/* EXTI->FTSR |= 1<<0; */ /* TR2 P212 falling edge enabled== if keyUp is pressed */
	EXTI->RTSR |= 1<<0; /* TR0 rising edge enabled */
	
	NVIC->IP[6]= 0x35; /* set priority of this interrupt */
	/* DDI0337E page-8-3 */
	NVIC->ISER[0] |= 1<<6; /* IRQ8, set NVIC SET ENABLE REGISTER */
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

/* Interrupt handler for EXTI2 (KEY2 press) */
void EXTI2_IRQHandler(void){
	u8 i;
	DS1_off;
	for(i=0;i<10;i++){ /* blink 10 times */
		DS0_on;
		Delay(1000000);
		DS0_off;
		Delay(1000000);
	}
	EXTI->PR = 1<<2; /* clear this exception pending bit */
}

/* Interrupt handler for EXTI0 (KEYUP press) */
void EXTI0_IRQHandler(void){
	u8 i;
	DS0_off;
	for(i=0;i<10;i++){ /* blink 10 times */
		DS1_on;
		Delay(1000000);
		DS1_off;
		Delay(1000000);
	}
	EXTI->PR = 1<<0; /* clear this exception pending bit */
}

/* Main function */
int main(void){
	IERG3810_LED_Init(); /* for LEDs, Keys and buzzer */
	IERG3810_KEY_Init(); /* for LEDs, Keys and buzzer */
	IERG3810_Buzzer_Init(); /* for LEDs, Keys and buzzer */
	
	IERG3810_clocktree_init();
	nvic_setPriorityGroup(5); /* set PRIGROUP */
	key2_exitInit();
	keyUp_exitInit();
	DS0_off;
	
	while(1){ /* DS1 blink constantly */
	}
}

/* if key2 is pressed, blink DS0 10 times */
/* if keyUp is pressed, blink DS1 10 times */
/* whichever key has higher priority(smaller value) will interrupt the other one */
/* if same priority then whichever key is pressed first finish blinking then the other one blinks */
/* when key2 has lower priority than keyUp */
/* if keyUp pressed during key2, DS0 blinks 10 times after DS1 finishes */
/* if key2 pressed during keyUp, DS1 blinks 10 times and then DS0 finish blinking the rest */