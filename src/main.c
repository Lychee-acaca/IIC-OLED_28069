#include "DSP28x_Project.h"
#include "oled.h"
#include "stdio.h"

int main(void)
{
    InitSysCtrl();
    DINT;
    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;

    OLED_Init();
    OLED_Clear();

    /* ʾ����ʹ��sprintf������ʽ��������ַ���
     * ������оƬ�ڴ��С�������С�̶�Ϊ12������һ��Ӣ����ĸΪ12������ * 6������
     * �����ԣ�CCS��֧�ָ�������ʽ�����
     * ��Ҫ������������Բ�������д��
     *
     * ��OLED��ʾ�㷨�������Ż�����ʹ��OLED_ShowString����������ʾ����Ļ�ϣ�����Ҫ����OLED_Refresh_Gram����ˢ��
     * ��ͬһ�н���OLED_ShowString��ֱ�Ӹ�����һ�ε���������������ͬһ�д�ǰ���ȴ��ڱ����������������
     * �Ƽ��ڵ���OLED_ShowString֮ǰ�ȵ���OLED_Clear_COL�������н������
     */
    char* testStr = "";
    float testFloat = 1.23;
    sprintf(testStr, "%s %c%d.%d", "Test", 'f', (int)testFloat, (int)(testFloat * 100) % 100);
    OLED_ShowString(18, 16, testStr);
    OLED_Refresh_Gram();

    //������OLED_Clear_COL���������
    sprintf(testStr, "abcdefg");
    OLED_ShowString(0, 28, testStr);
    sprintf(testStr, "hijkl");
    OLED_ShowString(0, 28, testStr);
    OLED_Refresh_Gram();

    //����OLED_Clear_COL�����һ�������
    sprintf(testStr, "mnopqrstu");
    OLED_ShowString(0, 40, testStr);
    OLED_Clear_COL(40);
    sprintf(testStr, "vwxyz");
    OLED_ShowString(0, 40, testStr);
    OLED_Refresh_Gram();

    //�������ˢ���ʲ���
    int cnt = 0;
    while(1)
    {
        cnt++;
        sprintf(testStr, "NOW %d", cnt);
        OLED_Clear_COL(52);
        OLED_ShowString(0, 52, testStr);
        OLED_Refresh_Gram();
        if (cnt == 100)
            cnt = 0;
    }
}
