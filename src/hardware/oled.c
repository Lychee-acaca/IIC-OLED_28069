#include "DSP28x_Project.h"
#include "oled.h"
#include "oledfont.h"
#include "iic.h"
#include "string.h"

Uint8 OLED_GRAM[128][8];
Uint8 OLED_GRAM_PRE[8];
Uint8 OLED_GRAM_COL[8];
Uint8 OLED_GRAM_COL_END[8];

//дָ��
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

//д����
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

//��ʼ��
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

    oled_cmd(0xAE); //�ر���ʾ
    oled_cmd(0xD5); //����ʱ�ӷ�Ƶ����,��Ƶ��
    oled_cmd(80);   //[3:0],��Ƶ����;[7:4],��Ƶ��
    oled_cmd(0xA8); //��������·��
    oled_cmd(0X3F); //Ĭ��0X3F(1/64)
    oled_cmd(0xD3); //������ʾƫ��
    oled_cmd(0X00); //Ĭ��Ϊ0

    oled_cmd(0x40); //������ʾ��ʼ�� [5:0],����.

    oled_cmd(0x8D); //��ɱ�����
    oled_cmd(0x14); // bit2������/�ر�
    oled_cmd(0x20); //�����ڴ��ַģʽ
    oled_cmd(0x02); //[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
    oled_cmd(0xA1); //���ض�������,bit0:0,0->0;1,0->127;
    oled_cmd(0xC0); //����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��
    oled_cmd(0xDA); //����COMӲ����������
    oled_cmd(0x12); //[5:4]����

    oled_cmd(0x81); //�Աȶ�����

    oled_cmd(0x01); //1~255;Ĭ��0X7F (��������,Խ��Խ��)

    oled_cmd(0xD9); //����Ԥ�������
    oled_cmd(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;
    oled_cmd(0xDB); //����VCOMH ��ѹ����
    oled_cmd(0x30); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

    oled_cmd(0xA4); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
    oled_cmd(0xA6); //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ
    oled_cmd(0xAF); //������ʾ
}

//�������򣬸�ÿҳÿ�ж�д��0
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

//���һ��
void OLED_Clear_COL(Uint8 y)
{
    Uint8 i, j;
    for (i = 0; i < 128; ++i)
        for(j = 0; j < 12; ++j)
            OLED_DrawPoint(i, y + j, 0);
}

//ˢ����ʾ
void OLED_Refresh_Gram(void)
{
    Uint8 i, n;
    for (i = 0; i < 8; i++)
        if (OLED_GRAM_PRE[i])
        {
            oled_cmd(0xB0 + i); //����ҳ��ַ��0~7��
            oled_cmd(OLED_GRAM_COL[i] & 0x0F);     //������ʾλ�á��е͵�ַ
            oled_cmd((OLED_GRAM_COL[i] >> 4) | 0x10);     //������ʾλ�á��иߵ�ַ
            for (n = OLED_GRAM_COL[i]; n <= OLED_GRAM_COL_END[i]; n++)
                oled_data(OLED_GRAM[n][i]);
            OLED_GRAM_PRE[i] = 0;
            OLED_GRAM_COL[i] = 0xFF;
            OLED_GRAM_COL_END[i] = 0;
        }
}

//����
void OLED_DrawPoint(Uint8 x, Uint8 y, Uint8 t)
{
    Uint8 pos, bx, temp = 0;
    if (x > 127 || y > 63)
        return; //������Χ��.
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
    Uint8 csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2); //�õ�����һ���ַ���Ӧ������ռ���ֽ���
    chr = chr - ' ';                                              //�õ�ƫ�ƺ��ֵ
    for (t = 0; t < csize; t++)
    {
        if (size == 12)
            temp = asc2_1206[chr][t]; //����1206����
        else
            return; //û�е��ֿ�
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
    while ((*p <= '~') && (*p >= ' ')) //�ж��ǲ��ǷǷ��ַ�!
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
