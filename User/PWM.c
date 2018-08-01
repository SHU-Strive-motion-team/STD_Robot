#include "PWM.h"
#include "GPIO.h"
#include "usart.h"
 #include "delay.h"


//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数

void PWM_Init(void)
{
	TIM9_CH1_PWM_Init(4000-1,84-1);	//1M/4000=250hz PE5，铲球电机
	TIM9_CH2_PWM_Init(4000-1,84-1);	//PE6，铲球电机
	
	
	//TIM5_CH2_PWM_Init(42-1,1-1);		//2MHz    HCTL2020   时钟 PA1
	
	TIM3_CH1_PWM_Init(2000-1,84-1);	//Pa6 Motor 1
	TIM3_CH2_PWM_Init(2000-1,84-1);	//Pa7 Motor 2
	TIM3_CH3_PWM_Init(2000-1,84-1);	//PC8 Motor 3
	
}



//PE5 铲球点电机
void TIM9_CH1_PWM_Init(u32 arr,u32 psc)
{		 					 
	 
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);	//TIM14时钟使能  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 	//使能PORTF时钟	
	
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource5,GPIO_AF_TIM9); 	//GPIOE5复用为定时器9
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;           	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        	//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      	//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        	//上拉
	GPIO_Init(GPIOE,&GPIO_InitStructure);              	//初始化
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  				//定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	//向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   				//自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM9,&TIM_TimeBaseStructure);			//初始化定时器9
	
	//初始化TIM9 CHannel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 			//选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 		//输出极性:TIM输出比较极性低
	TIM_OC1Init(TIM9, &TIM_OCInitStructure);  					//根据T指定的参数初始化外设TIM1 4OC1

	TIM_OC1PreloadConfig(TIM9, TIM_OCPreload_Enable);  			//使能TIM14在CCR1上的预装载寄存器
 
	TIM_ARRPreloadConfig(TIM9,ENABLE);	//ARPE使能 
	
	TIM_Cmd(TIM9, ENABLE);  			//使能TIM14
										  
}  



void TIM9_CH2_PWM_Init(u32 arr,u32 psc)
{		 					 
	 
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
	   
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 	//使能PORTF时钟	
	
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource6,GPIO_AF_TIM9); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;           
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        	//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     	//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        	//上拉
	GPIO_Init(GPIOE,&GPIO_InitStructure);              
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  				//定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	//向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   				//自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM9,&TIM_TimeBaseStructure);
	
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 			//选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 		//输出极性:TIM输出比较极性低
	TIM_OC2Init(TIM9, &TIM_OCInitStructure);  					//根据T指定的参数初始化外设TIM1 4OC1

	TIM_OC2PreloadConfig(TIM9, TIM_OCPreload_Enable);  			//使能TIM14在CCR1上的预装载寄存器
 
	TIM_ARRPreloadConfig(TIM9,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM9, ENABLE);  
										  
}  



void TIM5_CH2_PWM_Init(u32 arr,u32 psc)
{		 					 
	 
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTF时钟	
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;           
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        	//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      	//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       	//上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);              
	
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  				//定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	//向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   				//自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
			
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 			//选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputState_Enable; 	//比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 		//输出极性:TIM输出比较极性低
	TIM_OC2Init(TIM5, &TIM_OCInitStructure);  

	TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);  
 
	TIM_ARRPreloadConfig(TIM5,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM5, ENABLE);  
										  
}  


void TIM3_CH1_PWM_Init(u32 arr,u32 psc)
{		 					 
	 
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTF时钟	
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3); 		
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;           	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        	//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      	//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        	//上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);              	
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  				//定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	//向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   				//自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
			
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);	
	
		
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 			//选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputState_Enable; 	//比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 		//输出极性:TIM输出比较极性低
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  	

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  
 
	TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM3, ENABLE); ;	
										  
}  


void TIM3_CH2_PWM_Init(u32 arr,u32 psc)
{		 					 
	 
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTF时钟	
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3); 	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;           
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        	//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      	//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        	//上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);              
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  				//定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	//向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   				//自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
			
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 			//选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputState_Enable; 	//比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 		//输出极性:TIM输出比较极性低
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  					//根据T指定的参数初始化外设TIM1 4OC1

	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  			//使能TIM14在CCR1上的预装载寄存器
 
	TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM3, ENABLE);
	
										  
} 



void TIM3_CH3_PWM_Init(u32 arr,u32 psc)
{		 					 
	 
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  		    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	//使能PORTF时钟	
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM3); 	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;           	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        	//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      	//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        	//上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure);              	
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  				//定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	//向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;  				 	//自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
			
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);	


	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 			//选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputState_Enable; 	//比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 		//输出极性:TIM输出比较极性低
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  					

	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  			
 
	TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM3, ENABLE);    
} 


