//============================================================================

#ifndef _APP_IT2_H
	#define _APP_IT2_H

//============================================================================
//帧命令定义
#define B_ONLINE   		0x01
#define B_DIS_BUS   		0x02
#define B_EN_BUS   		0x03
#define B_EXEC_SCENE		0x04
#define B_DEL_SCENE		0x05
#define F_ASK_TIME		0x08
#define F_ACK_TIME		0x09
#define F_SET_TIME		0x0A
#define F_ASK_VERSION		0x0B
#define F_ACK_VERSION		0x0C

#define F_ASK_UPDATE		0x0D
#define F_ACK_UPDATE		0x0E
#define F_ASK_UPDATE_START	0x0F
#define F_ACK_UPDATE_FINISH	0x10

#define F_ASK_CONFIG_ADDR	0x20
#define F_ASK_CONFIG_MODULE	0x21
#define F_DEL_MODULE_CONFIG	0x23
#define F_REPORT_MODULE_DEL	0x24

#define F_ASK_GATEWAY_DATA	0x30
#define F_ACT_DEVICE 	        0x31
#define F_REPORT_STATUS 	0x32
#define F_REPORT_ONLINE		0x33

#define F_ASK_MODIFY		0x34
#define F_ASK_INFO		0x35
#define F_ASK_CONFIG_KEY	0x36
#define F_ASK_SYNC_KEY		0x37
#define F_SETUP_DEV_PARA	0x38
#define F_READ_DEV_PARA		0x39
#define F_REPORT_DEV_PARA	0x3A
#define F_REPORT_DEV_MODIFY	0x3B
#define F_ACK_DEV		0x3E

#define F_ACK_REQUEST_ADDR	0x3F
#define F_ASK_EXCHANGER_ADDCODE	0x40
#define F_ASK_EXCHANGER_DELCODE	0x41

#define F_REPORT_DEV_DEL		0x43

#define F_ACT_SCENE		0x50
#define F_ASK_SCENE_DATA	0x51
#define F_ACK_SCENE_DATA	0x52
#define F_ASK_SCENE_ADD		0x53
#define F_ASK_SCENE_DEL		0x55

#define F_ASK_TIMER_ADD		0x60
#define F_ASK_TIMER_EDIT	0x61
#define F_ASK_TIMER_DEL		0x62
#define F_ASK_TIMER_ONOFF	0x63

#define F_ASK_ARRAY_ADD		0x70
#define F_ASK_ARRAY_DEL		0x71

////系统命令
//#define C_ONLINE_B 	      	0X01
//#define C_USELINE_B           0X02
//#define C_LEAVELINE_B         0x03
//#define C_SCENE_B             0X04
//#define C_ONLINE              0X10
//#define C_STATUS              0X11
//#define C_REQUEST_ADDR	0X20
//#define C_ASK_ADDR            0X21
//#define C_REGISTER            0X22
//#define C_ASK                 0X23
//
//#define C_MODIFY_CHANNEL      0X26
//#define C_RETURN_MODIFY       0X27
//
//#define C_DEV_ACTION          0X30
//#define C_EDIT                0X31   
//#define C_DATA                0X40
//#define C_ADD_SCENE           0X50
//#define C_DEL_SCENE           0X51  
//#define C_SCENE_ACTION        0X52
//#define C_PANNEL_CONFIG       0X53   

#define MASTER_ADDR           0x00

//====================================================================================
//操作类型定义






//====================================================================================
//函数定义
void IT2_Frame_availability(uint8_t *p);
void IT2_ParseFrame(void);
void IT2_Function_Broadcast(uint8_t *pt);					
void IT2_Function_Time(uint8_t *pt);
void IT2_Function_Version(uint8_t *pt);
void IT2_Function_Update(uint8_t *pt);
void IT2_Function_Config(uint8_t *pt);
void IT2_Dev_Setup(uint8_t *pt);
void IT2_Dev_Action(uint8_t *pt);
void IT2_Key_Setup(uint8_t *pt);
void IT2_Scene_Action(uint8_t *pt);
void IT2_Timer_Action(uint8_t *pt);
void IT2_Array_Action(uint8_t *pt);
void IT2_Exchange_Action(uint8_t *pt);
void IT2_Load_RS485_Ack(uint8_t st);
void IT2_Dev_Report(uint8_t st,uint8_t index);
void IT2_Dev_RequestAddr(void);
void IT2_Dev_Register(void);
uint8_t IT2_PraseASK(uint8_t cmd);
void IT2_Pannel_DataLoad(uint8_t index,uint8_t value);
void IT2_Array_Handle(void);
void IT2_Offline_DataLoad(uint8_t key);
uint8_t IT2_Changer_GetType(uint8_t t1,uint8_t t2);
uint8_t IT2_Dev_To_Wireless(uint8_t index,uint8_t cmd,uint8_t cmd_dat,uint8_t scene_num);

////============================================================================================


#endif

//============================================================================================


































    



