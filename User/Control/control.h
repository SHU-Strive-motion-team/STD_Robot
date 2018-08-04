#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "lcd.h"
#include "GPIO.h"
#include "MPU6050.h"
#include "remote.h"
#include "tim.h"
#include "get_info.h"
#include "EXIT.h"
#include <math.h>

#define PI 		3.141592654f

#define ENCODER_FD 	4.0f			//��������Ƶ
#define ENCODER_MAX 500.0f		//������������
#define ENCODER_T 	0.01f   		//���������㵱ǰ�ٶ�ʱ��
#define ENCODER_L 	0.2006f		//�������ֵ����������ĵľ���
#define ENCODER_R 	0.0275f		//���������Ӱ뾶
#define RAD	 0.1570796327f		//������һ�������Ӧ�ĽǶ� pi/500/4/0.01

//#define MOTOR_L 0.2013f		//�ֵ����������ĵľ���
#define MOTOR_L 1.0f	//�ֵ����������ĵľ���
#define MOTOR_R 0.0508f		//���ӵİ뾶

#define MOTOR_STATIC_1 4000		//TIM9 CH1 PE5
#define MOTOR_STATIC_2 4000  		//TIM9 CH2 PE6

#define RADAR_MID 268	//�״ﶨλ����
#define VISION_MID 320	//�Ӿ���λ����
#define DIS_RADAR 2500	//�����״ﶨλ����
#define DIS_VISION 280	//�����Ӿ���λ����

#define Origin_X 0		//�Ӿ���Ļԭ��x���꣬�ּ���
#define Origin_Y 0		//�Ӿ���Ļԭ��y���꣬���޸�

#define Correction_X -0.3f	//����������ֵx
#define Correction_Y 0.3f	//����������ֵy

//PD����
typedef struct
{
	float Kp;
	float Kd;
}PD;

struct ROBOT
{
	float X;		//������������ϵ��x����
	float Y;		//������������ϵ��y����
	//float x;		//������������ϵ��x����
	//float y;		//������������ϵ��y����
	float PX;		//���x����
	float PY;		//���y����
	float ThetaR;	//�������������y��н� ����
	float ThetaD;	//�������������y��н� �Ƕ�

	float Vx;		//������������ϵx�����ٶ�
	float Vy;		//������������ϵy�����ٶ�	
	float W;		//�����˽��ٶȣ�˳ʱ��������
	
	PD xPD;			//������������ϵx���� PD
	PD yPD;
	PD wPD;
	
	int16_t  w[3];				//�������ٶ�
	int64_t encoderCount[3];	//�������ܼ���
	
	float Velocity[3];	//���ӵ��ٶ�
	
	float LastTheta;	//��һʱ�̣�������theta��
	float theta_offset[3];	//�Ƕ�ƫ�����
};

//�����״����ݣ�������
struct RADAR
{
	uint16_t RX_STA;
	
	uint8_t  RX_BUF[20];
	
	uint32_t Distance;  //����
	
	uint32_t Angle;	//�Ƕ�
	
	u8 State;	//״̬
};		

//�����Ӿ�����
struct VISION
{
	uint16_t RX_STA;
	
	uint8_t  RX_BUF[20];
	
	uint32_t Depth;	//��ȣ�����
	
	uint32_t X;		//Xλ�ã�����
	
	u8 State;	//״̬
};

//����������״̬
typedef enum
{
	STOP = 0,
	UP,
	DOWN
}shovemotor;	

extern struct ROBOT BasketballRobot;

extern struct RADAR Radar;
extern struct VISION Vision;


void Control_Init(void);		//�����˳�ʼ��


static void Velocity2PWM(float *V);			//����ٶ�ת����PWM��ֵ��ԭ������������ֲ�
void SetPWM(float V1,float V2,float V3); 	//������������PWM

void GetMotorVelocity(float vx,float vy,float w);		//�����������ٶ�������ӵ��ٶ�
void GetMotorVelocity_Self(float vx,float vy,float w);	//����������ϵ�ٶ�������ӵ��ٶ�


void GetInfraredState(void);	//��ȡ���⿪��״̬
void shoveMotor(shovemotor t);	//������״̬
void Robot_armDown(void);		//��е���½�
void Robot_armUp(void);			//��е������

void EXTIX_Disable(shovemotor t);
void EXTIX_Enable(shovemotor t);
void RobotArm_exit(shovemotor t);		//ͨ���жϿ��ƻ�е��

u8 DownShotUp(void);


//uint8_t GetVisionData(void);		//�Ӿ����ݴ���
//uint8_t GetRadarData(void);		//���⴦������


static float adjustAngleV(float D_Theta);	//����ƫ���С�������ٶ�
static float adjustVy(float D_Y);			//����ƫ���С����Y���ٶ�
static float adjustVx(float D_X);			//����ƫ���С����X���ٶ�

float adjustAngleV_PD(float D_Theta);	//����ƫ���С�������ٶ�
float adjustVy_PD(float D_Y);			//����ƫ���С����Y���ٶ�
float adjustVx_PD(float D_X);			//����ƫ���С����X���ٶ�


void RobotRotate(float theta);	//�����˶����������Ƕȣ��Զ�����
void RobotGoToAvoid(float X_I, float Y_I, float Theta_I);
void RobotGoTo(float X_I,float Y_I,float Theta_I);	//����ָ������
void RobotGoAvoidance(void);	//����ֱ��
#endif
