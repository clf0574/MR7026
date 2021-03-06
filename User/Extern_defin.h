
//============================================================================================
#ifndef _main_h
#define _main_h

	#ifdef MAIN_VARIABLE
		#define APP_EXT   
	#else
		#define APP_EXT extern       
	#endif
 
 //===========================================================================================
 //2017.5.12 增加长按设置键，清除本身表格，并发送删除54键遥控器所以配置的指令
   
 //============================================================================================  
 
 #include "my_queue_chl.h"
 #define Uint8 uint8_t
 #define Uint16 uint16_t	
 
 //============================================================================================ 
 //============================================================================================
 //取得唯一序列号方式
 //#define USE_NEWREGISTER
 //#define GET_SERIAL_FROM_CHIP_ID
 #define GET_SERIAL_FROM_DS28E02
 //#define USE_3MATCH								//连续3次收到，表示收到对码信号。否则为1次
 
 //以下两个二选一
 #define USE_A7139
 //#define USE_CC1101
 //定义是否使用扩频
 #define USE_DSSS_MODE
 
 //以下两个二选一,使用时注意要加载对应的.c文件,并且在bsp.h中选择对应的.h文件
 //#define USE_DS28E02
 #define USE_DS28E11
 //#define USE_LOCK_BY_DS28E11
 
 //使用串口测试功能
 //#define USE_SERIAL_DEBUG
 //============================================================================================
 //===========================================================================================================
 //转发器 转发表结构
 //转发表写入，是按条目写入的，修改这里需修改Write_Ex_Table函数
 //转发表通道，或可转换设备数目
 #define EX_TABLE_NUM         8 
 #define CHL_NUM              EX_TABLE_NUM
 //============================================================================================
 //协议定义 
 #define IT1         	        0 
 #define IT2 	 	       				2
 #define FREE1 		            1 
 
 
 //IT1协议中数据格式
 #define S_FM			0
 #define S_SEH			1
 #define S_TADD		2
 #define S_SEL			3
 #define S_ID			4
 #define S_CMD		5
 #define S_DATLEN	6
 #define S_DAT  	7
 //无线帧格式位置
 #define WL_LEN								0
 #define WL_FT								1
 #define WL_TGADDR0					2
 #define WL_TGADDR1					3
 #define WL_TGADDR2					4
 #define WL_TGCHL						5
 #define WL_SRADDR0					6
 #define WL_SRADDR1					7
 #define WL_SRADDR2					8
 #define WL_SRCHL						9
 #define WL_SYNCH							10
 #define WL_SYNCL							11
 #define WL_CMD								12
 #define WL_DAT               13
 #define WL_CRC								14
 #define WL_SUM								15
 //==============================================================================================
 //转发器在杜亚IT2的全局定义
 #define DEV_IT2_VER     100
 
 //==============================================================================================
 //设备(转发器)运行状态 注册态 设置态（对码或学习），正常运行态
 #define DEV_REQUESTADDR        0
 #define DEV_REGISTER           DEV_REQUESTADDR+1			//1
 #define DEV_NORMAL             DEV_REGISTER+1				//2
 #define DEV_SETUP              DEV_NORMAL+1					//3
 #define DEV_SETUP_OK           DEV_SETUP+1						//4
 
 
 
 
  //????,????
 #define ERR_EEPROM       0x80
 #define ERR_WIRELESS     0x40
 #define ERR_UART         0x20
 #define ERR_POWER        0x10                         //????,??
 #define ERR_RESET				0x08
 
 //===============================================================================================

 //#define REG_START_ADDR         0x30			//寄存器
 //#define REG_MODE1_ADDR         0x2f                    //是否透传(默认为透传，主动上报,或者寄存器读取)
 
 //==============================================================================================
 //默认网关地址,用于IT系统，对开放485协议不存在
 #define Master_Address 	0x00  //网关地址
 
 //==============================================================================================
 //程序返回状态，错误，正确，忽越
 #define RE_ERR 0xFF				//0xff
 #define RE_OK  RE_ERR-1			//0xfe
 #define RE_CANCEL RE_OK-1			//0xfd
 //程序参数 正确 错误
 #define OPT_ERR RE_CANCEL-1			//0xfc
 #define OPT_OK  OPT_ERR-1			//0xfb
 #define OPT_CANCEL OPT_OK-1
  #define OPT_ERR_SETUP		0xf9 
 
  #define OPT_SETKEY              0xf0
 
 //全局变量，错误标记
 #define ERR_EEPROM       0x80
 #define ERR_WIRELESS     0x40
 #define ERR_UART         0x20
 #define ERR_POWER        0x10                         //电压错误，低压
 //=================================================================================================
 //相对无线设备来说
 #define EX_DEV_MOTOR_R    0x02
 #define EX_DEV_MOTOR_E    0x04
 #define EX_DEV_BLIND_R    0x12
 #define EX_DEV_BLIND_E    0x11
 #define EX_DEV_SOCKET     0x21
 #define EX_DEV_LIGHT      0x31
 #define EX_DEV_LIGHT_ADJ  0x33
 
 
 #define EX_DEV_MEDIA			 0xd0
 
 //传感器类
 #define EX_DEV_SEN        0xe0
 #define EX_DEV_WSR    		 0xe0
 #define EX_DEV_WIND    	 0xe1
 #define EX_DEV_SUN     	 0xe2
 #define EX_DEV_RAIN       0xe3
  #define EX_DEV_SUN1      0xe4 //数值亮

 
 #define EX_DEV_KEY        0xf0
 #define EX_KEY_SCENE  		 0xf0
 #define EX_KEY_MOTOR  		 0xf1
 #define EX_KEY_LIGHT  		 0xf2
 #define EX_KEY_GENERAL 	 0xf3
 
 #define EX_KEY_SCENE1  	0xf5
 #define EX_KEY_MOTOR1  	0xf6
 #define EX_KEY_LIGHT1  	0xf7
 #define EX_KEY_NORMAL1 	0xf8
 #define EX_KEY_FULL1   	0xf9
 
 #define EX_KEY_54KEY      0xfa
 #define EX_KEY_42KEY      0xfb
 #define EX_KEY_TURN       0xfc
 //================================================================================================
 #define LEARN_TIME_MAX 	10
 #define ENCODE_TIME_MAX 	5
 
 //===============================================================================================

 //===============================================================================================
 //以下全局变量定义
 //设备数据结构
 typedef struct __Dev
 {
  //公用部分
	uint8_t   profile;									//使用的485协议
	uint8_t   flag;											//bit0-低压标记 bit1-是否写EPROM标记 bit2-转发表备份标记
	uint8_t   addr[2];									//设备地址,IT协议用addr[0],开放485协议addr[0]-ID_L,addr[1]-ID_H
	uint16_t  type;										  //设备类型
  uint8_t   stu;										  //设备工作状态
	uint8_t   rand;										  //随机数
	uint8_t   serial;                   //发送序列号
	uint8_t   power_time;								//上电时间
	uint8_t   wireless_busy;						//无线忙�
	uint8_t   learn_time;
	uint8_t   encode_time;							//
	uint8_t   mode;									    ///通讯模式，透传或缓冲
	uint8_t   err;									    //错误状态
	uint16_t  sync;	
 }_Dev;
 APP_EXT _Dev Dev;
 
 typedef struct __Exchange
 {
		uint8_t    type;								        								//通用随身贴，全功能随身贴，电机，照度传感器等
		uint8_t    addr;																				//其实就是通道号
		uint8_t    stu;    																			//电机正向或反向 等信息                    						
		uint8_t    wl_addr[3];
		uint8_t    channel;		
		uint8_t    wltype;																			//对应无线类型,有无线设备主动上报
		uint8_t    key;																					//对应无线设备的的按键值
		//以下加密时用到
    uint16_t   sync;                                  			//同步字
	  uint8_t    secret;
 }_Exchange;

 typedef struct _Ex_Table
 {
		uint8_t    index;
	  uint8_t    addr[4];																			//无线主机地址
		uint8_t    reg[9];																			//通道注册状态
	  uint8_t    SyncErr[EX_TABLE_NUM];
	  uint16_t   SyncNum[EX_TABLE_NUM];
		uint8_t    stu[EX_TABLE_NUM];					        					//最后一个命令状态
		uint8_t    rtdat[EX_TABLE_NUM];	                        //返回数据,无效0xff,0-100有效，表示灯开关和电机行程		
		_Exchange  Exchange[EX_TABLE_NUM];	                    //转发器通道数据-一共8通道
 }_Ex_Table;

 APP_EXT _Exchange Exchangebuf;							        				//转发器临时数据
 APP_EXT _Ex_Table Ex_Table;
 
 
 //===============================================================================================
 //===============================================================================================
 //以下为锁处理
 #define LOCK_ITEMNUM    16
 #define LOCK_KEYNUM     8
 #define LOCK_ALLNUM     80
 
 //总字节4+5+8+4+16+16+16=9+12+48=21+48=69
 typedef struct _LockItem
 {
		uint8_t    addr[4];								        								
		uint8_t    secret;	
		uint8_t    safe;
		uint8_t    cardnum; 
		uint8_t    encodenum;
		uint8_t    fingernum;
	  uint8_t    key[8];
	  uint16_t   sync;
	  uint16_t   stu;																							//锁的状态
	  uint16_t    Serail[LOCK_ITEMNUM];
	  uint8_t    IdType[LOCK_ITEMNUM];
	  uint8_t    UserType[LOCK_ITEMNUM];
 }_LockItem;

 typedef struct __Lock
 {
		uint8_t    index;
	  uint8_t    report;
	  uint8_t    setkeytime;
	  uint8_t    randkey[8];																	//8个随机密码
	  uint8_t    SyncErr;
	  uint8_t    waittime;										//等待时间
		_LockItem  item;	                   		
 }_Lock;
 
 APP_EXT _Lock lock;
 APP_EXT _LockItem LockItem;
 
 //===============================================================================================
 //场景数据结构-用于转发器
 //共需字节 (16*50)*8=6400个字节
  typedef struct __S_item
 {
		uint8_t    flag;												//使用标记
		uint16_t   id;													//场景号
		uint8_t    chl;                 				//通道号    
		uint8_t    cmd;
        uint8_t    len;
		uint8_t    dat[5];	        						//数据	
		uint16_t   dlytime;											//延时时间	
        uint8_t    sum;	 
 }_S_item;
 
 typedef struct __S_Dev
 {
		uint8_t    num;                 				//场景个数
		_S_item    item[50];            				//一个设备 总共有50个场景
       					                   													        		
 }_S_Dev;
 
 APP_EXT _S_Dev Scene[EX_TABLE_NUM];										//一个转发器 最多有8个设备
 
