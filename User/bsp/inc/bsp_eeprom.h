/*
*********************************************************************************************************
*
*	模块名称 : 按键驱动模块
*	文件名称 : bsp_button.h
*	版    本 : V2.0
*	说    明 : 头文件
*
*
*********************************************************************************************************
*/
#ifndef __BSP_EEPROM_H
#define __BSP_EEPROM_H

// #define uint16_t CPU_INT16U
// #define uint8_t CPU_INT08U


//外部调用函数
void bsp_EepromInit(void);		
uint8_t ReadDataFromFlash(uint16_t eep_addr,uint8_t* p_data,uint16_t n);
uint8_t WriteDataToFlash(uint16_t eep_addr,uint8_t* p_data,uint16_t n);
uint8_t FillDataToFlash(uint8_t FData,uint16_t eep_addr,uint16_t n);
uint8_t InsertDataToFlash(uint8_t *p_data,uint16_t eep_addr,uint16_t n);				//不用
uint8_t DeleteDataToFlash(uint16_t eep_addr,uint16_t n);												//不用
uint8_t CopyDataInFlash(uint16_t SSAddres,uint16_t DSAddres,uint16_t n);
void EraseAllFlash(void);
void EraseOneFlash(uint8_t i);
void I2C_WP_SET(uint8_t io_set);

//测试函数
void TestWRFlash(void);

//分配表格
#endif

/******************* END OF FILE********************/



