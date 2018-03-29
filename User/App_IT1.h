//============================================================================
//杜亚智能家居-IT-一代协议
#ifndef _APP_IT1_H
	#define _APP_IT1_H

//===============================================================================
//函数定义
unsigned char IT1_RS4852Wireless(unsigned char pa1,unsigned char pa2);
void IT1_ParseFrame(void);
void IT1_RS485_DataLoad(uint8_t index,uint8_t cmd,uint8_t dat);
void IT1_RS485_StuLoad(uint8_t index,uint8_t len,uint8_t *pdat);
void IT1_RS485_Report(uint8_t index);
uint8_t  IT1_PraseASK(void);
void IT1_Dev_Register(void); 
void IT1_Dev_Declare(uint8_t index,uint8_t cmd);
void IT1_Key_Handle(void);
void IT1_LearnDevWLR(void);
void IT1_IrSetup_Report(uint8_t st);
void IT1_Learn54Key(void);
void IT1_LearnLock(void);
void IT1_Key_DataLoad(uint8_t index,uint8_t cmd,uint8_t dat);
void IT1_Sensor_DataLoad(uint8_t index,uint8_t *pdat);
void IT1_IrDel_Report(uint8_t index);
void IT1_IR_UnLoadToWireless(uint8_t chl,uint8_t key,uint8_t mode);
void IT1_IR_LoadToWireless(uint8_t chl);
void IT1_IR_StuLoad(uint8_t index,uint8_t len,uint8_t *pdat);
void IT1_SendData(void);
void IT1_Dev_StuLoad(uint8_t index);
void IT1_Dlytime_Handle(void);
void IT1_Turn_DataLoad(uint8_t index,uint8_t cmd,uint8_t dat);
void IT1_Lock_DataLoad(uint8_t index,uint8_t type,uint8_t dat);
//============================================================================================


#endif

//============================================================================================


































    



