#include "sys.h"
#include "usart.h"	


//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
 


u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       			//接收状态标记	

u8 USART2_RX_BUF[USART_REC_LEN];	//接收缓冲,最大USART_REC_LEN个字节.
u16 USART2_RX_STA = 0;	  		//接收状态标记

u8 USART3_RX_BUF[USART_REC_LEN];	//接收缓冲,最大USART_REC_LEN个字节.
u16 USART3_RX_STA = 0;			//接收状态标记





//初始化串口 
//bound:波特率
void UART_Init(u32 bound){
	/********************************串口1*************************************/
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);	//使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);	//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); 	//GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); 	//GPIOA10复用为USART1
	
	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; 	//GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 			//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	 			//上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); 					//初始化PA9，PA10

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;									//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;							//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式
	USART_Init(USART1, &USART_InitStructure);	 			//初始化串口1
	
	USART_Cmd(USART1, ENABLE);  							//使能串口1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
	
#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);			//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;			//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);						//根据指定的参数初始化VIC寄存器、

#endif


	/********************************串口2*************************************/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);		//使能USART2时钟
 
	//串口2对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);	//GPIOA2复用为USART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); 	//GPIOA3复用为USART2
	
	//USART2端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; 	//GPIOA2与GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 			//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 			//上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);					//初始化PA2，PA3

	//USART2 初始化设置
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure); 				//初始化串口2
	
	USART_Cmd(USART2, ENABLE);  							//使能串口2
	
	USART_ClearFlag(USART2, USART_FLAG_TC);
	
#if EN_USART2_RX	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);			//开启相关中断

	//Usart2 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;			//串口2中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;			//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);						//根据指定的参数初始化VIC寄存器、

#endif


	/********************************串口3*************************************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 		//使能GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);		//使能USART3时钟
 
	//串口3对应引脚复用映射
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); 	//GPIOB10复用为USART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); 	//GPIOB11复用为USART3
	
	//USART3端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; 	//GPIOB10与GPIOB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOB,&GPIO_InitStructure); 					//初始化PB10，PB11

	//USART3 初始化设置
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure); 				//初始化串口3
	
	USART_Cmd(USART3, ENABLE);  							//使能串口3
	
	USART_ClearFlag(USART3, USART_FLAG_TC);
	
#if EN_USART3_RX	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);			//开启相关中断

	//Usart3 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;			//串口3中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;			//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

#endif
	
}

void usart_send_char(u8 c)
{
	USART_SendData(USART1,c);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
} 
void SendToPc(u8 cmd,u16 data1,u16 data2,u16 data3)
{
	u8 sum =0;
	
	usart_send_char('@');
	//sum += '@';
	
	usart_send_char('^');
	//sum +=  '@';
	
	usart_send_char(cmd);
	///sum += cmd;
	
	usart_send_char((data1>>8)&0xff);
	usart_send_char((data1)&0xff);
	
	usart_send_char((data2>>8)&0xff);
	usart_send_char(data2&0xff);
	
	usart_send_char((data3>>8)&0xff);
	usart_send_char(data3&0xff);
	
	sum = '@'+  '^'+cmd+((data1>>8)&0xff)+((data1)&0xff)+((data2>>8)&0xff)+(data2&0xff)+((data3>>8)&0xff)+(data3&0xff);
	
	usart_send_char(sum);
		
}



/* u8 receive = 0;
u8 End=0;
u8  CheckSum_1 = 0;

//串口1中断服务程序，视觉数据
void USART1_IRQHandler(void)  
{
	u8 Res;
	//USART_RX_STA=0;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(Res!='z')
			{	
				if(End!='z')
				{
				USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
			//	USART_SendData(USART1, USART_RX_BUF[USART_RX_STA&0X3FFF]);
				 CheckSum_1=Res+ CheckSum_1;
				
				USART_RX_STA++;
				if(USART_RX_STA>(USART_REC_LEN-1))
					USART_RX_STA=0;//接收数据错误,重新开始接收	 
				}		
				else if(End=='z')
				{
					if( CheckSum_1==Res)
					{	
						USART_RX_STA|=0x8000;
						receive=1;
				//		USART_SendData(USART1,  CheckSum_1);
					}
					else
						USART_RX_STA=0;
					
					End = 0;
					 CheckSum_1=0;
				}
			}
			else if(Res=='z') 
			{ 
				End='z';

				if( CheckSum_1=='z')
					 CheckSum_1= CheckSum_1+'1';
			}
		} 
	} 
	
} 


#if 0
u8 receive3 = 0;
u8 end=0;
u8  CheckSum_3 = 0;
void USART3_IRQHandler(void)                	//串口3中断服务程序,雷达数据
{
	u8 Res;
	//USART3_RX_STA=0;

	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{	

		Res =USART_ReceiveData(USART3);//(USART1->DR);	//读取接收到的数据
		
		if((USART3_RX_STA&0x8000)==0)//接收未完成
		{
			if(Res!='z')
			{	
				if(end!='z')
				{
				USART3_RX_BUF[USART3_RX_STA&0X3FFF]=Res ;
				//USART_SendData(USART3, USART3_RX_BUF[USART3_RX_STA&0X3FFF]);
				 CheckSum_3=Res+ CheckSum_3;
				
				USART3_RX_STA++;
				if(USART3_RX_STA>(USART_REC_LEN-1))
					USART3_RX_STA=0;//接收数据错误,重新开始接收	 
				}		
				else if(end=='z')
				{
					if( CheckSum_3==Res)
					{
			//		USART_SendData(USART3,  CheckSum_3);
						USART3_RX_STA|=0x8000;
						receive3=1;
					}
					else
						USART3_RX_STA=0;
					
					end = 0;
				   CheckSum_3=0;
				}
			}
			else if(Res=='z') 
			{ 
				end='z';

				if( CheckSum_3=='z')
					 CheckSum_3= CheckSum_3+'1';
			}
		} 
	} 
	

} 
#endif

#if 1
u8 receive3 = 0;
u8 end=0;
u8  CheckSum_3 = 0;
void USART3_IRQHandler(void)                	//串口3中断服务程序,雷达数据
{
	u8 Res;
	//USART3_RX_STA=0;
	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{	

		Res =USART_ReceiveData(USART3);//(USART1->DR);	//读取接收到的数据
		
		if((USART3_RX_STA&0x8000)==0)//接收未完成
		{
			if(Res!='z')
			{	
				if(end!='z')
				{
					USART3_RX_BUF[USART3_RX_STA&0X3FFF]=Res ;
					//USART_SendData(USART3, USART3_RX_BUF[USART3_RX_STA&0X3FFF]);
					// CheckSum_3=Res+ CheckSum_3;
					
					USART3_RX_STA++;
					if(USART3_RX_STA>(USART_REC_LEN-1))
						USART3_RX_STA=0;//接收数据错误,重新开始接收	 
				}		
				else if(end=='z')
				{
					USART3_RX_STA|=0x8000;
					receive3=1;
					
					end=0;
				}
			}
			else if(Res=='z') 
			{ 
				if( USART3_RX_STA ==7)
					end='z';
				else
					end=0;
				USART3_RX_STA=0;
			}
		} 
	} 
	
} 

#endif */


 



