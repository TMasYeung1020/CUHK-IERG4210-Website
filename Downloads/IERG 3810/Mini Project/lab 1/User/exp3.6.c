#include "stm32f10x.h"
#include "LCDinit.h"
#include "IERG3810_TFTLCD.h"


int tmp[4];
void Delay(u32 count){
	u32 i;
	for(i=0;i<count; i++);
}

void lcd_fillRectangle(u16 color, u16 start_x, u16 length_x, u16 start_y, u16 length_y){
	u32 index=0;
	lcd_wr_reg(0x2A);
	lcd_wr_data(start_x>>8);
	lcd_wr_data(start_x & 0xFF);
	lcd_wr_data((length_x + start_x -1)>>8);
	lcd_wr_data((length_x + start_x -1)& 0xFF);
	lcd_wr_reg(0x2B);
	lcd_wr_data(start_y>>8);
	lcd_wr_data(start_y & 0xFF);
	lcd_wr_data((length_y + start_y -1)>>8);
	lcd_wr_data((length_y + start_y -1)& 0xFF);
	lcd_wr_reg(0x2C);//LCD_WriteRAM_Prepare()
	Delay(100);
	for(index=0;index<length_x*length_y;index++)
		lcd_wr_data(color);
}

void lcd_showString(u16 x, u16 y, int index,char* s, u16 color, u16 bgcolor) {
	int i=0;
	if(index==3)
		i=0;//print 0-2(mine)
	else
		i=3;//print 3-5(groupmate)
	while (i<index) {
		lcd_showChinScaled(x, y, i, color, bgcolor);//print char
		x += 16; // advance by character width of 8
		i++;
	}
	while(*s){
		char ch=*s++;
		lcd_showChar(x, y, ch, color, bgcolor);
		x += 8;
	}
}

int main(void){
	lcd_init();
	Delay(1000000);
	lcd_fillRectangle(c_GREEN,30, 130, 150, 100);
	lcd_showString(10, 200, 3,"1155213238", c_RED, c_YELLOW);
	lcd_showString(10, 180,6, "1155203181",  c_BLUE,  c_YELLOW);
	while(1){
		//
	}
}
//0,0 at bottom left corner
//direction of x from left to right 0-239
//direction of y from bottom to top 0-319
//height 16, to center, start y 90
//width 8, start x 82.5 ~83

