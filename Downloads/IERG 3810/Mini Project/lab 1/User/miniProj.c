#include "stm32f10x.h"
#include "LCDinit.h"
#include "IERG3810_TFTLCD.h"
#include "IERG3810_LED.h"
#include "IERG3810_Buzzer.h"
#include "IERG3810_KEY.h"
#include <time.h>
#include "BKG.H"
#include "joypad.h"


/* -------------------------- 顏色定義 -------------------------- */
#define Red         0xF800
#define Blue        0x001F
#define bkg_color   0xEFDF
#define Black       0x0000
#define ICE_COLOR   0x001F          // 冰牆顏色

/* -------------------------- 地圖與格子設定 -------------------------- */
#define MAP_WIDTH   15              // 15 格寬  (240/16)
#define MAP_HEIGHT  20              // 20 格高  (320/16)
#define BLOCK_SIZE  16              // 每格 16x16 像素

/* -------------------------- 遊戲參數 -------------------------- */
#define PLAYER_MOVE_SPEED   4       // 每幀移動像素數
#define WAIT_TIME           30      // 方向鍵防連發間隔 (ms)
#define ICE_SPEED_MS        40      // 冰牆每格生成間隔 (ms)
#define PS2_TIMEOUT_MS      10      // PS/2 clock 逾時判斷 (ms)

/* -------------------------- PS/2 按鍵碼 -------------------------- */
#define left_KEY    0x7A            // Z
#define down_KEY    0x72            // Down
#define right_KEY   0x69            // Right
#define up_KEY      0x73            // Up
#define ICE_KEY     0x75            // U

/* -------------------------- 系統時間 -------------------------- */
volatile u32 sysTickCounter = 0;                    // 1ms Tick，全域最核心計時器
volatile u32 game_ticks     = 0;                    // 保留：10ms 為單位的遊戲總 tick 數（可用於精確計時）

/* -------------------------- PS/2 鍵盤接收狀態 -------------------------- */
volatile u16 ps2_shift            = 0;              // 正在接收的 11-bit 資料
volatile u8  ps2_bitcount         = 0;              // 已接收位元數
volatile u8  F0flag               = 0;              // 收到 0xF0 釋放碼旗標
volatile u32 ps2_last_clock_time  = 0;              // 最後收到 clock 的時間（逾時檢測用）
volatile u8  ps2_error_count      = 0;              // 保留：PS/2 連續錯誤計數（可用於除錯或重置）

/* -------------------------- 方向鍵狀態旗標 -------------------------- */
u8 key_up_pressed    = 0;
u8 key_down_pressed  = 0;
u8 key_left_pressed  = 0;
u8 key_right_pressed = 0;
/*  -------------------------- Joypad 專用旗標與冷卻時間 -------------------------- */
volatile u8 joy_up_pressed    = 0;
volatile u8 joy_down_pressed  = 0;
volatile u8 joy_left_pressed  = 0;
volatile u8 joy_right_pressed = 0;

volatile u32 last_joy_up_time    = 0;
volatile u32 last_joy_down_time  = 0;
volatile u32 last_joy_left_time  = 0;
volatile u32 last_joy_right_time = 0;
volatile u32 last_joy_ice_time   = 0;   // Joypad 放冰冷卻

/* -------------------------- 玩家 1 (鍵盤控制) -------------------------- */
u16 newP1_x = 0, newP1_y = 0;                       // 當前顯示座標
u16 oldP1_x = 0, oldP1_y = 0;                       // 上次顯示座標（清除用）
u16 player_color = bkg_color;                       // 玩家1 圖案顏色

volatile u8  player1_moving   = 0;                  // 是否正在格子對齊移動
volatile u16 target1_x        = 0, target1_y = 0;   // 目標像素座標
volatile u8  player1_map_x    = 0, player1_map_y = 0; // 當前邏輯格子座標

/* -------------------------- 玩家 2 (Joypad 控制) -------------------------- */
u16 newP2_x = 0, newP2_y = 0;
u16 oldP2_x = 0, oldP2_y = 0;
u16 player2_color = 0x07E0;                         // 綠色

volatile u8  player2_moving   = 0;
volatile u16 target2_x        = 0, target2_y = 0;
volatile u8  player2_map_x    = 0, player2_map_y = 0;

/* -------------------------- 地圖與金幣 -------------------------- */
u8 blockMap[MAP_HEIGHT][MAP_WIDTH];                                 // 1=牆, 0=可走
u8 coinMap[MAP_HEIGHT][MAP_WIDTH] = {0};                            // 1=有金幣
u8 collectFrameMap[MAP_HEIGHT][MAP_WIDTH] = {0};                    // 金幣被吃時的閃爍動畫幀數

