#include "stm32f10x.h"
#include "joypad.h"

//==========================================================================
//               JoyPad 腳位定義 (開發板 v3)
//==========================================================================
// Latch  = PB11 (Output)
// Data   = PB10 (Input)
// Clock  = PD3  (Output)

#define JOYPAD_LATCH_GPIO   GPIOB
#define JOYPAD_LATCH_PIN    GPIO_Pin_11
#define JOYPAD_DATA_GPIO    GPIOB
#define JOYPAD_DATA_PIN     GPIO_Pin_10
#define JOYPAD_CLOCK_GPIO   GPIOD
#define JOYPAD_CLOCK_PIN    GPIO_Pin_3

//==========================================================================
//               按鍵狀態全域變數 (1 = 按下, 0 = 未按下)
//==========================================================================
volatile u8 jp_A       = 0;
volatile u8 jp_B       = 0;
volatile u8 jp_Select  = 0;
volatile u8 jp_Start   = 0;
volatile u8 jp_Up      = 0;
volatile u8 jp_Down    = 0;
volatile u8 jp_Left    = 0;
volatile u8 jp_Right   = 0;

//==========================================================================
//                         簡易微秒延遲 (72MHz 時脈)
//==========================================================================
void delay_us(u32 us)
{
    u32 i;
    for (i = 0; i < us * 72; i++);      // 72MHz 下約 1μs 一個迴圈
}

//==========================================================================
//                         JoyPad 初始化
//==========================================================================
void JoyPad_Init(void)
{
    // 啟用 GPIOB 與 GPIOD 時脈
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPDEN;

    // PB11 (Latch) → Push-Pull 輸出 50MHz
    GPIOB->CRH &= 0xFFFF0FFF;
    GPIOB->CRH |= 0x00003000;

    // PB10 (Data) → 上拉輸入
    GPIOB->CRH &= 0xFFFFF0FF;
    GPIOB->CRH |= 0x00000800;
    GPIOB->ODR |= (1 << 10);            // 啟用內部上拉

    // PD3 (Clock) → Push-Pull 輸出 50MHz
    GPIOD->CRL &= 0xFFFF0FFF;
    GPIOD->CRL |= 0x00003000;

    // 初始狀態：Latch 與 Clock 皆為低電平
    GPIOB->BRR = (1 << 11);
    GPIOD->BRR = (1 << 3);
}

//==========================================================================
//                         讀取 JoyPad 按鍵狀態
//  順序：A → B → Select → Start → Up → Down → Left → Right
//==========================================================================
void Read_JoyPad(void)
{
    u8 i;
    u8 data[8];

    // 1. 發送 Latch 脈衝，讓手把鎖存目前按鍵狀態
    GPIOB->BSRR = (1 << 11);    // Latch HIGH
    delay_us(20);
    GPIOB->BRR  = (1 << 11);    // Latch LOW
    delay_us(20);

    // 2. 連續送 8 次 Clock，逐一讀取 8 個按鍵
    for (i = 0; i < 8; i++)
    {
        // 讀取資料線 (低電平 = 按下)
        data[i] = ((GPIOB->IDR & (1 << 10)) == 0) ? 1 : 0;

        // 送出 Clock 脈衝
        GPIOD->BSRR = (1 << 3); // Clock HIGH
        delay_us(10);
        GPIOD->BRR  = (1 << 3); // Clock LOW
        delay_us(10);
    }

    // 3. 將讀到的資料寫入全域變數
    jp_A       = data[0];
    jp_B       = data[1];
    jp_Select  = data[2];
    jp_Start   = data[3];
    jp_Up      = data[4];
    jp_Down    = data[5];
    jp_Left    = data[6];
    jp_Right   = data[7];
}

