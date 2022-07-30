#include "DSP28x_Project.h"
#include "oled.h"
#include "oledfont.h"
#include "iic.h"
#include "string.h"

Uint8 OLED_GRAM[128][8];
Uint8 OLED_GRAM_PRE[8];
Uint8 OLED_GRAM_COL[8];
Uint8 OLED_GRAM_COL_END[8];

//写指令
void oled_cmd(char cmd)
{
    IIC_Start();
    IIC_Send_Byte(0x78);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x00);
    IIC_Wait_Ack();
    IIC_Send_Byte(cmd);
    IIC_Wait_Ack();
    IIC_Stop();
}

//写数据
void oled_data(char Data)
{
    IIC_Start();
    IIC_Send_Byte(0x78);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x40);
    IIC_Wait_Ack();
    IIC_Send_Byte(Data);
    IIC_Wait_Ack();
    IIC_Stop();
}

//初始化
void OLED_Init(void)
{
    IIC_Init();

    Uint8 i, j;
    for (i = 0; i < 128; ++i)
        for (j = 0; j < 8; ++j)
            OLED_GRAM[i][j] = 0;
    for (i = 0; i < 8; ++i)
    {
        OLED_GRAM_PRE[i] = 1;
        OLED_GRAM_COL[i] = 0;
        OLED_GRAM_COL_END[i] = 127;
    }

    oled_cmd(0xAE); //关闭显示
    oled_cmd(0xD5); //设置时钟分频因子,震荡频率
    oled_cmd(80);   //[3:0],分频因子;[7:4],震荡频率
    oled_cmd(0xA8); //设置驱动路数
    oled_cmd(0X3F); //默认0X3F(1/64)
    oled_cmd(0xD3); //设置显示偏移
    oled_cmd(0X00); //默认为0

    oled_cmd(0x40); //设置显示开始行 [5:0],行数.

    oled_cmd(0x8D); //电荷泵设置
    oled_cmd(0x14); // bit2，开启/关闭
    oled_cmd(0x20); //设置内存地址模式
    oled_cmd(0x02); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
    oled_cmd(0xA1); //段重定义设置,bit0:0,0->0;1,0->127;
    oled_cmd(0xC0); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
    oled_cmd(0xDA); //设置COM硬件引脚配置
    oled_cmd(0x12); //[5:4]配置

    oled_cmd(0x81); //对比度设置

    oled_cmd(0x01); //1~255;默认0X7F (亮度设置,越大越亮)

    oled_cmd(0xD9); //设置预充电周期
    oled_cmd(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;
    oled_cmd(0xDB); //设置VCOMH 电压倍率
    oled_cmd(0x30); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

    oled_cmd(0xA4); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
    oled_cmd(0xA6); //设置显示方式;bit0:1,反相显示;0,正常显示
    oled_cmd(0xAF); //开启显示
}

//清屏程序，给每页每列都写入0
void OLED_Clear(void)
{
    Uint8 i, j;
    for (i = 0; i < 128; ++i)
        for (j = 0; j < 8; ++j)
            if (OLED_GRAM[i][j] != 0)
            {
                OLED_GRAM[i][j] = 0;
                OLED_GRAM_PRE[j] = 1;
                if (i < OLED_GRAM_COL[j])
                    OLED_GRAM_COL[j] = i;
                if (i > OLED_GRAM_COL_END[j])
                    OLED_GRAM_COL_END[j] = i;
            }
}

//清除一行
void OLED_Clear_COL(Uint8 y)
{
    Uint8 i, j;
    for (i = 0; i < 128; ++i)
        for(j = 0; j < 12; ++j)
            OLED_DrawPoint(i, y + j, 0);
}

//刷新显示
void OLED_Refresh_Gram(void)
{
    Uint8 i, n;
    for (i = 0; i < 8; i++)
        if (OLED_GRAM_PRE[i])
        {
            oled_cmd(0xB0 + i); //设置页地址（0~7）
            oled_cmd(OLED_GRAM_COL[i] & 0x0F);     //设置显示位置—列低地址
            oled_cmd((OLED_GRAM_COL[i] >> 4) | 0x10);     //设置显示位置—列高地址
            for (n = OLED_GRAM_COL[i]; n <= OLED_GRAM_COL_END[i]; n++)
                oled_data(OLED_GRAM[n][i]);
            OLED_GRAM_PRE[i] = 0;
            OLED_GRAM_COL[i] = 0xFF;
            OLED_GRAM_COL_END[i] = 0;
        }
}

//画点
void OLED_DrawPoint(Uint8 x, Uint8 y, Uint8 t)
{
    Uint8 pos, bx, temp = 0;
    if (x > 127 || y > 63)
        return; //超出范围了.
    pos = 7 - y / 8;
    bx = y % 8;
    temp = 1 << (7 - bx);
    Uint8 before;
    before = OLED_GRAM[x][pos];
    if (t)
        OLED_GRAM[x][pos] |= temp;
    else
        OLED_GRAM[x][pos] &= ~temp;
    if (OLED_GRAM[x][pos] != before)
    {
        OLED_GRAM_PRE[pos] = 1;
        if (x < OLED_GRAM_COL[pos])
            OLED_GRAM_COL[pos] = x;
        if (x > OLED_GRAM_COL_END[pos])
            OLED_GRAM_COL_END[pos] = x;
    }
}

void OLED_ShowChar(Uint8 x, Uint8 y, char chr, Uint8 mode)
{
    Uint8 temp, t, t1;
    Uint8 y0 = y;
    Uint8 csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2); //得到字体一个字符对应点阵集所占的字节数
    chr = chr - ' ';                                              //得到偏移后的值
    for (t = 0; t < csize; t++)
    {
        if (size == 12)
            temp = asc2_1206[chr][t]; //调用1206字体
        else
            return; //没有的字库
        for (t1 = 0; t1 < 8; t1++)
        {
            if (temp & 0x80)
                OLED_DrawPoint(x, y, mode);
            else
                OLED_DrawPoint(x, y, !mode);
            temp <<= 1;
            y++;
            if ((y - y0) == size)
            {
                y = y0;
                x++;
                break;
            }
        }
    }
}

Uint32 mypow(Uint8 m, Uint8 n)
{
    Uint32 result = 1;
    while (n--)
        result *= m;
    return result;
}

void OLED_ShowNum(Uint8 x, Uint8 y, Uint32 num, Uint8 len)
{
    Uint8 t, temp;
    Uint8 enshow = 0;
    for (t = 0; t < len; t++)
    {
        temp = (num / mypow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                OLED_ShowChar(x + (size / 2) * t, y, ' ', 1);
                continue;
            }
            else
                enshow = 1;
        }
        OLED_ShowChar(x + (size / 2) * t, y, temp + '0', 1);
    }
}

void OLED_ShowString(Uint8 x, Uint8 y, const char *p)
{
    while ((*p <= '~') && (*p >= ' ')) //判断是不是非法字符!
    {
        if (x > (128 - (size / 2)))
        {
            x = 0;
            y += size;
        }
        if (y > (64 - size))
        {
            y = x = 0;
            OLED_Clear();
        }
        OLED_ShowChar(x, y, *p, 1);
        x += size / 2;
        p++;
    }
}
