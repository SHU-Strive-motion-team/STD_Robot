#ifndef __FINDBALL_H__
#define __FINDBALL_H__

#include "control.h"





void FindVolleyball_red(void);
void FindVolleyball_yellow(void);
void FindBasketball_blue(void);
void FindBasketball_red(void);


void FindBall_vision(u8 ball);		//�Ӿ�����,����4������
void FindBall_radar(void);			//���ü�������
void FindBall_VandR(u8 ball);			//�Ӿ����״�������


void GoBack_Vision(u8 ball);         //�Ӿ���λ
void FindBasketry(void);       //������ 


#endif
