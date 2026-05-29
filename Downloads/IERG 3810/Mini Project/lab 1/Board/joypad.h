#ifndef JOYPAD_H

#define JOYPAD_H

void JoyPad_Init(void);
void Read_JoyPad(void);

extern volatile u8 jp_Up;
extern volatile u8 jp_Down;
extern volatile u8 jp_Left;
extern volatile u8 jp_Right;
extern volatile u8 jp_A;
extern volatile u8 jp_B;
extern volatile u8 jp_Select;
extern volatile u8 jp_Start;

#endif
