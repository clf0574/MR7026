 #ifndef _FUNC_H
	#define _FUNC_H

	  uint8_t Check_SelfSend(uint8_t *pt,uint8_t cnt);
	  uint8_t GetRandTime(void);
	  uint8_t  Crc8CheckSum(uint8_t *ptr,uint8_t length);
  	void Dev_Init(void);
		
		void Init_Ex_Table(uint8_t st);	
  	uint8_t Read_Ex_Table(void);
  	uint8_t Write_Ex_Table(uint8_t index);
		
		
		uint8_t Read_Pannel_Table(void);																	//用于保存遥控器信息
  	uint8_t Write_Pannel_Table(uint8_t index);                        //用于保存遥控器信息
		
		void Init_Scene_Table(void);
		uint8_t Read_Scene(uint8_t dev_index);
		void Write_Scene(uint8_t dev_index,uint8_t sc_index);
		
  	uint8_t Write_Dev(uint8_t dat,uint8_t type);																				//清除初始化表格
  	uint8_t Read_Dev(uint8_t type);
		void Get_SerialNum(uint8_t *pt);

		void Ir_SetupLoad_Test(void);
		
		//void Get_WLR_Data(uint16_t dat,uint8_t *pt);
		
		void ReadWriteKEY(uint8_t *pt,uint8_t flag);
		
		void Ir_Config_Del(uint8_t index,uint8_t key,uint8_t mode);
		void Ir_Config_Add(uint8_t index,uint8_t key,uint8_t mode);
		
		void Init_Pannel_Table(void);
		void Pannel_Table_Refresh(uint8_t index,uint8_t serial,uint8_t *pdat);

		void RegTimeClr(void);
		void Remote_Key_Dis(uint8_t index,uint8_t st);
		void Remote_Key_En(uint8_t index,uint8_t st);
		void Msg_hanndle(void);
		void DlyEvent_handle(uint8_t channel,uint8_t type,uint8_t dat,uint16_t time);
		
		void Init_Lock_Table(uint8_t st);
		uint8_t Write_Lock_Table(uint8_t index);
		uint8_t Read_Lock_Table(void);
		
		//=======================================================================================================
//以下加密用函数
unsigned char GetSecret(unsigned char *pdat,unsigned char len);
void ComposePacket(unsigned char *pdat,unsigned char len,unsigned char index);
void InfoDecode(unsigned char *pdat,unsigned char len,unsigned char key);
void InfoEncode(unsigned char *pdat,unsigned char len,unsigned char key);
void ComposePacketFromDS28E02(unsigned char *pid,unsigned char *pdat,unsigned char len,unsigned char st);
unsigned char CompareSync(unsigned char *pdat,unsigned char index);
void Secret_Verify(unsigned char *pdat,unsigned char len);
/*========================================================================*/
#endif



