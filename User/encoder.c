#include "encoder.h"
#include "usart.h"
#include "GPIO.h"
#include "control.h"
#include "stm32f4xx.h"

void Encoder_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///使能TIM2时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///使能TIM4钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);  ///使能TIM8时钟

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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(Encoder1_GPIO_Port, &GPIO_InitStructure);//初始化

	GPIO_PinAFConfig(Encoder1_GPIO_Port,GPIO_PinSource1,GPIO_AF_TIM2); 
	GPIO_PinAFConfig(Encoder1_GPIO_Port,GPIO_PinSource5,GPIO_AF_TIM2); 
	
	TIM_TimeBaseInitStructure.TIM_Period = 65535; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler= 0x0;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);

	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
	
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);

//	TIM_ClearFlag(TIM2, TIM_FLAG_Update);//清除TIM的更新标志位
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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(Encoder2_GPIO_Port, &GPIO_InitStructure);//初始化

	GPIO_PinAFConfig(Encoder2_GPIO_Port,GPIO_PinSource13,GPIO_AF_TIM4); 
	GPIO_PinAFConfig(Encoder2_GPIO_Port,GPIO_PinSource12,GPIO_AF_TIM4); 
	
	TIM_TimeBaseInitStructure.TIM_Period = 65535; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler= 0x0;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);

	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
	
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);

//	TIM_ClearFlag(TIM4, TIM_FLAG_Update);//清除TIM的更新标志位
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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(Encoder3_GPIO_Port, &GPIO_InitStructure);//初始化

	GPIO_PinAFConfig(Encoder3_GPIO_Port,GPIO_PinSource7,GPIO_AF_TIM8); 
	GPIO_PinAFConfig(Encoder3_GPIO_Port,GPIO_PinSource6,GPIO_AF_TIM8); 
	
	TIM_TimeBaseInitStructure.TIM_Period = 65535; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler= 0x0;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseInitStructure);

	TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
	
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM8, &TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInit(TIM8, &TIM_ICInitStructure);

//	TIM_ClearFlag(TIM8, TIM_FLAG_Update);//清除TIM的更新标志位
//	TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);
	 
	TIM_SetCounter(TIM8,0);
	TIM_Cmd(TIM8, ENABLE);
}


