/*
*********************************************************************************************************
*
*	ģ������ : ʱ�䴥��ģʽ
*	�ļ����� : Time_Triggered.h
*	��    �� : V1.0
*	˵    �� : ʱ�䴥��Ƕ��ʽģʽͷ�ļ�
*
*	�޸ļ�¼ :
*		�汾��    ����         ����            ˵��
*       V1.0    2015-10-08   Eric2013    1. ST�̼��⵽V3.6.1�汾
*                                        2. BSP������V1.2
*
*	Copyright (C), 2015-2020, ���������� www.armfly.com
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
	void (*pTask)();	   		  /*ָ�������ָ�������һ��*void(void)*������*/
	
	tWord Delay;  		   		  /*��ʱ��ʱ�֪꣩����һ������������*/
	
	tWord Period;		   		  /*��������֮��ļ��*/
	
	tByte RunMe;		   		 /*��������Ҫ���е�ʱ���ɵ�������1*/

    tByte Co_op;		   		 /*���ʽ���Ⱥͺ���ʽ���ȵ����� ����ʽ1 ��ռʽ0*/
}sTask;

void   hSCH_Update(void);
tByte  hSCH_Add_Task(void (*pFuntion)(void),
				  tWord DELAY,
				  tWord PERIOD,
				  tByte Co_op);
tByte  hSCH_Task_Delete(tByte TASK_INDEX);
void   hSCH_Dispatch_Tasks(void);  

extern sTask hSCH_task_G[hSCH_MAX_TASKS];  /*������������*/ 

#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
