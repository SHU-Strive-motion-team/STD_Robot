#include "stm32f4xx.h"
#include "pwm.h"
#include "remote.h"
#include "EXIT.h"
#include "control.h"
#include "encoder.h"


u8 zhongquan_case;
u8 changdi;
u8 chengxu;
u8 sanfen_case;

int main(void)
{
	u8 key = 0;					//按键值
	//u8 chengxu = 0;				//程序选择
	u8 flag=0;
	u8 qiu = 0;				//找球
	int16_t time = 0;			//延时
	u8 findballtime = 0;			//找球时调整角度次数
	u8 i;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置系统中断优先级分组2   2位抢占优先 2位响应优先
	zhongquan_case=10;
	changdi=1;					//左场右场
	chengxu=0;

	delay_init(168);  			//初始化延时函数
	UART_Init(9600);	 		//串口初始化为9600

	ALL_GPIO_Init();
	Remote_Init();
	PWM_Init();				//初始化PWM发生器
	LCD_Init();					//LCD初始化
	LCD_Show_Title();			//液晶屏显示内容初始化
	
	Control_Init();				//机器人初始化
	Encoder_Init();
	TIM5_Init(10000-1,83);
	EXTIX_Init();
	
while (1)
  {
	  
	  
	    key = Remote_Scan();
		flag = 0;
		key = 0;
		chengxu = 0;
	    changdi = 0;
		//选择程序
		while(1)
		{
//			printf("%x  \r\n",huart2.RxState);
//			printf("errr%x  \n",huart2.ErrorCode);
//			delay_ms(100);
			LCD_ShowString(30+200,400,200,16,16,"chengxu:");
			key = Remote_Scan();
			//key = KEY_Scan(0);
			switch(key)
			{
				case 0:		//没有按键按下
					
					break;
				case KEY_RIGHT:		//右
					LCD_ShowString(30+200,400,200,16,16,"qiu:    ");
					flag = 1;
					break;
				case KEY_DOWN:		//下
					LCD_ShowString(30+200,400,200,16,16,"chengxu-");
					if(chengxu != 0)
						chengxu--;
					break;
				case KEY_LEFT:		//左
					LCD_ShowString(30+200,400,200,16,16,"clear   ");
					chengxu = 0;
					break;
				case KEY_UP:		//上
					LCD_ShowString(30+200,400,200,16,16,"chengxu+");
					chengxu++;
					break;
			}
			
			LCD_ShowNum(30+200+48+8+10,320,chengxu,4,16);
			
			if(flag)
				break;
		}
		
		flag = 0;
		key = 0;
		
		//选择球
		while(1)
		{
			key = Remote_Scan();
			//key = KEY_Scan(0);
			switch(key)
			{
				case 0:		//没有按键按下
					
					break;
				case KEY_RIGHT:		//右
					LCD_ShowString(30+200,400,200,16,16,"changdi   ");
					flag = 1;
					break;
				case KEY_DOWN:		//下
					LCD_ShowString(30+200,400,200,16,16,"qiu-");
					if(qiu != 0)
						qiu--;
					break;
				case KEY_LEFT:		//左
					LCD_ShowString(30+200,400,200,16,16,"clear   ");
					qiu = 0;
					break;
				case KEY_UP:		//上
					LCD_ShowString(30+200,400,200,16,16,"qiu+");
					qiu++;
					break;

			}
			
			LCD_ShowNum(30+200+48+8+10,340,qiu,4,16);
			
			if(flag)
				break;
		}
		
		flag = 0;
		key = 0;
		while(1)
		{
			key = Remote_Scan();
			switch(key)
			{
				case 0:		//没有按键按下
					break;
				case KEY_RIGHT:		//右
					LCD_ShowString(30+200,400,200,16,16,"start   ");
					flag = 1;
					break;
				case KEY_DOWN:		//下
					LCD_ShowString(30+200,400,200,16,16,"changdi-");
					if(changdi != 0)
						changdi--;
					break;
				case KEY_LEFT:		//左
					LCD_ShowString(30+200,400,200,16,16,"clear   ");
					changdi=0;
					break;
				case KEY_UP:		//上
					LCD_ShowString(30+200,400,200,16,16,"changdi+");
					changdi++;
					break;
				case KEY_POWER:
					flag = 1;
					chengxu = 99;
					break;
			}
			
			LCD_ShowNum(30+200+48+8+10,360,changdi,4,16);
			
			if(flag)
				break;
		}
	
		
		while(1){
		switch(chengxu)
		{
			case 0:	    	//测试程序
				switch(qiu)
				{
					case 0:
						RobotRotate(180);
						//printf("err\r\n");
						//顺时针180°
						break;
					case 1:
						//机械臂下降
						Robot_armDown();
//						2高电平往下，接红线，正转
//						__HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_1,300);
//						__HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_2,4000);
						LED0 = !LED0;
						break;
					case 2:
						//机械臂上升
						Robot_armUp();
						LED0 = !LED0;
						break;
					case 3:
						//红外测试
						GetInfraredState();
						LED0 = !LED0;
						break;
					case 4:
						//限位开关测试6
						if(LimitSwitchDowm == 1)
							LED1 = 0;
						else
							LED1 = 1;
						if(LimitSwitchUp == 1)
							LED0 = 0;
						else
							LED0 = 1;
						break;
					case 5:
						//避障测试
						RobotGoAvoidance();
						LED0 = !LED0;
						break;
					case 6:
						//视觉测试
						FindBall_vision(qiu);
						LED0 = !LED0;
						break;
				}
				break;
			case 1:			//传球第一回合				
				switch(changdi)
				{
					case 0:			//上场
						//延时10s
						for(i = 0 ;i <10;i++){
							SetPWM(0,0,0);
							delay_ms(1000);
						}
			
						RobotGoTo(0-Correction_X,2+Correction_Y,-110);
						if(!DownShotUp())
							break;
						RobotRotate(0);
						FindBall_VandR(qiu);
			
						//判断是否找到球，如果没有
						if(findballtime==0){			//没找到球
							RobotGoTo(-3-Correction_X,2+Correction_Y,0);
							FindBall_VandR(qiu);
						}				
				
						RobotGoTo(0-Correction_X,2+Correction_Y,-110);
					
						if(!DownShotUp())
							break;
				
						RobotGoTo(0,1+Correction_Y,-180);
				
						//加视觉找框回位
						break;
					case 1:			//下场
						//延时10s
						for(i = 0 ;i <10;i++){
							SetPWM(0,0,0);
							delay_ms(1000);
						}
			
						RobotGoTo(0+Correction_X,2+Correction_Y,110);
						if(!DownShotUp())
							break;
						RobotRotate(0);
						FindBall_VandR(qiu);
						
			
						//判断是否找到球，如果没有
						if(findballtime==0){			//没找到球
							RobotGoTo(3+Correction_X,2+Correction_Y,0);
							FindBall_VandR(qiu);
						}				
				
						RobotGoTo(0+Correction_X,2+Correction_Y,110);
					
						if(!DownShotUp())
							break;
				
						RobotGoTo(0,1+Correction_Y,180);
				
						//加视觉找框回位
						break;
				}				
				break;
			case 2:			//传球第二回合
				switch(changdi)
				{
					case 0:			//上场
						RobotGoTo(0-Correction_X,2+Correction_Y,0);
						FindBall_VandR(qiu);
			
						//判断是否找到球，如果没有
						if(findballtime==0){		//没找到球
							RobotGoTo(-3-Correction_X,2+Correction_Y,0);
							FindBall_VandR(qiu);
						}
				
						RobotGoTo(-6-Correction_X,2+Correction_Y,0);
						RobotGoTo(-6-Correction_X,4+Correction_Y,-210);
				
						if(!DownShotUp())
							break;
				
						RobotGoTo(-5-Correction_X,2+Correction_Y,-45);
						FindBall_VandR(qiu);
						RobotGoTo(-6-Correction_X,4+Correction_Y,-210);
				
						if(!DownShotUp())
							break;
				
						RobotGoTo(0,1+Correction_Y,-180);
				
						//加视觉找框回位
						break;
					case 1:			//下场
						RobotGoTo(0+Correction_X,2+Correction_Y,0);
						FindBall_VandR(qiu);
			
						//判断是否找到球，如果没有
						if(findballtime==0){		//没找到球
							RobotGoTo(3+Correction_X,2+Correction_Y,0);
							FindBall_VandR(qiu);
						}
				
						RobotGoTo(6+Correction_X,2+Correction_Y,0);
						RobotGoTo(6+Correction_X,4+Correction_Y,210);
				
						if(!DownShotUp())
							break;
				
						RobotGoTo(5+Correction_X,2+Correction_Y,45);
						FindBall_VandR(qiu);
						RobotGoTo(6+Correction_X,4+Correction_Y,210);
				
						if(!DownShotUp())
							break;
				
						RobotGoTo(0+Correction_X,1+Correction_Y,180);
				
						//加视觉找框回位
						break;
				}				
				break;
			case 3:			//传球第三回合
				switch(changdi)
				{
					case 0:			//上场
						RobotGoTo(-5-Correction_X,2+Correction_Y,-45);
						FindBall_VandR(qiu);
				
						//记录铲球点A的位置
						BasketballRobot.PX=BasketballRobot.X;
						BasketballRobot.PY=BasketballRobot.Y;
				
						RobotGoTo(-5-Correction_X,2+Correction_Y,-45);
						RobotGoTo(-6-Correction_X,4+Correction_Y,-210);
						if(!DownShotUp())
							break;
				
						//原路返回
						RobotGoTo(-5-Correction_X,2+Correction_Y,-45);
						RobotGoTo(BasketballRobot.PX-Correction_X,BasketballRobot.PY+Correction_Y,-210);
			
						RobotGoTo(-6.75-Correction_X,2+Correction_Y,-90);
						//FindBall_VandR(qiu);
						FindBall_radar();
			
						//原路返回
						RobotGoTo(BasketballRobot.PX-Correction_X,BasketballRobot.PY+Correction_Y,-210);
						RobotGoTo(-5-Correction_X,2+Correction_Y,-45);
				
						RobotGoTo(-6-Correction_X,4+Correction_Y,-210);
						if(!DownShotUp())
							break;
				
						RobotGoTo(0-Correction_X,1+Correction_Y,-180);
			
						//加视觉找框回位
						break;
					case 1:			//下场
						RobotGoTo(5+Correction_X,2+Correction_Y,45);
						FindBall_VandR(qiu);
				
						//记录铲球点A的位置
						BasketballRobot.PX=BasketballRobot.X;
						BasketballRobot.PY=BasketballRobot.Y;
				
						RobotGoTo(5+Correction_X,2+Correction_Y,45);
						RobotGoTo(6+Correction_X,4+Correction_Y,210);
						if(!DownShotUp())
							break;
				
						//原路返回
						RobotGoTo(5+Correction_X,2+Correction_Y,45);
						RobotGoTo(BasketballRobot.PX+Correction_X,BasketballRobot.PY+Correction_Y,210);
			
						RobotGoTo(6.75+Correction_X,2+Correction_Y,90);
						//FindBall_VandR(qiu);
						FindBall_radar();
			
						//原路返回
						RobotGoTo(BasketballRobot.PX+Correction_X,BasketballRobot.PY+Correction_Y,210);
						RobotGoTo(5+Correction_X,2+Correction_Y,45);
				
						RobotGoTo(6+Correction_X,4+Correction_Y,210);
						if(!DownShotUp())
							break;
				
						RobotGoTo(0+Correction_X,1+Correction_Y,180);
			
						//加视觉找框回位
						break;
				}					
				break;
			case 4:			//投篮第一回合
				switch(changdi)
				{
					case 0:			//上场
						RobotGoTo(-4-Correction_X,3.7+Correction_Y,-90);
			
						//雷达找框
						FindBasketry();
			
						if(!DownShotUp())
							break;
			
						RobotGoTo(0-Correction_X,2+Correction_Y,0);
						FindBall_VandR(qiu);
			
						//判断是否找到球，如果没有			
						if(findballtime==0){		//没找到球
							RobotGoTo(-3-Correction_X,2+Correction_Y,0);
							FindBall_VandR(qiu);
						}
				
						RobotGoTo(-9-Correction_X,3.7+Correction_Y,-90);
				
						//雷达找框
						FindBasketry();
				
						if(!DownShotUp())
							break;
						break;
					case 1:			//下场
						RobotGoTo(4+Correction_X,3.7+Correction_Y,90);
			
						//雷达找框
						FindBasketry();
			
						if(!DownShotUp())
							break;
			
						RobotGoTo(0+Correction_X,2+Correction_Y,0);
						FindBall_VandR(qiu);
			
						//判断是否找到球，如果没有			
						if(findballtime==0){		//没找到球
							RobotGoTo(3+Correction_X,2+Correction_Y,0);
							FindBall_VandR(qiu);
						}
				
						RobotGoTo(9+Correction_X,3.7+Correction_Y,90);
				
						//雷达找框
						FindBasketry();
				
						if(!DownShotUp())
							break;
						break;
				}
				break;
			case 5:			//投篮第二回合
				switch(changdi)
				{
					case 0:			//上场
						RobotGoTo(-5-Correction_X,2+Correction_Y,-45);
						FindBall_VandR(qiu);
			
						//记录铲球点A的位置
						BasketballRobot.PX=BasketballRobot.X;
						BasketballRobot.PY=BasketballRobot.Y;
			
						RobotGoTo(-9-Correction_X,3.7+Correction_Y,-90);
			
						//雷达找框
						FindBasketry();
			
						if(!DownShotUp())
							break;
			
						//RobotGoTo(A);
						RobotGoTo(BasketballRobot.PX-Correction_X,BasketballRobot.PY+Correction_Y,-90);
			
						RobotGoTo(0-Correction_X,2+Correction_Y,0);
						FindBall_VandR(qiu);
			
						//判断是否找到球，如果没有			
						if(findballtime==0){		//没找到球
							RobotGoTo(-3-Correction_X,2+Correction_Y,0);
							FindBall_VandR(qiu);
						}
				
						RobotGoTo(BasketballRobot.PX-Correction_X,BasketballRobot.PY+Correction_Y,0);
						RobotGoTo(-9-Correction_X,3.7+Correction_Y,-90);
				
						//雷达找框
						FindBasketry();
				
						if(!DownShotUp())
							break;
						break;
					case 1:			//下场
						RobotGoTo(5+Correction_X,2+Correction_Y,45);
						FindBall_VandR(qiu);
			
						//记录铲球点A的位置
						BasketballRobot.PX=BasketballRobot.X;
						BasketballRobot.PY=BasketballRobot.Y;
			
						RobotGoTo(9+Correction_X,3.7+Correction_Y,90);
			
						//雷达找框
						FindBasketry();
			
						if(!DownShotUp())
							break;
			
						//RobotGoTo(A);
						RobotGoTo(BasketballRobot.PX+Correction_X,BasketballRobot.PY+Correction_Y,90);
			
						RobotGoTo(0+Correction_X,2+Correction_Y,0);
						FindBall_VandR(qiu);
			
						//判断是否找到球，如果没有			
						if(findballtime==0){		//没找到球
							RobotGoTo(3+Correction_X,2+Correction_Y,0);
							FindBall_VandR(qiu);
						}
				
						RobotGoTo(BasketballRobot.PX+Correction_X,BasketballRobot.PY+Correction_Y,0);
						RobotGoTo(9+Correction_X,3.7+Correction_Y,90);
				
						//雷达找框
						FindBasketry();
				
						if(!DownShotUp())
							break;
						break;
				}					
				break;
			case 6:			//投篮第三回合
				switch(changdi)
				{
					case 0:			//上场
						RobotGoTo(-5-Correction_X,2-Correction_Y,-30);
						FindBall_VandR(qiu);
			
						RobotRotate(-90);
						RobotGoTo(-9-Correction_X,3.7+Correction_Y,-90);
			
						//雷达找框
						FindBasketry();
			
						if(!DownShotUp())
							break;
			
						RobotRotate(-240);
						FindBall_VandR(qiu);
						RobotGoTo(-9-Correction_X,3.7+Correction_Y,-90);
			
						//雷达找框
						FindBasketry();
			
						if(!DownShotUp())
							break;
						break;
					case 1:			//下场
						RobotGoTo(5+Correction_X,2+Correction_Y,30);
						FindBall_VandR(qiu);
			
						RobotRotate(90);
						RobotGoTo(9.0f+Correction_X,3.7+Correction_Y,90);
			
						//雷达找框
						FindBasketry();
			
						if(!DownShotUp())
							break;
			
						RobotRotate(240);
						FindBall_VandR(qiu);
						RobotGoTo(9+Correction_X,3.7+Correction_Y,90);
			
						//雷达找框
						FindBasketry();
			
						if(!DownShotUp())
							break;
						break;
				}				
				break;
				
		}
					
	}

	}		
}

