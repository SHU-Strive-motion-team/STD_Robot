#include "EXIT.h"



//外部中断初始化
void EXTIX_Init(void)
{
 
 	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);		//使能SYSCFG时钟
	
 
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource0);	//PC0 连接到中断线0
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource1);	//PC1 连接到中断线1

	//LimitSwitchDown 	C1
	//LimitSwitchUp 	C0

	// 配置EXTI_Line0 
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;				//LINE0
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 	//上升沿触发 
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;				//使能LINE0
	EXTI_Init(&EXTI_InitStructure);
	
	//配置EXTI_Line1
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;				//LINE1
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 	//上升沿触发 
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;				//使能LINE1
	EXTI_Init(&EXTI_InitStructure);						//配置
		
 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;				//外部中断0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;			//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置
		
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;				//外部中断1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;			//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置
		

}



//外部中断0服务程序 
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
	EXTI_ClearITPendingBit(EXTI_Line0);			//清除中断标志位
	
}
 
//外部中断1服务程序
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
	EXTI_ClearITPendingBit(EXTI_Line1);			//清除中断标志位
	
}


