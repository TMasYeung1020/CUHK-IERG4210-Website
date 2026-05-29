#include "stm32f10x.h"
#include "LCDinit.h"

void Delay(u32 count){
	u32 i;
	for(i=0;i<count; i++);
}

int main(void){
	int dots=100;
	lcd_init();
	Delay(1000000);
	
	while(1){
		int i;
		for(i=0;i<dots;i++){
			//black line
			lcd_drawDot(10, i+10, c_black);//start pos x 10 y 10
			//white line
			lcd_drawDot(20, i+10, c_white);
			//green line
			lcd_drawDot(30, i+10, c_GREEN);
			//red line
			lcd_drawDot(40, i+10, c_RED);
			//blue line
			lcd_drawDot(50, i+10, c_BLUE);
			
		}
	}
}
//0,0 at bottom left corner
//direction of x from left to right 0-239
//direction of y from bottom to top 0-319
