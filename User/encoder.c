#include "encoder.h"
#include "usart.h"
#include "GPIO.h"
#include "control.h"
#include "stm32f4xx.h"

void Encoder_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///ʹ��TIM2ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///ʹ��TIM4��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);  ///ʹ��TIM8ʱ��

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

	encoder1_init();
	encoder2_init();
	encoder3_init();

}

void encoder1_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = Encoder1_B_Pin|Encoder1_A_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(Encoder1_GPIO_Port, &GPIO_InitStructure);//��ʼ��

	GPIO_PinAFConfig(Encoder1_GPIO_Port,GPIO_PinSource1,GPIO_AF_TIM2); 
	GPIO_PinAFConfig(Encoder1_GPIO_Port,GPIO_PinSource5,GPIO_AF_TIM2); 
	
	TIM_TimeBaseInitStructure.TIM_Period = 65535; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler= 0x0;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);

	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3
	
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);

//	TIM_ClearFlag(TIM2, TIM_FLAG_Update);//���TIM�ĸ��±�־λ
//	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	 
	TIM_SetCounter(TIM2,0);
	TIM_Cmd(TIM2, ENABLE);
}
void encoder2_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = Encoder2_B_Pin|Encoder2_A_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(Encoder2_GPIO_Port, &GPIO_InitStructure);//��ʼ��

	GPIO_PinAFConfig(Encoder2_GPIO_Port,GPIO_PinSource13,GPIO_AF_TIM4); 
	GPIO_PinAFConfig(Encoder2_GPIO_Port,GPIO_PinSource12,GPIO_AF_TIM4); 
	
	TIM_TimeBaseInitStructure.TIM_Period = 65535; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler= 0x0;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);

	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3
	
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);

//	TIM_ClearFlag(TIM4, TIM_FLAG_Update);//���TIM�ĸ��±�־λ
//	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
//	 
	TIM_SetCounter(TIM4,0);
	TIM_Cmd(TIM4, ENABLE);
}

void encoder3_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = Encoder3_B_Pin|Encoder3_A_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(Encoder3_GPIO_Port, &GPIO_InitStructure);//��ʼ��

	GPIO_PinAFConfig(Encoder3_GPIO_Port,GPIO_PinSource7,GPIO_AF_TIM8); 
	GPIO_PinAFConfig(Encoder3_GPIO_Port,GPIO_PinSource6,GPIO_AF_TIM8); 
	
	TIM_TimeBaseInitStructure.TIM_Period = 65535; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler= 0x0;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseInitStructure);

	TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3
	
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM8, &TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInit(TIM8, &TIM_ICInitStructure);

//	TIM_ClearFlag(TIM8, TIM_FLAG_Update);//���TIM�ĸ��±�־λ
//	TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);
	 
	TIM_SetCounter(TIM8,0);
	TIM_Cmd(TIM8, ENABLE);
}


