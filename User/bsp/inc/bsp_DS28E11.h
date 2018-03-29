/*
*********************************************************************************************************
*
*	模块名称 : 按键驱动模块
*	文件名称 : bsp_switch.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
模块共有10位拨码开关

*********************************************************************************************************
*/

#ifndef __BSP_24E11_H
#define __BSP_24E11_H

#define uint8_t unsigned char
#define bool    unsigned char


/* 供外部调用的函数声明 */

void 	TEST_28E11(uint8_t *pDatIn,uint8_t *pDatOut);
void bsp_Init28E11(void);
unsigned char Read_RomID(unsigned char *RomID);
unsigned char ComputeAndReadPageMAC(unsigned char Anonymous_Mode,unsigned char PageNum,unsigned char *MAC_Value);
unsigned char ReadWriteScratchpad(bool ReadMode, unsigned char *Buffer);
unsigned char   WriteMemory(unsigned char Segment, unsigned char Page,unsigned char *Buffer);
#endif


