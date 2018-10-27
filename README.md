# STD_Robot
The new code of basketball robot based on STD lib.kkkk

Libraries:STM32库文件

User：代码文件
* **get_info.c**<br>
*串口、TIM5中断服务函数在此，三个串口分别用于接收视觉、雷达、陀螺仪信息，定时器中断获取编码器信息并结合陀螺仪进行坐标运算从而获得实时坐标<br>*
	 * void USART1_IRQHandler(void)
	 * void USART2_IRQHandler(void)
	 * void USART3_IRQHandler(void)
	 * void TIM5_IRQHandler(void)
	 * void ReadEncoder(void);
	 * void GetYaw(void);
	 * void GetPosition(void);		//坐标转换，具体推导见
	 * void GetVisionData(void);		//视觉数据处理
	 * void GetRadarData(void);		//激光处理数据<br>
	 
* **control.c**<br>
*主要控制代码，具体功能详见代码注释。其中机械臂控制（比赛临时驱动出现问题导致控制代码修改）分为使用中断和不使用中断两套，使用中断机器人可在机械臂下降的同时移动。<br>
前代机器人速度控制采用分段控制，现主要使用PD控制，操作以及调参更方便。<br>
比赛使用壁障为RobotGoAvoidance()，壁障程序需要进一步完善<br>*

* **find_ball.c**<br>
*根据雷达数据和视觉数据左右移动找球。<br>
注意视觉、雷达数据内标志的改变和清零，可以进一步优化使代码更直观。<br>
对速度的控制可以改为PD控制，调好PD参数，有利于减少代码重复量<br>*

* **usart.c**<br>
*串口初始化、printf函数支持、向上位机发送数据函数支持（未完全完成）<br>*

* **mpu6050.c**<br>
*陀螺仪初始化，命令见陀螺仪手册<br>*

*  **GPIO.c**<br>
*开关、按键等普通GPIO初始化，头文件中对各个引角进行了宏定义（采用正点原子方法）*
* **EXIT.c**<br>
*限位开关外部中断初始化，用于铲球控制<br>*

* **tim.c** <br>
*定时器初始化*
* **PWM.c** <br>
*PWM初始化，控制底盘电机、铲球电机*
* **encoder.c**<br> 
*设定三个定时器工作正交编码器模式<br>*

* **remote.c**<br>
*红外遥控初始化，原理参考正点原子教程*
* **lcd.c**<br>
*采用正点原子驱动代码，删去没用到的代码。<br>*
	
	
