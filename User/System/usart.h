#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
 	
#define USART_REC_LEN  			200  	//定义最大接收字节数 200  //  10
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
#define EN_USART2_RX			1		//使能（1）/禁止（0）串口2接收
#define EN_USART3_RX			1		//使能（1）/禁止（0）串口2接收
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	
extern u8  USART2_RX_BUF[USART_REC_LEN];//接收缓冲，最大USART_REC_LEN个字节，末字节为换行符
extern u16 USART2_RX_STA;				//接收状态标记
extern u8  USART3_RX_BUF[USART_REC_LEN];//接收缓冲，最大USART_REC_LEN个字节，末字节为换行符
extern u16 USART3_RX_STA;				//接收状态标记

// extern u8  receive;
// extern u8  receive2;					//USART2的接受数据状态
// extern u8  receive3;					//USART2的接受数据状态

//初始化串口
void UART_Init(u32 bound);
void SendToPc(u8 cmd,u16 data1,u16 data2,u16 data3);
#endif


