/*
*********************************************************************************************************
*
*	模块名称 : 时间触发模式
*	文件名称 : Time_Triggered.C
*	版    本 : V1.0
*	说    明 : 时间触发嵌入式模式的实现
*
*	修改记录 :
*		版本号    日期         作者            说明
*       V1.0    2015-10-08   Eric2013    1. ST固件库到V3.6.1版本
*                                        2. BSP驱动包V1.2
*
*	Copyright (C), 2015-2020, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
#include "Time_Triggered.h"
#include "stm32f10x.h"


#define  RETURN_ERROR                     0x00;
#define  RETURN_NORMOL                    0x01;
#define  ERROR_SCH_CANOT_DELETE_TASK  	  0x02;
#define  ERROR_SCH_TOO_MANY_TASKS	  0x03;

tByte Error_code_G;

sTask hSCH_task_G[hSCH_MAX_TASKS]; /*建立的任务数*/

extern void AppTask_RivalrySend(void);

/*
*********************************************************************************************************
*	函 数 名: hSCH_Update(void)
*	功能说明: 调度器的刷新函数，每个时标中断执行一次。在嘀嗒定时器中断里面执行。
			  当刷新函数确定某个任务要执行的时候，将RunMe加1，要注意的是刷新任务
			  不执行任何函数，需要运行的任务有调度函数激活。
			  抢占式任务需要执行的话，立即就会得到执行。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void hSCH_Update(void)
{
	tByte index;
	/*注意计数单位是时标，不是毫秒*/
	for(index = 0; index < hSCH_MAX_TASKS; index++)
	{
		if(hSCH_task_G[index].pTask)				   /*检测这里是否有任务*/
		{
			if(hSCH_task_G[index].Delay == 0)
			{
				if(hSCH_task_G[index].Co_op)
				{
					//hSCH_task_G[index].RunMe += 1;  /*合作式任务需要运行 将RunMe置1*/	
					if(hSCH_task_G[index].RunMe<200)hSCH_task_G[index].RunMe += 1;
				}	
				else
				{
					(*hSCH_task_G[index].pTask)();   /*抢占式任务需要立即运行 将RunMe减1*/	
										
					//hSCH_task_G[index].RunMe -= 1;
					if(hSCH_task_G[index].Period == 0)
					{						       /*单次执行的任务 则将其清除*/
						hSCH_task_G[index].pTask = 0;									
					}
				}
				if(hSCH_task_G[index].Period)
				{						           /*调度周期性的任务再次执行*/
					hSCH_task_G[index].Delay = hSCH_task_G[index].Period;									
				}
			}
			else
			{							              /*还有准备好运行*/
				hSCH_task_G[index].Delay -= 1;															
			}
		}		
	}
}

/*
*********************************************************************************************************
*	函 数 名: hSCH_Add_Task
*	功能说明: 添加任务。
*	形    参：void (*pFuntion)(void) tWord DELAY tWord PERIOD
*	返 回 值: 返回任务的ID号
*   使用说明：								   0 表示是抢占式任务  1 表示是合作式任务
*   （1）SCH_Add_Task(DOTASK,1000,0,1) DOTASK是函数的运行地址，1000是1000个时标以后开始运行，只运行一次；
*	（2）Task_ID = SCH_Add_Task(DOTASK,1000,0,1); 将任务标示符保存 以便以后删除任务 
*	（3）SCH_Add_Task(DOTASK,0,1000,1); 每个1000个时标周期性的运行一次；
*********************************************************************************************************
*/
tByte hSCH_Add_Task(void (*pFuntion)(void),
							tWord DELAY,
							tWord PERIOD,
							tByte Co_op)	  /*任务合作式和抢占式的区分*/
{
	tByte index = 0; /*首先在队列中找到一个空隙，（如果有的话）*/
	
	while((hSCH_task_G[index].pTask != 0) && (index <hSCH_MAX_TASKS))
	{
		index ++;		
	}
	if(index == hSCH_MAX_TASKS)/*超过最大的任务数目 则返错误信息*/
	{
		Error_code_G = ERROR_SCH_TOO_MANY_TASKS;/*设置全局错误变量*/
		return hSCH_MAX_TASKS;	
	}
	
	hSCH_task_G[index].pTask = pFuntion;	/*运行到这里说明申请的任务块成功*/
	hSCH_task_G[index].Delay = DELAY;
	hSCH_task_G[index].Period = PERIOD;
	hSCH_task_G[index].RunMe = 0;
	hSCH_task_G[index].Co_op = Co_op;
	
	return index;					   /*返回任务的位置，以便于以后删除*/
}
/*
*********************************************************************************************************
*	函 数 名: hSCH_Task_Delete
*	功能说明: 删除任务。
*	形    参：tByte index
*	返 回 值: 是否删除成功
*********************************************************************************************************
*/
tByte hSCH_Task_Delete(tByte index)
{
	tByte Return_code;
	
	if(hSCH_task_G[index].pTask == 0)				   /*这里没有任务*/
	{
		Error_code_G = ERROR_SCH_CANOT_DELETE_TASK;/*设置全局错误变量*/
		Return_code  = RETURN_ERROR;
	}
	else
	{
		Return_code  = RETURN_NORMOL;		
	}
	
	hSCH_task_G[index].pTask = 0x0000;
	hSCH_task_G[index].Delay = 0;
	hSCH_task_G[index].Period = 0;
	hSCH_task_G[index].RunMe =0;
	
	return Return_code;						/*返回状态*/
}
/*
*********************************************************************************************************
*	函 数 名: hSCH_Dispatch_Tasks
*	功能说明: 在主任务里面执行的调度函数。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void hSCH_Dispatch_Tasks(void)
{
	tByte index;
	/*运行下一个任务，如果下一个任务准备就绪的话*/
	for(index = 0; index < hSCH_MAX_TASKS; index++)
	{
		if((hSCH_task_G[index].RunMe >0) && (hSCH_task_G[index].Co_op))	/*只调度合作式任务*/
		{
			(*hSCH_task_G[index].pTask)();     /*执行任务 */
			hSCH_task_G[index].RunMe -= 1;	   /*执行任务完成后，将RunMe减一 */
			if(hSCH_task_G[index].Period == 0) /*如果是单次任务的话，则将任务删除 */
			{
				hSCH_Task_Delete(index);
			}
		}
	}
  //=======================================================================================================	
  //此任务要求不停运行，并且间隔时间短
  AppTask_RivalrySend();								 //竞争发送		
}


/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
