#include "MPU6050.h"
#include "usart.h"
#include "delay.h"

/*******************************

Ϊģ�黯��MPU6050��д����ģ���Դ��������˲��㷨
ʹ��ǰ����ͨ����λ���Լ��ٶȡ����ٶ�������
MPU_Init()����ΪZ��У׼

********************************/

void IMU_Init()
{
	//�Ƕȳ�ʼ��ָ�0XFF,0XAA,0X01,0X04,0X00
	 USART_SendData(USART2,0xFF);
	 while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
	 USART_SendData(USART2,0xAA);
	 while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
	
	 USART_SendData(USART2,0x01);
	 while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
	 USART_SendData(USART2,0x04);
	 while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
	 USART_SendData(USART2,0x00);
	 while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);

	//�Ƕȳ�ʼ�� ʹ Z ��Ƕȹ���  0xFF 0xAA 0x52
//	USART_SendData(USART2,0xFF);
//	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
//	USART_SendData(USART2,0xAA);
//	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
//	USART_SendData(USART2,0x52);
//	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);

	delay_ms(10);
}
