#include "stm32f10x.h"
//reset = on/low
//set = off/high
//key2 e2 → ds0 b5
//key1 e3 → ds1 e5
//key up a0 → buzzer b8

void Delay(u32 count){
    u32 i;
    for(i=0; i<count; i++);
}

int main(void){
    // Enable clocks
    RCC->APB2ENR |= 1<<3; // GPIOB
    RCC->APB2ENR |= 1<<6; // GPIOE
    RCC->APB2ENR |= 1<<2; // GPIOA

    // DS0 (PB5)
    GPIOB->CRL &= 0xFF0FFFFF;
    GPIOB->CRL |= 0x00300000;
    GPIOB->BSRR = 1<<5; // set (off)

    // DS1 (PE5)
    GPIOE->CRL &= 0xFF0FFFFF;
    GPIOE->CRL |= 0x00300000;
    GPIOE->BSRR = 1<<5; // set (off)

    // Buzzer (PB8)
    GPIOB->CRH &= 0xFFFFFFF0;
    GPIOB->CRH |= 0x00000003;
    GPIOB->BRR = 1<<8; // reset (on)

    // KEY2 (PE2) input with pull-up
    GPIOE->CRL &= 0xFFFFF0FF;
    GPIOE->CRL |= 0x00000800;
    GPIOE->BSRR |= 1<<2;

    // KEY1 (PE3) input with pull-up
    GPIOE->CRL &= 0xFFFF0FFF;
    GPIOE->CRL |= 0x00008000;
    GPIOE->BSRR |= 1<<3;

    // KEY_UP (PA0) input with pull-down
    GPIOA->CRL &= 0xFFFFFFF0;
    GPIOA->CRL |= 0x00000008;
    GPIOA->BRR |= 1<<0;

    while(1){
        // KEY2 → DS0
        if(GPIOE->IDR & (1<<2))
            GPIOB->BSRR = 1<<5; // set (off)
        else
            GPIOB->BRR = 1<<5; // reset (on)

        // KEY1 → toggle DS1
        if(!(GPIOE->IDR & (1<<3))){
            Delay(500000);
            if(!(GPIOE->IDR & (1<<3))){
                while(!(GPIOE->IDR & (1<<3))); // wait release
                if(GPIOE->ODR & (1<<5)){
                    GPIOE->BSRR &= ~(1<<5);
                    GPIOE->BRR = 1<<5; // reset (on)
                }
                else{
                    GPIOE->BRR &= ~(1<<5);
                    GPIOE->BSRR = 1<<5; // set (off)
                }
            }
        }

        // KEY_UP → toggle buzzer
        if(GPIOA->IDR & (1<<0)){
            Delay(500000);
            if(GPIOA->IDR & (1<<0)){
                while(GPIOA->IDR & (1<<0)); // wait release
                if(GPIOB->ODR & (1<<8)){
                    GPIOB->BSRR &= ~(1<<8);
                    GPIOB->BRR = 1<<8; // reset (on)
                }
                else{
                    GPIOB->BRR &= ~(1<<8);
                    GPIOB->BSRR = 1<<8; // set (off)
                }
            }
        }
    }
}