/* -------------------------- 冰牆系統（兩人共用） -------------------------- */
volatile u8 player1_direction = 0;   // 1上 2下 3左 4右
volatile u8 player2_direction = 0;   // 1上 2下 3左 4右
volatile u8  ice_animating     = 0;                 // 是否正在生成冰牆
volatile u32 next_IceTime      = 0;                 // 下一格冰的生成時間
volatile u16 ice_x = 0, ice_y = 0;                  // 當前正在生成的冰格座標
volatile s16 ice_dx = 0, ice_dy = 0;                // 生成方向增量
volatile u16 iceExpireTime[MAP_HEIGHT][MAP_WIDTH] = {0};  // 每格冰的消失時間點
volatile u32 ice_melt_start_time = 0;               // 開始快速融化的基準時間
volatile u8 ice_hard_stun = 0;    // 1 = 正在放冰硬直，完全不能動！
volatile u32 global_ice_cooldown = 0;    // 全域放冰冷卻計時器（單位：毫秒）

/* -------------------------- 按鍵冷卻時間 -------------------------- */
volatile u32 last_up_time    = 0;
volatile u32 last_down_time  = 0;
volatile u32 last_left_time  = 0;
volatile u32 last_right_time = 0;
volatile u32 last_ice_time   = 0;                   // 放冰牆冷卻
volatile u32 last_update     = 0;                   // 上次畫面更新時間

/* -------------------------- 遊戲狀態 -------------------------- */
volatile u16 coin1_count = 0;                       // Player 1 金幣數
volatile u16 coin2_count = 0;                       // Player 2 金幣數
volatile u32 game_seconds = 0;                      // 遊戲已過秒數（顯示用）

/* -------------------------- +1 彈出動畫（保留變數） -------------------------- */
char pop_buf[8];                                    // 「+1」字串緩衝
u8   collect_popup_active = 0;                      // 是否啟動彈出動畫
u16  collect_popup_x = 0, collect_popup_y = 0;      // 彈出起始座標
u8   collect_popup_frame = 0;                       // 剩餘幀數


///=======function part========

void Delay(u32 count)
{
    while (count--);
}


u8 ps2_sample_data(void) // 讀取 PS/2 鍵盤資料線（PC10）
{ 
    return (GPIOC->IDR >> 10) & 1;
}




