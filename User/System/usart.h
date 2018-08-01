#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
 	
#define USART_REC_LEN  			200  	//�����������ֽ��� 200  //  10
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
#define EN_USART2_RX			1		//ʹ�ܣ�1��/��ֹ��0������2����
#define EN_USART3_RX			1		//ʹ�ܣ�1��/��ֹ��0������2����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
extern u8  USART2_RX_BUF[USART_REC_LEN];//���ջ��壬���USART_REC_LEN���ֽڣ�ĩ�ֽ�Ϊ���з�
extern u16 USART2_RX_STA;				//����״̬���
extern u8  USART3_RX_BUF[USART_REC_LEN];//���ջ��壬���USART_REC_LEN���ֽڣ�ĩ�ֽ�Ϊ���з�
extern u16 USART3_RX_STA;				//����״̬���

// extern u8  receive;
// extern u8  receive2;					//USART2�Ľ�������״̬
// extern u8  receive3;					//USART2�Ľ�������״̬

//��ʼ������
void UART_Init(u32 bound);
void SendToPc(u8 cmd,u16 data1,u16 data2,u16 data3);
#endif


