#include "control.h"

struct ROBOT BasketballRobot;

struct RADAR Radar;
struct VISION Vision;

void Control_Init(void)
{
	BasketballRobot.X = 0;		//机器人在坐标系中x坐标
	BasketballRobot.Y = 0;		//机器人在坐标系中y坐标
	BasketballRobot.ThetaR = 0;	//机器人正方向和y轴夹角
	BasketballRobot.ThetaR = 0;	//机器人正方向和y轴夹角
	
	BasketballRobot.Vx = 0;		//机器人在坐标系x方向速度
	BasketballRobot.Vy = 0;		//机器人在坐标系y方向速度
	BasketballRobot.W = 0;		//机器人角速度，顺时针正方向
	
	BasketballRobot.xPD.Kp = 0;
	BasketballRobot.xPD.Kd = 0;	
	BasketballRobot.yPD.Kp = 0;
	BasketballRobot.yPD.Kd = 0;
	BasketballRobot.wPD.Kp = 0;
	BasketballRobot.wPD.Kd = 0;
	
	BasketballRobot.w[1] = 0; //第一个编码器速度
	BasketballRobot.w[2] = 0; //第二个编码器速度
	BasketballRobot.w[0] = 0; //第三个编码器速度

	BasketballRobot.encoderCount[1] = 0; //第一个编编码器总计数
	BasketballRobot.encoderCount[2] = 0; //第二个编编码器总计数
	BasketballRobot.encoderCount[0] = 0; //第三个编码器总计数

	
	BasketballRobot.LastTheta = 0;	//上一时刻，机器人theta角
	BasketballRobot.theta_offset[0] = 0; //角度偏差矫正
	BasketballRobot.theta_offset[1] = 0; //角度偏差矫正
	BasketballRobot.theta_offset[2] = 0; //角度偏差矫正

	//雷达、视觉数据清空
	Radar.Angle = 0;
	Radar.Distance = 0;
	
	Vision.Depth = 0;
	Vision.X = 0;
	
	SetPWM(0,0,0);
	shoveMotor(STOP);
	
	IMU_Init();			//MPU6050初始化
	IMU_Init();	
	
}



//电机速度转换成PWM数值，原理看电机驱动板手册
//计算公式： V = Vmax *（占空比*100 – 50） /50
static void Velocity2PWM(float *V)
{
	*V+=1000;//*V=1000 - *V;
	if(*V>=1900)
		*V=1900;
	if(*V<=100)
		*V=100;
}

//设置三个轮子PWM
//V1:	电机1速度
//V2:	电机2速度
//V3;	电机3速度
void SetPWM(float V1,float V2,float V3)
{
	BasketballRobot.Velocity[0] = V1;
	BasketballRobot.Velocity[1] = V2;
	BasketballRobot.Velocity[2] = V3;
	
	//转换
	Velocity2PWM(&V1);
	Velocity2PWM(&V2);
	Velocity2PWM(&V3);
	
	//向CCR1寄存器写值   发出PWM波
 	TIM_SetCompare1(TIM3,(uint32_t)V1);
	
 	TIM_SetCompare2(TIM3,(uint32_t)V2);
	
 	TIM_SetCompare3(TIM3,(uint32_t)V3);	
}

//给定球场坐标速度求得轮子的速度
//vx：球场坐标的x轴速度
//vy：球场坐标的y轴速度
//w:机器人原地旋转的角速度
//注意最大速度！
void GetMotorVelocity(float vx,float vy,float w)
{
	u8 i,j,k;
	float L[3][3];
	float theta[3][3];
	float V[3];
	float tem[3][3];
					
	//v(PWM)=L*Theta*V
	//   cos(60)	sin(60)	-MOTOR_L
	//L= cos(180) 	sin(180)	-MOTOR_L
	//   cos(-60)	sin(-60)	-MOTOR_L
	L[0][0] =  0.5;					L[0][1] =  0.8660254037844386;		L[0][2] = -MOTOR_L;
	L[1][0] = -1;						L[1][1] =  0;						L[1][2] = -MOTOR_L;
	L[2][0] =  0.5;					L[2][1] = -0.8660254037844386;		L[2][2] = -MOTOR_L;
	//		cos(theta)	sin(theta)	0
	//theta= -sin(theta)	cos(theta) 	0
	//		 	0			0		1
	theta[0][0]= cos(BasketballRobot.ThetaR);	theta[0][1] = sin(BasketballRobot.ThetaR);	theta[0][2] = 0;
	theta[1][0]= -sin(BasketballRobot.ThetaR);	theta[1][1] = cos(BasketballRobot.ThetaR);	theta[1][2] = 0;
	theta[2][0]= 0;						theta[2][1] = 0;						theta[2][2] = 1;
	//V
	V[0] = -vx*10;
	V[1] = -vy;
	V[2] = -w;
	
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			tem[i][j] = 0;
			for(k=0;k<3;k++)
				tem[i][j] += L[i][k] * theta[k][j];			
		}
	}
	
	for(i=0;i<3;i++)
	{
		BasketballRobot.Velocity[i] = 0;
		for(j=0;j<3;j++)
			BasketballRobot.Velocity[i] += tem[i][j]*V[j];
	}
	
	LCD_Show_pwm();
}

