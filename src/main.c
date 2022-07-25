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

    /* 示例：使用sprintf函数格式化输出到字符串
     * 受限于芯片内存大小，字体大小固定为12，其中一个英文字母为12行像素 * 6列像素
     * 经尝试，CCS不支持浮点数格式化输出
     * 若要输出浮点数可以参照以下写法
     *
     * 本OLED显示算法经过了优化，仅使用OLED_ShowString函数不会显示在屏幕上，还需要调用OLED_Refresh_Gram进行刷新
     * 对同一行进行OLED_ShowString会直接覆盖上一次的输出，但不会清空同一行此前长度大于本次输出的其他内容
     * 推荐在调用OLED_ShowString之前先调用OLED_Clear_COL函数对行进行清空
     */
    char* testStr = "";
    float testFloat = 1.23;
    sprintf(testStr, "%s %c%d.%d", "Test", 'f', (int)testFloat, (int)(testFloat * 100) % 100);
    OLED_ShowString(18, 16, testStr);
    OLED_Refresh_Gram();

    //不调用OLED_Clear_COL，输出覆盖
    sprintf(testStr, "abcdefg");
    OLED_ShowString(0, 28, testStr);
    sprintf(testStr, "hijkl");
    OLED_ShowString(0, 28, testStr);
    OLED_Refresh_Gram();

    //调用OLED_Clear_COL，清空一行再输出
    sprintf(testStr, "mnopqrstu");
    OLED_ShowString(0, 40, testStr);
    OLED_Clear_COL(40);
    sprintf(testStr, "vwxyz");
    OLED_ShowString(0, 40, testStr);
    OLED_Refresh_Gram();

    //连续输出刷新率测试
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
