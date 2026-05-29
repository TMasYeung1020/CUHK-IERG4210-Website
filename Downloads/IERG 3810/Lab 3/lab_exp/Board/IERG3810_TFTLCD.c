#include "stm32f10x.h"
#include "IERG3810_Clock.h"
#include "IERG3810_TFTLCD.h"
#include "FONT.H"
#include "CFONT.H"
#include "LCDinit.h"

void lcd_showChar(u16 x, u16 y, u8 ascii, u16 color, u16 bgcolor){
	u8 i,b,temp1,temp2;
	u16 tempX,tempY;
	if(ascii<32||ascii>127) return;
	ascii -=32;
	tempX=x;
	for(i=0;i<16;i+=2){
		temp1 = asc2_1608[ascii][i];
		temp2 = asc2_1608[ascii][i+1];
		tempY=y;
		for(b=0;b<8;b++){
			if(temp1%2==1) lcd_drawDot(tempX,tempY+8,color); // if 1 then have color, and show word
			if(temp2%2==1) lcd_drawDot(tempX,tempY,color);
			temp1=temp1>>1;
			temp2=temp2>>1;
			tempY++;
		}
		tempX++;
	}
}

void lcd_showChinScaled(u16 x, u16 y, u8 index, u16 color, u16 bgcolor) {
    u8 col, bit, dy, dx;
	int scale=1;//for scaling if want to show larger character
    if (index >= (sizeof(chi_1616)/sizeof(chi_1616[0]))) return;

    for (col = 0; col < 16; col++) {//16 columns for chin char
        u8 upper = chi_1616[index][col*2 + 0];//2 bytes per column
        u8 lower = chi_1616[index][col*2 + 1];

        // Upper half rows y+8..y+15
        for (bit = 0; bit < 8; bit++) {
            u8 on = (upper >> bit) & 1;//extract the bit
						//Base x,y position
            u16 px = x + col*scale;
            u16 py = y + (8 + bit)*scale;
            for (dx = 0; dx < scale; dx++) {
                for (dy = 0; dy < scale; dy++) {
                    if (on) lcd_drawDot(px + dx, py + dy, color);//only draw when the bit ==1
                }
            }
        }

        // Lower half rows y..y+7
        for (bit = 0; bit < 8; bit++) {
            u8 on = (lower >> bit) & 1;
            u16 px = x + col*scale;
            u16 py = y + bit*scale;
            for (dx = 0; dx < scale; dx++) {
                for (dy = 0; dy < scale; dy++) {
                    if (on) lcd_drawDot(px + dx, py + dy, color);
                }
            }
        }
    }
}
