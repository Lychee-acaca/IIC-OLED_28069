#ifndef IIC_H
#define IIC_H

#define IIC_SDA GPIO6
#define IIC_SCL GPIO7

void IIC_Init(void);
void IIC_SDA_SETH(void);
void IIC_SDA_SETL(void);
void IIC_SCL_SETH(void);
void IIC_SCL_SETL(void);
void SDA_OUT(void);
void SDA_IN(void);
void IIC_Start(void);
void IIC_Stop(void);
unsigned char IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);
void IIC_Send_Byte(unsigned char txd);
unsigned char IIC_Read_Byte(unsigned char ack);

#endif