void map_init(void) // 初始化遊戲地圖（四周牆、中間空）
{ 
    // 先全填成牆
    for (u8 x = 0; x < MAP_WIDTH; x++)
        for (u8 y = 0; y < MAP_HEIGHT; y++)
            blockMap[y][x] = 1;

    // 挖空可走區域
    for (u8 x = 1; x < MAP_WIDTH - 1; x++)
        for (u8 y = 3; y < MAP_HEIGHT - 1; y++)
            blockMap[y][x] = 0;
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


void DrawPlayer1(u16 x, u16 y, u16 color) {
    		lcd_showP1(x, y, color);
}


void ClearPlayer1(u16 x, u16 y) {
    lcd_clearP1(x, y, bkg_color); // ????
}

void DrawPlayer2(u16 x, u16 y, u16 color) {
    		lcd_showP2(x, y, color);
}

void ClearPlayer2(u16 x, u16 y) {
    lcd_clearP2(x, y, bkg_color); // ????
}
void player2_init(){
        // Player 2 起始位置（例如右下角）
    newP2_x = oldP2_x = 13 * BLOCK_SIZE;   // x=13
    newP2_y = oldP2_y = 18 * BLOCK_SIZE;   // y=18
		oldP2_x = newP2_x;   // 一定要再寫一次！
    oldP2_y = newP2_y;
    player2_map_x = 13;
    player2_map_y = 18;
    target2_x = newP2_x;
    target2_y = newP2_y;
    player2_moving = 0;

    DrawPlayer2(newP2_x, newP2_y, player2_color);  // 畫出 Player 2
}
//=========================================================================Buzzer=========================================================
void Delay_us(uint32_t us)
{
    us *= 8;                    // 72MHz 大約 8 個指令週期 = 1us
    while(us--) 
    {
        __NOP();
    }
}
void Delay_ms(uint32_t ms)
{
    while(ms--)
    {
        Delay_us(1000);
    }
}
//=====================================================================================buzzer===================================================================
void nvic_setPriorityGroup(u8 priGroup){
    //-- set PRIGROUP AIRCR[10:8]
    u32 tmp1, tmp2;
    tmp2 = priGroup & 0x00000007 << 8;     // only concern 3 bits
    tmp1 = SCB->AIRCR &= 0x0000F8FF;       // ARMDI0337 page 8-22
    tmp1 |= 0x05FA0000;                    // *** must read ARMDI0337 page 8-22
    SCB->AIRCR = tmp1 |= tmp2;
}
void Game_Start(void) {
    game_ticks = 0;
    game_seconds = 0;
    coin1_count = 0;
		coin2_count = 0; 	// 真正啟動 10ms 計時器
	
		ice_animating = 0;
    player1_direction = 0;
		player2_direction = 0;
    ice_melt_start_time = 0;
    for(int y=0; y<MAP_HEIGHT; y++)
        for(int x=0; x<MAP_WIDTH; x++)
					iceExpireTime[y][x] = 0;
		Music_Start();
}

void Collect1_Coin(void) {
    coin1_count++;	// 金幣數量 +1
    //Buzzer_On(); DelayMs(50); Buzzer_Off(); // 可選音效
}
void Collect2_Coin(void) {
    coin2_count++;	// 金幣數量 +1
    //Buzzer_On(); DelayMs(50); Buzzer_Off(); // 可選音效
}


void LCD_ShowTime(u16 x, u16 y) {
    u16 min = game_seconds / 60;
    u16 sec = game_seconds % 60;
    
    // 清除舊的位置（用背景色覆蓋）
    lcd_fillRectangle(bkg_color, x, 100, y, 16);  // 白底清掉舊文字
    
    // 顯示 "Time: 01:23"
    lcd_showChar(x+0,  y, 'T', Red, bkg_color);
    lcd_showChar(x+8,  y, 'I', Red, bkg_color);
    lcd_showChar(x+16, y, 'M', Red, bkg_color);
    lcd_showChar(x+24, y, 'E', Red, bkg_color);
    lcd_showChar(x+32, y, '-', Red, bkg_color);
    
    // 分鐘十位與個位
    lcd_showChar(x+40, y, '0' + (min / 10), Red, bkg_color);
    lcd_showChar(x+48, y, '0' + (min % 10), Red, bkg_color);
    lcd_showChar(x+56, y, ':', Red, bkg_color);
    
    // 秒鐘十位與個位
    lcd_showChar(x+64, y, '0' + (sec / 10), Red, bkg_color);
    lcd_showChar(x+72, y, '0' + (sec % 10), Red, bkg_color);
}

void LCD_ShowCoinCountP1(u16 x, u16 y) {
    // 清除舊分數區域
    lcd_fillRectangle(bkg_color, x, 80, y, 16);
    
    lcd_showChar(x+0,  y, 'P', Blue, bkg_color);
    lcd_showChar(x+8,  y, '1', Blue, bkg_color);
		lcd_showChar(x+16, y, ':', Blue, bkg_color);
    
    // 顯示金幣數量（最多支援 999）
    u16 temp = coin1_count;
    if (temp == 0) {
        lcd_showChar(x+24, y, '0', Blue, bkg_color);
    } else {
        u8 digit[3];
        u8 i = 0;
        while (temp > 0 && i < 3) {
            digit[i++] = temp % 10;
            temp /= 10;
        }
        u16 pos = x + 32;
        while (i > 0) {
            lcd_showChar(pos, y, '0' + digit[--i], Blue, bkg_color);
            pos += 8;
        }
    }
}

void LCD_ShowCoinCountP2(u16 x, u16 y) {
    // 清除舊分數區域
    lcd_fillRectangle(bkg_color, x, 80, y, 16);
    
    lcd_showChar(x+0,  y, 'P', Blue, bkg_color);
    lcd_showChar(x+8,  y, '2', Blue, bkg_color);
		lcd_showChar(x+16, y, ':', Blue, bkg_color);
    
    // 顯示金幣數量（最多支援 999）
    u16 temp = coin2_count;
    if (temp == 0) {
        lcd_showChar(x+24, y, '0', Blue, bkg_color);
    } else {
        u8 digit[3];
        u8 i = 0;
        while (temp > 0 && i < 3) {
            digit[i++] = temp % 10;
            temp /= 10;
        }
        u16 pos = x + 32;
        while (i > 0) {
            lcd_showChar(pos, y, '0' + digit[--i], Blue, bkg_color);
            pos += 8;
        }
    }
}

void StartIceWall(u8 dir, u16 start_x, u16 start_y)
{
    // 計算第一格冰要出現在玩家「正前方」
    ice_dx = 0; ice_dy = 0;
    switch(dir)
    {
        case 1: ice_dy = 16; break;  // 上
        case 2: ice_dy = -16; break;  // 下
        case 3: ice_dx = -16; break;  // 左
        case 4: ice_dx = +16; break;  // 右
    }

    ice_x = start_x + ice_dx;
    ice_y = start_y + ice_dy;

    // 檢查第一格能不能放（不能出界、不能有牆）
    u8 mx = ice_x / BLOCK_SIZE;
    u8 my = ice_y / BLOCK_SIZE;
    if (mx >= MAP_WIDTH || my >= MAP_HEIGHT || blockMap[my][mx] == 1)
        return;  // 第一格就擋住 → 不放

    // 正式開始生成！
    ice_animating = 1;
    next_IceTime = sysTickCounter + ICE_SPEED_MS;  // ICE_Speed_MS 後放下一格
    Play_SFX(150, 400);  // 放冰音效
}
	
// Player 1 用鍵盤 U 鍵放冰（你已經有了，但改成共用函數更好）
void CreateIceWall_P1(void)
{
    if (ice_animating || player1_moving || player1_direction == 0) return;
    if (sysTickCounter < global_ice_cooldown) return;

		StartIceWall(player1_direction, newP1_x, newP1_y);  // 永遠從 P1 自己射
    global_ice_cooldown = sysTickCounter + 500;        // 冷卻 0.5 秒
    ice_hard_stun = 1;
}

// Player 2 用 Joypad A 鍵放冰
void CreateIceWall_P2(void)
{
	if (ice_animating || player2_moving || player2_direction == 0) return;
    if (sysTickCounter < global_ice_cooldown) return;

    StartIceWall(player2_direction, newP2_x, newP2_y);  // 永遠從 P2 自己射
    global_ice_cooldown = sysTickCounter + 500;
    ice_hard_stun = 1;
}

void coin_init(void) {
    // Step 2.1: 清空所有格子 (全設 0)
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            coinMap[y][x] = 0;
        }
    }
    // 清空動畫地圖（確保無殘留）
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
        collectFrameMap[y][x] = 0;
        }
    }
    // Step 2.2: 放置 35 個固定隨機金幣 (避開邊緣 + 玩家起始位，避免一開始就收)
    coinMap[ 4][ 2] = 1; coinMap[ 4][ 6] = 1; coinMap[ 4][10] = 1;
    coinMap[ 6][ 3] = 1; coinMap[ 6][ 7] = 1; coinMap[ 6][11] = 1;
    coinMap[ 8][ 2] = 1; coinMap[ 8][ 5] = 1; coinMap[ 8][ 9] = 1; coinMap[ 8][12] = 1;
    coinMap[10][ 4] = 1; coinMap[10][ 8] = 1; coinMap[10][11] = 1;
    coinMap[12][ 2] = 1; coinMap[12][ 6] = 1; coinMap[12][10] = 1; 

    coinMap[ 5][ 4] = 1; coinMap[ 5][ 9] = 1;
    coinMap[ 7][ 3] = 1; coinMap[ 7][ 7] = 1; coinMap[ 7][11] = 1;
    coinMap[ 9][ 5] = 1; coinMap[ 9][ 9] = 1;
    coinMap[11][ 3] = 1; coinMap[11][ 7] = 1; coinMap[11][11] = 1;
    coinMap[13][ 4] = 1; coinMap[13][10] = 1;

    coinMap[15][ 2] = 1; coinMap[15][ 6] = 1; coinMap[15][10] = 1; coinMap[15][12] = 1;
    coinMap[17][ 3] = 1; coinMap[17][11] = 1;
    coinMap[18][ 5] = 1;
}
void PS2_ResetAllKeyStates(void){
    // ???????????
    key_up_pressed    = 0;
    key_down_pressed  = 0;
    key_left_pressed  = 0;
    key_right_pressed = 0;
    
    // ??!???????,???????????
    last_up_time    = sysTickCounter;
    last_down_time  = sysTickCounter;
    last_left_time  = sysTickCounter;
    last_right_time = sysTickCounter;
    last_ice_time   = sysTickCounter - 1000;  // ???????
    
}

