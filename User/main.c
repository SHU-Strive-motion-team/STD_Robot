#include "stm32f4xx.h"
#include "pwm.h"
#include "remote.h"
#include "EXIT.h"
#include "control.h"
#include "encoder.h"
#include "FindBall.h"

u8 zhongquan_case;
u8 changdi;
u8 chengxu;
u8 sanfen_case;

int main(void)
{
	u8 key = 0; //按键值
	//u8 chengxu = 0;				//程序选择
	u8 flag = 0;
	u8 qiu = 0;			 //找球
//	int16_t time = 0;			//延时
	u8 findballtime = 0; //找球时调整角度次数
	u8 i;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置系统中断优先级分组2   2位抢占优先 2位响应优先
	zhongquan_case = 10;
	changdi = 1; //左场右场
	chengxu = 0;

	delay_init(168);   //初始化延时函数
	UART_Init(115200); //串口初始化为9600

	ALL_GPIO_Init();
	Remote_Init();
	PWM_Init();		  //初始化PWM发生器
	LCD_Init();		  //LCD初始化
	LCD_Show_Title(); //液晶屏显示内容初始化

	Control_Init(); //机器人初始化
	Encoder_Init();
	TIM5_Init(10000 - 1, 83);
	EXTIX_Init();
	
	while (1)
	{

		key = Remote_Scan();
		flag = 0;
		key = 0;
		chengxu = 0;
		changdi = 0;
		//选择程序
		while (1)
		{
			//			printf("%x  \r\n",huart2.RxState);
			//			printf("errr%x  \n",huart2.ErrorCode);
			//			delay_ms(100);
			LCD_ShowString(30 + 200, 400, 200, 16, 16, "chengxu:");
			key = Remote_Scan();
			//key = KEY_Scan(0);
			switch (key)
			{
			case 0: //没有按键按下

				break;
			case KEY_RIGHT: //右
				LCD_ShowString(30 + 200, 400, 200, 16, 16, "qiu:    ");
				flag = 1;
				break;
			case KEY_DOWN: //下
				LCD_ShowString(30 + 200, 400, 200, 16, 16, "chengxu-");
				if (chengxu != 0)
					chengxu--;
				break;
			case KEY_LEFT: //左
				LCD_ShowString(30 + 200, 400, 200, 16, 16, "clear   ");
				chengxu = 0;
				break;
			case KEY_UP: //上
				LCD_ShowString(30 + 200, 400, 200, 16, 16, "chengxu+");
				chengxu++;
				break;
			}

			LCD_ShowNum(30 + 200 + 48 + 8 + 10, 320, chengxu, 4, 16);

			if (flag)
				break;
		}

		flag = 0;
		key = 0;

		//选择球
		while (1)
		{
			key = Remote_Scan();
			//key = KEY_Scan(0);
			switch (key)
			{
			case 0: //没有按键按下

				break;
			case KEY_RIGHT: //右
				LCD_ShowString(30 + 200, 400, 200, 16, 16, "changdi   ");
				flag = 1;
				break;
			case KEY_DOWN: //下
				LCD_ShowString(30 + 200, 400, 200, 16, 16, "qiu-");
				if (qiu != 0)
					qiu--;
				break;
			case KEY_LEFT: //左
				LCD_ShowString(30 + 200, 400, 200, 16, 16, "clear   ");
				qiu = 0;
				break;
			case KEY_UP: //上
				LCD_ShowString(30 + 200, 400, 200, 16, 16, "qiu+");
				qiu++;
				break;
			}

			LCD_ShowNum(30 + 200 + 48 + 8 + 10, 340, qiu, 4, 16);

			if (flag)
				break;
		}

		flag = 0;
		key = 0;
		//选择场地
		while (1)
		{
			key = Remote_Scan();
			switch (key)
			{
			case 0: //没有按键按下
				break;
			case KEY_RIGHT: //右
				LCD_ShowString(30 + 200, 400, 200, 16, 16, "start   ");
				flag = 1;
				break;
			case KEY_DOWN: //下
				LCD_ShowString(30 + 200, 400, 200, 16, 16, "changdi-");
				if (changdi != 0)
					changdi--;
				break;
			case KEY_LEFT: //左
				LCD_ShowString(30 + 200, 400, 200, 16, 16, "clear   ");
				changdi = 0;
				break;
			case KEY_UP: //上
				LCD_ShowString(30 + 200, 400, 200, 16, 16, "changdi+");
				changdi++;
				break;
			case KEY_POWER:
				flag = 1;
				chengxu = 99;
				break;
			}

			LCD_ShowNum(30 + 200 + 48 + 8 + 10, 360, changdi, 4, 16);

			if (flag)
				break;
		}

		switch (chengxu)
		{
		case 0: //测试程序
			switch (qiu)
			{
			case 0:
				RobotRotate(180);
				//printf("err\r\n");
				//顺时针180°
				break;
			case 1:
				//机械臂下降
				Robot_armDown();
				//RobotArm_exit(DOWN);
				//BEEP = 1;
				delay_ms(2000);
				BEEP = 0;
				LED0 = !LED0;
				break;
			case 2:
				//机械臂上升
				//Robot_armUp();
				RobotArm_exit(UP);
				//LED0 = !LED0;
				//BEEP = 1;
				delay_ms(2000);
				BEEP = 0;
				//						TIM_SetCompare2(TIM9,0);
				//						TIM_SetCompare1(TIM9,1500+1000);
				break;
			case 3:
				//红外测试
				//GetInfraredState();
				while(1)
				{
					if(LimitSwitchUp ==1)
						BEEP = 1;
					else
						BEEP = 0;
				}
				BEEP = 1;
				delay_ms(2000);
				BEEP = 0;
				break;
			case 4:
				//弹射测试
				while (1)
				{
					key = Remote_Scan();
					switch (key)
					{
					case KEY_1:
						CHARGE = 1;
						break;

					case KEY_2:
						CHARGE = 0;
						break;

					case KEY_3:
						CHARGE = 0;
						delay_ms(1000);
						SHOT = 1;
						delay_ms(1000);
						SHOT = 0;

						break;
					}
				}
				break;

			case 5:
				//避障测试
				FindBall_radar();
				BEEP = 1;
				delay_ms(2000);
				BEEP = 0;

				//					while(1)
				//					{
				//					GetRadarData();
				//					}
				LED0 = !LED0;
				break;
			case 6:
				//视觉测试
				FindBall_VandR(changdi);
				RobotGoTo(BasketballRobot.X,BasketballRobot.Y-0.5f,BasketballRobot.ThetaD);
				LED0 = !LED0;
				break;
			case 7:
				RobotGoTo(-9.0f,3.0f,77);
				FindBasketry();
				BEEP = 1;
				delay_ms(500);
				BEEP = 0;
				break;
			case 8:
//				GoBack_Vision();
				RobotGoBrokenLine(1,1,0,0,1,0);
				SetPWM(0,0,0);
				break;
			case 9:
				FindBall_VandR(5);
				RobotRotate(270);
				delay_ms(3000);
				RobotGoAvoidance();
				break;
			case 10:
				FindBall_vision(changdi);
				break;
			}
			break;
			
		case 1: //传球第一回合
			switch (changdi)
			{
			case 1: //下场
				//延时10s
				CHARGE=1;
				for (i = 0; i < 5; i++)
				{
					SetPWM(0, 0, 0);
					delay_ms(1000);
				}
				SetPWM(-600,0,600);
				delay_ms(1000);
				//RobotArm_exit(DOWN);
				RobotGoTo(1, 2.5f, 245);
				//while(LimitSwitchDown ==0);
				Robot_armDown();
				while (ShotUp());
				RobotArm_exit(UP);
				//while(LimitSwitchUp==0);
				CHARGE=1;
				RobotRotate(-30);
			
				FindBall_VandR_middle(5);
 
				//判断是否找到球，如果没有
//				if (findballtime == 0)
//				{ //没找到球
//					RobotGoTo(-3 - Correction_X, 2 + Correction_Y, 0);
//					FindBall_VandR(qiu);
//				}

				RobotGoTo(1, 2.5f, 240);
				while(LimitSwitchUp == 0);
				Robot_armDown();
				while (ShotUp());
				RobotArm_exit(UP);
				RobotGoTo(0, 1 ,180);
				RobotGoTo(0,0,180);
				//加视觉找框回位
				break;
			case 0: //上场
				//延时10s
				CHARGE=1;
				for (i = 0; i <5; i++)
				{
					SetPWM(0, 0, 0);
					delay_ms(1000);
				}
				SetPWM(-600,0,600);
				delay_ms(1000);
				//RobotArm_exit(DOWN);
				RobotGoTo(-1.0f, 2.5f, 115);
				Robot_armDown();
				//while(LimitSwitchDown==0);
				while (ShotUp());
				//delay_ms(5000);
				RobotArm_exit(UP);
				CHARGE=1;
//				RobotRotate(0);
				//SetPWM(-100,-100,-100);
				RobotRotate(-45);
				FindBall_VandR_middle(5);

				//判断是否找到球，如果没有
//				if (findballtime == 0)
//				{ //没找到球
//					RobotGoTo(-3 - Correction_X, 2 + Correction_Y, 0);
//					FindBall_VandR(qiu);
//				}

				RobotGoTo(-1.0f, 2.5f , 115);
				while(LimitSwitchUp == 0);
				Robot_armDown();
				while (ShotUp());
				RobotArm_exit(UP);
				RobotGoTo(0, 1 ,180);
				RobotGoTo(0,0,180);
				break;
			}
			break;
		case 2: //传球第二回合
			switch (changdi)
			{
			case 0: //上场
				CHARGE=1;
				for (i = 0; i < 12; i++)
				{
					SetPWM(0, 0, 0);
					delay_ms(1000);
				}
				
				SetPWM(-600,0,600);
				delay_ms(1000);
				RobotGoTo(-1, 2.5 ,-45);
				
				FindBall_VandR_middle(5);
//				//判断是否找到球，如果没有
//				if (findballtime == 0)
//				{ //没找到球
//					RobotGoTo(3 + Correction_X, 2 + Correction_Y, 0);
//					FindBall_VandR(qiu);
//				}
				
				RobotGoTo(-1, 2.5 ,120);
				while(LimitSwitchUp == 0);
				Robot_armDown();
				//while(LimitSwitchDown == 0);
//				delay_ms(2000);
				while (ShotUp());
				RobotArm_exit(UP);
				CHARGE=1;
				RobotGoTo(-4.3f, 2.5f , 70);
				FindBall_VandR(5);
				RobotGoTo(-1, 2.5f ,120);
				while(LimitSwitchUp==0);
				Robot_armDown();
				//delay_ms(2000);
				while(ShotUp());
				RobotArm_exit(UP);
				RobotGoTo(0 , 1 , 180);
				RobotGoTo(0 , 0 , 180);

				//加视觉找框回位
				break;
				
				
			case 1: //下场
				CHARGE=1;
				for (i = 0; i < 5; i++)
				{
					SetPWM(0, 0, 0);
					delay_ms(1000);
				}
				SetPWM(-600,0,600);
				delay_ms(1000);
				RobotGoTo(1, 2.5 ,-30);
				
				FindBall_VandR(5);
//				//判断是否找到球，如果没有
//				if (findballtime == 0)
//				{ //没找到球
//					RobotGoTo(3 + Correction_X, 2 + Correction_Y, 0);
//					FindBall_VandR(qiu);
//				}
				
				RobotGoTo(1, 2.5 ,245);
				while(LimitSwitchUp == 0);
				Robot_armDown();
				//while(LimitSwitchDown == 0);
				while (ShotUp());
				RobotArm_exit(UP);
				CHARGE=1;
				RobotGoTo(4.3f, 2.5f , 270);
				FindBall_VandR(5);
				RobotGoTo(1, 2.5f ,243);
				while(LimitSwitchUp==0);
				Robot_armDown();
				//delay_ms(2000);
				while(ShotUp());
				RobotArm_exit(UP);
				RobotGoTo(0 , 1 , 180);
				RobotGoTo(0 , 0 , 180);

				//加视觉找框回位
				break;
			}
			break;
			
		case 3: //传球第三回合
			switch (changdi)
			{
			case 0: //上场
				CHARGE =1;
				for (i = 0; i < 5; i++)
				{
				SetPWM(0, 0, 0);
					delay_ms(1000);
				}
				SetPWM(-600,0,600);
				delay_ms(1000);
				RobotGoTo(-4.3f, 2.5f, 90);
				FindBall_VandR(7);

				//记录铲球点A的位置
				BasketballRobot.PX = BasketballRobot.X;
				BasketballRobot.PY = BasketballRobot.Y;
//				RobotGoTo(-5.3f, BasketballRobot.PY,-160);
//				RobotGoTo(-5.3f, 3.0f,-160);
				RobotGoBrokenLine(-5.3f,3.5f,-160,-5.3,BasketballRobot.PY,-160);
				while(LimitSwitchUp==0);
				Robot_armDown();
				//delay_ms(2000);
				//RobotArm_exit(UP);
				while(ShotUp());
				RobotArm_exit(UP);
				CHARGE = 1;
//				//原路返回
//				RobotGoTo(-5.3 , BasketballRobot.PY , 90);
//				RobotGoTo(BasketballRobot.X+0.2f , BasketballRobot.PY ,90);
				if(BasketballRobot.PY <2.5f)
					RobotGoBrokenLine(BasketballRobot.PX-0.2f,BasketballRobot.PY,60,-5.3f,BasketballRobot.PY,60);
				else
					RobotGoBrokenLine(BasketballRobot.PX-0.2f,BasketballRobot.PY,90,-5.3f,BasketballRobot.PY,90);
				
				FindBall_VandR(5);
//				FindBall_radar();

				//原路返回
				RobotGoTo(BasketballRobot.PX+0.2f, BasketballRobot.PY , 90);
//				RobotGoTo(-5.3f, BasketballRobot.PY,-160);
//				RobotGoTo(-5.3f, 3.0f,-160);
				RobotGoBrokenLine(-5.3f,3,-160,-5.3,BasketballRobot.PY,-160);
				while(LimitSwitchUp==0);
				Robot_armDown();
				
				while(ShotUp());
				RobotArm_exit(UP);
				
				RobotGoTo(0 , 1 , 0);
				RobotGoTo(0 , 0 , 0);

				//加视觉找框回位
				break;
				
			case 1: //下场
				CHARGE = 1;
				for (i = 0; i < 5; i++)
				{
					SetPWM(0, 0, 0);
					delay_ms(1000);
				}
				
				SetPWM(-600,0,600);
				delay_ms(1000);
				RobotGoTo(4.3f, 2.5f, 260);
				FindBall_VandR(7);

				//记录铲球点A的位置
				BasketballRobot.PX = BasketballRobot.X;
				BasketballRobot.PY = BasketballRobot.Y;
//				RobotGoTo(5.3f, BasketballRobot.PY,160);
//				RobotGoTo(5.3f, 3.0f,160);
				RobotGoBrokenLine(5.3f,3.5,160,5.3f,BasketballRobot.PY,160);
				while(LimitSwitchUp==0);
				Robot_armDown();
				while(ShotUp());
				RobotArm_exit(UP);
				
				CHARGE = 1;
//				//原路返回
//				RobotGoTo(5.3 , BasketballRobot.PY , 270);
//				RobotGoTo(BasketballRobot.X+0.2f , BasketballRobot.PY , 270);
				if(BasketballRobot.PY<2.5f)
					RobotGoBrokenLine(BasketballRobot.PX+0.2f,BasketballRobot.PY,300,5.3f,BasketballRobot.PY,300);
				else
					RobotGoBrokenLine(BasketballRobot.PX+0.2f,BasketballRobot.PY,270,5.3f,BasketballRobot.PY,270);
				
				FindBall_VandR(5);
//				FindBall_radar();

				//原路返回
				RobotGoTo(BasketballRobot.PX+0.2f, BasketballRobot.PY , 270);
//				RobotGoTo(5.3f, BasketballRobot.PY,160);
//				RobotGoTo(5.3f, 3.0f,160);
				RobotGoBrokenLine(5.3f,3.5,160,5.3f,BasketballRobot.PY,160);
//				RobotGoTo(1.5,2.5,250);
				while(LimitSwitchUp==0);
				Robot_armDown();
				while(ShotUp());
				RobotArm_exit(UP);
				
				RobotGoBrokenLine(0.2,2,180,3,3,180);
				
			//	RobotGoTo(0 , 1 , 0);
				

				break;
			}
			break;
			
		case 4: //投篮第一回合
			switch (changdi)
			{
			case 0: //上场
				CHARGE=1;
				for (i = 0; i < 5; i++)
				{
					SetPWM(0, 0, 0);
					delay_ms(1000);
				}
				SetPWM(-600,0,600);
				delay_ms(2000);
				
				RobotGoTo(-9.0f, 3.0f,77);

				//雷达找框
				FindBasketry();

				while (ShotUp());
				RobotArm_exit(UP);
				CHARGE=1;
				RobotGoTo(-1.0f, 2.5f, 30);
				FindBall_VandR_middle(6);

				//判断是否找到球，如果没有
//				if (findballtime == 0)
//				{ //没找到球
//					RobotGoTo(3 + Correction_X, 2 + Correction_Y, 0);
//					FindBall_VandR(qiu);
//				}

				RobotGoTo(-9.0f, 3.0f,77);
//				RobotGoTo(-4,3.6,-270);
				//雷达找框
				FindBasketry();

				while(ShotUp());
				//RobotGoTo(-9.0f, 2.0f,73);
				SetPWM(0,0,0);
				SetPWM(0,0,0);
				
				break;
				
			case 1: //下场
				CHARGE=1;
				for (i = 0; i < 5; i++)
				{
					SetPWM(0, 0, 0);
					delay_ms(1000);
				}
				SetPWM(-600,0,600);
				delay_ms(2000);
				
				RobotGoTo(9.0f, 3.0f,-77);

				//雷达找框
				FindBasketry();

				while (ShotUp());
				
				RobotArm_exit(UP);
				CHARGE=1;
				RobotGoTo(1.0f, 2.5f, 30);
				FindBall_VandR(6);

				//判断是否找到球，如果没有
//				if (findballtime == 0)
//				{ //没找到球
//					RobotGoTo(3 + Correction_X, 2 + Correction_Y, 0);
//					FindBall_VandR(qiu);
//				}

				RobotGoTo(9.0f, 3.0f,-77);
//				RobotGoTo(4,3.6,270);
				//雷达找框
				FindBasketry();

				while(ShotUp());
				RobotGoTo(9.0f, 2.0f,-73);
				SetPWM(0,0,0);
				SetPWM(0,0,0);
				break;
				
			}
			break;
		case 5: //投篮第二回合
			switch (changdi)
			{
			case 0: //上场
				CHARGE=1;
				for (i = 0; i < 5; i++)
				{
					SetPWM(0, 0, 0);
					delay_ms(1000);
				}
				
				SetPWM(-600,0,600);
				delay_ms(2000);
				RobotGoTo(-4.3f, 2.5f,70);
				FindBall_VandR(8);

				//记录铲球点A的位置
				BasketballRobot.PX = BasketballRobot.X;
				BasketballRobot.PY = BasketballRobot.Y;

					
				
				RobotGoTo(-9.0f, 3.0f,77);
//				RobotGoTo(4.3f, 3.6f, 270);

				//雷达找框
				FindBasketry();

				while(ShotUp());
				RobotArm_exit(UP);
				CHARGE=1;
				//RobotGoTo(A);
				RobotGoTo(BasketballRobot.PX , BasketballRobot.PY , 70);

				RobotGoTo(-1.0f, 2.5f, 30);
				FindBall_VandR_middle(6);

//				//判断是否找到球，如果没有
//				if (findballtime == 0)
//				{ //没找到球
//					RobotGoTo(3 + Correction_X, 2 + Correction_Y, 0);
//					FindBall_VandR(qiu);
//				}

				RobotGoTo(BasketballRobot.PX, BasketballRobot.PY, 77);
				RobotGoTo(-9.0f, 3.0f,77);

				//雷达找框
				FindBasketry();

				while(ShotUp());
//				RobotGoTo(-9.0f, 2.0f,77);
				SetPWM(0,0,0);
				SetPWM(0,0,0);
				SetPWM(0,0,0);
				break;
			case 1: //下场
				CHARGE=1;
				for (i = 0; i < 5; i++)
				{
					SetPWM(0, 0, 0);
					delay_ms(1000);
				}
				SetPWM(-600,0,600);
				delay_ms(2000);
				RobotGoTo(4.3f, 2.5f, 290);
				FindBall_VandR(8);

				//记录铲球点A的位置
				BasketballRobot.PX = BasketballRobot.X;
				BasketballRobot.PY = BasketballRobot.Y;

				RobotGoTo(9.0f, 3.0f,-77);
//				RobotGoTo(4.3f, 3.6f, 270);

				//雷达找框
				FindBasketry();

				while(ShotUp());
				
				RobotArm_exit(UP);
				CHARGE=1;
				//RobotGoTo(A);
				RobotGoTo(BasketballRobot.PX , BasketballRobot.PY , 260);
				
				RobotGoTo(5,3,30);
				RobotGoTo(1.5f, 3, 30);
				FindBall_VandR(6);

//				//判断是否找到球，如果没有
//				if (findballtime == 0)
//				{ //没找到球
//					RobotGoTo(3 + Correction_X, 2 + Correction_Y, 0);
//					FindBall_VandR(qiu);
//				}
				RobotGoTo(5,3,30);
				RobotGoTo(BasketballRobot.PX, BasketballRobot.PY, 0);
				RobotGoTo(9.0f, 2.0f,-77);

				//雷达找框
				FindBasketry();

				while(ShotUp());
				RobotGoTo(9.0f, 2.0f,-77);
				SetPWM(0,0,0);
				break;
			}
			break;
		case 6: //投篮第三回合
			switch (changdi)
			{
			case 0: //上场
				CHARGE=1;
				for (i = 0; i < 5; i++)
				{
					SetPWM(0, 0, 0);
					delay_ms(1000);
				}
				SetPWM(-400,0,400);
				delay_ms(2000);
				RobotGoTo(-4.3f, 2.5f, 70);
				FindBall_VandR(8);
				RobotRotate(90);
				while(LimitSwitchUp==0);
				RobotGoAvoidance();
				RobotGoTo(-9.0f, 3.0f,77);
//				RobotGoTo(4.3f, 3.6f, 270);

				//雷达找框
				FindBasketry();
				while (ShotUp());
				RobotArm_exit(UP);
				CHARGE=1;
			
				RobotGoTo(BasketballRobot.X-1, 3.0f,260);
				FindBall_VandR(8);
				
				//雷达找框
				RobotGoTo(-9.0f, 3.0f,77);
				FindBasketry();
				while(ShotUp());
				RobotGoTo(-9.0f, 2.0f,77);
				SetPWM(0,0,0);
				break;
			case 1: //下场
				CHARGE=1;
				for (i = 0; i < 5; i++)
				{
					SetPWM(0, 0, 0);
					delay_ms(1000);
				}
				SetPWM(-600,0,600);
				delay_ms(2000);
				
				RobotGoTo(4.3f, 2.5f, 290);
				FindBall_VandR(8);
				RobotRotate(270);
				while(LimitSwitchUp==0);
				
				RobotGoAvoidance();

				RobotGoTo(9.0f, 3.0f,-77);
//				RobotGoTo(4.3f, 3.6f, 270);

				//雷达找框
				FindBasketry();
				while (ShotUp());

				RobotArm_exit(UP);
				CHARGE=1;
				RobotGoTo(BasketballRobot.X+1, 3.0f,100);
				FindBall_VandR(6);
				
				RobotGoTo(9.0f,3.0f,-77);
				//雷达找框
				FindBasketry();
				while(ShotUp());
				RobotGoTo(-9.0f, 2.0f,-77);
				SetPWM(0,0,0);
				SetPWM(0,0,0);
				break;
			}
			break;
		}
	}
}
