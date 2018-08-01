#ifndef __GETPOSITION_H
#define __GETPOSITION_H

#include "tim.h"
#include "control.h"
#include "sys.h"



void ReadEncoder(void);

void receiveIMUData(void);
u8 GetYaw(void);

void GetPosition(void);		//����ת��

void receiveVisionData(void);
u8 GetVisionData(void);		//�Ӿ����ݴ���

void receiveRadarData(void);
u8 GetRadarData(void);		//���⴦������

#endif

