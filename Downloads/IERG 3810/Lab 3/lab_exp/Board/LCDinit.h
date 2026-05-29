#ifndef LCDinit_H

#define LCDinit_H

#define LCD_LIGHT_ON GPIOE->BSRR = 1 << 5

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

void lcd_wr_reg(u16 regval);
void lcd_wr_data(u16 data);
void lcd_9341_setParameter(void);
void lcd_backlight_init(void);
void lcd_init(void);
void lcd_drawDot(u16 x, u16 y, u16 color);

#endif