/* ==================== Joypad 按鍵處理函數（跟鍵盤完全平行） ==================== */
void set_joypad_flags(void)
{
    u32 now = sysTickCounter;

    // 上
    if (jp_Up) {
        if (!joy_up_pressed) {  // 剛按下
            joy_up_pressed = 1;
            if (now - last_joy_up_time >= WAIT_TIME) {
                last_joy_up_time = now;
                player2_direction = 1;           // 面向上（放冰牆用）
            }
        }
    } else {
        joy_up_pressed = 0;
    }

    // 下
    if (jp_Down) {
        if (!joy_down_pressed) {
            joy_down_pressed = 1;
            if (now - last_joy_down_time >= WAIT_TIME) {
                last_joy_down_time = now;
                player2_direction = 2;
            }
        }
    } else {
        joy_down_pressed = 0;
    }

    // 左
    if (jp_Left) {
        if (!joy_left_pressed) {
            joy_left_pressed = 1;
            if (now - last_joy_left_time >= WAIT_TIME) {
                last_joy_left_time = now;
                player2_direction = 3;
            }
        }
    } else {
        joy_left_pressed = 0;
    }

    // 右
    if (jp_Right) {
        if (!joy_right_pressed) {
            joy_right_pressed = 1;
            if (now - last_joy_right_time >= WAIT_TIME) {
                last_joy_right_time = now;
                player2_direction = 4;
            }
        }
    } else {
        joy_right_pressed = 0;
    }

    // 放冰牆（用 A 鍵，jp_A）
    if (jp_A) {
        if (now - last_joy_ice_time >= 500) {  // 500ms 冷卻
            CreateIceWall_P2();                // Player 2 專用放冰
            last_joy_ice_time = sysTickCounter;
        }
    }
}