//给自身坐标系速度求得轮子的速度
//vx：自身坐标标的x轴速度
//vy：自身坐标的y轴速度
//w:  机器人原地旋转的角速度
//注意最大速度！
void GetMotorVelocity_Self(float vx,float vy,float w)
{
	u8 i,j,k;
	float L[3][3];
	float theta[3][3];
	float V[3];
	float tem[3][3];
					
	//v(PWM)=L*Theta*V
	//   cos(60)	sin(60)	-MOTOR_L
	//L= cos(180) 	sin(180)	-MOTOR_L
	//   cos(-60)	sin(-60)	-MOTOR_L
	L[0][0] =  0.5;					L[0][1] =  0.8660254037844386;		L[0][2] = -MOTOR_L;
	L[1][0] = -1;						L[1][1] =  0;						L[1][2] = -MOTOR_L;
	L[2][0] =  0.5;					L[2][1] = -0.8660254037844386;		L[2][2] = -MOTOR_L;
	//		cos(0)	sin(0)	0
	//theta= -sin(0)	cos(0) 	0
	//		 0			0		1
	theta[0][0]= 1;						theta[0][1] = 0;					theta[0][2] = 0;
	theta[1][0]= 0;						theta[1][1] = 1;					theta[1][2] = 0;
	theta[2][0]= 0;						theta[2][1] = 0;					theta[2][2] = 1;
	//V
	V[0] = -vx*10;
	V[1] = -vy;
	V[2] = -w;
	
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			tem[i][j] = 0;
			for(k=0;k<3;k++)
				tem[i][j] += L[i][k] * theta[k][j];			
		}
	}
	
	for(i=0;i<3;i++)
	{
		BasketballRobot.Velocity[i] = 0;
		for(j=0;j<3;j++)
			BasketballRobot.Velocity[i] += tem[i][j]*V[j];
	}
	
	LCD_Show_pwm();
}


//获取红外开关状态
void GetInfraredState(void)
{	// return GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_9);
	while(1)
	{
		if(!INFRARED)
			break;
	}
}

//铲球电机状态
void shoveMotor(shovemotor t)
{
	u16 speed = 2000;
	
	if(t == STOP)
	{
		TIM_SetCompare2(TIM9,0);
		TIM_SetCompare1(TIM9,0);
	}
	else if(t == UP)
	{
		//CH1高电平,铲子向上，接黑线，电机反转
		TIM_SetCompare2(TIM9,0);
		TIM_SetCompare1(TIM9,speed+1500);
	}
	else if(t == DOWM)
	{
		//CH2高电平,铲子向下，接红线，电机正转
		TIM_SetCompare1(TIM9,0);
		TIM_SetCompare2(TIM9,speed-1000);
	}
	
}

