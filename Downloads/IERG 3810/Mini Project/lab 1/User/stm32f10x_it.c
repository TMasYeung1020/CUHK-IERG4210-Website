/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

extern volatile u32 game_ticks;
extern volatile u32 game_seconds;
extern void	LCD_ShowTime(u16 x,u16 y);
extern void LCD_ShowCoinCountP1(u16 x,u16 y);
extern void LCD_ShowCoinCountP2(u16 x,u16 y);
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//===================================================================
//                          ?????????
//===================================================================

extern volatile u32 sysTickCounter;     // ?? 1ms ???
extern volatile u32 game_ticks;         // ??????(??)
extern volatile u32 game_seconds;       // ??????
extern volatile u16 coin1_count;        // Player1 ???
extern volatile u16 coin2_count;        // Player2 ???

//============== ????? ==============
volatile u8  buzzer_on           = 0;    // ?????????
volatile u32 buzzer_toggle_time  = 0;    // ????? PB8 ???
volatile u32 buzzer_half_period  = 0;    // ???(????,500000/freq)
volatile u32 note_end_time       = 0;    // ????????
volatile u32 rest_end_time       = 0;    // ????????????

//============== ?????? ==============
const u16 melody[][2] = {
    {440, 150}, {440, 150}, {440, 150}, {0, 0}   // ???? {0,0} ????
};

volatile u16 music_index    = 0;         // ??????????
volatile u8  music_playing  = 0;         // ??????????

//===================================================================
//                          ??????
//===================================================================

void Music_Start(void)  
{ 
    music_playing = 1; 
    music_index   = 0; 
    rest_end_time = sysTickCounter; 
}

void Music_Stop(void)  
{ 
    music_playing = 0; 
    buzzer_on     = 0; 
    GPIOB->BRR    = (1 << 8);               // ?????
}

void Play_SFX(u16 freq, u16 ms);            // ??(?????)

//===================================================================
//                          ???????
//===================================================================

void Buzzer_Play_Note(u16 freq, u16 duration_ms)
{
    if (freq == 0)                                      // ???
    {
        buzzer_on     = 0;
        GPIOB->BRR    = (1 << 8);
        note_end_time = sysTickCounter + duration_ms;
        rest_end_time = note_end_time + 80;             // ????
        return;
    }

    buzzer_on          = 1;
    GPIOB->BSRR        = (1 << 8);                      // ?????
    buzzer_half_period = 500000 / freq;                 // ?????(??)
    buzzer_toggle_time = sysTickCounter;                // ??????
    note_end_time      = sysTickCounter + duration_ms;
    rest_end_time      = note_end_time + 80;
}

void Play_SFX(u16 freq, u16 ms)
{
    Buzzer_Play_Note(freq, ms);
}

//===================================================================
//                     ? 1ms ????(?? SysTick)
//===================================================================

void Buzzer_Update_1ms(void)
{
    // ??????? ? ?????
    if (buzzer_on && sysTickCounter >= note_end_time)
    {
        buzzer_on = 0;
        GPIOB->BRR = (1 << 8);
    }

    // ????:????? PB8
    if (buzzer_on && sysTickCounter >= buzzer_toggle_time)
    {
        GPIOB->ODR ^= (1 << 8);
        buzzer_toggle_time += buzzer_half_period / 1000 + 1;   // ??????
    }

    // ?????? ? ???????(????)
    if (!buzzer_on && sysTickCounter >= rest_end_time && music_playing)
    {
        u16 freq = melody[music_index][0];
        u16 dur  = melody[music_index][1];

        if (freq == 0)                                      // ????
        {
            music_index = 0;
            rest_end_time = sysTickCounter + 500;           // ? 0.5 ????
        }
        else
        {
            Buzzer_Play_Note(freq, dur);
            music_index++;
        }
    }
}

//===================================================================
//                          SysTick ????
//===================================================================

void SysTick_Handler(void)
{
    sysTickCounter++;
    game_ticks++;

    if (game_ticks >= 1000)
    {
        game_ticks = 0;
        game_seconds++;

        if ((coin1_count + coin2_count) < 35 && game_seconds < 60)
        {
            LCD_ShowTime(10, 10);
            LCD_ShowCoinCountP1(110, 10);
            LCD_ShowCoinCountP2(170, 10);
        }
    }

    Buzzer_Update_1ms();       // ?????????????
}

