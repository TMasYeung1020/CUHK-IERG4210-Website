#include "stm32f10x.h"
#include "IERG3810_LED.h"
#include "IERG3810_KEY.h"
#include "IERG3810_Clock.h"
#include "IERG3810_Buzzer.h"
#include "IERG3810_USART.h"

volatile u16 ps2_shift = 0;
volatile u8 ps2_bitcount=0;
volatile u8 ps2_ready=0;
volatile u8 ps2_last_byte =0;
volatile u32 ps2count=0;
volatile u8 F0flag=0;

u8 ps2_sample_data(void){
	return (GPIOC->IDR >> 10) &1;//check PC10 any input
}

void ps2_fifo_push(u8 b){
	ps2_last_byte=b;//store last byte
}


void Delay(u32 count){
	u32 i;
	for(i=0;i<count; i++);
}

void usart_print_txe(char* st){
	u8 i=0;
	while(st[i] != 0x00){
			while(!(USART1->SR & (1<<7)));
			USART1->DR = st[i];
		
		if(i==255) break;
		i++;
	}
}

static int utoa10(unsigned int val, char* buf){//turn val into string and reverse it
    if (val == 0){
        buf[0] = '0';
        buf[1] = '\0';
        return 1;
    }
    int i = 0;
    while (val > 0){
        buf[i++] = (char)('0' + (val % 10));//convert number to string
        val /= 10;
    }
    // reverse string
    for (int l = 0, r = i - 1; l < r; l++, r--){
        char t = buf[l]; buf[l] = buf[r]; buf[r] = t;
    }
    buf[i] = '\0';
    return i;
}

// Print unsigned int via USART
void usart_print_uint(unsigned int val){
    // Max decimal length for 32-bit uint is 10 + null
    char buf[11];
    utoa10(val, buf);//val to little endian string
    usart_print_txe(buf);//print string
}

void key2_exitInit(void){
	//KEY2 at PE2, EXTI-2, IRQ#8
	RCC->APB2ENR |= 1<<6;//RM0008 v21 P146, enable port E clock
	GPIOE->CRL &= 0xFFFFF0FF;//P171 modify PE2
	GPIOE->CRL |= 0x00000800;//pull high/low mode 10, input00
	GPIOE->ODR |= 1<<2;//P173 pull high
	RCC->APB2ENR |=0x01;//ARIOEN P146 alternate function IO clock enabled
	AFIO->EXTICR[0] &= 0xFFFFF0FF;// AFIO_EXTICR1 P191
	AFIO->EXTICR[0] |= 0x00000400;//set EXTI2 0100= PE[2] pin
	EXTI->IMR |= 1<<2; //MR2 P211 edge trigger interrupt reqeust from line 2 is not masked
	EXTI->FTSR |= 1<<2;//TR2 P212 falling edge enabled== if key2 is pressed
	//EXTI->RTSR |= 1<<2;// TR2 rising edge enabled
	
	NVIC->IP[8]= 0x65;//set priority of this interrupt
	//DDI0337E page-8-3
	NVIC->ISER[0] |= 1<<8;//IRQ8, set NVIC SET ENABLE REGISTER
}

void keyUp_exitInit(void){
	//KEYUP at PA0, EXTI-0,EXTICR1 0 0, IRQ#6
	RCC->APB2ENR |= 1<<2;//RM0008 v21 P146, enable port A clock
	GPIOA->CRL &= 0xFFFFFFF0;//P171 modify PA0
	GPIOA->CRL |= 0x00000008;//pull high/low mode 10, input00
	GPIOA->ODR &= ~(1<<0);//P173 pull low
	RCC->APB2ENR |=0x01;//ARIOEN P146 alternate function IO clock enabled
	AFIO->EXTICR[0] &= 0xFFFFFFF0;// AFIO_EXTICR1 P191
	AFIO->EXTICR[0] |= 0x00000000;//set EXTI0 0000= PA[0] pin
	EXTI->IMR |= 1<<0; //MR0 P211 edge trigger interrupt reqeust from line 0 is not masked
	//EXTI->FTSR |= 1<<0;//TR2 P212 falling edge enabled== if keyUp is pressed
	EXTI->RTSR |= 1<<0;// TR0 rising edge enabled
	
	NVIC->IP[6]= 0x35;//set priority of this interrupt
	//DDI0337E page-8-3
	NVIC->ISER[0] |= 1<<6;//IRQ8, set NVIC SET ENABLE REGISTER
}

