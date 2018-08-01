#ifndef _PWM_H
#define _PWM_H
#include "sys.h"


//PWM≤®≥ı ºªØ
void PWM_Init(void);


void TIM9_CH1_PWM_Init(u32 arr,u32 psc);
void TIM9_CH2_PWM_Init(u32 arr,u32 psc);


void TIM5_CH2_PWM_Init(u32 arr,u32 psc);


void TIM3_CH1_PWM_Init(u32 arr,u32 psc);
void TIM3_CH2_PWM_Init(u32 arr,u32 psc);
void TIM3_CH3_PWM_Init(u32 arr,u32 psc);
//void TIM3_CH4_PWM_Init(u32 arr,u32 psc);




#endif
