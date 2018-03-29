/*
*********************************************************************************************************
*
*	ģ������ : ��������ģ��
*	�ļ����� : bsp_switch.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
ģ�鹲��10λ���뿪��

*********************************************************************************************************
*/

#ifndef __BSP_24E11_H
#define __BSP_24E11_H

#define uint8_t unsigned char
#define bool    unsigned char


/* ���ⲿ���õĺ������� */

void 	TEST_28E11(uint8_t *pDatIn,uint8_t *pDatOut);
void bsp_Init28E11(void);
unsigned char Read_RomID(unsigned char *RomID);
unsigned char ComputeAndReadPageMAC(unsigned char Anonymous_Mode,unsigned char PageNum,unsigned char *MAC_Value);
unsigned char ReadWriteScratchpad(bool ReadMode, unsigned char *Buffer);
unsigned char   WriteMemory(unsigned char Segment, unsigned char Page,unsigned char *Buffer);
#endif


