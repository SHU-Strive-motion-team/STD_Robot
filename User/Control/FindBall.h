#ifndef __FINDBALL_H__
#define __FINDBALL_H__

#include "control.h"





void FindVolleyball_red(void);
void FindVolleyball_yellow(void);
void FindBasketball_blue(void);
void FindBasketball_red(void);


void FindBall_vision(u8 ball);		//视觉找球,限制4米以内
void FindBall_radar(void);			//利用激光找球
void FindBall_VandR(u8 ball);			//视觉、雷达找球结合


void GoBack_Vision(u8 ball);         //视觉回位
void FindBasketry(void);       //找篮筐 


#endif
