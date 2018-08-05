#include "FindBall.h"
#include "control.h"


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

void FindFrame(void)
{
	USART_SendData(USART1, 'a');
}


//视觉找球
//限制4米以内
void FindBall_vision(u8 ball)
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
	}

	SetPWM(0, 0, 0);

	LCD_Show_pwm();

	//while(receive != 1);
	Vision.RX_STA = 0;
	do
	{
		//等待数据接收完成
		while (((Vision.RX_STA & 0x8000) == 0));

		//所得数据无效
		if (!GetVisionData())
		{
			if (time == 0);

			//尝试五次
			else if (time++ < 5)
			{
				//SetPWM(0,0,0);
				continue;
			}
			//尝试五次后仍未找到球
			else if (time != 0)
				time = 0;
		}
		//数据有效
		else
			time = 1;

		//没找到球,自转
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

		//无效数据
		else if (Vision.Depth > 4000)
		{
			GetMotorVelocity(0, 0, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}

		else if ((Vision.X < VISION_MID - 30) && Vision.Depth > 1500)
		{
			GetMotorVelocity_Self(10, 200, 0); //原来 -50 10 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if ((Vision.X > VISION_MID + 30) && Vision.Depth > 1500)
		{
			GetMotorVelocity_Self(-10, 200, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if (Vision.Depth > 1300)
		{
			GetMotorVelocity_Self(0, 20, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if ((Vision.X < VISION_MID - 20) && Vision.Depth > 1000)
		{
			GetMotorVelocity_Self(7, 0, 0); //原来-40 0 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if ((Vision.X > VISION_MID + 20) && (Vision.Depth > 1000))
		{
			GetMotorVelocity_Self(-5, 0, 0); //原来4 0 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if (Vision.Depth > 1000)
		{
			GetMotorVelocity_Self(0, 120, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if (Vision.X < VISION_MID - 30)
		{
			GetMotorVelocity_Self(4, 0, 0); //原来-30 0 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			;
		}
		else if (Vision.X > VISION_MID + 30)
		{
			GetMotorVelocity_Self(-4, 0, 0); //原来30 0 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if (Vision.X <= VISION_MID + 30 && Vision.X > VISION_MID + 10)
		{
			GetMotorVelocity_Self(-3, 0, 0); //原来15 0 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if (Vision.X >= VISION_MID - 30 && Vision.X < VISION_MID - 10)
		{
			GetMotorVelocity_Self(3, 0, 0); //原来-15 0 0
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
	} while (1);

	GetInfraredState();

	SetPWM(0, 0, 0);

	Robot_armUp();
	LCD_Show_pwm();
}

//利用激光找球
//不用区分颜色
//限制3m以内
void FindBall_radar(void)
{
	float w = 100;
	float theta = BasketballRobot.ThetaD, D_theta = 0;

	SetPWM(0, 0, 0);
	LCD_Show_pwm();
	do
	{
		//while(!(Radar.RX_STA&0x8000));

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
			GetMotorVelocity_Self(-10, 0, 0); //原来-80 0 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if (Radar.Angle > RADAR_MID + 3)
		{
			GetMotorVelocity_Self(10, 0, 0); //原来80 0 0
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

//视觉、雷达找球结合
//限制4m以内
void FindBall_VandR(u8 ball)
{

	float w = 200;
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

	//清空视觉串口数据
	//while (Vision.RX_STA&0x8000);

	Vision.RX_STA = 0;

	//清空串口接收数据缓存
	Radar.RX_STA = 0;

	SetPWM(0, 0, 0);
	LCD_Show_pwm();

	//防止无效数据
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

		//没找到球,自转
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
		
		else if (Vision.Depth > 4000)
		{

			SetPWM(0, 0, 0);
		}
		else if ((Vision.X < VISION_MID - 30) && Vision.Depth > 1000)
		{
			GetMotorVelocity_Self(-10, 200, 0); //原来-5 10 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if ((Vision.X > VISION_MID + 30) && Vision.Depth > 1000)
		{
			GetMotorVelocity_Self(10, 200, 0); //原来 5 10 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if (Vision.Depth > 1000)
		{
			GetMotorVelocity_Self(0, 200, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else
			break;
	} while (1);

	if (Radar.Distance > 800)
	{
		LCD_ShowString(30 + 200, 500, 200, 16, 16, "View!");
		//雷达距离过大时，按照视觉数据寻找
		while (1)
		{
			while ((Vision.RX_STA & 0x8000) == 0);

			//GetVisionData();
			GetRadarData();

			if (Radar.Distance < 800)
				break;

			if (!GetVisionData())
				SetPWM(0, 0, 0);

			if (Vision.X < VISION_MID - 20)
			{
				GetMotorVelocity_Self(-6, 0, 0); //原来-4 0 0
				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			}
			else if (Vision.X > VISION_MID + 20)
			{
				GetMotorVelocity_Self(6, 0, 0); //原来4 0 0
				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			}

			else if (Vision.Depth > 700)
			{
				GetMotorVelocity_Self(0, 100, 0);
				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			}

			else if (Vision.X < VISION_MID - 30)
			{
				GetMotorVelocity_Self(-6, 0, 0); //原来-3 0 0
				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			}
			else if (Vision.X > VISION_MID + 30)
			{
				GetMotorVelocity_Self(6, 0, 0); //原来 3 0 0
				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			}
			else if (Vision.X <= VISION_MID + 30 && Vision.X > VISION_MID + 10)
			{
				GetMotorVelocity_Self(3, 0, 0); //原来 1.5 0 0
				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			}
			else if (Vision.X >= VISION_MID - 30 && Vision.X < VISION_MID - 10)
			{
				GetMotorVelocity_Self(-3, 0, 0); //原来-1.5 0 0
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
		//雷达距离合适，按照雷达数据寻找
		while (1)
		{
			while (Radar.RX_STA & 0x8000)
				;

			GetRadarData();
			GetVisionData();

			if (!GetRadarData())
			{
				GetMotorVelocity_Self(0, 0, 0);
				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			}

			else if ((Radar.Angle < RADAR_MID - 10) && Radar.Distance > 700)
			{
				GetMotorVelocity_Self(-10, 0, 0); //原来-10 0 0
				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			}
			else if ((Radar.Angle > RADAR_MID + 10) && Radar.Distance > 700)
			{
				GetMotorVelocity_Self(10, 0, 0); //原来10 0 0
				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			}
			else if (Radar.Distance > 700)
			{
				GetMotorVelocity_Self(0, 150, 0); //原来 0 14 0
				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			}
			else if (Radar.Angle < RADAR_MID - 5)
			{
				GetMotorVelocity_Self(-6, 0, 0); //原来-10 0 0
				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			}
			else if (Radar.Angle > RADAR_MID + 5)
			{
				GetMotorVelocity_Self(6, 0, 0); //原来10 0 0
				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			}
			/*	else if(Radar.Angle <= RADAR_MID+5 && Radar.Angle > RADAR_MID+2)
					{
						GetMotorVelocity_Self(1,0,0);
						SetPWM(BasketballRobot.Velocity[0],BasketballRobot.Velocity[1],BasketballRobot.Velocity[2]);
						 
						 
					}
					else if(Radar.Angle >= RADAR_MID-5 && Radar.Angle < RADAR_MID -2)
					{
						GetMotorVelocity_Self(-1,0,0);
						SetPWM(BasketballRobot.Velocity[0],BasketballRobot.Velocity[1],BasketballRobot.Velocity[2]);
						 
						 
					}
					*/
			else
			{
				SetPWM(0, 0, 0);
				Robot_armDown();
				GetMotorVelocity_Self(0, 70, 0); //原来0 7 0
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

//雷达找篮筐
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
		if (Radar.Distance > 3000)
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
		else if (Radar.Angle < RADAR_MID - 10)
		{
			GetMotorVelocity_Self(10, 0, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if (Radar.Angle > RADAR_MID + 10)
		{
			GetMotorVelocity_Self(-10, 0, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if (Radar.Distance > 3000)
		{
			GetMotorVelocity_Self(0, 20, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if (Radar.Distance > DIS_RADAR + 100)
		{
			GetMotorVelocity_Self(0, 20, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if (Radar.Distance < DIS_RADAR - 100)
		{
			GetMotorVelocity_Self(0, -20, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if (Radar.Angle < RADAR_MID - 5)
		{
			GetMotorVelocity_Self(5, 0, 0); //?-à′-80 0 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if (Radar.Angle > RADAR_MID + 5)
		{
			GetMotorVelocity_Self(-5, 0, 0); //?-à′80 0 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if ((Radar.Angle < RADAR_MID - 5) && (Radar.Angle > RADAR_MID - 2))
		{
			GetMotorVelocity_Self(3, 0, 0); //?-à′-80 0 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if ((Radar.Angle > RADAR_MID + 5) && (Radar.Angle > RADAR_MID + 2))
		{
			GetMotorVelocity_Self(-3, 0, 0); //?-à′80 0 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else
		{
			SetPWM(0, 0, 0);
			//			Robot_armDown();
			break;
		}
	} while (1);
}

//视觉回位
void GoBack_Vision(u8 ball)
{
	float w = 200;

	u8 time = 1;

	float theta = BasketballRobot.ThetaR, D_theta = 0;

	//发a给视觉调用回位代码
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
		//等待数据接收完成
		while (Vision.RX_STA & 0x8000)
			;

		//所得数据无效
		if (!GetVisionData())
		{
			if (time == 0)
				;

			//尝试五次
			else if (time++ < 10)
			{
				//SetPWM(0,0,0);
				continue;
			}
			//尝试五次后仍未找到框
			else if (time != 0)
				time = 0;
		}
		//数据有效
		else{
			time = 1;
		}

		//没找到框,自转
		if (time == 0)
		{
			RobotGoTo(0,0,0);
		}

		//无效数据

		else if ((Vision.X < VISION_MID - 30) )
		{
			GetMotorVelocity_Self(5, 0, 0); //原来 -50 10 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if ((Vision.X > VISION_MID + 30) )
		{
			GetMotorVelocity_Self(-5, 0, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		
		else if ((Vision.X < VISION_MID - 20))
		{
			GetMotorVelocity_Self(4, 0, 0); //原来-40 0 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if ((Vision.X > VISION_MID + 20))
		{
			GetMotorVelocity_Self(-4, 0, 0); //原来4 0 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		
		else if (Vision.X < VISION_MID - 30)
		{
			GetMotorVelocity_Self(3, 0, 0); //原来-30 0 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			;
		}
		else if (Vision.X > VISION_MID + 30)
		{
			GetMotorVelocity_Self(-3, 0, 0); //原来30 0 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if (Vision.X <= VISION_MID + 30 && Vision.X > VISION_MID + 10)
		{
			GetMotorVelocity_Self(-1.5, 0, 0); //原来15 0 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else if (Vision.X >= VISION_MID - 30 && Vision.X < VISION_MID - 10)
		{
			GetMotorVelocity_Self(1.5, 0, 0); //原来-15 0 0
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else
		{
			BasketballRobot.X=0;
			SetPWM(0, 0, 0);
			RobotGoTo(0,0,180);
			break;
		}
	} while (1);
}

void Findball_PD(u8 ball)
{
	float w = 200;
	u8 time = 1;
	float sx, sy;
	float D_X, D_Y;
	float theta = BasketballRobot.ThetaR, D_theta = 0;
	switch (ball)
	{
//	case 1:
//		FindBasketball();
//		delay_ms(10000);
//		FindBasketball();
//		delay_ms(10000);
//		FindBasketball();
//		break;
//	case 3:
//		FindVolleyball();

//		delay_ms(10000);

//		FindVolleyball();
//		delay_ms(10000);
//		FindVolleyball();

//		break;
	}
	SetPWM(0, 0, 0);
	LCD_Show_pwm();

	//清空视觉串口数据
	//while (Vision.RX_STA&0x8000);

	Vision.RX_STA = 0;

	//清空串口接收数据缓存
	Radar.RX_STA = 0;

	SetPWM(0, 0, 0);
	LCD_Show_pwm();

	//防止无效数据
	//while (Radar.RX_STA&0x8000);
	Radar.RX_STA = 0;

	do
	{
		while (Vision.RX_STA & 0x8000)
			;
		while (Radar.RX_STA & 0x8000)
			;

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
			{
				w = -200;
			}
			if ((D_theta < -PI / 6.0f && D_theta > -PI) || (D_theta > PI && D_theta < PI * 11.0f / 6.0f))
			{
				w = 200;
			}
			GetMotorVelocity(0, 0, w);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			LCD_Show_pwm();
		}
		else if (Vision.Depth > 4000)
		{

			SetPWM(0, 0, 0);
		}
		else if ((Vision.X < VISION_MID - 30) && Vision.Depth > 1000)
		{
			//			GetMotorVelocity_Self(-10, 200, 0); //原来-5 10 0
			//			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			//		}
			//		else if ((Vision.X > VISION_MID + 30) && Vision.Depth > 1000)
			//		{
			//			GetMotorVelocity_Self(10, 200, 0); //原来 5 10 0
			//			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			//		}
			//		else if (Vision.Depth > 1000)
			//		{
			//			GetMotorVelocity_Self(0, 200, 0);
			//			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			D_X = (VISION_MID - Vision.X);
			D_Y = Vision.Depth;
			sx = adjustVx_PD(D_X);
			sy = adjustVy_PD(D_Y);
			GetMotorVelocity_Self(sx, sy, 0);
			SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
		}
		else
			break;
	} while (1);

	if (Radar.Distance > 800)
	{
		LCD_ShowString(30 + 200, 500, 200, 16, 16, "View!");
		//雷达距离过大时，按照视觉数据寻找
		while (1)
		{
			while (Vision.RX_STA & 0x8000)
				;

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
				sx = adjustVx_PD(D_X);
				sy = adjustVy_PD(D_Y);
				GetMotorVelocity_Self(sx, sy, 0); //原来-4 0 0
				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			}
			//			if (Vision.X < VISION_MID - 20)
			//			{
			//				GetMotorVelocity_Self(-6, 0, 0); //原来-4 0 0
			//				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			//			}
			//			else if (Vision.X > VISION_MID + 20)
			//			{
			//				GetMotorVelocity_Self(6, 0, 0); //原来4 0 0
			//				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			//			}

			//			else if (Vision.Depth > 700)
			//			{
			//				GetMotorVelocity_Self(0, 100, 0);
			//				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			//			}

			//			else if (Vision.X < VISION_MID - 30)
			//			{
			//				GetMotorVelocity_Self(-6, 0, 0); //原来-3 0 0
			//				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			//			}
			//			else if (Vision.X > VISION_MID + 30)
			//			{
			//				GetMotorVelocity_Self(6, 0, 0); //原来 3 0 0
			//				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			//			}
			//			else if (Vision.X <= VISION_MID + 30 && Vision.X > VISION_MID + 10)
			//			{
			//				GetMotorVelocity_Self(3, 0, 0); //原来 1.5 0 0
			//				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			//			}
			//			else if (Vision.X >= VISION_MID - 30 && Vision.X < VISION_MID - 10)
			//			{
			//				GetMotorVelocity_Self(-3, 0, 0); //原来-1.5 0 0
			//				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			//			}
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
		//雷达距离合适，按照雷达数据寻找
		while (1)
		{
			while (Radar.RX_STA & 0x8000)
				;

			GetRadarData();
			GetVisionData();

			if (!GetRadarData())
			{
				GetMotorVelocity_Self(0, 0, 0);
				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			}

			else if ((Radar.Angle < RADAR_MID - 10) && Radar.Distance > 700)
			{
				D_X = (VISION_MID - Vision.X);
				D_Y = Vision.Depth;
				sx = adjustVx_PD(D_X);
				sy = adjustVy_PD(D_Y);
				GetMotorVelocity_Self(sx, sy, 0);
				SetPWM(BasketballRobot.Velocity[0], BasketballRobot.Velocity[1], BasketballRobot.Velocity[2]);
			}
			/*	else if(Radar.Angle <= RADAR_MID+5 && Radar.Angle > RADAR_MID+2)
					{
						GetMotorVelocity_Self(1,0,0);
						SetPWM(BasketballRobot.Velocity[0],BasketballRobot.Velocity[1],BasketballRobot.Velocity[2]);
						 
						 
					}
					else if(Radar.Angle >= RADAR_MID-5 && Radar.Angle < RADAR_MID -2)
					{
						GetMotorVelocity_Self(-1,0,0);
						SetPWM(BasketballRobot.Velocity[0],BasketballRobot.Velocity[1],BasketballRobot.Velocity[2]);
						 
						 
					}
					*/
			else
			{
				SetPWM(0, 0, 0);
				Robot_armDown();
				GetMotorVelocity_Self(0, 70, 0); //原来0 7 0
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
