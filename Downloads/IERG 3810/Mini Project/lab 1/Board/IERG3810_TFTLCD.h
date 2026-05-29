#ifndef IERG3810_TFTLCD_H

#define IERG3810_TFTLCD_H

void lcd_fillRectangle(u16 color, u16 start_x, u16 length_x, u16 start_y, u16 length_y);
void lcd_showChar(unsigned short int x, unsigned short int y, unsigned char ascii, unsigned short int color, unsigned short int bgcolor);
void lcd_showP1(unsigned short int x,unsigned short int y, unsigned short int color_key);
void lcd_clearP1(unsigned short int x,unsigned short int y, unsigned short int color_key);
void lcd_showIce(unsigned short int x,unsigned short int y, unsigned short int color_key);
void lcd_clearIce(unsigned short int x,unsigned short int y, unsigned short int color_key);
void lcd_showCoin(u16 x, u16 y, u16 color_key);
void lcd_clearCoin(u16 x, u16 y, u16 color_key);
void lcd_showP2(unsigned short int x,unsigned short int y, unsigned short int color_key);
void lcd_clearP2(unsigned short int x,unsigned short int y, unsigned short int color_key);
void lcd_showString(u16 x, u16 y, const char* s, u16 color, u16 bgcolor);
#endif