//机械臂下降
void Robot_armDown(void)
{
	//原来板子：3000
	//V1.0：1750
	u16 i,t;
//	u16 W=2700;
	u16 nms=2000;

	
	if(LimitSwitchDowm==1)
	{
		shoveMotor(STOP);
		return;
	}
	shoveMotor(DOWM);
	while(1){
		if(LimitSwitchDowm==1){
			delay_ms(10);
			if(LimitSwitchDowm==1){
				shoveMotor(STOP);
				break;
			}
		}
	}
		shoveMotor(STOP);
	//EXTIX_Enable(1);
	#ifdef ZQD_DEBUG
	BEEP = 1;
	#endif
//	shoveMotor(DOWM);
	
//	LED1 = 1;
//	for(i=0;i<nms;i++)
//	{	  
//		if(LimitSwitchDowm == 1)
//		{	
//			for(t=0;t<0xff;t++);
//			if(LimitSwitchDowm==1)
//			{
//				shoveMotor(STOP);				
//				break;
//			}
//		}
//		for(t=0;t<0x4fff;t++)
//			if(LimitSwitchDowm == 1)
//				break;
//	}
//	shoveMotor(STOP);

	#ifdef ZQD_DEBUG
	BEEP = 0;
	#endif
}

//机械臂上升
void Robot_armUp(void)
{
	//原来板子：1960
	//V1.0:550
	u16 i,t;
//	u16 W=2700;
	u16 nms=2000;

	
	if(LimitSwitchUp==1)
	{
		shoveMotor(STOP);
		return;
	}
	shoveMotor(UP);
	while(1){
		if(LimitSwitchUp==1){
			delay_ms(10);
			if(LimitSwitchUp==1){
				shoveMotor(STOP);
				break;
			}
		}
	}
	//EXTIX_Enable(0);
	#ifdef ZQD_DEBUG
	BEEP = 1;
	#endif
	
//	shoveMotor(UP);
//	for(i=0;i<nms;i++)
//	{
//		if(LimitSwitchUp == 1)
//		{
//			for(t=0;t<0xff;t++);
//			if(LimitSwitchUp == 1)
//			{
//				shoveMotor(STOP);
//				break;
//			}
//		}
//		for(t=0;t<0x4fff;t++)
//			if(LimitSwitchUp == 1)
//				break;
//	}
//	shoveMotor(STOP);

	#ifdef ZQD_DEBUG
	BEEP = 0;
	#endif
}


//PD调整角速度
float adjustAngleV_PD(float D_Theta)
{
		float Vw=0;
		if(D_Theta>0&&(D_Theta<180))  
		{
			Vw=D_Theta*5;
		}
		else if(D_Theta>0&&(D_Theta>180)) 
		{
			D_Theta = 2*180-D_Theta;
			Vw=-D_Theta*5;
		}
		else if(D_Theta<0&&(D_Theta>-180)) 
		{
			Vw=D_Theta*5;
		}
		else if(D_Theta<0&&(D_Theta<-180)) 
		{
			D_Theta = 2*180+D_Theta;
			Vw=D_Theta*5;
		}
		
		if(Vw > 300)
		Vw=300;
		else if(Vw<-300)
			Vw=-300;
		
		return Vw;
}
//PD调整Y轴速度
float adjustVy_PD(float D_Y)
{
		float sy;

	if (D_Y > 0.05f)
	{
			sy = D_Y*400;
			if (BasketballRobot.Vy < 0.1f)
				sy = 250;
			else if (BasketballRobot.Vy < 0.3f)
				sy = 300;
			else if (BasketballRobot.Vy < 0.5f)
				sy = 400;
			
			if(D_Y<1)
				sy = D_Y*400+100;
			
			if(sy>800)
				sy=800;
		}
		
	
	else if (D_Y < -0.05f)
	{
			sy = D_Y*400;
			if (BasketballRobot.Vy > -0.1f)
				sy = -250;
			else if (BasketballRobot.Vy > -0.3f)
				sy = -300;
			else if (BasketballRobot.Vy > -0.5f)
				sy = -400;
			
			if(D_Y>-1)
				sy = D_Y*400-100;
			
			if(sy < -800)
				sy = -800;
		}


	
	else
		sy = 0;

	return sy;
}
//PD调整X轴速度
float adjustVx_PD(float D_X)
{
	float sx,NOW_DX;
	static float Last_DX;
	NOW_DX=D_X;
	if (NOW_DX > 0.05f)
	{
			sx = NOW_DX*25;
			
			if (BasketballRobot.Vx< 0.1f)
				sx = 30;
			else if (BasketballRobot.Vx < 0.3f)
				sx = 50;
			else if (BasketballRobot.Vx < 0.5f)
				sx = 55;
			
			
			if(NOW_DX < 1)
				sx = NOW_DX*25+10*(NOW_DX-Last_DX)+15;
			
			
			
			
			if(sx>80)
				sx=80;
			
			
		}
		
	
	else if (NOW_DX < -0.05f)
	{
			sx = NOW_DX*25;
			
			if (BasketballRobot.Vx > -0.1f)
				sx = -30;
			else if (BasketballRobot.Vx > -0.3f)
				sx = -50;
			else if (BasketballRobot.Vx > -0.5f)
				sx = -55;
		
			if(NOW_DX > -1)
				sx = (NOW_DX*25+10*(NOW_DX-Last_DX))-15;
			
			
			
			if(sx < -80)
				sx = -80;
		}


	
	else
		sx = 0;
	
	Last_DX=NOW_DX;
	delay_ms(10);
	return sx;


}

