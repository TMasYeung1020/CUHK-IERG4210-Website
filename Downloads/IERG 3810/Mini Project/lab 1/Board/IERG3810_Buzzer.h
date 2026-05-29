#ifndef IERG3810_BUZZER_H

#define IERG3810_BUZZER_H

void Buzzer_Init(void);
void Music_Start(void);
void Music_Stop(void);
void Play_SFX(u16 freq, u16 ms);
void Buzzer_Play_Note(u16 freq, u16 duration_ms);

#endif
