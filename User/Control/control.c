#include "control.h"

struct ROBOT BasketballRobot;

struct RADAR Radar;
struct VISION Vision;

void Control_Init(void)
{
	BasketballRobot.X = 0;		//������������ϵ��x����
	BasketballRobot.Y = 0;		//������������ϵ��y����
	BasketballRobot.ThetaR = 0;	//�������������y��н�
	BasketballRobot.ThetaR = 0;	//�������������y��н�
	
	BasketballRobot.Vx = 0;		//������������ϵx�����ٶ�
	BasketballRobot.Vy = 0;		//������������ϵy�����ٶ�
	BasketballRobot.W = 0;		//�����˽��ٶȣ�˳ʱ��������
	
	BasketballRobot.xPD.Kp = 0;
	BasketballRobot.xPD.Kd = 0;	
	BasketballRobot.yPD.Kp = 0;
	BasketballRobot.yPD.Kd = 0;
	BasketballRobot.wPD.Kp = 0;
	BasketballRobot.wPD.Kd = 0;
	
	BasketballRobot.w[1] = 0; //��һ���������ٶ�
	BasketballRobot.w[2] = 0; //�ڶ����������ٶ�
	BasketballRobot.w[0] = 0; //�������������ٶ�

	BasketballRobot.encoderCount[1] = 0; //��һ����������ܼ���
	BasketballRobot.encoderCount[2] = 0; //�ڶ�����������ܼ���
	BasketballRobot.encoderCount[0] = 0; //�������������ܼ���

	
	BasketballRobot.LastTheta = 0;	//��һʱ�̣�������theta��
	BasketballRobot.theta_offset[0] = 0; //�Ƕ�ƫ�����
	BasketballRobot.theta_offset[1] = 0; //�Ƕ�ƫ�����
	BasketballRobot.theta_offset[2] = 0; //�Ƕ�ƫ�����

	//�״�Ӿ��������
	Radar.Angle = 0;
	Radar.Distance = 0;
	
	Vision.Depth = 0;
	Vision.X = 0;
	
	SetPWM(0,0,0);
	shoveMotor(STOP);
	
	IMU_Init();			//MPU6050��ʼ��
	IMU_Init();	
	
}



//����ٶ�ת����PWM��ֵ��ԭ��������������ֲ�
//���㹫ʽ�� V = Vmax *��ռ�ձ�*100 �C 50�� /50
static void Velocity2PWM(float *V)
{
	*V+=1000;//*V=1000 - *V;
	if(*V>=1900)
		*V=1900;
	if(*V<=100)
		*V=100;
}

//������������PWM
//V1:	���1�ٶ�
//V2:	���2�ٶ�
//V3;	���3�ٶ�
void SetPWM(float V1,float V2,float V3)
{
	if(V1>-1&&V1<1)
		V1=1;
	if(V2>-1&&V2<1)
		V2=1;
	if(V3>-1&&V3<1)
		V3=1;
	BasketballRobot.Velocity[0] = V1;
	BasketballRobot.Velocity[1] = V2;
	BasketballRobot.Velocity[2] = V3;
	
	//ת��
	Velocity2PWM(&V1);
	Velocity2PWM(&V2);
	Velocity2PWM(&V3);
	
	//��CCR1�Ĵ���дֵ   ����PWM��
 	TIM_SetCompare1(TIM3,(uint32_t)V1);
	
 	TIM_SetCompare2(TIM3,(uint32_t)V2);
	
 	TIM_SetCompare3(TIM3,(uint32_t)V3);	
}

//�����������ٶ�������ӵ��ٶ�
//vx���������x���ٶ�
//vy���������y���ٶ�
//w:������ԭ����ת�Ľ��ٶ�
//ע������ٶȣ�
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

//����������ϵ�ٶ�������ӵ��ٶ�
//vx������������x���ٶ�
//vy�����������y���ٶ�
//w:  ������ԭ����ת�Ľ��ٶ�
//ע������ٶȣ�
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


//��ȡ���⿪��״̬
void GetInfraredState(void)
{	// return GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_9);
	while(1)
	{
		if(!INFRARED)
			break;
	}
}

