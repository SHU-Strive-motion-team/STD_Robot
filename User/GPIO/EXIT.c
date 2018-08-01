#include "EXIT.h"
#include "GPIO.h"
#include "delay.h"
#include "usart.h"
//#include "control.h"


//�ⲿ�жϳ�ʼ��
void EXTIX_Init(void)
{
 
 	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);		//ʹ��SYSCFGʱ��
	
 
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource0);	//PC0 ���ӵ��ж���0
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource1);	//PC1 ���ӵ��ж���1

	

	// ����EXTI_Line0 
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;				//LINE0
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		//�ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 	//�����ش��� 
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;				//ʹ��LINE0
	EXTI_Init(&EXTI_InitStructure);
	
	//����EXTI_Line1
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;				//LINE1
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		//�ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 	//�����ش��� 
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;				//ʹ��LINE1
	EXTI_Init(&EXTI_InitStructure);						//����
		
 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;				//�ⲿ�ж�0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;			//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);//����
		
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;				//�ⲿ�ж�1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;			//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);//����
		

}

//�ر��ⲿ�ж�    
//exitx:  �ж���
void EXTIX_Disable(u8 extix)
{
	EXTI_InitTypeDef   EXTI_InitStructure;

	if(extix == 0)
	{
		EXTI_InitStructure.EXTI_Line = EXTI_Line0;//LINE0
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش��� 
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;//ʹ��LINE0
	}
	else if(extix == 1)
	{
		EXTI_InitStructure.EXTI_Line = EXTI_Line1;//LINE1
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش��� 
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;//ʹ��LINE1
	}
	else if(extix==9)
	{
		EXTI_InitStructure.EXTI_Line = EXTI_Line9;//LINE9
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش��� 
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;//ʹ��LINE9
	}
	else 
		return;
	
	EXTI_Init(&EXTI_InitStructure);//����
}

//ʹ���ⲿ�ж�    
//exitx:  �ж���
void EXTIX_Enable(u8 extix)
{
 	EXTI_InitTypeDef   EXTI_InitStructure;

	if(extix == 0)
	{
		EXTI_InitStructure.EXTI_Line = EXTI_Line0;//LINE0
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش��� 
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE0
	}
	else if(extix == 1)
	{
		EXTI_InitStructure.EXTI_Line = EXTI_Line1;//LINE1
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش��� 
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE1
	}
	else if(extix==9)
	{
		EXTI_InitStructure.EXTI_Line = EXTI_Line9;//LINE9
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش��� 
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE9
	}
	
	else 
		return;
	
	EXTI_Init(&EXTI_InitStructure);//����
}


/*
//�ⲿ�ж�0������� 
void EXTI0_IRQHandler(void)
{
	if(LimitSwitchUp==1)	 	 //Up
	{				 
		TIM_SetCompare1(TIM9,MOTOR_STATIC_1);
		TIM_SetCompare2(TIM9,MOTOR_STATIC_2);	
		EXTIX_Disable(0);
	}
	EXTI_ClearITPendingBit(EXTI_Line0);			//����жϱ�־λ
	
}
 
//�ⲿ�ж�1�������
void EXTI1_IRQHandler(void)
{
	if(LimitSwitchUp==1)	  //Down
	{
		TIM_SetCompare1(TIM9,MOTOR_STATIC_1);
		TIM_SetCompare2(TIM9,MOTOR_STATIC_2);	
		EXTIX_Disable(1);
	}		 
	EXTI_ClearITPendingBit(EXTI_Line1);			//����жϱ�־λ
	
}
*/

