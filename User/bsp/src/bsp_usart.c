/*
*********************************************************************************************************
*	                                  
*	ģ������ : ��������ģ��    
*	�ļ����� : bsp_usart.c
*	��    �� : V2.0
*
*********************************************************************************************************
*/

#define  BSP_UART_MODULE	
#include <bsp.h>
#include <Extern_defin.h>


typedef struct __Rx
{
	uint8_t cnt;					//���ո���
	uint8_t buf[RX_MAX];                            //���ջ�����
}_Rx;

static _Rx Rx;


#define RS485_RX_EN() 	   {GPIO_ResetBits(GPIOB,GPIO_Pin_2);} 
#define RS485_TX_EN() 	   {GPIO_SetBits(GPIOB,GPIO_Pin_2);} 

//void  BSP_Ser_ISR_Handler (void);

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitUart
*	����˵��: ��ʼ��CPU��USART1����Ӳ���豸��δ�����жϡ�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitUart(uint8_t rate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	//�ж�
	NVIC_InitTypeDef NVIC_InitStructure;
	

	/* ��1������GPIO��USART������ʱ�� */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	/* ��2������USART Tx��GPIO����Ϊ���츴��ģʽ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	

	/* ��3������USART Rx��GPIO����Ϊ��������ģʽ
		����CPU��λ��GPIOȱʡ���Ǹ�������ģʽ���������������費�Ǳ����
		���ǣ��һ��ǽ�����ϱ����Ķ������ҷ�ֹ�����ط��޸���������ߵ����ò���
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*  ��3���Ѿ����ˣ�����ⲽ���Բ���
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	*/
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//======================================================================
	//485���ƽ�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//==========================================================================

	/* ��4��������USART����
	    - ������   = 115200 baud
	    - ���ݳ��� = 8 Bits
	    - 1��ֹͣλ
	    - ��У��
	    - ��ֹӲ������(����ֹRTS��CTS)
	    - ʹ�ܽ��պͷ���
	*/
	USART_InitStructure.USART_BaudRate = 19200;

	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);

	/* ��5����ʹ�� USART�� ������� */
	USART_Cmd(USART3, ENABLE);

	/* 
		CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
		�����������1���ֽ��޷���ȷ���ͳ�ȥ�����⣺
	 	�巢����ɱ�־��Transmission Complete flag 
	*/
	USART_ClearFlag(USART3, USART_FLAG_TC); 
	

	/*************************************************************************************************/
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);  /* ʹ�ܽ����ж� */
	
	USART_Cmd(USART3, ENABLE);		                   /* ʹ�ܴ��� */
	
	USART_ClearFlag(USART3, USART_FLAG_TC);        /* �巢����ɱ�־��Transmission Complete flag */
	
	//=================================================================================================
	//�ж�����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//=====================================================================================================
	
	//=================================================================================================
// 	//�������ڶ���ITϵͳ�����߾���
// 	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;		     //�ⲿ�ж�2
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	   //��ռ���ȼ� 0
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			     //�����ȼ�0  
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				       //ʹ��
// 	NVIC_Init(&NVIC_InitStructure);
	
// 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11); //�ⲿ�ж�����AFIO--ETXI2
// 	
// 	EXTI_InitStructure.EXTI_Line = EXTI_Line11;					         //PA11 ��Ϊ���̵����ߡ����״̬
// 	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			     //�ж�ģʽ
// 	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		   //�½��ش���
// 	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
// 	EXTI_Init(&EXTI_InitStructure);
	//==================================================================================================
    
}
/*
*********************************************************************************************************
*	�� �� ��: RS485_Send
*	����˵��: 485�����ò�ѯ��ʽ����
*	��    �Σ���
*	�� �� ֵ: ��
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
	//�ٽ���============    ��ֹ����
	//CPU_CRITICAL_ENTER();
	for(i=0;i<len;i++)
	{	
		//while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);	
		USART_SendData(USART3, (uint8_t)buff[i]); 				//����һ���ַ�����
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);				
	}
	//CPU_CRITICAL_EXIT();
	//===================    �򿪵���
	//Delayx10us(1);
	//Delay2us();
	dly=300;
	while(dly--);
 	RS485_RX_EN();	
}
/*
*********************************************************************************************************
*	�� �� ��: bsp_InitUart
*	����˵��: ��ʼ��CPU��USART1����Ӳ���豸��δ�����жϡ�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USART3_IRQHandler(void)
{
  //�����ж�

  uint8_t tmp;
  tmp=tmp;
	
	//====================================================================================================
	//�в���ϵͳ��������ٽ�״̬����
		
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
	//�˳��ٽ�״̬����
	//=====================================================================================================
	
}

//============================================================================================
//�������߾���
// void EXTI15_10_IRQHandler(void)
// {
//   CPU_SR_ALLOC();
// 	
// 	CPU_CRITICAL_ENTER();                                       /* Tell the OS that we are starting an ISR            */
// 	OSIntEnter();
// 	CPU_CRITICAL_EXIT();
// 	
// 	if(EXTI_GetITStatus(EXTI_Line11) != RESET)				  //�б��Ƿ��м�����
// 	{
// 		EXTI_ClearITPendingBit(EXTI_Line11);					  //����ж������־
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