//������״̬
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
		//CH1�ߵ�ƽ,�������ϣ��Ӻ��ߣ������ת
		TIM_SetCompare2(TIM9,0);
		TIM_SetCompare1(TIM9,speed);
	}
	else if(t == DOWM)
	{
		//CH2�ߵ�ƽ,�������£��Ӻ��ߣ������ת
		TIM_SetCompare1(TIM9,0);
		TIM_SetCompare2(TIM9,speed);
	}
	
}

//��е���½�
void Robot_armDown(void)
{
	//ԭ�����ӣ�3000
	//V1.0��1750
	u16 i,t;
//	u16 W=2700;
	u16 nms=2000;

	
	if(LimitSwitchDowm==1)
	{
		shoveMotor(STOP);
		return;
	}
	//EXTIX_Enable(1);
	#ifdef ZQD_DEBUG
	BEEP = 1;
	#endif
	shoveMotor(DOWM);
	
	LED1 = 1;
	for(i=0;i<nms;i++)
	{	  
		if(LimitSwitchDowm == 1)
		{	
			for(t=0;t<0xff;t++);
			if(LimitSwitchDowm==1)
			{
				shoveMotor(STOP);				
				break;
			}
		}
		for(t=0;t<0x4fff;t++)
			if(LimitSwitchDowm == 1)
				break;
	}
	shoveMotor(STOP);

	#ifdef ZQD_DEBUG
	BEEP = 0;
	#endif
}

//��е������
void Robot_armUp(void)
{
	//ԭ�����ӣ�1960
	//V1.0:550
	u16 i,t;
//	u16 W=2700;
	u16 nms=2000;

	
	if(LimitSwitchUp==1)
	{
		shoveMotor(STOP);
		return ;
	}
	//EXTIX_Enable(0);
	#ifdef ZQD_DEBUG
	BEEP = 1;
	#endif
	
	shoveMotor(UP);
	for(i=0;i<nms;i++)
	{
		if(LimitSwitchUp == 1)
		{
			for(t=0;t<0xff;t++);
			if(LimitSwitchUp == 1)
			{
				shoveMotor(STOP);
				break;
			}
		}
		for(t=0;t<0x4fff;t++)
			if(LimitSwitchUp == 1)
				break;
	}
	shoveMotor(STOP);

	#ifdef ZQD_DEBUG
	BEEP = 0;
	#endif
}


//PD�������ٶ�
static float adjustAngleV_PD(float D_Theta)
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
//PD����Y���ٶ�
static float adjustVy_PD(float D_Y)
{
		float sy;

	if (D_Y > 0.05f)
	{
			sy = D_Y*400;
			if (BasketballRobot.Vy < 0.3f)
				sy = 200;
			else if (BasketballRobot.Vy < 0.5f)
				sy = 400;
			else if (BasketballRobot.Vy < 0.9f)
				sy = 400;
			if(sy>800)
				sy=800;
		}
		
	
	else if (D_Y < -0.05f)
	{
			sy = -D_Y*300;
			if (BasketballRobot.Vy > -0.3f)
				sy = -100;
			else if (BasketballRobot.Vy > -0.5f)
				sy = -200;
			else if (BasketballRobot.Vy > -0.9f)
				sy = -300;
			
			if(sy < -800)
				sy = -800;
		}


	
	else
		sy = 0;

	return sy;
}
//PD����X���ٶ�
static float adjustVx_PD(float D_X)
{
	float sx;

	if (D_X > 0.05f)
	{
			sx = D_X*350;
			if (BasketballRobot.Vx< 0.3f)
				sx = 100;
			else if (BasketballRobot.Vx < 0.5f)
				sx = 240;
			else if (BasketballRobot.Vx < 0.9f)
				sx = 300;
			if(sx>700)
				sx=700;
		}
		
	
	else if (D_X < -0.05f)
	{
			sx = -D_X*350;
			if (BasketballRobot.Vx > -0.3f)
				sx = -100;
			else if (BasketballRobot.Vx > -0.5f)
				sx = -200;
			else if (BasketballRobot.Vx > -0.9f)
				sx = -300;
			
			if(sx < -700)
				sx = -700;
		}


	
	else
		sx = 0;

	return sx;


}

