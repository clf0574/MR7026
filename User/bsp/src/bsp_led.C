

#include <bsp.h>

//type����˵��
//
//0-����˸��ԭ����ʲô״̬����ȻΪʲô״̬
//1-����˸�󣬳���
//2-����˸��Ϩ��

 #define LED_NUM  2


//====================================================================================================
//LED���й���
typedef struct __LED
{
	unsigned char ON_time;					//��ʱ�� ����10��Ϊһֱ��
	unsigned char OFF_time;					//��ʱ��
	unsigned char count;					//����	
	unsigned char back_ON_time;             		//
	unsigned char back_OFF_time;            		//
	unsigned char ON_OFF;					//��ǰ״̬
	unsigned char stu;	  				//��˸���״̬	
	unsigned char half;						//��α�ǣ�������������
}_LED;

_LED LED[LED_NUM];							//LED����



	/* �����ڶ�Ӧ��RCCʱ�� */
	#define RCC_ALL_LED 	(RCC_APB2Periph_GPIOB )

	#define GPIO_PORT_LED1   GPIOB
  #define GPIO_PORT_LED2   GPIOB
	#define GPIO_PIN_LED1	GPIO_Pin_4
  #define GPIO_PIN_LED2	GPIO_Pin_3

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitLed
*	����˵��: ����LEDָʾ����ص�GPIO,  �ú����� bsp_Init() ���á�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitLed(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	uint8_t i;

	/* ��GPIOʱ�� */
	RCC_APB2PeriphClockCmd(RCC_ALL_LED, ENABLE);

	/*
		�������е�LEDָʾ��GPIOΪ�������ģʽ
		���ڽ�GPIO����Ϊ���ʱ��GPIO����Ĵ�����ֵȱʡ��0����˻�����LED����.
		�����Ҳ�ϣ���ģ�����ڸı�GPIOΪ���ǰ���ȹر�LEDָʾ��
	*/

// 		  //����JTAG���ܣ�ʹ�ö�ӦIO,SW����ʹ��
// 	DBGMCU->CR = DBGMCU->CR & ~((unsigned int)1<<5);
// 	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		/* ��Ϊ����� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO������ٶ� */

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED1;
	GPIO_Init(GPIO_PORT_LED1, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED2;
	GPIO_Init(GPIO_PORT_LED2, &GPIO_InitStructure);
	
	for(i=0;i<LED_NUM;i++)
	 {
		  LED[i].ON_time=0;
		  LED[i].OFF_time=0;
		  LED[i].count=0;
		  LED[i].back_ON_time=0;
		  LED[i].back_OFF_time=0;
		  LED[i].ON_OFF=0;
		  LED[i].stu=0;
		  bsp_LedOn(i+1);
	 }

	 
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_LedOn
*	����˵��: ����ָ����LEDָʾ�ơ�
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_LedOff(uint8_t _no)
{
	_no--;

	if (_no == 0)
	{
		GPIO_PORT_LED1->BSRR = GPIO_PIN_LED1;
	}
	else if (_no == 1)
	{
		GPIO_PORT_LED2->BSRR = GPIO_PIN_LED2;
	}

}

/*
*********************************************************************************************************
*	�� �� ��: bsp_LedOff
*	����˵��: Ϩ��ָ����LEDָʾ�ơ�
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_LedOn(uint8_t _no)
{
	_no--;

	if (_no == 0)
	{
		GPIO_PORT_LED1->BRR = GPIO_PIN_LED1;
	}
	else if (_no == 1)
	{
		GPIO_PORT_LED2->BRR = GPIO_PIN_LED2;
	}

}

/*
*********************************************************************************************************
*	�� �� ��: bsp_LedToggle
*	����˵��: ��תָ����LEDָʾ�ơ�
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: ��������
*********************************************************************************************************
*/
void bsp_LedToggle(uint8_t _no)
{
	if (_no == 1)
	{
		GPIO_PORT_LED1->ODR ^= GPIO_PIN_LED1;
	}

	else if (_no == 2)
	{
		GPIO_PORT_LED2->ODR ^= GPIO_PIN_LED2;
	}

}

/*
*********************************************************************************************************
*	�� �� ��: bsp_IsLedOn
*	����˵��: �ж�LEDָʾ���Ƿ��Ѿ�������
*	��    ��:  _no : ָʾ����ţ���Χ 1 - 4
*	�� �� ֵ: 1��ʾ�Ѿ�������0��ʾδ����
*********************************************************************************************************
*/
uint8_t bsp_IsLedOn(uint8_t _no)
{
	if (_no == 1)
	{
		if ((GPIO_PORT_LED1->ODR & GPIO_PIN_LED1) == 0)
		{
			return 1;
		}
		return 0;
	}
	else if (_no == 2)
	{
		if ((GPIO_PORT_LED2->ODR & GPIO_PIN_LED2) == 0)
		{
			return 1;
		}
		return 0;
	}

	return 0;
}


/*********************************dooya****************************
//LED���ã���ʱ�䣬��ʱ�䣬����
******************************************************************/
void Led_set(unsigned char index,unsigned char ON_t,unsigned char OFF_t,unsigned char cnt,unsigned char type)
{
	if(index>=LED_NUM)return;

	 LED[index].ON_time=ON_t;
	 LED[index].OFF_time=OFF_t;
  	 LED[index].back_ON_time=ON_t;
  	 LED[index].back_OFF_time=OFF_t;
  	 LED[index].count=cnt;
	LED[index].ON_OFF=1;
	LED[index].half=0;

	 if(type==0)LED[index].stu=LED[index].ON_OFF;
	 else if(type==1)LED[index].stu=1;
	 else if(type==2)LED[index].stu=0;
}
/*********************************dooya****************************
//LED���й���
******************************************************************/
void led_run(void)
{
    unsigned char i;

    //=======================================================================================
	//�߼�����
	//if(Flag.f_LED_tick==1)
    {
		//Flag.f_LED_tick=0;
		for(i=0;i<LED_NUM;i++)
		{
			if(LED[i].count!=0)
			{
			//ԭ�������
			if((LED[i].ON_OFF)==0)
			{
				if(LED[i].OFF_time>0)
				{
					if(LED[i].OFF_time!=LOOP)LED[i].OFF_time--;
				}
				else  
				{
					LED[i].OFF_time=LED[i].back_OFF_time;
					LED[i].ON_OFF=1;
					LED[i].half++;
				}       
			}
			//ԭ��������
			if(LED[i].ON_OFF==1)
			{
				if(LED[i].ON_time>0)
				{
					if(LED[i].ON_time!=LOOP)LED[i].ON_time--;
				}
				else  
				{
					LED[i].ON_time=LED[i].back_ON_time;
					LED[i].ON_OFF=0;
					LED[i].half++;
				}
			}
			if(LED[i].half>=2)
			{
				LED[i].half=0;
				
				if(LED[i].count!=LOOP)LED[i].count--;
								//���һ��ѭ��
				if(LED[i].count==0)
				{
					//��ʾĬ��״̬ʱ����
					if((LED[i].stu)==1)
					{
						//Led_set(i,0xff,0,0xff,1);
						LED[i].ON_OFF=1;
					}
					else
					{
						//Led_set(i,0,0xff,0xff,2);
						LED[i].ON_OFF=0;
					}
				}
			}       
			}
		}
	}
	//�豸ִ��
	//=============================================================================
	for(i=0;i<LED_NUM;i++)
	{
				if(LED[i].ON_OFF==0)
				{
							bsp_LedOff(i+1);
				}
				else   
				{    
					    bsp_LedOn(i+1);
				}  
  }
}
/*********************** end of code ************************************/