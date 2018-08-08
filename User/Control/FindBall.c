#include "FindBall.h"
#include "control.h"
#include <math.h>

void FindVolleyball_red(void)
{
	USART_SendData(USART1, '1');
}
void FindVolleyball_yellow(void)
{
	USART_SendData(USART1, '2');
}
void FindBasketball_blue(void)
{
	USART_SendData(USART1, '3');
}
void FindBasketball_red(void)
{
	USART_SendData(USART1, '4');
}
void FindBasketball_double(void)
{
	USART_SendData(USART1, '5');
}
void FindFindVolleyball_double(void)
{
	USART_SendData(USART1, '6');
}
void FindFindVolleyball_jindian(void)
{
	USART_SendData(USART1, '7');
}
void FindFindbasketball_jindian(void)
{
	USART_SendData(USART1, '8');
}
void FindFrame(void)
{
	USART_SendData(USART1, 'a');
}


//�Ӿ�����
//����4������
void FindBall_vision(u8 ball)
{
	float w = 80;
	float sx, sy;
	float D_X, D_Y;
	u8 time = 1;

	float theta = BasketballRobot.ThetaD, D_theta = 0;

	switch (ball)
	{
	case 1:
		FindVolleyball_red();
		delay_ms(100);
		FindVolleyball_red();
		delay_ms(100);
		FindVolleyball_red();
		break;
	case 2:
		FindVolleyball_yellow();
		delay_ms(100);
		FindVolleyball_yellow();
		delay_ms(100);
		FindVolleyball_yellow();
		break;
	case 3:
		FindBasketball_blue();
		delay_ms(100);
		FindBasketball_blue();
		delay_ms(100);
		FindBasketball_blue();
		break;
	case 4:
		FindBasketball_red();
		delay_ms(100);
		FindBasketball_red();
		delay_ms(100);
		FindBasketball_red();
		break;
	}

	SetPWM(0, 0, 0);

	LCD_Show_pwm();

	//while(receive != 1);
	Vision.RX_STA = 0;
	do
	{
		//�ȴ����ݽ������
		while (((Vision.RX_STA & 0x8000) == 0));

		//����������Ч
		if (!GetVisionData())
		{
			if (time == 0);

			//�������
			else if (time++ < 5)
			{
				//SetPWM(0,0,0);
				continue;
			}
			//������κ���δ�ҵ���
			else if (time != 0)
				time = 0;
		}
		//������Ч
		else
			time = 1;

		//û�ҵ���,��ת
		if (time == 0)
		{
			D_theta = BasketballRobot.ThetaD - theta;
			if ((D_theta >30 && D_theta < 180) || (D_theta < -180 && D_theta > -330))
			{
				w = -80;
			}
			if ((D_theta < -30 && D_theta > -180) || (D_theta > 180 && D_theta < 330))
			{
				w = 80;
			}
			//GetMotorVelocity(0, 0, w);
			SetPWM(w,w,w);

			LCD_Show_pwm();
		}

		//��Ч����
		else if (Vision.Depth > 4000)
		{
			GetMotorVelocity(0, 0, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
#if 1
		else if ((Vision.X < VISION_MID - 30) && Vision.Depth > 1500)
		{
			GetMotorVelocity_Self(15, 300, 0); //ԭ�� -50 10 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if ((Vision.X > VISION_MID + 30) && Vision.Depth > 1500)
		{
			GetMotorVelocity_Self(-15, 300, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if (Vision.Depth > 1300)
		{
			GetMotorVelocity_Self(0, 250, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if ((Vision.X < VISION_MID - 20) && Vision.Depth > 1200)
		{
			GetMotorVelocity_Self(10, 0, 0); //ԭ��-40 0 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if ((Vision.X > VISION_MID + 20) && (Vision.Depth > 1200))
		{
			GetMotorVelocity_Self(-10,200, 0); //ԭ��4 0 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
//		else if (Vision.Depth > 1000&&Vision.Depth<1200)
//		{
//			RobotArm_exit(DOWN);
//			GetMotorVelocity_Self(0, 200, 0);
//			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
//		}
		else if (Vision.Depth > 1000)
		{
			GetMotorVelocity_Self(0, 200, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if (Vision.X < VISION_MID - 30)
		{
			GetMotorVelocity_Self(10, 0, 0); //ԭ��-30 0 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			
		}
		else if (Vision.X > VISION_MID + 30)
		{
			GetMotorVelocity_Self(-10, 0, 0); //ԭ��30 0 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if (Vision.X <= VISION_MID + 30 && Vision.X > VISION_MID + 10)
		{
			//RobotArm_exit(DOWN);
			GetMotorVelocity_Self(-5, 0, 0); //ԭ��15 0 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if (Vision.X >= VISION_MID - 30 && Vision.X < VISION_MID - 10)
		{
			//RobotArm_exit(DOWN);
			GetMotorVelocity_Self(5, 0, 0); //ԭ��-15 0 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
#else
{
		else if ((Vision.X < VISION_MID - 30) && Vision.Depth > 1500)
		{
			GetMotorVelocity_Self(10, 200, 0); //ԭ�� -50 10 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if ((Vision.X > VISION_MID + 30) && Vision.Depth > 1500)
		{
			GetMotorVelocity_Self(-10, 200, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		
		
		else if ((fabs(Vision.X - VISION_MID) >10) && Vision.Depth > 900)
		{
			
			D_X = (VISION_MID - Vision.X)*1.0f;
			D_Y = (Vision.Depth - 900)*1.0f;
			sx = adjustVx_findPD(D_X);
			sy = adjustVy_findPD(D_Y);
			GetMotorVelocity_Self(sx, sy, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if ((fabs(Vision.X - VISION_MID) >10) )
		{
			
			D_X = (VISION_MID - Vision.X)*1.0f/4;
			//D_Y = (Vision.Depth - 800)*1.0f/1000;
			//sx = adjustVx_PD(D_X);
			//sy = adjustVy_PD(D_Y);
			GetMotorVelocity_Self(sx, 0, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
}
#endif
		else
		{
			SetPWM(0, 0, 0);
			//RobotArm_exit(DOWN);			
			//while(LimitSwitchDown == 0);
			Robot_armDown();
			GetMotorVelocity_Self(0,150, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			break;
		}
	} while (1);

	GetInfraredState();

	SetPWM(0, 0, 0);

	//Robot_armUp();
	RobotArm_exit(UP);
	delay_ms(1000);
	LCD_Show_pwm();
}

//���ü�������
//����������ɫ
//����3m����
void FindBall_radar(void)
{
	float w = 100;
	float theta = BasketballRobot.ThetaD, D_theta = 0;

	SetPWM(0, 0, 0);
	LCD_Show_pwm();
	
	Radar.RX_STA = 0;
	
	do
	{
		while((Radar.RX_STA&0x8000) == 0);

		if (!GetRadarData())
		{

			//SetPWM(0, 0, 0);
			continue;
		}
		LED0 = !LED0;

		LCD_ShowString(30 + 200, 460, 200, 16, 16, "Radar:rad");
		LCD_ShowNum(30 + 200 + 48 + 8 + 45, 460, Radar.Angle, 4, 16);
		LCD_ShowString(30 + 200, 480, 200, 16, 16, "Radar:length");
		LCD_ShowNum(30 + 200 + 48 + 8 + 45, 480, Radar.Distance, 4, 16);

		Radar.State = 0;

		//		if(Radar.Distance < 10)
		//			continue;
		if (Radar.Distance > 3000)
		{
			//D_theta = BasketballRobot.ThetaD - theta;
			
			D_theta = BasketballRobot.ThetaD - theta;
			if ((D_theta >30 && D_theta < 180) || (D_theta < -180 && D_theta > -330))
			{
				w = -100;
			}
			if ((D_theta < -30 && D_theta > -180) || (D_theta > 180 && D_theta < 330))
			{
				w = 100;
			}
			//GetMotorVelocity(0, 0, w);
			SetPWM(w,w,w);

			LCD_Show_pwm();
		}
		else if (Radar.Angle < RADAR_MID - 15)
		{
			GetMotorVelocity_Self(-20, 0, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if (Radar.Angle > RADAR_MID + 15)
		{
			GetMotorVelocity_Self(20, 0, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if (Radar.Distance > 1000)
		{
			GetMotorVelocity_Self(0, 300, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if ((Radar.Angle < RADAR_MID - 10) && Radar.Distance > 700)
		{
			GetMotorVelocity_Self(-15, 0, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if ((Radar.Angle > RADAR_MID + 10) && Radar.Distance > 700)
		{
			GetMotorVelocity_Self(15, 0, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if (Radar.Distance > 700)
		{
			GetMotorVelocity_Self(0, 200, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if (Radar.Angle < RADAR_MID - 3)
		{
			GetMotorVelocity_Self(-10, 0, 0); //ԭ��-80 0 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if (Radar.Angle > RADAR_MID + 3)
		{
			GetMotorVelocity_Self(10, 0, 0); //ԭ��80 0 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else
		{
			SetPWM(0, 0, 0);
			Robot_armDown();
			GetMotorVelocity_Self(0, 150, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			if (Radar.Distance < 400)
				break;
		}
		LCD_Show_pwm();
	} while (1);

	//	GetMotorVelocity_Self(0, 100, 0);
	//	SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
	GetInfraredState();

	SetPWM(0, 0, 0);

	Robot_armUp();
	LCD_Show_pwm();
}

//�Ӿ����״�������
//����4m����
void FindBall_VandR(u8 ball)
{

	float w = 80;
	u8 time = 1;
	float theta = BasketballRobot.ThetaD, D_theta = 0;
	
	switch (ball)
	{
	case 1:
		FindVolleyball_red();
		delay_ms(100);
		FindVolleyball_red();
		delay_ms(100);
		FindVolleyball_red();
		break;
	case 2:
		FindVolleyball_yellow();
		delay_ms(100);
		FindVolleyball_yellow();
		delay_ms(100);
		FindVolleyball_yellow();
		break;
	case 3:
		FindBasketball_blue();
		delay_ms(100);
		FindBasketball_blue();
		delay_ms(100);
		FindBasketball_blue();
		break;
	case 4:
		FindBasketball_red();
		delay_ms(100);
		FindBasketball_red();
		delay_ms(100);
		FindBasketball_red();
		break;
	case 5:
		FindBasketball_double();
		delay_ms(100);
		FindBasketball_double();
		delay_ms(100);
		FindBasketball_double();
		break;
	case 6:
		FindFindVolleyball_double();
		delay_ms(100);
		FindFindVolleyball_double();
		delay_ms(100);
		FindFindVolleyball_double();
		break;
	case 7:
		FindFindVolleyball_jindian();
		delay_ms(100);
		FindFindVolleyball_jindian();
		delay_ms(100);
		FindFindVolleyball_jindian();
		break;
	case 8:
		FindFindbasketball_jindian();
		delay_ms(100);
		FindFindbasketball_jindian();
		delay_ms(100);
		FindFindbasketball_jindian();
		break;
		
	}
	SetPWM(0, 0, 0);
	LCD_Show_pwm();

	//����Ӿ���������
	//while (Vision.RX_STA&0x8000);

	Vision.RX_STA = 0;

	//��մ��ڽ������ݻ���
	Radar.RX_STA = 0;

	SetPWM(0, 0, 0);
	LCD_Show_pwm();

	//��ֹ��Ч����
	//while (Radar.RX_STA&0x8000);
	Radar.RX_STA = 0;

	do
	{
		while ((Vision.RX_STA & 0x8000) == 0);
		while ((Radar.RX_STA & 0x8000) == 0);

		if (!GetVisionData())
		{
			if (time == 0)
			{
			}
			else if (time++ < 5)
			{
				SetPWM(0, 0, 0);
				continue;
			}
			else if (time != 0)
				time = 0;
		}
		else
			time = 1;

		GetRadarData();

		//û�ҵ���,��ת
		if (time == 0)
		{
			D_theta = BasketballRobot.ThetaD - theta;
			if ((D_theta >30 && D_theta < 180) || (D_theta < -180 && D_theta > -330))
			{
				w = -100;
			}
			if ((D_theta < -30 && D_theta > -180) || (D_theta > 180 && D_theta < 330))
			{
				w = 100;
			}
			//GetMotorVelocity(0, 0, w);
			SetPWM(w,w,w);

			LCD_Show_pwm();
		}
		
		else if (Vision.Depth > 4000)
		{

			SetPWM(0, 0, 0);
		}
		else if ((Vision.X < VISION_MID - 30) && Vision.Depth > 2000)
		{
			GetMotorVelocity_Self(15, 300, 0); //ԭ��-5 10 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if ((Vision.X > VISION_MID + 30) && Vision.Depth > 2000)
		{
			GetMotorVelocity_Self(-15, 300, 0); //ԭ�� 5 10 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if ((Vision.X < VISION_MID - 20) && Vision.Depth > 1000)
		{
			GetMotorVelocity_Self(8, 300, 0); //ԭ��-5 10 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if ((Vision.X > VISION_MID + 20) && Vision.Depth > 1000)
		{
			GetMotorVelocity_Self(-8, 300, 0); //ԭ�� 5 10 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if (Vision.Depth > 1000)
		{
			GetMotorVelocity_Self(0, 300, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else
			break;
	} while (1);

	if (Radar.Distance > 1000)
	{
		LCD_ShowString(30 + 200, 500, 200, 16, 16, "View!");
		//�״�������ʱ�������Ӿ�����Ѱ��
		while (1)
		{
			while ((Vision.RX_STA & 0x8000) == 0);

			//GetVisionData();
			GetRadarData();

			if (Radar.Distance < 1000)
				break;

			if (!GetVisionData())
				SetPWM(0, 0, 0);

			if (Vision.X < VISION_MID - 20)
			{
				GetMotorVelocity_Self(10, 0, 0); //ԭ��-4 0 0
				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			}
			else if (Vision.X > VISION_MID + 20)
			{
				GetMotorVelocity_Self(-10, 0, 0); //ԭ��4 0 0
				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			}

			else if (Vision.Depth > 1000)
			{
				GetMotorVelocity_Self(0, 200, 0);
				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			}
			else if (Vision.Depth > 700)
			{
				GetMotorVelocity_Self(0, 150, 0);
				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			}

			else if (Vision.X < VISION_MID - 30)
			{
				GetMotorVelocity_Self(10, 0, 0); //ԭ��-3 0 0
				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			}
			else if (Vision.X > VISION_MID + 30)
			{
				GetMotorVelocity_Self(-10, 0, 0); //ԭ�� 3 0 0
				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			}
			else if (Vision.X <= VISION_MID + 30 && Vision.X > VISION_MID + 10)
			{
				GetMotorVelocity_Self(-5, 0, 0); //ԭ�� 1.5 0 0
				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			}
			else if (Vision.X >= VISION_MID - 30 && Vision.X < VISION_MID - 10)
			{
				GetMotorVelocity_Self(5, 0, 0); //ԭ��-1.5 0 0
				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			}
			else
			{
				SetPWM(0, 0, 0);
				Robot_armDown();
				GetMotorVelocity_Self(0, 150, 0);
				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);

				break;
			}
		}
	}

	if (Radar.Distance <= 1000)
	{
		LCD_ShowString(30 + 200, 500, 200, 16, 16, "Radar!");
		//�״������ʣ������״�����Ѱ��
		while (1)
		{
			
			while ((Radar.RX_STA & 0x8000) == 0);

			//GetRadarData();
			GetVisionData();

			if (!GetRadarData())
			{
				SetPWM(0,0,0);
				//GetMotorVelocity_Self(0, 0, 0);
				//SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
				continue;
			}		
			else if(Radar.Distance > 800)
			{
				RobotArm_exit(DOWN);
				GetMotorVelocity_Self(0, 200, 0); //ԭ�� 0 14 0
				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			}

			else if ((Radar.Angle < RADAR_MID - 10) && Radar.Distance > 700)
			{
				GetMotorVelocity_Self(-10, 0, 0); //ԭ��-10 0 0
				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			}
			else if ((Radar.Angle > RADAR_MID + 10) && Radar.Distance > 700)
			{
				GetMotorVelocity_Self(10, 0, 0); //ԭ��10 0 0
				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			}
			else if (Radar.Distance > 700)
			{
				GetMotorVelocity_Self(0, 150, 0); //ԭ�� 0 14 0
				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			}
			else if (Radar.Angle < RADAR_MID - 5)
			{
				GetMotorVelocity_Self(-5, 0, 0); //ԭ��-10 0 0
				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			}
			else if (Radar.Angle > RADAR_MID + 5)
			{
				GetMotorVelocity_Self(5, 0, 0); //ԭ��10 0 0
				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			}
//			else if (Radar.Angle < RADAR_MID - 3)
//			{
//				GetMotorVelocity_Self(-3, 0, 0); //ԭ��-80 0 0
//				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
//			}
//			else if (Radar.Angle > RADAR_MID + 3)
//			{
//				GetMotorVelocity_Self(3, 0, 0); //ԭ��80 0 0
//				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
//			}
			else
			{
				SetPWM(0, 0, 0);
//				if(LimitSwitchUp == 0)
//				{
//					while(LimitSwitchDown == 0);
					Robot_armDown();
					GetMotorVelocity_Self(0,150, 0); //ԭ��0 7 0
					SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
//				}
//				else
//					Robot_armDown();
				if (Radar.Distance < 350)
					break;
			}
		}
	}

	GetInfraredState();
	SetPWM(0, 0, 0);

	//Robot_armUp();
	RobotArm_exit(UP);
	delay_ms(500);
	LCD_Show_pwm();
	LCD_ShowString(30 + 200, 460, 200, 16, 16, "       ");
}

//�״�������
void FindBasketry(void)
{
	float w = 100;
	float theta = BasketballRobot.ThetaD, D_theta = 0;

	SetPWM(0, 0, 0);
	LCD_Show_pwm();
	do
	{
		while ((Radar.RX_STA & 0x8000) == 0);

		if (!GetRadarData())
		{

			SetPWM(0, 0, 0);
			//	continue;
		}
		LED0 = !LED0;

		if (Radar.Distance < 1500)
			continue;
		if (Radar.Distance > 4000)
		{
			D_theta = BasketballRobot.ThetaD - theta;
			
			if ((D_theta >30 && D_theta < 180) || (D_theta < -180 && D_theta > -330))
			{
				w = -100;
			}
			if ((D_theta < -30 && D_theta > -180) || (D_theta > 180 && D_theta < 330))
			{
				w = 100;
			}
			
			
			SetPWM(w,w,w);
		}
		else if (Radar.Distance > 3000 && Radar.Angle < RADAR_MID - 10)
		{ 
			GetMotorVelocity_Self(-20, 200, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if (Radar.Distance > 3000 && Radar.Angle > RADAR_MID + 10)
		{ 
			GetMotorVelocity_Self(20, 200, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if (Radar.Distance > 3000)
		{
			GetMotorVelocity_Self(0, 300, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if (Radar.Angle < RADAR_MID - 10)
		{
			GetMotorVelocity_Self(-20, 0, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if (Radar.Angle > RADAR_MID + 10)
		{
			GetMotorVelocity_Self(20, 0, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		
		else if (Radar.Distance > DIS_RADAR + 100)
		{
			GetMotorVelocity_Self(0, 200, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if (Radar.Distance < DIS_RADAR - 100)
		{
			GetMotorVelocity_Self(0, -200, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if ((Radar.Angle < RADAR_MID - 5)&&(Radar.Distance > DIS_RADAR + 40))
		{
			GetMotorVelocity_Self(-5, 80, 0); //?-����-80 0 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if ((Radar.Angle > RADAR_MID + 5)&&(Radar.Distance > DIS_RADAR + 40))
		{
			GetMotorVelocity_Self(5, 80, 0); //?-����80 0 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if ((Radar.Angle < RADAR_MID - 5)&&(Radar.Distance < DIS_RADAR - 40))
		{
			GetMotorVelocity_Self(-5, -80, 0); //?-����-80 0 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if ((Radar.Angle > RADAR_MID + 5)&&(Radar.Distance < DIS_RADAR - 40))
		{
			GetMotorVelocity_Self(5, -80, 0); //?-����80 0 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if (Radar.Angle < RADAR_MID - 5)
		{
			GetMotorVelocity_Self(-5, 0, 0); //?-����-80 0 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if (Radar.Angle > RADAR_MID + 5)
		{
			GetMotorVelocity_Self(5, 0, 0); //?-����80 0 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		
		else if ((Radar.Angle < RADAR_MID - 5) && (Radar.Angle > RADAR_MID - 2))
		{
			GetMotorVelocity_Self(-3, 0, 0); //?-����-80 0 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if ((Radar.Angle > RADAR_MID + 5) && (Radar.Angle > RADAR_MID + 2))
		{
			GetMotorVelocity_Self(3, 0, 0); //?-����80 0 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if (Radar.Distance > DIS_RADAR + 10)
		{
			GetMotorVelocity_Self(0, 60, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if (Radar.Distance < DIS_RADAR - 10)
		{
			GetMotorVelocity_Self(0, -60, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else
		{
			SetPWM(0, 0, 0);
			Robot_armDown();
			break;
		}
	} while (1);
}

//�Ӿ���λ
void GoBack_Vision(void)
{
	float w = 10;
	
	u16 vision_centre = 360;

	u8 time = 1;

	float x = BasketballRobot.X, D_x = 0;

	//��a���Ӿ����û�λ����
	FindFrame();
	delay_ms(100);
	FindFrame();
	delay_ms(100);
	FindFrame();
	delay_ms(100);

	SetPWM(0, 0, 0);

	LCD_Show_pwm();

	//while(receive != 1);
	Vision.RX_STA = 0;
	do
	{
		//�ȴ����ݽ������
		while ((Vision.RX_STA & 0x8000) == 0);

		//����������Ч
		if (!GetVisionData())
		{
			if (time == 0)
				;

			//�������
			else if (time++ < 10)
			{
				SetPWM(0,0,0);
				continue;
			}
			//������κ���δ�ҵ���
			else if (time != 0)
				time = 0;
		}
		//������Ч
		else{
			time = 1;
		}

		//û�ҵ���,�Լ���λ
		if (time == 0)
		{
			D_x = BasketballRobot.X - x;
			if (D_x  >0.3)
			{
				w = -10;
			}
			if (D_x < -0.3)
			{
				w = 10;
			}
			GetMotorVelocity(w, 0, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);

			LCD_Show_pwm();
		}

		//��Ч����

		else if ((Vision.X < vision_centre - 30) )
		{
			GetMotorVelocity_Self(-10, 0, 0); //ԭ�� -50 10 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if ((Vision.X > vision_centre + 30) )
		{
			GetMotorVelocity_Self(10, 0, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		
		else if ((Vision.X < vision_centre - 20))
		{
			GetMotorVelocity_Self(-8, 0, 0); //ԭ��-40 0 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if ((Vision.X > vision_centre + 20))
		{
			GetMotorVelocity_Self(8, 0, 0); //ԭ��4 0 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		
//		else if (Vision.X < vision_centre - 30)
//		{
//			GetMotorVelocity_Self(3, 0, 0); //ԭ��-30 0 0
//			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
//		}
//		else if (Vision.X > vision_centre + 30)
//		{
//			GetMotorVelocity_Self(-3, 0, 0); //ԭ��30 0 0
//			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
//		}
		else if (Vision.X <= vision_centre + 20 && Vision.X > vision_centre + 10)
		{
			GetMotorVelocity_Self(3, 0, 0); //ԭ��15 0 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if (Vision.X >= vision_centre - 20 && Vision.X < vision_centre - 10)
		{
			GetMotorVelocity_Self(-3, 0, 0); //ԭ��-15 0 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else
		{
			GetMotorVelocity_Self(0, 100, 0); //ԭ��-15 0 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		if(Vision.goBackSign == 'z')
		{
			SetPWM(0,0,0);
			BasketballRobot.X=0;
			RobotGoTo(BasketballRobot.X,BasketballRobot.Y+1.8,BasketballRobot.ThetaD);
			SetPWM(0,0,0);
			break;
		}
	} while (1);
}

void Findball_PD(u8 ball)
{
	float w = 80;
	u8 time = 1;
	float sx, sy;
	float D_X, D_Y;
	float theta = BasketballRobot.ThetaR, D_theta = 0;
	switch (ball)
	{
	case 1:
		FindVolleyball_red();
		delay_ms(100);
		FindVolleyball_red();
		delay_ms(100);
		FindVolleyball_red();
		break;
	case 2:
		FindVolleyball_yellow();
		delay_ms(100);
		FindVolleyball_yellow();
		delay_ms(100);
		FindVolleyball_yellow();
		break;
	case 3:
		FindBasketball_blue();
		delay_ms(100);
		FindBasketball_blue();
		delay_ms(100);
		FindBasketball_blue();
		break;
	case 4:
		FindBasketball_red();
		delay_ms(100);
		FindBasketball_red();
		delay_ms(100);
		FindBasketball_red();
		break;
	}
	SetPWM(0, 0, 0);
	LCD_Show_pwm();

	//����Ӿ���������
	//while (Vision.RX_STA&0x8000);

	Vision.RX_STA = 0;

	//��մ��ڽ������ݻ���
	Radar.RX_STA = 0;

	SetPWM(0, 0, 0);
	LCD_Show_pwm();

	//��ֹ��Ч����
	//while (Radar.RX_STA&0x8000);
	//Radar.RX_STA = 0;

	do
	{
		while ((Vision.RX_STA & 0x8000) != 0 );
		while ((Radar.RX_STA & 0x8000) != 0);

		if (!GetVisionData())
		{
			if (time == 0)
			{
			}
			else if (time++ < 5)
			{
				SetPWM(0, 0, 0);
				continue;
			}
			else if (time != 0)
				time = 0;
		}
		else
			time = 1;

		GetRadarData();

		if (time == 0)
		{
			D_theta = BasketballRobot.ThetaR - theta;
			if ((D_theta > PI / 6.0f && D_theta < PI) || (D_theta < -PI && D_theta > -PI * 11.0f / 6.0f))
				w = -80;
			
			if ((D_theta < -PI / 6.0f && D_theta > -PI) || (D_theta > PI && D_theta < PI * 11.0f / 6.0f))
				w = 80;
			
			GetMotorVelocity(0, 0, w);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			LCD_Show_pwm();
		}
		else if (Vision.Depth > 4000)
		{

			SetPWM(0, 0, 0);
		}
		
		else if ((Vision.X < VISION_MID - 30) && Vision.Depth > 1500)
		{
			GetMotorVelocity_Self(10, 200, 0); //ԭ�� -50 10 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if ((Vision.X > VISION_MID + 30) && Vision.Depth > 1500)
		{
			GetMotorVelocity_Self(-10, 200, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		
		
		else if ((fabs(Vision.X - VISION_MID) >30) && Vision.Depth > 800)
		{
			
			D_X = (VISION_MID - Vision.X);
			D_Y = (Vision.Depth - 800)*1.0f/1000;
			//sx = adjustVx_PD(D_X);
			//sy = adjustVy_PD(D_Y);
			GetMotorVelocity_Self(sx, sy, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if ((fabs(Vision.X - VISION_MID) >30) )
		{
			
			D_X = (VISION_MID - Vision.X);
			//D_Y = (Vision.Depth - 800)*1.0f/1000;
			//sx = adjustVx_PD(D_X);
			//sy = adjustVy_PD(D_Y);
			GetMotorVelocity_Self(sx, 0, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else
			break;
	} while (1);

	if (Radar.Distance > 800)
	{
		LCD_ShowString(30 + 200, 500, 200, 16, 16, "View!");
		//�״�������ʱ�������Ӿ�����Ѱ��
		while (1)
		{
			while ((Vision.RX_STA & 0x8000) != 0 );

			GetVisionData();
			GetRadarData();

			if (Radar.Distance < 800)
				break;

			if (!GetVisionData())
				SetPWM(0, 0, 0);
			if ((fabs(VISION_MID - Vision.X) > 10) && Vision.Depth > 700)
			{

				D_X = (VISION_MID - Vision.X);
				D_Y = (Vision.Depth-700)/1000;
				//sx = adjustVx_PD(D_X);
				//sy = adjustVy_PD(D_Y);
				GetMotorVelocity_Self(sx, sy, 0); //ԭ��-4 0 0
				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			}
			else if (fabs(VISION_MID - Vision.X) > 10)
			{

				D_X = (VISION_MID - Vision.X);
				//D_Y = (Vision.Depth-700)/1000;
				//sx = adjustVx_PD(D_X);
				//sy = adjustVy_PD(D_Y);
				GetMotorVelocity_Self(sx, 0, 0); //ԭ��-4 0 0
				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			}
			
			else
			{
				SetPWM(0, 0, 0);
				Robot_armDown();
				GetMotorVelocity_Self(0, 70, 0);
				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);

				break;
			}
		}
	}

	if (Radar.Distance <= 800)
	{
		LCD_ShowString(30 + 200, 500, 200, 16, 16, "Radar!");
		//�״������ʣ������״�����Ѱ��
		while (1)
		{
			while ((Radar.RX_STA & 0x8000) != 0);

			GetRadarData();
			GetVisionData();

			if (!GetRadarData())
			{
				//GetMotorVelocity_Self(0, 0, 0);
				SetPWM(0,0,0);
				LCD_Show_pwm();
			}

			else if (fabs(Radar.Angle - RADAR_MID)>2 && Radar.Distance > 700)
			{
				D_X = Radar.Angle - RADAR_MID;
				D_Y = (Vision.Depth - 700)/1000;
				//sx = adjustVx_PD(D_X);
				//sy = adjustVy_PD(D_Y);
				GetMotorVelocity_Self(sx, sy, 0);
				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			}
			else if (fabs(Radar.Angle - RADAR_MID)>2 )
			{
				D_X = (Radar.Angle - RADAR_MID);
				//D_Y = (Vision.Depth - 700)/1000;
				//sx = adjustVx_PD(D_X);
				//sy = adjustVy_PD(D_Y);
				GetMotorVelocity_Self(sx, 0, 0);
				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			}

			else
			{
				SetPWM(0, 0, 0);
				Robot_armDown();
				GetMotorVelocity_Self(0, 70, 0); //ԭ��0 7 0
				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);

				if (Radar.Distance < 350)
					break;
			}
		}
	}

	GetInfraredState();
	SetPWM(0, 0, 0);

	Robot_armUp();

	LCD_Show_pwm();
	LCD_ShowString(30 + 200, 460, 200, 16, 16, "       ");
}

static float adjustVy_findPD(float D_Y)
{
	float sy;

	
		sy = D_Y ;

		if (D_Y < 100)
			sy = 100;

		if (sy > 400)
			sy = 400;
	

	
		//sy = D_Y / 2;

//		if (D_Y > -100)
//			sy = D_Y/2 - 100;

		if (sy < -400)
			sy = -400;

	return sy;
}	
static float adjustVx_findPD(float D_X)
{
	float sx, NOW_DX;
	static float Last_DX;
	NOW_DX = D_X;

	sx = NOW_DX + (NOW_DX - Last_DX) + 5;;	

	if (sx > 20)
		sx = 20;
	if (sx < -20)
		sx = -20;

	Last_DX = NOW_DX;
	return sx;	

}	