//===============================================================================================
//====================================================================================
//按键配置表格
#define KEY_NUM   8																			//转发器配置为遥控器时，每一路遥控器最多4键
#define KEY_ITEM_NUM  16																//写入读出的一帧长度
typedef struct __PANNEL_ITEM
{
	uint8_t irflagbuf[8];																	//标记，表示是否使用
	uint8_t  onoffbuf[7];
	//uint8_t type;																				//类型 设备 场景 序列 反向
	//uint8_t tg_id;																			//目标地址
	//uint8_t datalen;																		//数据字节长度
	//uint8_t buf[10];																		//数据
}_PANNEL_ITEM;  

typedef struct __PANNEL_TABLE
{
	uint8_t cnt;		//已配通道总数
	_PANNEL_ITEM item[KEY_NUM];
}_PANNEL_TABLE;

APP_EXT _PANNEL_TABLE Pannel;						//转发器有8个通道，每通道的遥控器 相当于一个面板

//=======================================================================================================
//注册报文所带的所有通道信息
typedef struct __NewRegType_Item
 {
			uint8_t bitpos;
		  uint16_t type;
 }_NewRegType_Item;
 
 typedef struct __NewRegType
 {
			uint8_t num;
		  _NewRegType_Item item[8];
 }_NewRegType;
 
 APP_EXT _NewRegType NewRegType;
 
