/*
*********************************************************************************************************
说明：
1.杜亚智能家居-面板类程序模板
2.采用混合式调度程序
3.发送函数采用总线竞争算法，包括
	a.总线判断是否空闲，忙着退出
	b.等待空闲后发送数据
	c.发送完数据，判断接收缓冲区的数据是否和发送缓冲区的一致，不一致则判断失败，重发
	d.在预定时间内等待接收主机的应答，超时或出错，则判断失败，重发
	e.失败重发最多5次

*
*********************************************************************************************************
*/
#define MAIN_VARIABLE

#include "includes.h"


static void taskRxProcess(void);
static void AppTask_WirelessRecv(void);
static void AppTask_WirelessSend(void);
static void AppTask_Run(void);
void AppTask_RivalrySend(void);
void AppTask_BusMonitor(void);
void Check_Frame_Interval(void);
/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: c程序入口
*	形    参：无
*	返 回 值: 错误代码(无需处理)
*********************************************************************************************************
*/
int main(void)
{
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x1800);
	
	
	bsp_Init();		    /* 硬件初始化 */
	Dev_Init();				//
		
	/* 添加 个任务 */
	//hSCH_Add_Task(AppTask_RivalrySend, 1, 1, 1);           		//竞争发送任务 每2mS调用一次
	hSCH_Add_Task(taskRxProcess, 2, 10, 1);  								 		//接收任务 每10mS调用一次
	hSCH_Add_Task(AppTask_WirelessRecv, 3, 10, 1);        		 	//无线接收任务
	hSCH_Add_Task(AppTask_WirelessSend, 3, 20, 1);        	 		//无线发送任务
	
	hSCH_Add_Task(AppTask_Run, 4, 10, 1);        						 		//运行任务 //每10mS调用一次
	
	Led_set(0,100,100,3,1);	
  Led_set(1,100,100,3,2);		
	
	bsp_StartHardTimer(1,100,Check_Frame_Interval);							//每100uS调用一次帧间隔检测	
																																				
	__set_PRIMASK(0);										//开启总中断
	
	//======================================================================
	//不要挪动位置
	bsp_InitIwdg(2000);																				//使能看门狗
	if((Dev.err & ERR_RESET)==0x00)IWDG_Feed();
	//=======================================================================

	/* 进入主程序循环体 */
	while (1)
	{
		/* 任务执行函数 */
		hSCH_Dispatch_Tasks();
	}
}

