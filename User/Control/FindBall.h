#ifndef __FINDBALL_H__
#define __FINDBALL_H__

#include "control.h"





void FindBasketball(void);
void FindVolleyball(void);
void FindFrame(void);


void FindBall_vision(u8 ball);		//�Ӿ�����,����4������
void FindBall_radar(void);			//���ü�������
void FindBall_VandR(u8 ball);			//�Ӿ����״�������


void GoBack_Vision(u8 ball);         //�Ӿ���λ
void FindBasketry(void);       //������ 


#endif
