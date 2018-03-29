/*
*********************************************************************************************************
*
*	模块名称 : 时间触发模式
*	文件名称 : Time_Triggered.h
*	版    本 : V1.0
*	说    明 : 时间触发嵌入式模式头文件
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

#ifndef __TIME_TRIGGERED_H
#define __TIME_TRIGGERED_H


#define	 hSCH_MAX_TASKS     4


typedef unsigned char    tByte;
typedef unsigned int     tWord; 

typedef  struct
{
	void (*pTask)();	   		  /*指向任务的指针必须是一个*void(void)*函数；*/
	
	tWord Delay;  		   		  /*延时（时标）知道下一个函数的运行*/
	
	tWord Period;		   		  /*连续运行之间的间隔*/
	
	tByte RunMe;		   		 /*当任务需要运行的时候由调度器加1*/

    tByte Co_op;		   		 /*混合式调度和合作式调度的区分 合作式1 抢占式0*/
}sTask;

void   hSCH_Update(void);
tByte  hSCH_Add_Task(void (*pFuntion)(void),
				  tWord DELAY,
				  tWord PERIOD,
				  tByte Co_op);
tByte  hSCH_Task_Delete(tByte TASK_INDEX);
void   hSCH_Dispatch_Tasks(void);  

extern sTask hSCH_task_G[hSCH_MAX_TASKS];  /*建立的任务数*/ 

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