void set_key_flags(u8 code, u8 pressed) {
    u32 now = sysTickCounter;

    // ===== ???:?????????? + ???? =====
    if (code == up_KEY)    
    { 
        key_up_pressed = pressed; 
        if (pressed) {
            if (now - last_up_time >= WAIT_TIME) {
                last_up_time = now;
                player1_direction = 1;        // ? ??!???????
            }
        } else {
            key_up_pressed = 0;
        }
    }
    else if (code == down_KEY)  
    { 
        key_down_pressed = pressed; 
        if (pressed) {
            if (now - last_down_time >= WAIT_TIME) {
                last_down_time = now;
                player1_direction = 2;        // ? ??!???????
            }
        } else {
            key_down_pressed = 0;
        }
    }
    else if (code == left_KEY)  
    { 
        key_left_pressed = pressed; 
        if (pressed) {
            if (now - last_left_time >= WAIT_TIME) {
                last_left_time = now;
                player1_direction = 3;        // ? ??!???????
            }
        } else {
            key_left_pressed = 0;
        }
    }
    else if (code == right_KEY) 
    { 
        key_right_pressed = pressed; 
        if (pressed) {
            if (now - last_right_time >= WAIT_TIME) {
                last_right_time = now;
                player1_direction = 4;        // ? ??!???????
            }
        } else {
            key_right_pressed = 0;
        }
    }

    // ===== ??? U (0x75) =====
    else if (code == ICE_KEY) 
    {
        if (pressed && (now - last_ice_time >= 500))  // 500ms ??
        {
            CreateIceWall_P1();           // ????? current_direction ??!
            last_ice_time = sysTickCounter;
        }
    }

}

void EXTI15_10_IRQHandler(void){
    if (EXTI->PR & (1<<11))
    {
        ps2_last_clock_time = sysTickCounter;
        // ps2_error_count = 0;  // ????!???????

        uint8_t bit = ps2_sample_data();
        ps2_shift |= ((uint16_t)bit << ps2_bitcount);
        ps2_bitcount++;

        if (ps2_bitcount >= 11)
        {
            uint16_t f = ps2_shift;
            uint8_t start  = (f >> 0)  & 1;
            uint8_t data   = (f >> 1)  & 0xFF;
            uint8_t parity = (f >> 9)  & 1;
            uint8_t stop   = (f >> 10) & 1;
            uint8_t calc_parity = __builtin_popcount(data) & 1;

            if (start == 0 && stop == 1 && parity == (calc_parity ^ 1))
            {
                // ???? ? ???? + ???
                ps2_error_count = 0;

                if (data == 0xF0) {
                    F0flag = 1;
                } else {
                    if (F0flag) {
                        set_key_flags(data, 0);
                        F0flag = 0;
                    } else {
                        set_key_flags(data, 1);
                    }
                }
            }
            else
            {
                // ???? ? ????!?????!
                PS2_ResetAllKeyStates();
                ps2_error_count = 0;  // ??:?????
            }

            // ???????(?? retry)
            ps2_shift = 0;
            ps2_bitcount = 0;
        }

        EXTI->PR = (1<<11);
    }
}

// print game background
void printgamemap(void){
			int i,j,k;
			k = 0;
			// fill horizontal part
			for (i = 0; i < 16; i++){
				for (j = 0; j < 240; j++){
					lcd_fillRectangle(gamemap_horizontal_block[k], 0+j, 1, 320-i, 1);
					lcd_fillRectangle(gamemap_horizontal_block[k], 0+j, 1, 48-i, 1);
					k++;
				}
			}
			k = 0;
			
			// fill vertical part
			for (i = 0; i < 288; i++){
				for (j = 0; j < 16; j++){
					lcd_fillRectangle(gamemap_vertical_block[k], 0+j, 1, 320-i, 1);
					lcd_fillRectangle(gamemap_vertical_block[k], 225+j, 1, 320-i, 1);
					k++;
				}
			}
			k = 0;
			// fill background color
			lcd_fillRectangle(bkg_color, 16, 209, 48, 257);
			lcd_fillRectangle(bkg_color, 0, 240, 0, 32);
			
}

void printgroupinfo(void){
			int i;
			printgamemap();
			// Title: Group Information (B15)
			lcd_showString(30, 250, "Group Information (B15)", c_RED, 0xef17);
			lcd_showString(31, 250, "Group Information (B15)", c_RED, 0xef17);
			lcd_showString(32, 250, "-----------------------", c_RED, 0xef17);
			
			for (i = 0; i < 180; i++){
				lcd_showString(i + 30, 220, "=", c_black, 0xef17);
			}
			
			lcd_showString(30, 200, "1155203181", c_black, 0xef17);
			lcd_showString(31, 201, "1155203181", c_black, 0xef17);
			
			lcd_showString(30, 160, "Yeung Man, Thomas", c_black, 0xef17);
			lcd_showString(31, 161, "Yeung Man, Thomas", c_black, 0xef17);
			
			for (i = 0; i < 180; i++){
				lcd_showString(i + 30, 140, "=", c_black, 0xef17);
			}
			
			lcd_showString(30, 120, "1155213238", c_black, 0xef17);
			lcd_showString(31, 121, "1155213238", c_black, 0xef17);
			
			lcd_showString(30, 80, "Wong Man Hei", c_black, 0xef17);
			lcd_showString(31, 81, "Wong Man Hei", c_black, 0xef17);
			
			for (i = 0; i < 180; i++){
				lcd_showString(i + 30, 60, "=", c_black, 0xef17);
			}
		}

