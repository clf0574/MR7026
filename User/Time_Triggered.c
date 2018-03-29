/*
*********************************************************************************************************
*
*	ģ������ : ʱ�䴥��ģʽ
*	�ļ����� : Time_Triggered.C
*	��    �� : V1.0
*	˵    �� : ʱ�䴥��Ƕ��ʽģʽ��ʵ��
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
#include "Time_Triggered.h"
#include "stm32f10x.h"


#define  RETURN_ERROR                     0x00;
#define  RETURN_NORMOL                    0x01;
#define  ERROR_SCH_CANOT_DELETE_TASK  	  0x02;
#define  ERROR_SCH_TOO_MANY_TASKS	  0x03;

tByte Error_code_G;

sTask hSCH_task_G[hSCH_MAX_TASKS]; /*������������*/

extern void AppTask_RivalrySend(void);

/*
*********************************************************************************************************
*	�� �� ��: hSCH_Update(void)
*	����˵��: ��������ˢ�º�����ÿ��ʱ���ж�ִ��һ�Ρ�����શ�ʱ���ж�����ִ�С�
			  ��ˢ�º���ȷ��ĳ������Ҫִ�е�ʱ�򣬽�RunMe��1��Ҫע�����ˢ������
			  ��ִ���κκ�������Ҫ���е������е��Ⱥ������
			  ��ռʽ������Ҫִ�еĻ��������ͻ�õ�ִ�С�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void hSCH_Update(void)
{
	tByte index;
	/*ע�������λ��ʱ�꣬���Ǻ���*/
	for(index = 0; index < hSCH_MAX_TASKS; index++)
	{
		if(hSCH_task_G[index].pTask)				   /*��������Ƿ�������*/
		{
			if(hSCH_task_G[index].Delay == 0)
			{
				if(hSCH_task_G[index].Co_op)
				{
					//hSCH_task_G[index].RunMe += 1;  /*����ʽ������Ҫ���� ��RunMe��1*/	
					if(hSCH_task_G[index].RunMe<200)hSCH_task_G[index].RunMe += 1;
				}	
				else
				{
					(*hSCH_task_G[index].pTask)();   /*��ռʽ������Ҫ�������� ��RunMe��1*/	
										
					//hSCH_task_G[index].RunMe -= 1;
					if(hSCH_task_G[index].Period == 0)
					{						       /*����ִ�е����� �������*/
						hSCH_task_G[index].pTask = 0;									
					}
				}
				if(hSCH_task_G[index].Period)
				{						           /*���������Ե������ٴ�ִ��*/
					hSCH_task_G[index].Delay = hSCH_task_G[index].Period;									
				}
			}
			else
			{							              /*����׼��������*/
				hSCH_task_G[index].Delay -= 1;															
			}
		}		
	}
}

/*
*********************************************************************************************************
*	�� �� ��: hSCH_Add_Task
*	����˵��: ��������
*	��    �Σ�void (*pFuntion)(void) tWord DELAY tWord PERIOD
*	�� �� ֵ: ���������ID��
*   ʹ��˵����								   0 ��ʾ����ռʽ����  1 ��ʾ�Ǻ���ʽ����
*   ��1��SCH_Add_Task(DOTASK,1000,0,1) DOTASK�Ǻ��������е�ַ��1000��1000��ʱ���Ժ�ʼ���У�ֻ����һ�Σ�
*	��2��Task_ID = SCH_Add_Task(DOTASK,1000,0,1); �������ʾ������ �Ա��Ժ�ɾ������ 
*	��3��SCH_Add_Task(DOTASK,0,1000,1); ÿ��1000��ʱ�������Ե�����һ�Σ�
*********************************************************************************************************
*/
tByte hSCH_Add_Task(void (*pFuntion)(void),
							tWord DELAY,
							tWord PERIOD,
							tByte Co_op)	  /*�������ʽ����ռʽ������*/
{
	tByte index = 0; /*�����ڶ������ҵ�һ����϶��������еĻ���*/
	
	while((hSCH_task_G[index].pTask != 0) && (index <hSCH_MAX_TASKS))
	{
		index ++;		
	}
	if(index == hSCH_MAX_TASKS)/*��������������Ŀ �򷵴�����Ϣ*/
	{
		Error_code_G = ERROR_SCH_TOO_MANY_TASKS;/*����ȫ�ִ������*/
		return hSCH_MAX_TASKS;	
	}
	
	hSCH_task_G[index].pTask = pFuntion;	/*���е�����˵������������ɹ�*/
	hSCH_task_G[index].Delay = DELAY;
	hSCH_task_G[index].Period = PERIOD;
	hSCH_task_G[index].RunMe = 0;
	hSCH_task_G[index].Co_op = Co_op;
	
	return index;					   /*���������λ�ã��Ա����Ժ�ɾ��*/
}
/*
*********************************************************************************************************
*	�� �� ��: hSCH_Task_Delete
*	����˵��: ɾ������
*	��    �Σ�tByte index
*	�� �� ֵ: �Ƿ�ɾ���ɹ�
*********************************************************************************************************
*/
tByte hSCH_Task_Delete(tByte index)
{
	tByte Return_code;
	
	if(hSCH_task_G[index].pTask == 0)				   /*����û������*/
	{
		Error_code_G = ERROR_SCH_CANOT_DELETE_TASK;/*����ȫ�ִ������*/
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
	
	return Return_code;						/*����״̬*/
}
/*
*********************************************************************************************************
*	�� �� ��: hSCH_Dispatch_Tasks
*	����˵��: ������������ִ�еĵ��Ⱥ�����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void hSCH_Dispatch_Tasks(void)
{
	tByte index;
	/*������һ�����������һ������׼�������Ļ�*/
	for(index = 0; index < hSCH_MAX_TASKS; index++)
	{
		if((hSCH_task_G[index].RunMe >0) && (hSCH_task_G[index].Co_op))	/*ֻ���Ⱥ���ʽ����*/
		{
			(*hSCH_task_G[index].pTask)();     /*ִ������ */
			hSCH_task_G[index].RunMe -= 1;	   /*ִ��������ɺ󣬽�RunMe��һ */
			if(hSCH_task_G[index].Period == 0) /*����ǵ�������Ļ���������ɾ�� */
			{
				hSCH_Task_Delete(index);
			}
		}
	}
  //=======================================================================================================	
  //������Ҫ��ͣ���У����Ҽ��ʱ���
  AppTask_RivalrySend();								 //��������		
}


/***************************** ���������� www.armfly.com (END OF FILE) *********************************/