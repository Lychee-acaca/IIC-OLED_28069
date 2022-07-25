#include "DSP28x_Project.h"
#include "iic.h"

void IIC_Init(void)
{
    EALLOW;
    GpioCtrlRegs.GPAPUD.bit.IIC_SDA = 0;
    GpioCtrlRegs.GPADIR.bit.IIC_SDA = 1;
    GpioCtrlRegs.GPAMUX1.bit.IIC_SDA = 0;
    GpioCtrlRegs.GPAQSEL1.bit.IIC_SDA = 3;

    GpioCtrlRegs.GPAPUD.bit.IIC_SCL = 0;
    GpioCtrlRegs.GPADIR.bit.IIC_SCL = 1;
    GpioCtrlRegs.GPAMUX1.bit.IIC_SCL = 0;
    GpioCtrlRegs.GPAQSEL1.bit.IIC_SCL = 3;
    EDIS;
}

void IIC_SDA_SETH(void)
{
    GpioDataRegs.GPASET.bit.IIC_SDA = 1;
}

void IIC_SDA_SETL(void)
{
    GpioDataRegs.GPACLEAR.bit.IIC_SDA = 1;
}

void IIC_SCL_SETH(void)
{
    GpioDataRegs.GPASET.bit.IIC_SCL = 1;
}

void IIC_SCL_SETL(void)
{
    GpioDataRegs.GPACLEAR.bit.IIC_SCL = 1;
}

void SDA_OUT(void)
{
    EALLOW;
    GpioCtrlRegs.GPADIR.bit.IIC_SDA = 1;
    EDIS;
}

void SDA_IN(void)
{
    EALLOW;
    GpioCtrlRegs.GPADIR.bit.IIC_SDA = 0;
    EDIS;
}

void IIC_Start(void)
{
    SDA_OUT();
    IIC_SDA_SETH();
    IIC_SCL_SETH();
    DELAY_US(5);
    IIC_SDA_SETL();
    DELAY_US(6);
    IIC_SCL_SETL();
}

void IIC_Stop(void)
{
    SDA_OUT();
    IIC_SCL_SETL();
    IIC_SDA_SETL();
    IIC_SCL_SETH();
    DELAY_US(6);
    IIC_SDA_SETH();
    DELAY_US(6);
}

unsigned char IIC_Wait_Ack(void)
{
    unsigned char tempTime = 0;
    DELAY_US(1);
    SDA_IN();
    IIC_SCL_SETH();
    DELAY_US(1);
    while (GpioDataRegs.GPADAT.bit.IIC_SDA)
    {
        tempTime++;
        if (tempTime > 250)
        {
            IIC_Stop();
            return 1;
        }
    }
    IIC_SCL_SETL();
    return 0;
}

void IIC_Ack(void)
{
    IIC_SCL_SETL();
    SDA_OUT();
    IIC_SDA_SETL();
    DELAY_US(2);
    IIC_SCL_SETH();
    DELAY_US(5);
    IIC_SCL_SETL();
}

void IIC_NAck(void)
{
    IIC_SCL_SETL();
    SDA_OUT();
    IIC_SDA_SETH();
    DELAY_US(2);
    IIC_SCL_SETH();
    DELAY_US(5);
    IIC_SCL_SETL();
}

void IIC_Send_Byte(unsigned char txd)
{
    unsigned char t;
    SDA_OUT();
    IIC_SCL_SETL();
    for (t = 0; t < 8; t++)
    {
        if ((txd & 0x80) > 0)
            IIC_SDA_SETH();
        else
            IIC_SDA_SETL();
        txd <<= 1;
        DELAY_US(2);
        IIC_SCL_SETH();
        DELAY_US(2);
        IIC_SCL_SETL();
        DELAY_US(2);
    }
}

unsigned char IIC_Read_Byte(unsigned char ack)
{
    unsigned char i, receive = 0;
    SDA_IN();
    for (i = 0; i < 8; i++)
    {
        IIC_SCL_SETL();
        DELAY_US(2);
        IIC_SCL_SETH();
        receive <<= 1;
        if (GpioDataRegs.GPADAT.bit.IIC_SDA)
            receive++;
        DELAY_US(1);
    }
    if (!ack)
        IIC_NAck();
    else
        IIC_Ack();
    return receive;
}
