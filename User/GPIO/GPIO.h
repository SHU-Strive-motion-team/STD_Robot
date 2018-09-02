#ifndef __GPIO_H
#define __GPIO_H	 
#include "sys.h" 


///Beep�˿ڶ���
#define BEEP PDout(6)	
//��������ʼ��	
void BEEP_Init(void);


//LED�˿ڶ���
#define LED0 PFout(10)	
#define LED1 PFout(9)		 
//LED��ʼ��
void LED_Init(void);	


//������ʼ������
void KEY_Init(void);

//�����翪�ض˿ڶ���
#define CHARGE PGout(6)
//�����翪�س�ʼ��
void Charge_Init(void);

//���俪�ض˿ڶ���
#define SHOT PGout(7)
//�����ʼ��,GPIOG7
void ShotSwitch_Init(void);

//��λ���ض˿ڶ���	
#define LimitSwitchDown 	PCin(1)	
#define LimitSwitchUp   	PCin(0)

//��ʱ����������IO
#define IN1 PFout(0)
#define IN2 PFout(2)

//��λ���س�ʼ��
void LimitSwitch_init(void);

//���⿪�ض˿ڶ���
#define INFRARED PCin(2)
//���⿪�س�ʼ��
void InfraredSwitch_Init(void);

//��ʱ����������IO��ʼ�� 
void shoveMotorIO_Init(void);

void ALL_GPIO_Init(void);
	
#endif

















