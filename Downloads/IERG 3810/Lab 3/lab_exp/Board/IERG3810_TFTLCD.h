#ifndef IERG3810_TFTLCD_H

#define IERG3810_TFTLCD_H

void lcd_showChar(unsigned short int x, unsigned short int y, unsigned char ascii, unsigned short int color, unsigned short int bgcolor);
void lcd_showChinScaled(unsigned short int x, unsigned short int y, unsigned char index, unsigned short int color, unsigned short int bgcolor);

#endif
