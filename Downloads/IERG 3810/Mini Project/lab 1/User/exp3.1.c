#include "stm32f10x.h"
#include "LCDinit.h"

void Delay(u32 count){
	u32 i;
	for(i=0;i<count; i++);
}

int main(void){
	lcd_init();
	Delay(1000000);
	int dots=100;
	
	while(1){
		
		for(int i=0;i<dots;i++){
			//black line
			lcd_drawDot(10, i+10, c_black);
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
