#ifndef __FINDBALL_H__
#define __FINDBALL_H__

#include "control.h"


static float adjustVy_findPD(float D_Y);			//根据偏差大小调整Y轴速度
static float adjustVx_findPD(float D_X);			//根据偏差大小调整X轴速度


void FindVolleyball_red(void);
void FindVolleyball_yellow(void);
void FindBasketball_blue(void);
void FindBasketball_red(void);
void FindBasketball_double(void);
void FindFindVolleyball_double(void);

void FindBall_vision(u8 ball);		//视觉找球,限制4米以内
void FindBall_radar(void);			//利用激光找球
void FindBall_VandR(u8 ball);			//视觉、雷达找球结合
void FindBall_VandR_middle(u8 ball);

void GoBack_Vision(void);         //视觉回位
void FindBasketry(void);       //找篮筐 


#endif
