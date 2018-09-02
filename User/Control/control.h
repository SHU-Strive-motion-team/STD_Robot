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

#define ENCODER_FD 	4.0f			//编码器分频
#define ENCODER_MAX 500.0f		//编码器编码数
#define ENCODER_T 	0.01f   		//编码器计算当前速度时间
#define ENCODER_L 	0.2006f		//编码器轮到机器人中心的距离
#define ENCODER_R 	0.0275f		//编码器轮子半径
#define RAD	 0.1570796327f		//编码器一个脉冲对应的角度 pi/500/4/0.01

//#define MOTOR_L 0.2013f		//轮到机器人中心的距离
#define MOTOR_L 1.0f	//轮到机器人中心的距离
#define MOTOR_R 0.0508f		//轮子的半径

#define MOTOR_STATIC_1 4000		//TIM9 CH1 PE5
#define MOTOR_STATIC_2 4000  		//TIM9 CH2 PE6

#define RADAR_MID 268    //雷达定位中心
#define VISION_MID 256	//视觉定位中心
#define DIS_RADAR 2300	//篮筐雷达定位距离
#define DIS_VISION 280	//篮筐视觉定位距离

#define Origin_X 0		//视觉屏幕原点x坐标，现假设
#define Origin_Y 0		//视觉屏幕原点y坐标，待修改

#define Correction_X -0.3f	//球场坐标修正值x
#define Correction_Y 0.3f	//球场坐标修正值y

//PD参数
typedef struct
{
	float Kp;
	float Kd;
}PD;

struct ROBOT
{
	float X;		//机器人在坐标系中x坐标
	float Y;		//机器人在坐标系中y坐标
	//float x;		//机器人在坐标系中x坐标
	//float y;		//机器人在坐标系中y坐标
	float PX;		//点的x坐标
	float PY;		//点的y坐标
	float ThetaR;	//机器人正方向和y轴夹角 弧度
	float ThetaD;	//机器人正方向和y轴夹角 角度

	float Vx;		//机器人在坐标系x方向速度
	float Vy;		//机器人在坐标系y方向速度	
	float W;		//机器人角速度，顺时针正方向
	
//	PD xPD;			//机器人在坐标系x方向 PD
//	PD yPD;
//	PD wPD;
	
	int16_t  w[3];				//编码器速度
	int64_t encoderCount[3];	//编码器总计数
	
	float Velocity[3];	//轮子的速度
	
	float LastTheta;	//上一时刻，机器人theta角
	float theta_offset[3];	//角度偏差矫正
};

//接收雷达数据，极坐标
struct RADAR
{
	uint16_t RX_STA;
	
	uint8_t  RX_BUF[20];
	
	uint32_t Distance;  //距离
	
	uint32_t Angle;	//角度
	
	u8 State;	//状态
};		

//接收视觉数据
struct VISION
{
	uint16_t RX_STA;
	
	uint8_t  RX_BUF[20];
	
	uint32_t Depth;	//深度，纵轴
	
	uint32_t X;		//X位置，横轴
	
	u8 goBackSign; 	//回位标志
	
	u8 State;	//状态
};

//铲球电机运行状态
typedef enum
{
	STOP = 0,
	UP,
	DOWN
}shovemotor;	

extern struct ROBOT BasketballRobot;

extern struct RADAR Radar;
extern struct VISION Vision;


void Control_Init(void);		//机器人初始化


static void Velocity2PWM(float *V);			//电机速度转换成PWM数值，原理看电机驱动板手册
void SetPWM(float V1,float V2,float V3); 	//设置三个轮子PWM

void GetMotorVelocity(float vx,float vy,float w);		//给定球场坐标速度求得轮子的速度
void GetMotorVelocity_Self(float vx,float vy,float w);	//给自身坐标系速度求得轮子的速度


void GetInfraredState(void);	//获取红外开关状态
void shoveMotor(shovemotor t);	//铲球电机状态
void Robot_armDown(void);		//机械臂下降
void Robot_armUp(void);			//机械臂上升

void EXTIX_Disable(shovemotor t);
void EXTIX_Enable(shovemotor t);
void RobotArm_exit(shovemotor t);		//通过中断控制机械臂

u8 DownShotUp(void);
u8 ShotUp(void);


//uint8_t GetVisionData(void);		//视觉数据处理
//uint8_t GetRadarData(void);		//激光处理数据


static float adjustAngleV(float D_Theta);	//根据偏差大小调整角速度
static float adjustVy(float D_Y);			//根据偏差大小调整Y轴速度
static float adjustVx(float D_X);			//根据偏差大小调整X轴速度

static float adjustAngleV_PD(float D_Theta);	//根据偏差大小调整角速度
static float adjustVy_PD(float D_Y);			//根据偏差大小调整Y轴速度
static float adjustVx_PD(float D_X);			//根据偏差大小调整X轴速度


void RobotRotate(float theta);	//自旋运动，根据误差角度，自动调节
void RobotGoBrokenLine(float X_I,float Y_I,float Theta_I,float pointX, float pointY,float pointTheta);
void RobotGoToAvoid(float X_I, float Y_I, float Theta_I);
void RobotGoTo(float X_I,float Y_I,float Theta_I);	//行至指定坐标
void RobotGoAvoidance(void);	//避障直行
#endif
