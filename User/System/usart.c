#include "sys.h"
#include "usart.h"	


//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
 


u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       			//����״̬���	

u8 USART2_RX_BUF[USART_REC_LEN];	//���ջ���,���USART_REC_LEN���ֽ�.
u16 USART2_RX_STA = 0;	  		//����״̬���

u8 USART3_RX_BUF[USART_REC_LEN];	//���ջ���,���USART_REC_LEN���ֽ�.
u16 USART3_RX_STA = 0;			//����״̬���





//��ʼ������ 
//bound:������
void UART_Init(u32 bound){
	/********************************����1*************************************/
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);	//ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);	//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); 	//GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); 	//GPIOA10����ΪUSART1
	
	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; 	//GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 			//���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	 			//����
	GPIO_Init(GPIOA,&GPIO_InitStructure); 					//��ʼ��PA9��PA10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;									//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;							//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure);	 			//��ʼ������1
	
	USART_Cmd(USART1, ENABLE);  							//ʹ�ܴ���1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
	
#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);			//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;			//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);						//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif


	/********************************����2*************************************/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);		//ʹ��USART2ʱ��
 
	//����2��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);	//GPIOA2����ΪUSART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); 	//GPIOA3����ΪUSART2
	
	//USART2�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; 	//GPIOA2��GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 			//���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 			//����
	GPIO_Init(GPIOA,&GPIO_InitStructure);					//��ʼ��PA2��PA3

	//USART2 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure); 				//��ʼ������2
	
	USART_Cmd(USART2, ENABLE);  							//ʹ�ܴ���2
	
	USART_ClearFlag(USART2, USART_FLAG_TC);
	
#if EN_USART2_RX	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);			//��������ж�

	//Usart2 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;			//����2�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;			//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);						//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif


	/********************************����3*************************************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 		//ʹ��GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);		//ʹ��USART3ʱ��
 
	//����3��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); 	//GPIOB10����ΪUSART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); 	//GPIOB11����ΪUSART3
	
	//USART3�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; 	//GPIOB10��GPIOB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOB,&GPIO_InitStructure); 					//��ʼ��PB10��PB11

	//USART3 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure); 				//��ʼ������3
	
	USART_Cmd(USART3, ENABLE);  							//ʹ�ܴ���3
	
	USART_ClearFlag(USART3, USART_FLAG_TC);
	
#if EN_USART3_RX	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);			//��������ж�

	//Usart3 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;			//����3�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;			//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

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

//����1�жϷ�������Ӿ�����
void USART1_IRQHandler(void)  
{
	u8 Res;
	//USART_RX_STA=0;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
		
		if((USART_RX_STA&0x8000)==0)//����δ���
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
					USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	 
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
void USART3_IRQHandler(void)                	//����3�жϷ������,�״�����
{
	u8 Res;
	//USART3_RX_STA=0;

	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{	

		Res =USART_ReceiveData(USART3);//(USART1->DR);	//��ȡ���յ�������
		
		if((USART3_RX_STA&0x8000)==0)//����δ���
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
					USART3_RX_STA=0;//�������ݴ���,���¿�ʼ����	 
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
void USART3_IRQHandler(void)                	//����3�жϷ������,�״�����
{
	u8 Res;
	//USART3_RX_STA=0;
	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{	

		Res =USART_ReceiveData(USART3);//(USART1->DR);	//��ȡ���յ�������
		
		if((USART3_RX_STA&0x8000)==0)//����δ���
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
						USART3_RX_STA=0;//�������ݴ���,���¿�ʼ����	 
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


 



