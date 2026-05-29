#include "stm32f10x.h"
#include "LCDinit.h"


void Delay(u32 count){
	u32 i;
	for(i=0;i<count; i++);
}


void lcd_fillRectangle(u16 color, u16 start_x, u16 start_y, u16 length_x, u16 length_y)
{
    u32 index=0;
    lcd_wr_reg(0x2A);
    lcd_wr_data((start_x>>8));
    lcd_wr_data(start_x & 0xFF);
    lcd_wr_data(((length_x + start_x - 1)>>8));
    lcd_wr_data((length_x + start_x - 1) & 0xFF);
    lcd_wr_reg(0x2B);
    lcd_wr_data((start_y>>8));
    lcd_wr_data(start_y & 0xFF);
    lcd_wr_data(((length_y + start_y - 1)>>8));
    lcd_wr_data((length_y + start_y - 1) & 0xFF);
    lcd_wr_reg(0x2C); //LCD WriterRAM Prepare();
    Delay(100);
    for(index=0;index<length_x * length_y;index++)
    {
        lcd_wr_data(color);
    }
}

int main(void){
	lcd_init();
	Delay(1000000);
	lcd_fillRectangle(c_YELLOW, 0, 0, 240, 320);
	lcd_fillRectangle(c_BLUE, 95, 135, 50, 50);
}