//����ƫ���С�������ٶ�
static float adjustAngleV(float D_Theta)
{
	float Vw = 0;

	//����30�����Կ���
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

		//С��60�����30������
		//ʵ��PWMΪ100

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
	//С��30�����5��
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
	//С��5��
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

//����ƫ���С����Y���ٶ�
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

//����ƫ���С����X���ٶ�
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


//�����˶����������Ƕȣ��Զ�����
void RobotRotate(float theta)
{
	float D_Theta;
	float Vw=0;        //W����0 ��ʱ��

	//D_Theta = theta-BasketballRobot.ThetaD;
	D_Theta = theta-BasketballRobot.ThetaD;
	Vw = adjustAngleV_PD(D_Theta);
	
	
	while(D_Theta>5||D_Theta < -5)
	{
		GetMotorVelocity(0,0,Vw);	
		
		SetPWM(BasketballRobot.Velocity[0],BasketballRobot.Velocity[1],BasketballRobot.Velocity[2]);
		
		D_Theta = theta-BasketballRobot.ThetaD;
		
		Vw  = adjustAngleV_PD(D_Theta);
	}
	SetPWM(0,0,0);

	while(BasketballRobot.W);
}


//����ָ������
//X_I:Ŀ�������X
//Y_I:Ŀ�������Y
//Theta_I:Ŀ������ĽǶ�
void RobotGoTo(float X_I,float Y_I,float Theta_I)
{
	float D_Theta,D_X,D_Y,Vw=0,sx,sy=0;
	
	D_Theta =  Theta_I - BasketballRobot.ThetaD;	//�ǶȲ�
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
	delay_ms(100);
	RobotRotate(Theta_I);
}

//����ֱ��
//ֱ��1m
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
	GetMotorVelocity_Self(20,0,0);	
	SetPWM(BasketballRobot.Velocity[0],BasketballRobot.Velocity[1],BasketballRobot.Velocity[2]);
	delay_ms(1000);
	//RobotGoTo(BasketballRobot.PX+700,BasketballRobot.PY+500,60);
	SetPWM(0,0,0);
		
	
	
}

//����ָ������
//X_I:Ŀ�������X
//Y_I:Ŀ�������Y
//Theta_I:Ŀ������ĽǶ�
//angle:Ŀ������͵�ǰ����������ĽǶ�
//�ȶ�׼Ŀ�����꣬ǰ�й������ж��Ƿ����ϰ��������޷��б��������ǻ����ˣ���˽����������м�տ�����
void RobotGoToAvoid(float X_I, float Y_I, float Theta_I)
{
	float D_Theta, D_X, D_Y, Vw = 0, sx, sy = 0;
	float Offest_theta,angle,standard=35,Distance;

	
	D_X = X_I - BasketballRobot.X;
	D_Y = Y_I - BasketballRobot.Y;
	
	angle=atan2(D_Y,D_X);
	if(angle>0)
		angle=PI/2-angle;
	else
		angle=-PI/2-angle;
	RobotRotate(angle);
	
	D_Theta = Theta_I - BasketballRobot.ThetaD; //�ǶȲ�
	
	while (fabs(D_Y) > 0.05f || fabs(D_X) > 0.05f)
	{
			if(!GetRadarData()){
			sy = adjustVy(D_Y);

			sx = adjustVx(D_X);

			Vw = adjustAngleV(D_Theta);

			GetMotorVelocity(sx * 12, sy * 100, Vw);

			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);

			D_Theta = Theta_I - BasketballRobot.ThetaD;
			D_X = X_I - BasketballRobot.X;
			D_Y = Y_I - BasketballRobot.Y;
			}		
		  else{
				SetPWM(0,0,0);
			  Offest_theta=Radar.Angle-RADAR_MID;
				Distance=Radar.Distance*sin(Offest_theta);
				if(Offest_theta>0){
					
				while(Distance>35&&(!GetRadarData()))
				{
					GetMotorVelocity_Self(Distance-standard,0,0);
				}
				
				}
				else
				{
					while(Distance>35&&(!GetRadarData()))
				  {
					GetMotorVelocity_Self(standard-Distance,0,0);
			   	}
					
				}
				RobotGoTo(BasketballRobot.X,Y_I,angle);
				RobotGoTo(X_I,Y_I,Theta_I);
						
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







