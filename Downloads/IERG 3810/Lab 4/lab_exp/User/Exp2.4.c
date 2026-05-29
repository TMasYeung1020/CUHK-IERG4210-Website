// main.c: STM32F10x program to send a string via USART1
#include <stm32f10x.h>
#include "IERG3810_Clock.h"
#include "IERG3810_USART.h"

// Simple delay function
void delay(u32 count) {
    u32 i;
    for (i = 0; i < count; i++); // Loop for delay
}

// Send string via USART1 or USART2
void usart_print_txe(u8 USARTport, char* st) {
    u8 i = 0;
    while (st[i] != 0x00) { // Until null terminator
        if (USARTport == 1) {
            while (!(USART1->SR & (1 << 7))); // Wait for TXE
            USART1->DR = st[i]; // Send character
            delay(5000000); // Delay
        }
        if (USARTport == 2) {
            while (!(USART2->SR & (1 << 7))); // Wait for TXE
            USART2->DR = st[i]; // Send character
            delay(5000000); // Delay
        }
        if (i == 255) break; // Limit string length
        i++;
    }
    USART1->DR = 0x20; // Send space
    delay(5000000); // Delay
}

// Main: Initialize and send SID via USART1
int main(void) {
    IERG3810_clocktree_init(); // Init clock
    IERG3810_usart2_init(36, 9600); // Init USART2
    IERG3810_usart1_init(72, 9600); // Init USART1
    volatile u32 tmp = USART1->SR; tmp = USART1->DR; (void)tmp; // Reset USART1

    while (1) {
        usart_print_txe(1, "1155203181"); // Send new SID
    }
}