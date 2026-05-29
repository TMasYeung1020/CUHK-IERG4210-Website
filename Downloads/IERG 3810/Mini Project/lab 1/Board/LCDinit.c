#include "stm32f10x.h"
#define LCD_LIGHT_ON GPIOB->BSRR = 1 << 0

typedef struct{
	u16 LCD_REG;
	u16 LCD_RAM;
} LCD_TypeDef;

#define LCD_BASE ((u32)(0x6C000000| 0x000007FE))
#define LCD ((LCD_TypeDef *) LCD_BASE)

#define c_black 0x0000
#define c_white 0xFFFF
#define c_RED 0xF800
#define c_GREEN 0x07E0
#define c_BLUE 0x001F
#define c_YELLOW 0xFFE0

void lcd_wr_reg(u16 regval){
	LCD->LCD_REG=regval;
}

void lcd_wr_data(u16 data){
	LCD->LCD_RAM=data;
}

void lcd_9341_setParameter(void){
	lcd_wr_reg(0x01); //software reset
	lcd_wr_reg(0x11); //Exit_sleep_mode
	lcd_wr_reg(0x3A); //Set_pixel_format
	lcd_wr_data(0x55); //65536 colors
	lcd_wr_reg(0x29); //display on
	lcd_wr_reg(0x36); //memory access control
	lcd_wr_data(0xC8); //control display direction
}

void lcd_backlight_init(void){
	RCC->APB2ENR |= 1<<3;//PORTB
	GPIOB->CRL&=0xFFFFFFF0;//PB0
	GPIOB->CRL|=0x00000003;//PB0
}

void lcd_init(void){//set FSMC
	RCC->AHBENR |= 1<<8;//FSMC
	RCC->APB2ENR |= 1<<3;//PORTB
	RCC->APB2ENR |= 1<<5;//PORTD
	RCC->APB2ENR |= 1<<6;//PORTE
	RCC->APB2ENR |= 1<<8;//PORTG
	GPIOB->CRL&=0xFFFFFFF0;//PB0
	GPIOB->CRL|=0x00000003;//PB0
	//port D
	GPIOD->CRH&=0x00FFF000;
	GPIOD->CRH|=0xBBFFFBBB;
	GPIOD->CRL&=0xFF00FF00;
	GPIOD->CRL|=0x00BB00BB;
	//port E
	GPIOE->CRH&=0x00000000;
	GPIOE->CRH|=0xBBBBBBBB;
	GPIOE->CRL&=0x0FFFFFFF;
	GPIOE->CRL|=0xB0000000;
	//port G12
	GPIOG->CRH&=0xFFF0FFFF;
	GPIOG->CRH|=0x000B0000;
	GPIOG->CRL&=0xFFFFFFF0;//PG0->RS
	GPIOG->CRL|=0x0000000B;
	//LCD uses FSMC Bank 4 memory bank
	//use mode A
	FSMC_Bank1->BTCR[6]=0x00000000;//FSMC_BCR4 reset
	FSMC_Bank1->BTCR[7]=0x00000000;//FSMC_BTR4 reset
	FSMC_Bank1E->BWTR[6]=0x00000000;//FSMC_BWTR4 reset
	FSMC_Bank1->BTCR[6]|=1<<12;//FSMC_BCR4->WREN
	FSMC_Bank1->BTCR[6]|=1<<14;//FSMC_BCR4->EXTMOD
	FSMC_Bank1->BTCR[6]|=1<<4;//FSMC_BCR4->MWID
	FSMC_Bank1->BTCR[7]|=0<<28;//FSMC_BTR4->ACCMOD
	FSMC_Bank1->BTCR[6]|=1<<0;//FSMC_BTR4->ADDSET
	FSMC_Bank1->BTCR[7]|=0xF<<8;//FSMC_BTR4->DATAST
	FSMC_Bank1E->BWTR[6]|=0x28;//FSMC_BWTR4->ACCMOD
	FSMC_Bank1E->BWTR[6]|=0x0;//FSMC_BWTR4->ADDSET
	FSMC_Bank1E->BWTR[6]|=3<<8;//FSMC_BWTR4->DATAST
	FSMC_Bank1E->BWTR[6]|=1<<0;//FSMC_BWTR4->MBKEN

	//either one,check label on LCD
	lcd_9341_setParameter();
	//lcd_7789_setParameter();
	
	lcd_backlight_init();//PB0
	LCD_LIGHT_ON;

}

void lcd_drawDot(u16 x, u16 y, u16 color){
	lcd_wr_reg(0x2A);//set x position
	lcd_wr_data(x>>8);
  lcd_wr_data(x & 0xFF);
  lcd_wr_data(0x01);
  lcd_wr_data(0x3F);
  lcd_wr_reg(0x2B);//set y position
  lcd_wr_data(y>>8);
  lcd_wr_data(y & 0xFF);
  lcd_wr_data(0x01);
  lcd_wr_data(0xDF);
  lcd_wr_reg(0x2C);//set point with color
  lcd_wr_data(color);
}