//根据偏差大小调整角速度
static float adjustAngleV(float D_Theta)
{
	float Vw = 0;

	//大于30°线性控制
	if (D_Theta > 0 && (D_Theta < 180))
	{
		Vw = D_Theta;
	}
	else if (D_Theta > 0 && (D_Theta >= 180))
	{
		D_Theta = 360 - D_Theta;
		Vw = -D_Theta;
	}
	else if (D_Theta < 0 && (D_Theta >= -180))
	{
		D_Theta = -D_Theta;
		Vw = -D_Theta;
	}
	else if (D_Theta < 0 && (D_Theta < -180))
	{
		D_Theta = 360 + D_Theta;
		Vw = D_Theta;
	}
	else
		Vw=Vw;

		//小于60°大于30°匀速
		//实际PWM为100

		if (D_Theta < 120)
	{
		if (Vw > 0)
			Vw = 240;
		else
			Vw = -240;
	}

	if (D_Theta < 60)
	{
		if (Vw > 30)
			Vw = 80;
		else
			Vw = -80;
	}
	//小于30°大于5°
	if (D_Theta < 30)
	{
		if (Vw > 30)
			Vw = 40;
		else
			Vw = -40;
	}
	if (D_Theta < 15)
	{
		if (Vw > 0)
			Vw = 20;
		else
			Vw = -20;
	}
	//小于5°
	if (D_Theta < 5)
	{
		if (Vw > 0)
			Vw = 10;
		else
			Vw = -10;
	}
//	if (D_Theta <1)
//		Vw = 0;

	return Vw;
}

//根据偏差大小调整Y轴速度
static float adjustVy(float D_Y)
{
	float sy;

	if (D_Y > 0.05f)
	{

		if (D_Y >= 1.5f)
		{
			sy = 8;
			if (BasketballRobot.Vy < 0.3f)
				sy = 2;
			else if (BasketballRobot.Vy < 0.5f)
				sy = 4;
			else if (BasketballRobot.Vy < 0.9f)
				sy = 6;
		}
		if (D_Y < 1.5f && D_Y > 0.2f)
			sy = D_Y * 10 / 2;

		if (D_Y < 0.2f)
			sy = 2;
	}
	else if (D_Y < -0.05f)
	{
		if (D_Y < -1.5f)
		{
			sy = -8;
			if (BasketballRobot.Vy > -0.3f)
				sy = -2;
			else if (BasketballRobot.Vy > -0.5f)
				sy = -4;
			else if (BasketballRobot.Vy > -0.9f)
				sy = -6;
		}

		if (D_Y > -1.5f && D_Y < -0.2f)
			sy = D_Y * 10 / 2;

		if (D_Y > -0.2f)
			sy = -2;
	}
	else
		sy = 0;

	return sy;
}

