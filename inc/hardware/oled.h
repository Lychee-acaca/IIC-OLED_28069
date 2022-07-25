#ifndef OLED_H
#define OLED_H

#define size 12

void oled_cmd(char cmd);
void oled_data(char Data);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_Clear_COL(Uint8 y);
void OLED_Refresh_Gram(void);
void OLED_DrawPoint(Uint8 x, Uint8 y, Uint8 t);
void OLED_ShowChar(Uint8 x, Uint8 y, char chr, Uint8 mode);
Uint32 mypow(Uint8 m, Uint8 n);
void OLED_ShowNum(Uint8 x, Uint8 y, Uint32 num, Uint8 len);
void OLED_ShowString(Uint8 x, Uint8 y, const char *p);

#endif