void printmenuitems(void){
			int i,j,k;
			// button x: 45-195, y: 75-225
			k = 0;
			for (i = 0; i < 149; i++){
				for (j = 0; j < 150; j++){
					lcd_fillRectangle(playbutton[k], 45+j, 1, 225-i, 1);
					k++;
				}
			}
			k = 0;

			// Welcoming message
			lcd_showString(30, 250, "WELCOME TO IERG 3810!!!", c_RED, 0xef17);
			lcd_showString(31, 251, "WELCOME TO IERG 3810!!!", c_RED, 0xef17);
			
			// Click Start to Play
			lcd_showString(32, 10, "Click Start to Play!!!", c_black, 0xef17);
			lcd_showString(33, 11, "Click Start to Play!!!", c_black, 0xef17);
}

void printgamerules(void){
			printgamemap();
			lcd_showString(30, 270, "Game Rules of This Game", c_RED, 0xef17);
			lcd_showString(30, 265, "-----------------------", c_RED, 0xef17);
			lcd_showString(20, 240, "Keyboard Control-----", c_black, 0xef17);
			lcd_showString(20, 220, "Up: Key 5, Down: Key 2", c_black, 0xef17);
			lcd_showString(20, 200, "Left: Key 1, Right: Key 3", c_black, 0xef17);
			lcd_showString(20, 180, "Ice Wall: Key 8", c_black, 0xef17);
			
			lcd_showString(20, 140, "JoyPad Control-----", c_black, 0xef17);
			lcd_showString(20, 120, "Movements: +", c_black, 0xef17);
			lcd_showString(20, 100, "Ice Wall: RLow (A)", c_black, 0xef17);
			lcd_showString(20, 80, "Try to collect more coin", c_black, 0xef17);
			lcd_showString(20, 60, "Finish Game within 1 min", c_black, 0xef17);
}

void printendgamemenu(){
			printgamemap();
			lcd_showString(35, 270, "======Game Over======", c_black, 0xef17);
			if (game_seconds != 60){
				LCD_ShowTime(80, 220);
			}
			else{lcd_showString(70, 220, "Time is Over", c_RED, 0xef17);}
			LCD_ShowCoinCountP1(65, 170);
			LCD_ShowCoinCountP2(130, 170);
			if(coin1_count<coin2_count){
				lcd_showString(90, 100, "P2 Win!!!", c_RED, 0xef17);
				}
			else if(coin1_count > coin2_count){
				lcd_showString(90, 100, "P1 Win!!!", c_RED, 0xef17);
			}
			else{lcd_showString(80, 100, "No Winner!!!", c_RED, 0xef17);}
			lcd_showString(26, 60, "<Click Start to Restart>", c_black, 0xef17);
}

// 直接呼叫這函數就會響起經典 Windows XP 開機音