//========================================================================================================
typedef struct __ARRAY_BUF
{
	uint16_t id;
	uint8_t mode;
	uint16_t dly;
		
}_ARRAY_BUF;

typedef struct __ARRAY
{
	uint8_t flag;
	uint8_t key;
	uint8_t index;
	uint8_t cnt;
	_ARRAY_BUF buf[10];
}_ARRAY;
APP_EXT _ARRAY Array;		//

//===========================================================================================================
//延时处理结构
//需要4个定时器
typedef struct __DELAY_ITEM
{
		uint8_t    flag;												//使用标记
		uint16_t   index;
    uint8_t    cmd;
	  uint8_t    len;
		uint8_t    dat[3];                 				
}_DELAY_ITEM;  

typedef struct __DELAY_TABLE
{
	uint8_t cnt;			
	_DELAY_ITEM item[EX_TABLE_NUM];
}_DELAY_TABLE;

APP_EXT _DELAY_TABLE Dly_Table;	


 //====================================================================================================
 //用于本地时间
 typedef struct __Reg
 {
			uint8_t 	 cnt;
	    uint8_t    wait;
	    uint8_t    fullflag;
	    uint8_t    flashflag;
			uint16_t   time;
 }_Reg;
 APP_EXT _Reg Reg;

 
 //模块信息
 APP_EXT uint8_t IT2_INFO[8];