void ps2key_exitInit(void){
	//ps2key at PC11, EXTI-11,EXTICR3 3 2, IRQ#40
	RCC->APB2ENR |= 1<<4;//RM0008 v21 P146, enable port C clock
	GPIOC->CRH &= 0xFFFF00FF;//P171 modify PC11
	GPIOC->CRH |= 0x00008800;//pull high/low mode 10, input00
	GPIOC->ODR |= (1<<11);//P173 pull up
	GPIOC->ODR |= (1<<10); // pull-up on data
	RCC->APB2ENR |=0x01;//ARIOEN P146 alternate function IO clock enabled
	AFIO->EXTICR[2] &= 0xFFFF0FFF;// AFIO_EXTICR3 P191
	AFIO->EXTICR[2] |= 0x00002000;//set EXTI3 0010= PC[11] pin
	EXTI->IMR |= 1<<11; //MR11 P211 edge trigger interrupt reqeust from line 11 is not masked
	EXTI->FTSR |= 1<<11;//TR11 P212 falling edge enabled== if ps2key is pressed
	//EXTI->RTSR |= 1<<0;// TR0 rising edge enabled
	NVIC->IP[40]=0x10;
	NVIC->ISER[1]|=1<<8;
	
}

void nvic_setPriorityGroup(u8 priGroup){
	//set PRIGROUP AIRCR[10:8]
	u32 tmp1, tmp2;
	tmp2 = priGroup & 0x00000007 <<8;//only concern 3 bits, 0101&0111= 0101
	tmp1 = SCB->AIRCR &= 0x0000F8FF; //ARMDI0337 page 8-22
	tmp1 |= 0x05FA0000; // must read ARMDI0337 page 8-22 05FA allow writing
	SCB->AIRCR = tmp1 |= tmp2;//0x05FA0500,2.6 indicates two bits of pre-emption priority, six bits of subpriority
}


void EXTI2_IRQHandler(void){
	u8 i;
	DS1_off;
	for(i=0;i<10;i++){//blink 10 times
		DS0_on;
		Delay(1000000);
		DS0_off;
		Delay(1000000);
	}
	EXTI->PR = 1<<2;//clear this exception pending bit
}

void EXTI0_IRQHandler(void){
	u8 i;
	DS0_off;
	for(i=0;i<10;i++){//blink 10 times
		DS1_on;
		Delay(1000000);
		DS1_off;
		Delay(1000000);
	}
	EXTI->PR = 1<<0;//clear this exception pending bit
}

void EXTI15_10_IRQHandler(void){
	
	//design
	if(EXTI->PR & (1<<11)){//if PR is clear
		// Shift in LSB-first: put current bit at position ps2_bitcount
		uint8_t bit = ps2_sample_data();
		ps2_shift |= ((uint16_t)bit << ps2_bitcount);//store in MSB
		ps2_bitcount++;

    if(ps2_bitcount >= 11){
        // Bits 0..10 now filled
        uint16_t f = ps2_shift;//store frame
        // Reset for next frame
        ps2_bitcount = 0;
        ps2_shift = 0;

        // Decode frame: b0=start, b1..b8=data, b9=parity, b10=stop
        uint8_t start  =  (f >> 0) & 1;
        uint8_t data   =  (f >> 1) & 0xFF;     // 8 data bits
        uint8_t parity =  (f >> 9) & 1;
        uint8_t stop   =  (f >> 10) & 1;
				
        // Validate start=0, stop=1, odd parity
        uint8_t ones = __builtin_popcount((unsigned)data) + parity;
        if(start==0 && stop==1 && (ones & 1)==1){
						
            ps2_fifo_push(data);
            ps2_ready = 1;
						//Delay(10000);
        }
				if (ps2_ready) {
						uint8_t b = ps2_last_byte;
            ps2_ready = 0;
                
            // Handle the F0 prefix (key release)
            if(b == 240) {
                F0flag = 1;  // Next byte will be a release code
            }
            else if(F0flag == 1) {
                // This is a key release event
                if(b == 105) {
                    DS0_off;  // Turn off when '1' is released
                }
                else if(b == 114) {
                    DS1_off;  // Turn off when '2' is released
                }
                F0flag = 0;  // Reset the flag
            }
            else {
                // This is a key press event
                if(b == 105) {
                    DS0_on;   // Turn on when '1' is pressed
                }
                else if(b == 114) {
                    DS1_on;   // Turn on when '2' is pressed
                }
						}
        }
        // else: framing/parity error -> discard
    }
		EXTI->PR = 1<<11;//clear exception pending bit
		EXTI->PR = 1<<11;//make sure clear
	}
}



int main(void){
	LED_Init();// for LEDs, Keys and buzzer
	KEY_Init();// for LEDs, Keys and buzzer
	Buzzer_Init();// for LEDs, Keys and buzzer

	clocktree_init();
	nvic_setPriorityGroup(5); //set PRIGROUP
	key2_exitInit();
	keyUp_exitInit();
	ps2key_exitInit();
	DS0_off;
	DS1_off;
	
	IERG3810_usart1_init(72,9600);
	
	
	while(1){
		
	}
}
