#include "tim.h"
#include "usart.h"
#include "GPIO.h"
#include "control.h"
#include "stm32f4xx.h"

void TIM5_Init(u32 arr,u32 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  ///使能TIM5时钟
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);//初始化TIM5
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn; //定时器5中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x1; //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);//清除TIM的更新标志位
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
	 
	TIM_Cmd(TIM5, ENABLE);
}









//定时器2中断服务函数，定时读取编码器，获取机器人位置
//void TIM2_IRQHandler(void)
//{

//	//printf("中断");
//	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) 
//	{	
//		
//	}
//	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //清除中断标志位
//	
//	
//}