//根据偏差大小调整X轴速度
static float adjustVx(float D_X)
{
	float sx;

	if (D_X > 0.05f)
	{
		if (D_X > 1.5f)
		{
			sx = 8;
			if (BasketballRobot.Vx < 0.1f)
				sx = 1;
			else if (BasketballRobot.Vx < 0.2f)
				sx = 2;
			else if (BasketballRobot.Vx < 0.4f)
				sx = 2;
			else if (BasketballRobot.Vx < 0.58f)
				sx = 4;
			else if (BasketballRobot.Vx < 7.5f)
				sx = 6;
		}
		else if (D_X < 1.5f)
		{
			if (D_X > 0.2f)
				sx = 2;

			else if (D_X > 0.15f)
				sx =2;

			else if (D_X > 0.1f)
				sx = 1.5f;

			else
				sx = 1;
		}
	}
	else if (D_X < -0.05f)
	{
		if (D_X < -1.5f)
		{
			sx = -8;
			if (BasketballRobot.Vx > -0.1f)
				sx = -1;
			else if (BasketballRobot.Vx > -0.2f)
				sx = -2;
			else if (BasketballRobot.Vx > -0.4f)
				sx = -2;
			else if (BasketballRobot.Vx > -0.58f)
				sx = -4;
			else if (BasketballRobot.Vx > -7.5f)
				sx = -6;
		}
		else if (D_X > -1.5f)
		{
			if (D_X < -0.2f)
				sx = -2;

			else if (D_X < -0.15f)
				sx = -2;

			else if (D_X < -0.1f)
				sx = -1.5f;
			else
				sx = -1;
		}
	}
	else
		sx = 0;

	return sx;
}


//自旋运动，根据误差角度，自动调节
void RobotRotate(float theta)
{
	float D_Theta;
	float Vw=0;        //W大于0 逆时针
	
	while(theta < 0)
			theta = theta + 360;
			
		while (theta >360)
			theta = theta - 360;
	
	//D_Theta = theta-BasketballRobot.ThetaD;
	D_Theta = theta-BasketballRobot.ThetaD;
	
	Vw = adjustAngleV_PD(D_Theta)+2;
	
	
	while(D_Theta>5||D_Theta < -5)
	{
		
		
		SetPWM(Vw,Vw,Vw);
		
		D_Theta = theta-BasketballRobot.ThetaD;
		
		Vw  = adjustAngleV_PD(D_Theta)+2;
		LCD_Show_pwm();
	}
	SetPWM(0,0,0);

//	while(BasketballRobot.W);
}


//行至指定坐标
//X_I:目标坐标的X
//Y_I:目标坐标的Y
//Theta_I:目标坐标的角度
void RobotGoTo(float X_I,float Y_I,float Theta_I)
{
	float D_Theta,D_X,D_Y,Vw=0,sx,sy=0;
	
	D_Theta =  Theta_I - BasketballRobot.ThetaD;	//角度差
	D_X = X_I - BasketballRobot.X;				
	D_Y = Y_I - BasketballRobot.Y;
	
	while(fabs(D_Y) > 0.05f || fabs(D_X) > 0.05f)
	{
		sy = adjustVy_PD(D_Y);		
		
		sx = adjustVx_PD(D_X);
		
		Vw = adjustAngleV_PD(D_Theta)/2;
		
		GetMotorVelocity(sx,sy,Vw);
		
		SetPWM(BasketballRobot.Velocity[0],BasketballRobot.Velocity[1],BasketballRobot.Velocity[2]);
		
		LCD_Show_V();
		
		D_Theta =  Theta_I - BasketballRobot.ThetaD;
		D_X = X_I - BasketballRobot.X;
		D_Y = Y_I - BasketballRobot.Y;
	}
	SetPWM(0,0,0);
	delay_ms(1000);
	RobotRotate(Theta_I);
}

//避障直行
//直行1m
void RobotGoAvoidance(void)
{
	float D_Theta,Distance,D_X;
	float standard=35,sx=0;
//	float StraightDistance=0;
	
	D_Theta=Radar.Angle-RADAR_MID;
	Distance=Radar.Distance*sin(D_Theta);
	D_X = Distance-standard;
	
	while(fabs(D_X) > 0.05f){
	
	sx = adjustVx_PD(D_X);
		
	GetMotorVelocity_Self(sx,0,0);
		
	SetPWM(BasketballRobot.Velocity[0],BasketballRobot.Velocity[1],BasketballRobot.Velocity[2]);
		
	D_Theta=Radar.Angle-RADAR_MID;
	Distance=Radar.Distance*sin(D_Theta);
	D_X = Distance-standard;
		
	}
	
	
//	BasketballRobot.PX=BasketballRobot.X;
//	BasketballRobot.PY=BasketballRobot.Y;
	
/*
	while(StraightDistance<=1000){
		
		StraightDistance=sqrt(pow(BasketballRobot.PX-BasketballRobot.X,2)+pow(BasketballRobot.PY-BasketballRobot.Y,2));
		
		GetMotorVelocity_Self(0,10,0);
		SetPWM(BasketballRobot.Velocity[0],BasketballRobot.Velocity[1],BasketballRobot.Velocity[2]);
		delay_ms(1000);
	}
*/
	GetMotorVelocity_Self(0,40,0);	
	SetPWM(BasketballRobot.Velocity[0],BasketballRobot.Velocity[1],BasketballRobot.Velocity[2]);
	delay_ms(1000);
	//RobotGoTo(BasketballRobot.PX+700,BasketballRobot.PY+500,60);
	SetPWM(0,0,0);
		
	
	
}

