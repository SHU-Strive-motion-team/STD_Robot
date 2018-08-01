#include "tim.h"
#include "usart.h"
#include "GPIO.h"
#include "control.h"
#include "stm32f4xx.h"

void TIM5_Init(u32 arr,u32 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  ///ʹ��TIM5ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);//��ʼ��TIM5
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn; //��ʱ��5�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x1; //��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; //�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);//���TIM�ĸ��±�־λ
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
	 
	TIM_Cmd(TIM5, ENABLE);
}









//��ʱ��2�жϷ���������ʱ��ȡ����������ȡ������λ��
//void TIM2_IRQHandler(void)
//{

//	//printf("�ж�");
//	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) 
//	{	
//		
//	}
//	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //����жϱ�־λ
//	
//	
//}