//========================================================================================================= 

//=========================================================================================================
//以下硬件相关定义=========================================================================================
 //===============================================================================================
 //用于串口
 //接收缓冲区结构
 #define RX_MAX 200
 #define RX_IDLE 0
 #define RX_BUSY 1
 typedef struct __RxDat
 {
	uint8_t cnt;					//接收个数
	uint8_t index;					//接收帧序列号
	uint8_t buf[RX_MAX];                            //接收缓冲区
	uint8_t stu;					//状态0-空闲;1-正在接收;2-等待发送
 }_RxDat;

 APP_EXT _RxDat RxDat;

 //发送缓冲区结构
 #define TX_MAX 100
 #define TX_IDLE 0
 #define TX_BUSY 1
 typedef struct __TxDat
 {
	uint8_t cnt;					//接收个数
	uint8_t index;					//接收帧序列号
	uint8_t f_tx;
	uint8_t buf[TX_MAX];            		//接收缓冲区
	uint8_t stu;					//状态0-空闲;1-正在接收;2-正在处理
	//uint8_t isoffline;				//是否离线
 }_TxDat;

 APP_EXT _TxDat TxDat;

 //=====================================================================================================
 //用于总线检测
 typedef struct __BusStu
 {
			uint8_t 	WaitForSend;
			uint8_t     	SelfSend;
			uint8_t 	time;
			uint8_t 	interval;
    	uint8_t     	randtime;
    	uint16_t     	distime;
    	uint8_t         rxok;				//2016.3.5 add
	    uint8_t   timeout;					//用于超时计时
 }_BusStu;
 APP_EXT _BusStu Bus;
 
 //====================================================================================================
 //用于本地时间
  typedef struct __Time
 {
			uint8_t 	year;
			uint8_t     	month;
			uint8_t 	time;
			uint8_t 	date;
    	uint8_t     	hour;
    	uint8_t     	min;
    	uint8_t         sec;				//2016.3.5 add
 }_Time;
 APP_EXT _Time Time;

 //=====================================================================================================
 #define SEND_TIME_MAX            600  //每100mS递减，发送允许时间最大10S
 #define IR_DAT_MAX               100
 #define IR_TYPE_DAT              0x00
 #define IR_TYPE_KEY              0x40
 #define IR_TYPE_DEL              0x80
 //红外配置下发数据
 typedef struct __Ir
 {
			uint8_t   addr;							//表格通道地址
	    uint8_t   flag;							//标记，=1表示收到总线红外数据，正在下发
	    uint8_t   wait;							//等待发送装入
	    uint8_t   TgAddr[4];					//54键遥控器地址
	    uint8_t   FrameNum;					//设置数据总的帧个数
	    uint8_t   serial;						//发送帧序列号
	    uint8_t   cmd_key;					//对应键值 0xfe 0xff 为测试指令
	    uint8_t   key_mode;					//按键模式
	    uint8_t   type_len;					//bit7 bit6表示类型;bit5-bit0实际数据长度
	    uint8_t   curps;						//当前发送的起始地址
	    uint8_t   curtype;					//当前发送的是遥控器类型或键值
	    uint16_t  sendtime;					//发送时间
	    uint8_t   sendmode;					//发送模式 0-集中发送，最后应答;1-发送-应答-发送
			uint8_t 	typelen;					//类型数据长度
			uint8_t   typebuf[IR_DAT_MAX];			//类型数据缓存
			uint8_t 	keylen;						//按键数据长度
			uint8_t 	keybuf[IR_DAT_MAX];			//按键数据缓存
	    uint8_t   typechm;					//类型数据累加和
	    uint8_t   keychm;						//按键数据累加和
 }_Ir;
 APP_EXT _Ir Ir;
 //=====================================================================================================
 //485发送缓冲队列，无线发送缓冲
 APP_EXT QueueBUF RS485_Queue,WirelessSend_Queue;
 