//行至指定坐标
//X_I:目标坐标的X
//Y_I:目标坐标的Y
//Theta_I:目标坐标的角度
//angle:目标坐标和当前机器人坐标的角度
//先对准目标坐标，前行过程中判断是否有障碍，但是无法判别是篮球还是机器人，因此仅试用与球场中间空旷区域
void RobotGoToAvoid(float X_I, float Y_I, float Theta_I)
{
	float D_Theta, D_X, D_Y, Vw = 0, sx, sy = 0;
	float Offest_theta,angle,standard=350,Distance;

	
	D_X = X_I - BasketballRobot.X;
	D_Y = Y_I - BasketballRobot.Y;
	
	angle=atan2(D_Y,D_X);
	
	if(angle>0)
		angle=PI/2-angle;
	
	else
		angle=-PI/2-angle;
	
	RobotRotate(-angle/PI*180);
	
	D_Theta = Theta_I - BasketballRobot.ThetaD; //角度差
	
	while (fabs(D_Y) > 0.05f || fabs(D_X) > 0.05f)
	{
			
		//while((Radar.RX_STA&0x8000) != 0);
		
		GetRadarData();
		
			if(Radar.Distance>1500 || (Radar.State==0)){
				
			sx = adjustVx_PD(D_X)/3;

			sy = adjustVy_PD(D_Y)/3;

			Vw = adjustAngleV_PD(angle - BasketballRobot.ThetaD+360);

			GetMotorVelocity(sx , sy , 0);

			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);

			D_Theta = Theta_I - BasketballRobot.ThetaD;
			D_X = X_I - BasketballRobot.X;
			D_Y = Y_I - BasketballRobot.Y;
			//	RobotGoTo(X_I,Y_I,Theta_I);
				//Radar.State = 0;
			}		
		  else{
				
//				SetPWM(0,0,0);
				
			  Offest_theta=(Radar.Angle*1.0f-RADAR_MID)*1.0f*PI/180;
				Distance=Radar.Distance*sin(Offest_theta);
				
			  if(fabs(Distance)>350)
			  {
				 RobotGoTo(BasketballRobot.X-1.0f*sin(BasketballRobot.ThetaR),BasketballRobot.Y+1.0f*cos(BasketballRobot.ThetaR),BasketballRobot.ThetaD);
				  continue;
				  
			  }
			  
				if(Offest_theta>0){
					
					if(Distance<350)
					{
						
							GetMotorVelocity_Self((Distance-standard)/5,0,0);
							SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
						while(((Radar.RX_STA & 0x8000) == 0));
//							Offest_theta=(Radar.Angle*1.0f-RADAR_MID)*PI/180;
//							Distance=Radar.Distance*sin(Offest_theta);
					}
				
				}
				else
				{
					if(Distance>-350)
					{
						GetMotorVelocity_Self((Distance-standard)/5,0,0);
						SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
						
						while(((Radar.RX_STA & 0x8000) == 0));
//						Offest_theta=(Radar.Angle*1.0f-RADAR_MID)*PI/180;
//						Distance=Radar.Distance*sin(Offest_theta);
					}
					
				}
//				RobotGoTo(BasketballRobot.X,Y_I,angle);
//				RobotGoTo(X_I,Y_I,Theta_I);
			Radar.State=0;			
			}
			
	}
	SetPWM(0, 0, 0);
	delay_ms(1000);
	RobotRotate(Theta_I);
}




u8 DownShotUp(void)
{
	Robot_armDown();
	CHARGE = 0;
	if(LimitSwitchDowm == 1)
	{
		CHARGE = 0;
		delay_ms(500);
		SHOT = 1;
		delay_ms(500);
		SHOT = 0;
		
		return 0;
	}
	
	Robot_armUp();
	return 1;
}








