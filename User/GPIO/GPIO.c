#include "GPIO.h" 
	 

	    
//BEEP IO��ʼ��
void BEEP_Init(void)
{   
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��GPIOFʱ��
  
	//��ʼ����������Ӧ����GPIOF8
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��GPIO
		
	GPIO_ResetBits(GPIOD,GPIO_Pin_8);  //��������Ӧ����GPIOF8���ͣ� 
}

//LED IO��ʼ��
void LED_Init(void)
{ 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOFʱ��

	//GPIOF9,F10��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��
	
	GPIO_SetBits(GPIOF,GPIO_Pin_9 | GPIO_Pin_10);//GPIOF9,F10���øߣ�����

}


//������ʼ������
void KEY_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOA,GPIOEʱ��
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4; //KEY0 KEY1 KEY2��Ӧ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4
	
	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//WK_UP��Ӧ����PA0
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA0
 
} 

//�����翪�س�ʼ��
void Charge_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//ʹ��GPIOGʱ��

	//GPIOG_6��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ��
	
	GPIO_ResetBits(GPIOG,GPIO_Pin_6);//GPIOG6
}

//�����ʼ��,GPIOG7
void ShotSwitch_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ��

	GPIO_ResetBits(GPIOG,GPIO_Pin_7);
}


//��λ���س�ʼ��
void LimitSwitch_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOE0
	
}

//���⿪�س�ʼ��
void InfraredSwitch_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOEʱ��
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOF9

}

//��ʱ����������IO��ʼ�� 
void shoveMotorIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_2; //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��

	GPIO_ResetBits(GPIOF,GPIO_Pin_0);
	GPIO_ResetBits(GPIOF,GPIO_Pin_2);
}

void ALL_GPIO_Init(void)
{
	LED_Init();
	BEEP_Init();
	Charge_Init();
	ShotSwitch_Init();
	LimitSwitch_init();
	InfraredSwitch_Init();
	shoveMotorIO_Init();
}
