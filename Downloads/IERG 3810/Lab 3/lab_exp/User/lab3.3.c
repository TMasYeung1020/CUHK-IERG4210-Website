#include "stm32f10x.h"
#include "LCDinit.h"

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

void lcd_sevenSegment(u16 color, u16 start_x, u16 start_y, u8 digit){
	int letters[7][4]={
		{10,55,130,10},//a
		{65,10,75,55},//b
		{65,10,10,55},//c
		{10,55,0,10},//d
		{0,10,10,55},//e
		{0,10,75,55},//f
		{10,55,65,10}//g
	};
	int masks[10]={
			 //  abcdefg inverse
	0x3F,//0 abcdef  0011 1111
	0x06,//1  bc     0000 0110
	0x5B,//2 ab de g 0101 1011
	0x4F,//3 abcd  g 0100 1111
	0x66,//4  bc  fg 0110 0110
	0x6D,//5 a cd fg 0110 1101
	0x7D,//6 a cdefg 0111 1101
	0x07,//7 abc     0000 0111
	0x7F,//8 abcdefg 0111 1111
	0x6F,//9 abcd fg 0110 1111
	};
	u8 mask = masks[digit];
	for(int i=0;i<7;i++){
		if(mask & (1<<i)){
			int* letter = letters[i];
			lcd_fillRectangle(
				color,
				letter[0]+start_x,//start of x + offset
				letter[1],//length x
				letter[2]+start_y,//start of y + offset
				letter[3]//length y
			);
		}
	}
}

void countdown(u16 color){
	for(int i=9;i>=0;i--){
		lcd_sevenSegment(color,83,90,i);
		Delay(11000000);
		lcd_fillRectangle(c_YELLOW,0,240,0,320);//clear screen
	}
}
int main(void){
	lcd_init();
	Delay(1000000);
	lcd_fillRectangle(c_YELLOW,0,240,0,320);
	while(1){
		countdown(c_BLUE);
	}
}
//0,0 at bottom left corner
//direction of x from left to right 0-239
//direction of y from bottom to top 0-319
//height 0-140, to center, start y 90
//width 75, start x 82.5 ~83
//a (10,55,130,10)
//b (65,10,75,55)
//c (65,10,10,55)
//d (10,55,0,10)
//e (0,10,10,55)
//f (0,10,75,55)
//g (10,55,65,10)