//==================================================================================================================
//延时处理结构
//需要4个定时器
typedef struct __BUS_DELAY_ITEM
{
	uint8_t flag;			//使用标记
	uint8_t index;		//用于延时发送的帧类型
	uint8_t dat;		  //按键模式
}_BUS_DELAY_ITEM;  

typedef struct __BUS_DELAY_TABLE
{
	uint8_t cnt;			
	_BUS_DELAY_ITEM item[CHL_NUM];
}_BUS_DELAY_TABLE;

APP_EXT _BUS_DELAY_TABLE DlyTable;		//


//================================================================================================
//============================以下可能需要修改====================================================

//================================================================================================
//=================================================================================================
//============================调试模式进入=========================================================
    #define DEBUG			

//=================================================================================================
//=================================================================================================
//============================程序用到常量=========================================================
//=================================================================================================   
  
     
    #define LED_OFF_FOREVER    		0XFF	
    #define LED_ON_FOREVER   			0XFF
    #define LED_LOOP_FOREVER  		0XFF
         
    #define LEARN_DLY_TIME      	8				//学习时间8秒
    #define ENCODE_DLY_TIME     	3				//对码时间2秒
  
    #define TRUE 									1
    #define FALSE									0
    #define NULL    							0
		#define ALL                   0xff
		
	  //=================================================================================	
		//无线协议帧格式
		#define ACK                   0x00
		#define MTD										0x10
		#define DTM                   0x11														//遥控器主动上报
		#define DTM1                  0x12                            //旋转编码遥控器主动上报

		#define MTD_IR_SETUP					0x40														//主机到设备-红外
		#define MTD_WL_SETUP					0x41                            //主机到设置-无线
		#define DTM_IR_SETUP_FINISH   0X42														//设置完成
		#define DTM_WL_SETUP_FINISH		0x43														//设置完成	
		#define MTD_IR_DEL            0x44
		
			


//=================================================================================================
//=================================================================================================
//=================================================================================================  


//=================================================================================================
//=================================================================================================
//============================数据结构和变量定义===================================================
//=================================================================================================   





#endif
//================================================================================================= 
//=======================================================================================
