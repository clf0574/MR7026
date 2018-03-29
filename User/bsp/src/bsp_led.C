

#include <bsp.h>

//type类型说明
//
//0-灯闪烁后，原先是什么状态，依然为什么状态
//1-灯闪烁后，常亮
//2-灯闪烁后，熄灭

 #define LED_NUM  2


//====================================================================================================
//LED运行管理
typedef struct __LED
{
	unsigned char ON_time;					//亮时间 大于10秒为一直亮
	unsigned char OFF_time;					//灭时间
	unsigned char count;					//次数	
	unsigned char back_ON_time;             		//
	unsigned char back_OFF_time;            		//
	unsigned char ON_OFF;					//当前状态
	unsigned char stu;	  				//闪烁后的状态	
	unsigned char half;						//半次标记，总是先亮后灭
}_LED;

_LED LED[LED_NUM];							//LED个数



	/* 按键口对应的RCC时钟 */
	#define RCC_ALL_LED 	(RCC_APB2Periph_GPIOB )

	#define GPIO_PORT_LED1   GPIOB
  #define GPIO_PORT_LED2   GPIOB
	#define GPIO_PIN_LED1	GPIO_Pin_4
  #define GPIO_PIN_LED2	GPIO_Pin_3

/*
*********************************************************************************************************
*	函 数 名: bsp_InitLed
*	功能说明: 配置LED指示灯相关的GPIO,  该函数被 bsp_Init() 调用。
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitLed(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	uint8_t i;

	/* 打开GPIO时钟 */
	RCC_APB2PeriphClockCmd(RCC_ALL_LED, ENABLE);

	/*
		配置所有的LED指示灯GPIO为推挽输出模式
		由于将GPIO设置为输出时，GPIO输出寄存器的值缺省是0，因此会驱动LED点亮.
		这是我不希望的，因此在改变GPIO为输出前，先关闭LED指示灯
	*/

// 		  //屏蔽JTAG功能，使用对应IO,SW正常使用
// 	DBGMCU->CR = DBGMCU->CR & ~((unsigned int)1<<5);
// 	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		/* 设为输出口 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO口最大速度 */

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
*	函 数 名: bsp_LedOn
*	功能说明: 点亮指定的LED指示灯。
*	形    参:  _no : 指示灯序号，范围 1 - 4
*	返 回 值: 无
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
*	函 数 名: bsp_LedOff
*	功能说明: 熄灭指定的LED指示灯。
*	形    参:  _no : 指示灯序号，范围 1 - 4
*	返 回 值: 无
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
*	函 数 名: bsp_LedToggle
*	功能说明: 翻转指定的LED指示灯。
*	形    参:  _no : 指示灯序号，范围 1 - 4
*	返 回 值: 按键代码
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
*	函 数 名: bsp_IsLedOn
*	功能说明: 判断LED指示灯是否已经点亮。
*	形    参:  _no : 指示灯序号，范围 1 - 4
*	返 回 值: 1表示已经点亮，0表示未点亮
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
//LED设置，亮时间，灭时间，次数
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
//LED运行管理
******************************************************************/
void led_run(void)
{
    unsigned char i;

    //=======================================================================================
	//逻辑控制
	//if(Flag.f_LED_tick==1)
    {
		//Flag.f_LED_tick=0;
		for(i=0;i<LED_NUM;i++)
		{
			if(LED[i].count!=0)
			{
			//原先是灭的
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
			//原先是亮的
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
								//最后一次循环
				if(LED[i].count==0)
				{
					//表示默认状态时长亮
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
	//设备执行
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
