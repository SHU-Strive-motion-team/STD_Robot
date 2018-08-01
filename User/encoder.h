#ifndef __ENCODER_H
#define __ENCODER_H
#include "sys.h"
#include "lcd.h"
//#include "control.h"
#define Encoder1_B_Pin GPIO_Pin_1
#define Encoder1_A_Pin GPIO_Pin_5
#define Encoder1_GPIO_Port GPIOA

#define Encoder2_A_Pin GPIO_Pin_12
#define Encoder2_B_Pin GPIO_Pin_13
#define Encoder2_GPIO_Port GPIOD

#define Encoder3_A_Pin GPIO_Pin_6
#define Encoder3_B_Pin GPIO_Pin_7
#define Encoder3_GPIO_Port GPIOC


void Encoder_Init(void);

void encoder1_init(void);
void encoder2_init(void);
void encoder3_init(void);

void TIM2_Init(u32 arr,u32 psc);	//����ʱ����ʼ����TIM2����ȡ��̼���Ϣ��



#endif