int main(void)
{
    /*==================== 硬體初始化 ====================*/
		Buzzer_Init();
    LED_Init();
    KEY_Init();
    ps2key_exitInit();      // PS/2 鍵盤中斷設定
    lcd_init();
    JoyPad_Init();
    player2_init();
    Delay(1000000);         // 給所有外設穩定時間

    nvic_setPriorityGroup(5);

    // Player 1（鍵盤玩家）出生在「最左邊 + 正中間高度」
		newP1_x = oldP1_x = 1 * BLOCK_SIZE;           // x = 1（最左邊第 1 格，避開邊牆）
		newP1_y = oldP1_y = 10 * BLOCK_SIZE;          // y = 10（20 格高的一半 = 正中間）
		player1_map_x = 1;
		player1_map_y = 10;
		target1_x = newP1_x;
		target1_y = newP1_y;
		player1_moving = 0;

		// Player 2（Joypad 玩家）出生在「最右邊 + 正中間高度」
		newP2_x = oldP2_x = 13 * BLOCK_SIZE;          // x = 13（15-2=13，最右邊第 1 格）
		newP2_y = oldP2_y = 10 * BLOCK_SIZE;          // y = 10（一樣在中間）
		player2_map_x = 13;
		player2_map_y = 10;
		target2_x = newP2_x;
		target2_y = newP2_y;
		player2_moving = 0;

    printgamemap();
    printmenuitems();

    /*==================== 主選單迴圈 ====================*/
    while(1)
    {
        Read_JoyPad();
				set_joypad_flags();
        if (!jp_Start) continue;			// 沒按 Start 就持續等待
				
				DS0_on;
        printgroupinfo();               // 顯示組別資訊
        Delay(20000000);
        printgamerules();               // 顯示遊戲規則
        Delay(50000000);

        /*==================== 遊戲開始初始化 ====================*/
        printgamemap();
        DrawPlayer1(oldP1_x, oldP1_y, player_color);
        DrawPlayer2(oldP2_x, oldP2_y, player2_color);
        map_init();
        coin_init();
        SysTick_Config(SystemCoreClock / 1000);

        u32 lastUpdateTime = 0;
        const u32 MOVE_INTERVAL = 10;   // 約 100fps 主循環
				u32 game_start_time = sysTickCounter;
        Game_Start();                   // 重置分數、時間、開音樂
				DS0_off;

        /*==================== 主要遊戲迴圈 ====================*/
        while((coin1_count + coin2_count) < 35 && (sysTickCounter - game_start_time) < 60000)
        {
            if (sysTickCounter - lastUpdateTime >= MOVE_INTERVAL)
            {
                Read_JoyPad();
								set_joypad_flags();
                lastUpdateTime = sysTickCounter;
                /*---------- Player 1 吃金幣 ----------*/
                if (coinMap[player1_map_y][player1_map_x] == 1 &&
                    collectFrameMap[player1_map_y][player1_map_x] == 0)
                {
                    collectFrameMap[player1_map_y][player1_map_x] = 2;
                    Collect1_Coin();
                    Play_SFX(659, 60);
                    DS1_on;
                    LCD_ShowCoinCountP1(110,10);
                    collect_popup_active = 1;
                    collect_popup_x = player1_map_x * BLOCK_SIZE + 4;
                    collect_popup_y = player1_map_y * BLOCK_SIZE + 16;
                    collect_popup_frame = 10;
                }

                /*---------- 金幣繪製與動畫 ----------*/
                for (u8 mapY = 0; mapY < MAP_HEIGHT; mapY++)
                {
                    for (u8 mapX = 0; mapX < MAP_WIDTH; mapX++)
                    {
                        if (coinMap[mapY][mapX] == 1)
                        {
                            u8 frame = collectFrameMap[mapY][mapX];
                            if (frame > 0)
                            {
                                u16 coin_color = (frame % 3 == 0) ? 0x07E0 : 0xFFE0;
                                lcd_showCoin(mapX * 16, mapY * 16, coin_color);
                                collectFrameMap[mapY][mapX]--;
                                if (collectFrameMap[mapY][mapX] == 0)
                                    coinMap[mapY][mapX] = 0;
                            }
                            else
                            {
                                lcd_showCoin(mapX * 16, mapY * 16, 0xFFE0);
                            }
                        }
                    }
                }

                /*---------- Player 2 吃金幣 ----------*/
                if (coinMap[player2_map_y][player2_map_x] == 1 &&
                    collectFrameMap[player2_map_y][player2_map_x] == 0)
                {
										collectFrameMap[player2_map_y][player2_map_x] = 2;
                    Collect2_Coin();
                    Play_SFX(659, 60);
                    DS1_on;
                    LCD_ShowCoinCountP2(170,10);
                    collect_popup_active = 1;
                    collect_popup_x = player2_map_x * BLOCK_SIZE + 4;
                    collect_popup_y = player2_map_y * BLOCK_SIZE + 16;
                    collect_popup_frame = 10;
                }
                /*---------- Player 2 移動判定（改用旗標版，超穩不卡鍵）----------*/
						if (!player2_moving && !ice_hard_stun)
						{
								u8 try_x = player2_map_x;
								u8 try_y = player2_map_y;

								if (joy_up_pressed)    { try_y++; }
								else if (joy_down_pressed)  { try_y--; }
								else if (joy_left_pressed)  { try_x--; }
								else if (joy_right_pressed) { try_x++; }

								if (try_x < MAP_WIDTH && try_y < MAP_HEIGHT &&
										blockMap[try_y][try_x] == 0 &&
										!(try_x == player1_map_x && try_y == player1_map_y))
								{
										player2_moving = 1;
										target2_x = try_x * BLOCK_SIZE;
										target2_y = try_y * BLOCK_SIZE;
										player2_map_x = try_x;
										player2_map_y = try_y;
								}
						}

                /*---------- Player 2 實際移動 ----------*/
                if (player2_moving)
                {
                    if (newP2_x < target2_x) newP2_x += PLAYER_MOVE_SPEED;
                    if (newP2_x > target2_x) newP2_x -= PLAYER_MOVE_SPEED;
                    if (newP2_y < target2_y) newP2_y += PLAYER_MOVE_SPEED;
                    if (newP2_y > target2_y) newP2_y -= PLAYER_MOVE_SPEED;

                    if (newP2_x == target2_x && newP2_y == target2_y)
                        player2_moving = 0;

                    if (oldP2_x != newP2_x || oldP2_y != newP2_y)
                    {
                        ClearPlayer2(oldP2_x, oldP2_y);
                        DrawPlayer2(newP2_x, newP2_y, player2_color);
                        oldP2_x = newP2_x;
                        oldP2_y = newP2_y;
                    }
                }

                /*---------- Player 1 移動判定 ----------*/
                if (!player1_moving && !ice_hard_stun)
                {
                    u8 try_map_x = player1_map_x;
                    u8 try_map_y = player1_map_y;
                    if (key_up_pressed)    { try_map_y++; player1_direction = 1; }
                    else if (key_down_pressed) { try_map_y--; player1_direction = 2; }
                    else if (key_left_pressed) { try_map_x++; player1_direction = 3; }
                    else if (key_right_pressed){ try_map_x--; player1_direction = 4; }

                    if (try_map_x < MAP_WIDTH && try_map_y < MAP_HEIGHT &&
                        blockMap[try_map_y][try_map_x] == 0 && !(try_map_x == player2_map_x && try_map_y == player2_map_y))
                    {
                        player1_moving = 1;
                        target1_x = try_map_x * BLOCK_SIZE;
                        target1_y = try_map_y * BLOCK_SIZE;
                        player1_map_x = try_map_x;
                        player1_map_y = try_map_y;
                    }
                }

                /*---------- Player 1 實際移動 ----------*/
                if (player1_moving)
                {
                    if (newP1_x < target1_x) newP1_x += PLAYER_MOVE_SPEED;
                    if (newP1_x > target1_x) newP1_x -= PLAYER_MOVE_SPEED;
                    if (newP1_y < target1_y) newP1_y += PLAYER_MOVE_SPEED;
                    if (newP1_y > target1_y) newP1_y -= PLAYER_MOVE_SPEED;

                    if (newP1_x == target1_x && newP1_y == target1_y)
                        player1_moving = 0;

                    if (oldP1_x != newP1_x || oldP1_y != newP1_y)
                    {
                        ClearPlayer1(oldP1_x, oldP1_y);
                        DrawPlayer1(newP1_x, newP1_y, player_color);
                        oldP1_x = newP1_x;
                        oldP1_y = newP1_y;
                    }
                }

                /*==================== 冰牆持續生成 + 硬直控制 ====================*/
								if (ice_animating && sysTickCounter >= next_IceTime)
								{
										// 1. 畫出當前這一格冰
										lcd_showIce(ice_x, ice_y, ICE_COLOR);

										// 2. 轉成地圖格子座標
										u8 mapX = ice_x / BLOCK_SIZE;
										u8 mapY = ice_y / BLOCK_SIZE;

										// 3. 正式變成牆 + 設定 2 秒後自動消失
										blockMap[mapY][mapX] = 1;
										iceExpireTime[mapY][mapX] = sysTickCounter + 2000;   // 2 秒融化

										// 4. 計算「下一格」的位置
										u16 nextX = ice_x + ice_dx;
										u16 nextY = ice_y + ice_dy;
										u8  nextMapX = nextX / BLOCK_SIZE;
										u8  nextMapY = nextY / BLOCK_SIZE;

										// 5. 判斷下一格能不能繼續放（出界 或 碰到牆/冰）
										if (nextX >= 240 || nextY >= 320 ||                     // 超出螢幕
												nextMapX >= MAP_WIDTH || nextMapY >= MAP_HEIGHT ||  // 超出地圖陣列
												blockMap[nextMapY][nextMapX] == 1)                  // 已經有牆或冰
										{
												// ★ 這裡就是「下一格不能放」的情況！★
												ice_animating = 0;        // 停止整條冰牆生成
												ice_hard_stun = 0;        // 立即解除硬直 → 玩家現在可以動了！
												// global_ice_cooldown 已經在 TryCreateIceWall() 裡設成 + 2000ms
										}
										else
										{
												// 還能繼續放 → 更新到下一格，準備 40ms 後再畫
												ice_x = nextX;
												ice_y = nextY;
												next_IceTime = sysTickCounter + ICE_SPEED_MS;   // 40ms 一格
										}
								}

								// 冰牆自動融化（5 秒後）
								for (u8 y = 0; y < MAP_HEIGHT; y++)
								{
										for (u8 x = 0; x < MAP_WIDTH; x++)
										{
												if (iceExpireTime[y][x] != 0 && sysTickCounter >= iceExpireTime[y][x])
												{
														blockMap[y][x] = 0;
														iceExpireTime[y][x] = 0;
														lcd_fillRectangle(bkg_color, x*16, 16, y*16, 16);  // 清掉冰
												}
										}
								}

                /*---------- PS/2 超時處理 ----------*/
                if (ps2_bitcount > 0)
                {
                    if (sysTickCounter - ps2_last_clock_time > PS2_TIMEOUT_MS)
                    {
                        PS2_ResetAllKeyStates();
                        ps2_shift = 0;
                        ps2_bitcount = 0;
                        F0flag = 0;
                    }
                }
            }

            /*---------- 時間顯示更新（每 500ms）----------*/
            if (game_ticks - last_update >= 50)
            {
                last_update = game_ticks;
                LCD_ShowTime(10, 10);
            }
            DS1_off;
        }

        /*==================== 遊戲結束 ====================*/
        DS1_off;
        Music_Stop();
        printendgamemenu();
    }
}