/*
*********************************************************************************************************
*	函 数 名: AppTask_RivalrySend
*	功能说明: 总线竞争发送
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void AppTask_RivalrySend(void)
{
		IT1_SendData();
}

/*
*********************************************************************************************************
*	函 数 名: taskRxProcess
*	功能说明: 任务函数，用于总线接收到的处理
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void taskRxProcess(void)
{

	if((Bus.SelfSend==0)&&(Bus.rxok==1))
	{
		Bus.rxok=0;

		IT1_ParseFrame();	
		
		Bus.distime=3;												//总线阻塞
		    
		//如果有应答要求，则应答(此处直接应答，不判断总线)
		if(TxDat.f_tx==1)
		{					
					RS485_Send(TxDat.buf,TxDat.cnt);								
		    	TxDat.f_tx=0;		
		}
	}
}


/*
*********************************************************************************************************
*	函 数 名: AppTask_BusMonitor
*	功能说明: 任务函数，用于总线的监控
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
//总线监控程序，不可堵塞，每1mS调用一次
void AppTask_BusMonitor(void)
{

	if(Bus.distime>0)Bus.distime--;															//总线禁止时间
	//if(Bus.interval<10)Bus.interval++;													//总线帧之间间隔时间
	if(Bus.time<200)Bus.time++;																	//总线空闲时间
	if(Bus.timeout<250)Bus.timeout++;
	Dev.rand++;	
	

// 	 //这个常量取得过大，会使自发自收和应答方的应答粘合在一起。
// 	 //但过小的话，如果发送方发送时有中断导致一帧数据有小间隔，这里会看做是两帧，而引起错误�  测试时重点测试
//   if(Bus.interval==2)
// 	{
// 		Bsp_Get_UsartData(&RxDat.buf[0],&RxDat.cnt);
// 		Bus.rxok=1;																								//接收到一帧数据
// 	}	
}
//===============================================================================================
void Check_Frame_Interval(void)
{
		if(Bus.interval<100)Bus.interval++;
    if(Bus.interval==20)
	  {
				Bsp_Get_UsartData(&RxDat.buf[0],&RxDat.cnt);
				Bus.rxok=1;																								//接收到一帧数据
		}		
	  bsp_StartHardTimer(1,100,Check_Frame_Interval);
}

/*
*********************************************************************************************************
*	函 数 名: AppTask_WirelessRecv
*	功能说明: 接收无线数据，并处理
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void AppTask_WirelessRecv(void)
{
		//static uint8_t cnt1=0;
	
	   //cnt1++;
	   //if(cnt1>=10)
		 {
		 //	    cnt1=0;
					Wireless_Recv();																				//没10ms查询一次			
		 }
}

/*
*********************************************************************************************************
*	函 数 名: AppTask_WirelessRecv
*	功能说明: 接收无线数据，并处理
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void AppTask_WirelessSend(void)
{
		static uint8_t cnt=0;
	  //uint8_t buf[16];
	
	  cnt++;
	  if(cnt>=5)
		{
				cnt=0;
				Wireless_Send();																				//没100ms查询一次
		}
}

/*
*********************************************************************************************************
*	函 数 名: AppTask_LedToggle
*	功能说明: 四个LED闪烁的程序
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void AppTask_Run(void)
{
	static uint8_t t10ms,t100ms;
	static uint8_t ttest;
	uint8_t key,i,lin,row;
		
	t10ms++;
	bsp_KeyScan();													//按键轮询
	led_run();
	
	IT1_IR_LoadToWireless(Ir.addr);					//分包数据发送
	
	
	//============================================================
	//注册间隔和闪烁处理
	if(Dev.stu==DEV_REGISTER)
	{
				if(Reg.wait==0)
				{
						Reg.time=Reg.cnt*4;
						if(Reg.cnt<245)Reg.cnt++;
						Reg.wait=1; 
				}
				else
				{
						if(Reg.time>0)Reg.time--;
				}
	}
	if((Dev.stu==DEV_REGISTER)&&(Reg.time==0)&&(Reg.fullflag==0))
	{
				IT1_Dev_Register();
				Reg.wait=0;
	}
	//============================================================================
	//如果是锁，必须上报一次状态
	if((lock.report==0)&&(Dev.stu>DEV_REGISTER))
	{
				//1.查询无线锁的状态
				//2.实际执行时有无线转发上报
				IT1_Lock_DataLoad(0,0x01,0x01);
		    //上报指纹使能和禁止状态
		    for(i=0;i<8;i++)
				{
					  lin=i/8;
						row=i%8;
						if((Pannel.item[0].onoffbuf[lin]&(0x01<<row))!=0x00)
						{
									DlyEvent_handle(i+1,0x04,0x01,1000);
						}
				}
	}
	//============================================================================			
	//以下每100mS调用	
	if(t10ms>=10)
	{
		  t10ms=0;
		  Dev.rand++;
			t100ms++;
		   if(lock.setkeytime>0)lock.setkeytime--;
		  if((Dev.err & ERR_RESET)==0x00)IWDG_Feed();										//喂狗100ms
		
		  //===========================================================================
		  //红外配置时间限定
		  if(Ir.sendtime>0)Ir.sendtime--;
		  if((Ir.sendtime==0)&&(Ir.flag==1))Ir.flag=0;
		  //===========================================================================
			//===========================================================================
			//按键判断
			key=bsp_GetKey();
			if(key!=0x00)
			{
							if(key==1)
							{
									//Ir_SetupLoad_Test();			//测试 模拟发送红外数据
								  //控制转发器进入学习54键遥控器状态
									if((Dev.type&0xf000)==0x3000)
									{
												IT1_LearnLock();
									}
									else
									{
												//IT1_Learn54Key();
												RegTimeClr();
												Dev.stu=DEV_REGISTER;				//重新注册
									}
							}
							else if(key==3)
							{
									
								  //54键遥控器也有本身删除
									//IT1_IR_UnLoadToWireless(0xff,0xff);
									if((Dev.type&0xf000)==0xb000)
									{
											//删除本身表格
											Init_Pannel_Table();
											//删除地址
											Init_Ex_Table(ALL);
									}
									else if((Dev.type&0xf000)==0x3000)
									{
											//删除锁表格
											Init_Lock_Table(ALL);
									}
								  Dev.learn_time=0;
									Dev.encode_time=0;
									Dev.stu=DEV_NORMAL;
								  Led_set(0,25,25,8,1);
								  Led_set(1,25,25,8,2);
							}
		  }
			//=============================================================================
			//延时处理
			IT1_Dlytime_Handle();
			//=============================================================================
			//信号缓冲处理 100mS
			Msg_hanndle();
			//=============================================================================
			//以下每1000mS处理
		  if(t100ms>=10)
			{
						t100ms=0;		
				    //======================================================================
				    //时间累积
						ttest++;
				    if(ttest>=2)										//测试用
						{
							    ttest=0;
									//Wireless_Data_Send(0,0);
									//wl_test_send();  //ceshi
									//Wireless_Lock_Action(0,0xcf);
						}
				    //=======================================================================
				    if(lock.waittime>0)lock.waittime--;
						if(Dev.power_time<100)Dev.power_time++;						// 上电时间			
				    if(Dev.encode_time>0)Dev.encode_time--;						//对码时间
						if(Dev.learn_time>0)Dev.learn_time--;							//学码时间			
						if((Dev.stu==DEV_SETUP)&&(Dev.encode_time==0)&&(Dev.learn_time==0))
						{
								Dev.stu=DEV_NORMAL;
								Led_set(1,25,25,1,2);	
						}
						//=======================================================================
			}

	}
}

/*****************************  (END OF FILE) *********************************/
