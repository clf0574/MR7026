/*
*********************************************************************************************************
*	                                  
*	模块名称 : 串口驱动模块    
*	文件名称 : bsp_usart.c
*	版    本 : V2.0
*
*********************************************************************************************************
*/

#define  BSP_UART_MODULE	
#include <bsp.h>
#include <Extern_defin.h>


typedef struct __Rx
{
	uint8_t cnt;					//接收个数
	uint8_t buf[RX_MAX];                            //接收缓冲区
}_Rx;

static _Rx Rx;


#define RS485_RX_EN() 	   {GPIO_ResetBits(GPIOB,GPIO_Pin_2);} 
#define RS485_TX_EN() 	   {GPIO_SetBits(GPIOB,GPIO_Pin_2);} 

//void  BSP_Ser_ISR_Handler (void);

/*
*********************************************************************************************************
*	函 数 名: bsp_InitUart
*	功能说明: 初始化CPU的USART1串口硬件设备。未启用中断。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitUart(uint8_t rate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	//中断
	NVIC_InitTypeDef NVIC_InitStructure;
	

	/* 第1步：打开GPIO和USART部件的时钟 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	/* 第2步：将USART Tx的GPIO配置为推挽复用模式 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	

	/* 第3步：将USART Rx的GPIO配置为浮空输入模式
		由于CPU复位后，GPIO缺省都是浮空输入模式，因此下面这个步骤不是必须的
		但是，我还是建议加上便于阅读，并且防止其它地方修改了这个口线的设置参数
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*  第3步已经做了，因此这步可以不做
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	*/
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//======================================================================
	//485控制脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//==========================================================================

	/* 第4步：配置USART参数
	    - 波特率   = 115200 baud
	    - 数据长度 = 8 Bits
	    - 1个停止位
	    - 无校验
	    - 禁止硬件流控(即禁止RTS和CTS)
	    - 使能接收和发送
	*/
	USART_InitStructure.USART_BaudRate = 19200;

	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);

	/* 第5步：使能 USART， 配置完毕 */
	USART_Cmd(USART3, ENABLE);

	/* 
		CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题：
	 	清发送完成标志，Transmission Complete flag 
	*/
	USART_ClearFlag(USART3, USART_FLAG_TC); 
	

	/*************************************************************************************************/
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);  /* 使能接收中断 */
	
	USART_Cmd(USART3, ENABLE);		                   /* 使能串口 */
	
	USART_ClearFlag(USART3, USART_FLAG_TC);        /* 清发送完成标志，Transmission Complete flag */
	
	//=================================================================================================
	//中断配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//=====================================================================================================
	
	//=================================================================================================
// 	//以下用于杜亚IT系统，总线竞争
// 	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;		     //外部中断2
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	   //抢占优先级 0
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			     //子优先级0  
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				       //使能
// 	NVIC_Init(&NVIC_InitStructure);
	
// 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11); //外部中断配置AFIO--ETXI2
// 	
// 	EXTI_InitStructure.EXTI_Line = EXTI_Line11;					         //PA11 作为键盘的行线。检测状态
// 	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			     //中断模式
// 	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		   //下降沿触发
// 	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
// 	EXTI_Init(&EXTI_InitStructure);
	//==================================================================================================
    
}
/*
*********************************************************************************************************
*	函 数 名: RS485_Send
*	功能说明: 485发送用查询方式发送
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void RS485_Send(uint8_t *buff,uint8_t len)
{
	uint8_t i;
	uint16_t dly;


	RS485_TX_EN();
	dly=200;
	while(dly--);

	//Delayx10us(1);
	//USART_ClearFlag(USART2,USART_FLAG_TC);
	//临界区============    禁止调度
	//CPU_CRITICAL_ENTER();
	for(i=0;i<len;i++)
	{	
		//while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);	
		USART_SendData(USART3, (uint8_t)buff[i]); 				//发送一个字符函数
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);				
	}
	//CPU_CRITICAL_EXIT();
	//===================    打开调度
	//Delayx10us(1);
	//Delay2us();
	dly=300;
	while(dly--);
 	RS485_RX_EN();	
}
/*
*********************************************************************************************************
*	函 数 名: bsp_InitUart
*	功能说明: 初始化CPU的USART1串口硬件设备。未启用中断。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void USART3_IRQHandler(void)
{
  //接收中断

  uint8_t tmp;
  tmp=tmp;
	
	//====================================================================================================
	//有操作系统，需进入临界状态处理
		
  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
  {
    	if(Rx.cnt<RX_MAX)
        {
         	   Rx.buf[Rx.cnt]=USART_ReceiveData(USART3);
            Rx.cnt++;
        }
        else
        {
          	tmp=USART_ReceiveData(USART3);
        }
        
        Bus.interval=0;
				Bus.time=0;
  }
	//退出临界状态处理
	//=====================================================================================================
	
}

//============================================================================================
//用于总线竞争
// void EXTI15_10_IRQHandler(void)
// {
//   CPU_SR_ALLOC();
// 	
// 	CPU_CRITICAL_ENTER();                                       /* Tell the OS that we are starting an ISR            */
// 	OSIntEnter();
// 	CPU_CRITICAL_EXIT();
// 	
// 	if(EXTI_GetITStatus(EXTI_Line11) != RESET)				  //判别是否有键按下
// 	{
// 		EXTI_ClearITPendingBit(EXTI_Line11);					  //清除中断请求标志
// 		Bus.interval=0;
// 	  Bus.time=0;
// 	} 
// 	
// 	OSIntExit(); 
// }
//============================================================================================
void Bsp_Get_UsartData(uint8_t *p,uint8_t *pt)
{
        uint8_t i;
		for(i=0;i<Rx.cnt;i++)
		{
			  *p++=Rx.buf[i];
			   Rx.buf[i]=0;
		}
		*pt=Rx.cnt;
		 Rx.cnt=0;
}



