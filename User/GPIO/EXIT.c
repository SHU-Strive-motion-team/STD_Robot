#include "EXIT.h"



//�ⲿ�жϳ�ʼ��
void EXTIX_Init(void)
{
 
 	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);		//ʹ��SYSCFGʱ��
	
 
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource0);	//PC0 ���ӵ��ж���0
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource1);	//PC1 ���ӵ��ж���1

	//LimitSwitchDown 	C1
	//LimitSwitchUp 	C0

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



//�ⲿ�ж�0������� 
void EXTI0_IRQHandler(void)
{
	if (LimitSwitchUp == 1)
	{
		delay_ms(5);
		if (LimitSwitchUp == 1)
		{
			shoveMotor(STOP);
			EXTIX_Disable(UP);
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line0);			//����жϱ�־λ
	
}
 
//�ⲿ�ж�1�������
void EXTI1_IRQHandler(void)
{
	if (LimitSwitchDown == 1)
	{
		delay_ms(5);
		if (LimitSwitchDown == 1)
		{
			shoveMotor(STOP);
			EXTIX_Disable(DOWN);
		}
	}		 
	EXTI_ClearITPendingBit(EXTI_Line1);			//����жϱ�־λ
	
}


