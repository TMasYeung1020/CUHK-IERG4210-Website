#include "stm32f10x.h"
#include "IERG3810_TFTLCD.h"
#include "FONT.H"
#include "LCDinit.h"
#include "ITEM.H"

#define bkg_color 0xEFDF

void lcd_fillRectangle(u16 color, u16 start_x, u16 length_x, u16 start_y, u16 length_y)
{
    u32 index = 0;
    lcd_wr_reg(0x2A);                                           
    lcd_wr_data(start_x >> 8);
    lcd_wr_data(start_x & 0xFF);
    lcd_wr_data((length_x + start_x - 1) >> 8);
    lcd_wr_data((length_x + start_x - 1) & 0xFF);
    lcd_wr_reg(0x2B);                                         
    lcd_wr_data(start_y >> 8);
    lcd_wr_data(start_y & 0xFF);
    lcd_wr_data((length_y + start_y - 1) >> 8);
    lcd_wr_data((length_y + start_y - 1) & 0xFF);
    lcd_wr_reg(0x2C);                                           
    for (index = 0; index < length_x * length_y; index++)
        lcd_wr_data(color);
}

void lcd_showChar(u16 x, u16 y, u8 ascii, u16 color, u16 bgcolor)
{
    u8  i, b, temp1, temp2;
    u16 tempX, tempY;

    if (ascii < 32 || ascii > 127) return;
    ascii -= 32;
    tempX = x;

    for (i = 0; i < 16; i += 2)
    {
        temp1 = asc2_1608[ascii][i];
        temp2 = asc2_1608[ascii][i + 1];
        tempY = y;

        for (b = 0; b < 8; b++)
        {
            if (temp1 % 2 == 1) lcd_drawDot(tempX, tempY + 8, color);
            if (temp2 % 2 == 1) lcd_drawDot(tempX, tempY,      color);
            temp1 = temp1 >> 1;
            temp2 = temp2 >> 1;
            tempY++;
        }
        tempX++;
    }
}

void lcd_showString(u16 x, u16 y, const char* s, u16 color, u16 bgcolor)
{
    while (*s)
    {
        char ch = *s++;
        if ((u8)ch < 32 || (u8)ch > 127) ch = ' ';
        lcd_showChar(x, y, (u8)ch, color, bgcolor);
        x += 8;
    }
}

//===================================== Player 1 ================================
void lcd_showP1(u16 x, u16 y, u16 color_key)
{
    u16 i = 0;
    for (u16 py = 0; py < ITEM_HEIGHT; py++)        
    {
        for (u16 px = 0; px < ITEM_WIDTH; px++)      
        {
            u16 color = player1[i++];
					if (color != 0xffff)
            lcd_fillRectangle(color, x + px, 1, y + py, 1);
        }
    }
}

void lcd_clearP1(u16 x, u16 y, u16 color_key)
{
    for (u16 py = 0; py < ITEM_HEIGHT; py++)
    {
        for (u16 px = 0; px < ITEM_WIDTH; px++)
        {
            lcd_fillRectangle(color_key, x + px, 1, y + py, 1);
        }
    }
}

//===================================================== ICE =========================================
void lcd_showIce(u16 x, u16 y, u16 color_key)
{
    u16 i = 0;
    for (u16 py = 0; py < 16; py++)               
    {
        for (u16 px = 0; px < 16; px++)
        {
            u16 color = Ice[i++];
            lcd_fillRectangle(color, x + px, 1, y + py, 1);
        }
    }
}

void lcd_clearIce(u16 x, u16 y, u16 color_key)
{
    for (u16 py = 0; py < 16; py++)
    {
        for (u16 px = 0; px < 16; px++)
        {
            lcd_fillRectangle(color_key, x + px, 1, y + py, 1);
        }
    }
}

//============================================= COIN (Strawberry) =========================================
void lcd_showCoin(u16 x, u16 y, u16 color_key)
{
    u16 i = 0;
    for (u16 py = 0; py < 16; py++)               
    {
        for (u16 px = 0; px < 16; px++)
        {
            u16 color = coin[i++];
            if (color != 0xffff)
            {
                lcd_fillRectangle(color, x + px, 1, y + py, 1);
            }
        }
    }
}

void lcd_clearCoin(u16 x, u16 y, u16 color_key)
{
    for (u16 py = 0; py < 16; py++)
    {
        for (u16 px = 0; px < 16; px++)
        {
            lcd_fillRectangle(color_key, x + px, 1, y + py, 1);
        }
    }
}

//===================================== Player 2 ================================
void lcd_showP2(u16 x, u16 y, u16 color_key)
{
    u16 i = 0;
    for (u16 py = 0; py < ITEM_HEIGHT; py++)
    {
        for (u16 px = 0; px < ITEM_WIDTH; px++)
        {
            u16 color = player2[i++];
            if (color != 0x0000)
                lcd_fillRectangle(color, x + px, 1, y + py, 1);
        }
    }
}

void lcd_clearP2(u16 x, u16 y, u16 color_key)
{
    u16 i = 0;
    for (u16 py = 0; py < ITEM_HEIGHT; py++)
    {
        for (u16 px = 0; px < ITEM_WIDTH; px++)
        {
            u16 color = player2[i++];
            if (color != 0xffff)
            {
                lcd_fillRectangle(color_key, x + px, 1, y + py, 1);
            }
        }
    }
}

