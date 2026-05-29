#include "stm32f10x.h"
#include "IERG3810_KEY.h"
#include "IERG3810_LED.h"
#include "IERG3810_Buzzer.h"

// Delay function
void Delay(u32 count) {
    u32 i;
    for (i = 0; i < count; i++);
}

int main(void) {
    // Initialization
    IERG3810_KEY_Init();
    IERG3810_LED_Init();
    IERG3810_Buzzer_Init();

    while (1) {
        // KEY2 → DS0 control
        if (GPIOE->IDR & (1 << 2)) {
            GPIOB->BSRR = 1 << 5; // Set (LED OFF)
        } else {
            GPIOB->BRR = 1 << 5; // Reset (LED ON)
        }

        // KEY1 → toggle DS1
        if (!(GPIOE->IDR & (1 << 3))) {
            Delay(1000000);
            if (!(GPIOE->IDR & (1 << 3))) {
                while (!(GPIOE->IDR & (1 << 3))); // Wait for release

                if (GPIOE->ODR & (1 << 5)) {
                    GPIOE->BSRR &= ~(1 << 5);
                    GPIOE->BRR = 1 << 5; // Reset (LED ON)
                } else {
                    GPIOE->BRR &= ~(1 << 5);
                    GPIOE->BSRR = 1 << 5; // Set (LED OFF)
                }
            }
        }

        // KEY_UP → toggle buzzer
        if (GPIOA->IDR & (1 << 0)) {
            Delay(1000000);
            if (GPIOA->IDR & (1 << 0)) {
                while (GPIOA->IDR & (1 << 0)); // Wait for release

                if (GPIOB->ODR & (1 << 8)) {
                    GPIOB->BSRR &= ~(1 << 8);
                    GPIOB->BRR = 1 << 8; // Reset (Buzzer ON)
                } else {
                    GPIOB->BRR &= ~(1 << 8);
                    GPIOB->BSRR = 1 << 8; // Set (Buzzer OFF)
                }
            }
        }
    }
}