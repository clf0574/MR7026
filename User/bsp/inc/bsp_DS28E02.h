/*
*********************************************************************************************************
*
*	模块名称 : 加密芯片驱动程序
*	文件名称 : bsp_DS28E15.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*
*********************************************************************************************************
*/
#ifndef __BSP_DS28E02_H
#define __BSP_DS28E02_H




#define bool unsigned char

void DS28E11_Cal(void);
unsigned char Read_RomID(unsigned char *RomID);
unsigned char WriteMemory(unsigned char Segment, unsigned char Page,unsigned char *Buffer);
unsigned char ReadMemory(unsigned char Segment, unsigned char Page,unsigned char *Receive);
unsigned char ReadWriteScratchpad(bool ReadMode, unsigned char *Buffer);
unsigned char ComputeAndReadPageMAC(bool Anonymous_Mode,unsigned char PageNum,unsigned char *MAC_Value);
unsigned char ReadBlockStatus(unsigned char BlockNum,bool PersonalityByteIndicator ,unsigned char *BlockStatus);
//void OW_Delay_us(unsigned char dly);
unsigned char ANON_READ_AuthPage(unsigned char *Receive_buffer, unsigned char* Scratchpad, unsigned char PageNum);

void bsp_DS28E02_Init(void);
#endif
/******************* END OF FILE********************/



