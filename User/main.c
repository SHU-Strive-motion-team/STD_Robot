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
	u8 key = 0; //����ֵ
	//u8 chengxu = 0;				//����ѡ��
	u8 flag = 0;
	u8 qiu = 0;			 //����
						 //	int16_t time = 0;			//��ʱ
	u8 findballtime = 0; //����ʱ�����Ƕȴ���
	u8 i;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����ϵͳ�ж����ȼ�����2   2λ��ռ���� 2λ��Ӧ����
	zhongquan_case = 10;
	changdi = 1; //���ҳ�
	chengxu = 0;

	delay_init(168);   //��ʼ����ʱ����
	UART_Init(115200); //���ڳ�ʼ��Ϊ9600

	ALL_GPIO_Init();
	Remote_Init();
	PWM_Init();		  //��ʼ��PWM������
	LCD_Init();		  //LCD��ʼ��
	LCD_Show_Title(); //Һ������ʾ���ݳ�ʼ��

	Control_Init(); //�����˳�ʼ��
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
		//ѡ�����
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
			case 0: //û�а�������

				break;
			case KEY_RIGHT: //��
				LCD_ShowString(30 + 200, 400, 200, 16, 16, "qiu:    ");
				flag = 1;
				break;
			case KEY_DOWN: //��
				LCD_ShowString(30 + 200, 400, 200, 16, 16, "chengxu-");
				if (chengxu != 0)
					chengxu--;
				break;
			case KEY_LEFT: //��
				LCD_ShowString(30 + 200, 400, 200, 16, 16, "clear   ");
				chengxu = 0;
				break;
			case KEY_UP: //��
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

		//ѡ����
		while (1)
		{
			key = Remote_Scan();
			//key = KEY_Scan(0);
			switch (key)
			{
			case 0: //û�а�������

				break;
			case KEY_RIGHT: //��
				LCD_ShowString(30 + 200, 400, 200, 16, 16, "changdi   ");
				flag = 1;
				break;
			case KEY_DOWN: //��
				LCD_ShowString(30 + 200, 400, 200, 16, 16, "qiu-");
				if (qiu != 0)
					qiu--;
				break;
			case KEY_LEFT: //��
				LCD_ShowString(30 + 200, 400, 200, 16, 16, "clear   ");
				qiu = 0;
				break;
			case KEY_UP: //��
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
		while (1)
		{
			key = Remote_Scan();
			switch (key)
			{
			case 0: //û�а�������
				break;
			case KEY_RIGHT: //��
				LCD_ShowString(30 + 200, 400, 200, 16, 16, "start   ");
				flag = 1;
				break;
			case KEY_DOWN: //��
				LCD_ShowString(30 + 200, 400, 200, 16, 16, "changdi-");
				if (changdi != 0)
					changdi--;
				break;
			case KEY_LEFT: //��
				LCD_ShowString(30 + 200, 400, 200, 16, 16, "clear   ");
				changdi = 0;
				break;
			case KEY_UP: //��
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
		case 0: //���Գ���
			switch (qiu)
			{
			case 0:
				RobotRotate(180);
				//printf("err\r\n");
				//˳ʱ��180��
				break;
			case 1:
				//��е���½�
				//Robot_armDown();
				RobotArm_exit(DOWN);
				BEEP = 1;
				delay_ms(2000);
				BEEP = 0;
				LED0 = !LED0;
				break;
			case 2:
				//��е������
				//Robot_armUp();
			RobotArm_exit(UP);
				LED0 = !LED0;
				BEEP = 1;
				delay_ms(2000);
				BEEP = 0;
				//						TIM_SetCompare2(TIM9,0);
				//						TIM_SetCompare1(TIM9,1500+1000);
				break;
			case 3:
				//�������
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
				//�������
				while(1)
				{
					key = Remote_Scan();
						switch(key)
						{
							case KEY_1:
							CHARGE=1;
							break;
							
							case KEY_2:
							CHARGE=0;
							break;
							
							case KEY_3:
							CHARGE=0;
							delay_ms(1000);
							SHOT=1;
							delay_ms(1000);
							SHOT=0;
							
							break;
						}
				}				
				break;
				
			case 5:
				//���ϲ���
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
				//�Ӿ�����
				FindBall_VandR(changdi);
				RobotGoTo(BasketballRobot.X,BasketballRobot.Y-0.5f,BasketballRobot.ThetaD);
				LED0 = !LED0;
				break;
			case 7:
				FindBasketry();
				BEEP = 1;
				delay_ms(500);
				BEEP = 0;
				break;
			case 8:
				GoBack_Vision();
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
			
		case 1: //�����һ�غ�
			switch (changdi)
			{
			case 1: //�³�
				//��ʱ10s
				for (i = 0; i < 10; i++)
				{
					SetPWM(0, 0, 0);
					delay_ms(1000);
				}
				SetPWM(-400,0,400);
				delay_ms(1000);
				RobotArm_exit(DOWN);
				RobotGoTo(1.5, 2.5, 250);
				while(LimitSwitchDown ==0);
				while (ShotUp());
				
				RobotArm_exit(UP);
				RobotRotate(0);
				FindBall_VandR(qiu);

				//�ж��Ƿ��ҵ������û��
//				if (findballtime == 0)
//				{ //û�ҵ���
//					RobotGoTo(-3 - Correction_X, 2 + Correction_Y, 0);
//					FindBall_VandR(qiu);
//				}

				RobotGoTo(1.5, 2.5 , 250);
				while(LimitSwitchUp == 0);
				Robot_armDown();
				while (ShotUp());
				RobotArm_exit(UP);
				RobotGoTo(0, 1 ,0);
				RobotGoTo(0,0,0);
				//���Ӿ��ҿ��λ
				break;
			case 0: //�ϳ�
				//��ʱ10s
				for (i = 0; i < 10; i++)
				{
					SetPWM(0, 0, 0);
					delay_ms(1000);
				}
				SetPWM(-400,0,400);
				delay_ms(1000);
				RobotArm_exit(DOWN);
				RobotGoTo(-1.5f, 2.5f, 110);
				while(LimitSwitchDown==0);
				while (ShotUp());
				RobotArm_exit(UP);
				RobotRotate(0);
				FindBall_VandR(qiu);

				//�ж��Ƿ��ҵ������û��
//				if (findballtime == 0)
//				{ //û�ҵ���
//					RobotGoTo(-3 - Correction_X, 2 + Correction_Y, 0);
//					FindBall_VandR(qiu);
//				}

				RobotGoTo(-1.5f, 2.5f , 110);
				while(LimitSwitchUp == 0);
				Robot_armDown();
				while (ShotUp());
				RobotArm_exit(UP);
				RobotGoTo(0, 1 ,180);
				RobotGoTo(0,0,180);
				break;
			}
			break;
		case 2: //����ڶ��غ�
			switch (changdi)
			{
			case 0: //�ϳ�
				for (i = 0; i < 10; i++)
				{
					SetPWM(0, 0, 0);
					delay_ms(1000);
				}
				SetPWM(-400,0,400);
				delay_ms(1000);
				RobotGoTo(-1.5, 2.5 ,0);
				
				FindBall_VandR(5);
//				//�ж��Ƿ��ҵ������û��
//				if (findballtime == 0)
//				{ //û�ҵ���
//					RobotGoTo(3 + Correction_X, 2 + Correction_Y, 0);
//					FindBall_VandR(qiu);
//				}
				
				RobotGoTo(-1.5, 2.5 ,110);
				while(LimitSwitchUp == 0);
				Robot_armDown();
				//while(LimitSwitchDown == 0);
				delay_ms(2000);
				while (ShotUp());
				RobotArm_exit(UP);
				RobotGoTo(-4.3f, 2.5f , 70);
				FindBall_VandR(5);
				RobotGoTo(-1.5f, 2.5f ,110);
				while(LimitSwitchUp==0);
				Robot_armDown();
				delay_ms(2000);
				while(ShotUp());
				RobotArm_exit(UP);
				RobotGoTo(0 , 1 , 0);
				RobotGoTo(0 , 0 , 0);

				//���Ӿ��ҿ��λ
				break;
				
				
			case 1: //�³�
				for (i = 0; i < 10; i++)
				{
					SetPWM(0, 0, 0);
					delay_ms(1000);
				}
				SetPWM(-400,0,400);
				delay_ms(1000);
				RobotGoTo(1.5, 2.5 ,0);
				
				FindBall_VandR(5);
//				//�ж��Ƿ��ҵ������û��
//				if (findballtime == 0)
//				{ //û�ҵ���
//					RobotGoTo(3 + Correction_X, 2 + Correction_Y, 0);
//					FindBall_VandR(qiu);
//				}
				
				RobotGoTo(1.5, 2.5 ,250);
				while(LimitSwitchUp == 0);
				Robot_armDown();
				//while(LimitSwitchDown == 0);
				delay_ms(2000);
				while (ShotUp());
				RobotArm_exit(UP);
				RobotGoTo(4.3f, 2.5f , 290);
				FindBall_VandR(5);
				RobotGoTo(1.5f, 2.5f ,250);
				while(LimitSwitchUp==0);
				Robot_armDown();
				delay_ms(2000);
				while(ShotUp());
				RobotArm_exit(UP);
				RobotGoTo(0 , 1 , 0);
				RobotGoTo(0 , 0 , 0);

				//���Ӿ��ҿ��λ
				break;
			}
			break;
			
		case 3: //��������غ�
			switch (changdi)
			{
			case 0: //�ϳ�
				for (i = 0; i < 10; i++)
				{
					SetPWM(0, 0, 0);
					delay_ms(1000);
				}
				SetPWM(-400,0,400);
				delay_ms(1000);
				RobotGoTo(-3.8f, 1.8f, 70);
				FindBall_VandR(7);

				//��¼�����A��λ��
				BasketballRobot.PX = BasketballRobot.X;
				BasketballRobot.PY = BasketballRobot.Y;
				RobotGoTo(-5.3f, BasketballRobot.PY,-160);
				RobotGoTo(-5.3f, 3.3f,-160);
				while(LimitSwitchUp==0);
				Robot_armDown();
				delay_ms(2000);
				RobotArm_exit(UP);
				while(ShotUp());
//				//ԭ·����
				RobotGoTo(-5.3 , BasketballRobot.PY , 90);
				RobotGoTo(BasketballRobot.X+0.2 , BasketballRobot.PY ,90);
				
				FindBall_VandR(5);
//				FindBall_radar();

				//ԭ·����
				RobotGoTo(BasketballRobot.PX+0.2, BasketballRobot.PY , 90);
				RobotGoTo(-5.3f, BasketballRobot.PY,-160);
				RobotGoTo(-5.3f, 3.3f,-160);
				while(LimitSwitchUp==0);
				Robot_armDown();
				delay_ms(2000);
				RobotArm_exit(UP);
				while(ShotUp());

				RobotGoTo(0 , 1 , 0);
				RobotGoTo(0 , 0 , 0);

				//���Ӿ��ҿ��λ
				break;
				
			case 1: //�³�
				for (i = 0; i < 10; i++)
				{
					SetPWM(0, 0, 0);
					delay_ms(1000);
				}
				SetPWM(-400,0,400);
				delay_ms(1000);
				RobotGoTo(3.8f, 1.8f, 290);
				FindBall_VandR(7);

				//��¼�����A��λ��
				BasketballRobot.PX = BasketballRobot.X;
				BasketballRobot.PY = BasketballRobot.Y;
				RobotGoTo(5.3f, BasketballRobot.PY,160);
				RobotGoTo(5.3f, 3.3f,160);
				while(LimitSwitchUp==0);
				Robot_armDown();
				delay_ms(2000);
				RobotArm_exit(UP);
				while(ShotUp());
//				//ԭ·����
				RobotGoTo(5.3 , BasketballRobot.PY , 270);
				RobotGoTo(BasketballRobot.X+0.2 , BasketballRobot.PY , 270);
				
				FindBall_VandR(5);
//				FindBall_radar();

				//ԭ·����
				RobotGoTo(BasketballRobot.PX+0.2, BasketballRobot.PY , 270);
				RobotGoTo(5.3f, BasketballRobot.PY,160);
				RobotGoTo(5.3f, 3.3f,160);
				while(LimitSwitchUp==0);
				Robot_armDown();
				delay_ms(2000);
				RobotArm_exit(UP);
				while(ShotUp());

				RobotGoTo(0 , 1 , 0);
				RobotGoTo(0 , 0 , 0);

				//���Ӿ��ҿ��λ
				break;
			}
			break;
			
		case 4: //Ͷ����һ�غ�
			switch (changdi)
			{
			case 0: //�ϳ�
				for (i = 0; i < 10; i++)
				{
					SetPWM(0, 0, 0);
					delay_ms(1000);
				}
				SetPWM(-400,0,400);
				delay_ms(1000);
				
				RobotGoTo(-8.5f, 4.2f,-270);
				SetPWM(-400,0,400);
				delay_ms(2000);
				RobotGoTo(-4,3.6,-270);

				//�״��ҿ�
				FindBasketry();

				while (ShotUp());

				RobotGoTo(-1.5f, 2.5f, 0);
				FindBall_VandR(qiu);

				//�ж��Ƿ��ҵ������û��
//				if (findballtime == 0)
//				{ //û�ҵ���
//					RobotGoTo(3 + Correction_X, 2 + Correction_Y, 0);
//					FindBall_VandR(qiu);
//				}

				RobotGoTo(-8.5f, 4.2f, -270);
//				RobotGoTo(-4,3.6,-270);
				//�״��ҿ�
				FindBasketry();

				while(ShotUp());
				SetPWM(0,0,0);
				
				break;
				
			case 1: //�³�
				for (i = 0; i < 10; i++)
				{
					SetPWM(0, 0, 0);
					delay_ms(1000);
				}
				SetPWM(-400,0,400);
				delay_ms(1000);
				
				RobotGoTo(8.5f, 4.2f,270);
				SetPWM(-400,0,400);
				delay_ms(2000);
				RobotGoTo(4,3.6,270);

				//�״��ҿ�
				FindBasketry();

				while (ShotUp());

				RobotGoTo(1.5f, 2.5f, 0);
				FindBall_VandR(qiu);

				//�ж��Ƿ��ҵ������û��
//				if (findballtime == 0)
//				{ //û�ҵ���
//					RobotGoTo(3 + Correction_X, 2 + Correction_Y, 0);
//					FindBall_VandR(qiu);
//				}

				RobotGoTo(8.5f, 4.2f, 270);
//				RobotGoTo(4,3.6,270);
				//�״��ҿ�
				FindBasketry();

				while(ShotUp());
				SetPWM(0,0,0);
				
				break;
				
			}
			break;
		case 5: //Ͷ���ڶ��غ�
			switch (changdi)
			{
			case 0: //�ϳ�
				for (i = 0; i < 10; i++)
				{
					SetPWM(0, 0, 0);
					delay_ms(1000);
				}
				
				SetPWM(-400,0,400);
				delay_ms(2000);
				RobotGoTo(-4.3f, 2.5f, -290);
				FindBall_VandR(qiu);

				//��¼�����A��λ��
				BasketballRobot.PX = BasketballRobot.X;
				BasketballRobot.PY = BasketballRobot.Y;

				RobotGoTo(-8.5f, 4.2f, -270);
//				RobotGoTo(4.3f, 3.6f, 270);

				//�״��ҿ�
				FindBasketry();

				while(ShotUp());
				delay_ms(2000);
				RobotArm_exit(UP);
				//RobotGoTo(A);
				RobotGoTo(BasketballRobot.PX , BasketballRobot.PY , -290);

				RobotGoTo(-1.5f, 2.5f, 0);
				FindBall_VandR(qiu);

//				//�ж��Ƿ��ҵ������û��
//				if (findballtime == 0)
//				{ //û�ҵ���
//					RobotGoTo(3 + Correction_X, 2 + Correction_Y, 0);
//					FindBall_VandR(qiu);
//				}

				RobotGoTo(BasketballRobot.PX, BasketballRobot.PY, 0);
				RobotGoTo(-4.3f, 3.6f, -270);

				//�״��ҿ�
				FindBasketry();

				while(ShotUp());
				SetPWM(0,0,0);
				break;
			case 1: //�³�
				for (i = 0; i < 10; i++)
				{
					SetPWM(0, 0, 0);
					delay_ms(1000);
				}
				SetPWM(-400,0,400);
				delay_ms(2000);
				RobotGoTo(4.3f, 2.5f, 290);
				FindBall_VandR(qiu);

				//��¼�����A��λ��
				BasketballRobot.PX = BasketballRobot.X;
				BasketballRobot.PY = BasketballRobot.Y;

//				RobotGoTo(8.5f, 4.2f, 270);
				RobotGoTo(4.3f, 3.6f, 270);

				//�״��ҿ�
				FindBasketry();

				while(ShotUp());
				delay_ms(2000);
				RobotArm_exit(UP);
				//RobotGoTo(A);
				RobotGoTo(BasketballRobot.PX , BasketballRobot.PY , 290);

				RobotGoTo(1.5f, 2.5f, 0);
				FindBall_VandR(qiu);

//				//�ж��Ƿ��ҵ������û��
//				if (findballtime == 0)
//				{ //û�ҵ���
//					RobotGoTo(3 + Correction_X, 2 + Correction_Y, 0);
//					FindBall_VandR(qiu);
//				}

				RobotGoTo(BasketballRobot.PX, BasketballRobot.PY, 0);
				RobotGoTo(4.3f, 3.6f, 270);

				//�״��ҿ�
				FindBasketry();
				delay_ms(2000);

				while(ShotUp());
				SetPWM(0,0,0);
				break;
			}
			break;
		case 6: //Ͷ�������غ�
			switch (changdi)
			{
			case 0: //�ϳ�
				for (i = 0; i < 10; i++)
				{
					SetPWM(0, 0, 0);
					delay_ms(1000);
				}
				SetPWM(-400,0,400);
				delay_ms(2000);
				RobotGoTo(-4.3f, 2.5f, -290);
				FindBall_VandR(qiu);

				RobotGoTo(-8.5f, 4.2f, -270);
//				RobotGoTo(4.3f, 3.6f, 270);

				//�״��ҿ�
				FindBasketry();
				delay_ms(2000);
				while (ShotUp());

				RobotArm_exit(UP);
				RobotRotate(-160);
				while(LimitSwitchUp==0);
				FindBall_VandR(qiu);
				
				//�״��ҿ�
				RobotGoTo(-8.5f, 4.2f, -270);
				FindBasketry();
				while(ShotUp());
				SetPWM(0,0,0);
				break;
			case 1: //�³�
				for (i = 0; i < 10; i++)
				{
					SetPWM(0, 0, 0);
					delay_ms(1000);
				}
				SetPWM(-400,0,400);
				delay_ms(2000);
				
				RobotGoTo(4.3f, 2.5f, 290);
				FindBall_VandR(qiu);

				RobotGoTo(8.5f, 4.2f, 270);
//				RobotGoTo(4.3f, 3.6f, 270);

				//�״��ҿ�
				FindBasketry();
				delay_ms(2000);
				while (ShotUp());

				RobotArm_exit(UP);
				RobotRotate(145);
				while(LimitSwitchUp==0);
				FindBall_VandR(qiu);
				
				//�״��ҿ�
				RobotGoTo(4.3f, 3.6f, 270);
				FindBasketry();
				delay_ms(2000);
				while(ShotUp());
				SetPWM(0,0,0);
				break;
			}
			break;
		}
	}
}
