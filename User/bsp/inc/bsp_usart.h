/*
*********************************************************************************************************
*	                                  
*	ģ������ : ��������ģ��    
*	�ļ����� : bsp_usart.h
*	��    �� : V2.0
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2010-2011, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __BSP_USART_H
#define __BSP_USART_H

#ifdef   BSP_SER_MODULE
#define  BSP_SER_EXT
#else
#define  BSP_SER  extern
#endif

//  #define uint8_t CPU_INT08U

 void bsp_InitUart(uint8_t rate);
 void RS485_Send(uint8_t *buff,uint8_t len);
 void Bsp_Get_UsartData(uint8_t *p,uint8_t *pt);

#endif


