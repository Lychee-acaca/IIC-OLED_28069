#ifndef OLED_H
#define OLED_H

#define size 12

//从FLASH启动时，自动将程序拷贝入RAM运行
#pragma CODE_SECTION(oled_cmd, "ramfuncs");
#pragma CODE_SECTION(oled_data, "ramfuncs");
#pragma CODE_SECTION(OLED_Clear, "ramfuncs");
#pragma CODE_SECTION(OLED_Clear_COL, "ramfuncs");
#pragma CODE_SECTION(OLED_Refresh_Gram, "ramfuncs");
#pragma CODE_SECTION(OLED_DrawPoint, "ramfuncs");
#pragma CODE_SECTION(OLED_ShowChar, "ramfuncs");
#pragma CODE_SECTION(mypow, "ramfuncs");
#pragma CODE_SECTION(OLED_ShowNum, "ramfuncs");
#pragma CODE_SECTION(OLED_ShowString, "ramfuncs");
#pragma CODE_SECTION(Int_to_Str, "ramfuncs");
#pragma CODE_SECTION(Float_to_Str, "ramfuncs");

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
char *Int_to_Str(Uint32 data);
char *Float_to_Str(float data,Uint8 digit);

#endif
