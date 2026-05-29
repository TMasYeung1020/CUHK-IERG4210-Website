// main.c: STM32F10x program to init clock and USART, send SID via USART1 and chars via USART2
#include <stm32f10x.h>
void clocktree_init(void);
void usart2_init(u32 pclk1, u32 baud);
void usart1_init(u32 pclk1, u32 baud);

// Simple delay function
void delay(u32 count) {
    while (count > 0) count--; // Decrement until zero
}

// Init system clock tree
void clocktree_init(void) {
    u8 PLL = 7;
    unsigned char temp = 0;
    RCC->CFGR &= 0xF8FF0000; // Clear CFGR bits
    RCC->CR &= 0xFEF6FFFF;   // Clear CR bits
    RCC->CR |= 0x00010000;   // Enable HSE
    while (!(RCC->CR >> 17)); // Wait for HSE ready
    RCC->CFGR &= 0xFFFF00FF; // Clear PLL config
    RCC->CFGR |= 0x00001400; // Set HSE as PLL source, div by 2
    RCC->CFGR |= (PLL << 18); // Set PLL multiplier
    RCC->CFGR |= 1 << 16;     // Enable PLL
    RCC->CFGR |= 1 << 16;     // Ensure PLL enabled
    FLASH->ACR |= 0x32;       // Set flash latency, enable prefetch
    RCC->CR |= 0x01000000;    // Enable PLL
    while (!(RCC->CR >> 25)); // Wait for PLL ready
    RCC->CFGR |= 0x00000002;  // Select PLL as system clock
    while (temp != 0x02) {    // Wait for system clock switch
        temp = RCC->CFGR >> 2;
        temp &= 0x03;
    }
}

// Init USART2 with given clock and baud rate
void usart2_init(u32 pclk1, u32 baud) {
    float temp;
    u16 mantissa;
    u16 fraction;
    temp = (float)(pclk1 * 1000000) / (baud * 16); // Calc baud divider
    mantissa = temp;
    fraction = (temp - mantissa) * 16;
    mantissa <<= 4;
    mantissa += fraction;
    RCC->APB2ENR |= 1 << 2;   // Enable GPIOA clock
    RCC->APB1ENR |= 1 << 17;  // Enable USART2 clock
    GPIOA->CRL &= 0xFFFF00FF; // Clear PA2, PA3 config
    GPIOA->CRL |= 0x00008B00; // Set PA2 AF push-pull, PA3 input
    RCC->APB1RSTR |= 1 << 17; // Reset USART2
    RCC->APB1RSTR &= ~(1 << 17); // Clear reset
    USART2->BRR = mantissa;   // Set baud rate
    USART2->CR1 |= 0x2008;    // Enable USART2 and TX
}

// Init USART1 with given clock and baud rate
void usart1_init(u32 pclk1, u32 baud) {
    float temp;
    u16 mantissa;
    u16 fraction;
    temp = (float)(pclk1 * 1000000) / (baud * 16); // Calc baud divider
    mantissa = temp;
    fraction = (temp - mantissa) * 16;
    mantissa <<= 4;
    mantissa += fraction;
    RCC->APB2ENR |= 1 << 2;   // Enable GPIOA clock
    RCC->APB2ENR |= 1 << 14;  // Enable USART1 clock
    GPIOA->CRH &= 0xFFFFF00F; // Clear PA9, PA10 config
    GPIOA->CRH |= 0x000008B0; // Set PA9 AF push-pull, PA10 input
    RCC->APB2RSTR |= 1 << 14; // Reset USART1
    RCC->APB2RSTR &= ~(1 << 14); // Clear reset
    USART1->BRR = (int)mantissa; // Set baud rate
    USART1->CR1 |= 0x2008;    // Enable USART1 and TX
}

// Main: Init and send SID via USART1, chars via USART2
int main(void) {
    clocktree_init(); // Init clock
    usart1_init(72, 9600); // Init USART1
    usart2_init(36, 9600); // Init USART2
    delay(5500000); // Initial delay
    // usart_print(2, "1234567890"); // Commented out print function
    while (1) {
        int i;
        int sid[] = {1, 1, 5, 5, 2, 0, 3, 1, 8, 1}; // SID array
        for (i = 0; i < 10; i++) { // Send each digit
            if (sid[i] == 1) USART1->DR = 0x31; // Send '1'
            else if (sid[i] == 0) USART1->DR = 0x30; // Send '0'
            else if (sid[i] == 2) USART1->DR = 0x32; // Send '2'
            else if (sid[i] == 3) USART1->DR = 0x33; // Send '3'
            else if (sid[i] == 5) USART1->DR = 0x35; // Send '5'
            else if (sid[i] == 8) USART1->DR = 0x38; // Send '8'
            delay(5000000); // Delay between digits
        }
        USART1->DR = 0x20; // Send space
        delay(5000000); // Delay after space
        USART2->DR = 0x41; // Send 'A'
        delay(5000000); // Delay
        USART2->DR = 0x42; // Send 'B'
        delay(5000000); // Delay
        delay(1000000); // Extra delay
    }
}