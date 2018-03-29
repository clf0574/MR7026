//============================================================================
//杜亚智能家居-IT-一代协议
#ifndef _APP_WIRELESS_H
	#define _APP_WIRELESS_H

//#define uint8_t CPU_INT08U
//===============================================================================
//函数定义
	void Wireless_Send(void);
  void Wireless_Recv(void);
  uint8_t Wireless_Dev_Action(uint8_t index,uint8_t cmd,uint8_t route,uint8_t scene_num);
  uint8_t Wireless_Data_Send(uint8_t *pt,uint8_t num);  
	uint8_t  SumCheck(uint8_t  *p,uint8_t length);
	uint8_t Wireless_Control_IR(uint8_t index,uint8_t len,uint8_t *pdat);
	void wl_wakeup_frame(uint8_t index);
	void wl_test_send(void);
	uint8_t Wireless_Lock_Action(uint8_t index,uint8_t cmd);
#endif

//============================================================================================


































    



