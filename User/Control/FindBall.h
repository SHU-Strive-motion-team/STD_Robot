#ifndef __FINDBALL_H__
#define __FINDBALL_H__

#include "control.h"


static float adjustVy_findPD(float D_Y);			//����ƫ���С����Y���ٶ�
static float adjustVx_findPD(float D_X);			//����ƫ���С����X���ٶ�


void FindVolleyball_red(void);
void FindVolleyball_yellow(void);
void FindBasketball_blue(void);
void FindBasketball_red(void);
void FindBasketball_double(void);
void FindFindVolleyball_double(void);

void FindBall_vision(u8 ball);		//�Ӿ�����,����4������
void FindBall_radar(void);			//���ü�������
void FindBall_VandR(u8 ball);			//�Ӿ����״�������


void GoBack_Vision(void);         //�Ӿ���λ
void FindBasketry(void);       //������ 


#endif
