
//============================================================================================
#ifndef _main_h
#define _main_h

	#ifdef MAIN_VARIABLE
		#define APP_EXT   
	#else
		#define APP_EXT extern       
	#endif
 
 //===========================================================================================
 //2017.5.12 ���ӳ������ü�����������񣬲�����ɾ��54��ң�����������õ�ָ��
   
 //============================================================================================  
 
 #include "my_queue_chl.h"
 #define Uint8 uint8_t
 #define Uint16 uint16_t	
 
 //============================================================================================ 
 //============================================================================================
 //ȡ��Ψһ���кŷ�ʽ
 //#define USE_NEWREGISTER
 //#define GET_SERIAL_FROM_CHIP_ID
 #define GET_SERIAL_FROM_DS28E02
 //#define USE_3MATCH								//����3���յ�����ʾ�յ������źš�����Ϊ1��
 
 //����������ѡһ
 #define USE_A7139
 //#define USE_CC1101
 //�����Ƿ�ʹ����Ƶ
 #define USE_DSSS_MODE
 
 //����������ѡһ,ʹ��ʱע��Ҫ���ض�Ӧ��.c�ļ�,������bsp.h��ѡ���Ӧ��.h�ļ�
 //#define USE_DS28E02
 #define USE_DS28E11
 //#define USE_LOCK_BY_DS28E11
 
 //ʹ�ô��ڲ��Թ���
 //#define USE_SERIAL_DEBUG
 //============================================================================================
 //===========================================================================================================
 //ת���� ת����ṹ
 //ת����д�룬�ǰ���Ŀд��ģ��޸��������޸�Write_Ex_Table����
 //ת����ͨ�������ת���豸��Ŀ
 #define EX_TABLE_NUM         8 
 #define CHL_NUM              EX_TABLE_NUM
 //============================================================================================
 //Э�鶨�� 
 #define IT1         	        0 
 #define IT2 	 	       				2
 #define FREE1 		            1 
 
 
 //IT1Э�������ݸ�ʽ
 #define S_FM			0
 #define S_SEH			1
 #define S_TADD		2
 #define S_SEL			3
 #define S_ID			4
 #define S_CMD		5
 #define S_DATLEN	6
 #define S_DAT  	7
 //����֡��ʽλ��
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
 //ת�����ڶ���IT2��ȫ�ֶ���
 #define DEV_IT2_VER     100
 
 //==============================================================================================
 //�豸(ת����)����״̬ ע��̬ ����̬�������ѧϰ������������̬
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

 //#define REG_START_ADDR         0x30			//�Ĵ���
 //#define REG_MODE1_ADDR         0x2f                    //�Ƿ�͸��(Ĭ��Ϊ͸���������ϱ�,���߼Ĵ�����ȡ)
 
 //==============================================================================================
 //Ĭ�����ص�ַ,����ITϵͳ���Կ���485Э�鲻����
 #define Master_Address 	0x00  //���ص�ַ
 
 //==============================================================================================
 //���򷵻�״̬��������ȷ����Խ
 #define RE_ERR 0xFF				//0xff
 #define RE_OK  RE_ERR-1			//0xfe
 #define RE_CANCEL RE_OK-1			//0xfd
 //������� ��ȷ ����
 #define OPT_ERR RE_CANCEL-1			//0xfc
 #define OPT_OK  OPT_ERR-1			//0xfb
 #define OPT_CANCEL OPT_OK-1
  #define OPT_ERR_SETUP		0xf9 
 
  #define OPT_SETKEY              0xf0
 
 //ȫ�ֱ�����������
 #define ERR_EEPROM       0x80
 #define ERR_WIRELESS     0x40
 #define ERR_UART         0x20
 #define ERR_POWER        0x10                         //��ѹ���󣬵�ѹ
 //=================================================================================================
 //��������豸��˵
 #define EX_DEV_MOTOR_R    0x02
 #define EX_DEV_MOTOR_E    0x04
 #define EX_DEV_BLIND_R    0x12
 #define EX_DEV_BLIND_E    0x11
 #define EX_DEV_SOCKET     0x21
 #define EX_DEV_LIGHT      0x31
 #define EX_DEV_LIGHT_ADJ  0x33
 
 
 #define EX_DEV_MEDIA			 0xd0
 
 //��������
 #define EX_DEV_SEN        0xe0
 #define EX_DEV_WSR    		 0xe0
 #define EX_DEV_WIND    	 0xe1
 #define EX_DEV_SUN     	 0xe2
 #define EX_DEV_RAIN       0xe3
  #define EX_DEV_SUN1      0xe4 //��ֵ��

 
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
 //����ȫ�ֱ�������
 //�豸���ݽṹ
 typedef struct __Dev
 {
  //���ò���
	uint8_t   profile;									//ʹ�õ�485Э��
	uint8_t   flag;											//bit0-��ѹ��� bit1-�Ƿ�дEPROM��� bit2-ת�����ݱ��
	uint8_t   addr[2];									//�豸��ַ,ITЭ����addr[0],����485Э��addr[0]-ID_L,addr[1]-ID_H
	uint16_t  type;										  //�豸����
  uint8_t   stu;										  //�豸����״̬
	uint8_t   rand;										  //�����
	uint8_t   serial;                   //�������к�
	uint8_t   power_time;								//�ϵ�ʱ��
	uint8_t   wireless_busy;						//����æ�
	uint8_t   learn_time;
	uint8_t   encode_time;							//
	uint8_t   mode;									    ///ͨѶģʽ��͸���򻺳�
	uint8_t   err;									    //����״̬
	uint16_t  sync;	
 }_Dev;
 APP_EXT _Dev Dev;
 
 typedef struct __Exchange
 {
		uint8_t    type;								        								//ͨ����������ȫ������������������նȴ�������
		uint8_t    addr;																				//��ʵ����ͨ����
		uint8_t    stu;    																			//���������� ����Ϣ                    						
		uint8_t    wl_addr[3];
		uint8_t    channel;		
		uint8_t    wltype;																			//��Ӧ��������,�������豸�����ϱ�
		uint8_t    key;																					//��Ӧ�����豸�ĵİ���ֵ
		//���¼���ʱ�õ�
    uint16_t   sync;                                  			//ͬ����
	  uint8_t    secret;
 }_Exchange;

 typedef struct _Ex_Table
 {
		uint8_t    index;
	  uint8_t    addr[4];																			//����������ַ
		uint8_t    reg[9];																			//ͨ��ע��״̬
	  uint8_t    SyncErr[EX_TABLE_NUM];
	  uint16_t   SyncNum[EX_TABLE_NUM];
		uint8_t    stu[EX_TABLE_NUM];					        					//���һ������״̬
		uint8_t    rtdat[EX_TABLE_NUM];	                        //��������,��Ч0xff,0-100��Ч����ʾ�ƿ��غ͵���г�		
		_Exchange  Exchange[EX_TABLE_NUM];	                    //ת����ͨ������-һ��8ͨ��
 }_Ex_Table;

 APP_EXT _Exchange Exchangebuf;							        				//ת������ʱ����
 APP_EXT _Ex_Table Ex_Table;
 
 
 //===============================================================================================
 //===============================================================================================
 //����Ϊ������
 #define LOCK_ITEMNUM    16
 #define LOCK_KEYNUM     8
 #define LOCK_ALLNUM     80
 
 //���ֽ�4+5+8+4+16+16+16=9+12+48=21+48=69
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
	  uint16_t   stu;																							//����״̬
	  uint16_t    Serail[LOCK_ITEMNUM];
	  uint8_t    IdType[LOCK_ITEMNUM];
	  uint8_t    UserType[LOCK_ITEMNUM];
 }_LockItem;

 typedef struct __Lock
 {
		uint8_t    index;
	  uint8_t    report;
	  uint8_t    setkeytime;
	  uint8_t    randkey[8];																	//8���������
	  uint8_t    SyncErr;
	  uint8_t    waittime;										//�ȴ�ʱ��
		_LockItem  item;	                   		
 }_Lock;
 
 APP_EXT _Lock lock;
 APP_EXT _LockItem LockItem;
 
 //===============================================================================================
 //�������ݽṹ-����ת����
 //�����ֽ� (16*50)*8=6400���ֽ�
  typedef struct __S_item
 {
		uint8_t    flag;												//ʹ�ñ��
		uint16_t   id;													//������
		uint8_t    chl;                 				//ͨ����    
		uint8_t    cmd;
        uint8_t    len;
		uint8_t    dat[5];	        						//����	
		uint16_t   dlytime;											//��ʱʱ��	
        uint8_t    sum;	 
 }_S_item;
 
 typedef struct __S_Dev
 {
		uint8_t    num;                 				//��������
		_S_item    item[50];            				//һ���豸 �ܹ���50������
       					                   													        		
 }_S_Dev;
 
 APP_EXT _S_Dev Scene[EX_TABLE_NUM];										//һ��ת���� �����8���豸
 
//===============================================================================================
//====================================================================================
//�������ñ��
#define KEY_NUM   8																			//ת��������Ϊң����ʱ��ÿһ·ң�������4��
#define KEY_ITEM_NUM  16																//д�������һ֡����
typedef struct __PANNEL_ITEM
{
	uint8_t irflagbuf[8];																	//��ǣ���ʾ�Ƿ�ʹ��
	uint8_t  onoffbuf[7];
	//uint8_t type;																				//���� �豸 ���� ���� ����
	//uint8_t tg_id;																			//Ŀ���ַ
	//uint8_t datalen;																		//�����ֽڳ���
	//uint8_t buf[10];																		//����
}_PANNEL_ITEM;  

typedef struct __PANNEL_TABLE
{
	uint8_t cnt;		//����ͨ������
	_PANNEL_ITEM item[KEY_NUM];
}_PANNEL_TABLE;

APP_EXT _PANNEL_TABLE Pannel;						//ת������8��ͨ����ÿͨ����ң���� �൱��һ�����

//=======================================================================================================
//ע�ᱨ������������ͨ����Ϣ
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
//��ʱ����ṹ
//��Ҫ4����ʱ��
typedef struct __DELAY_ITEM
{
		uint8_t    flag;												//ʹ�ñ��
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
 //���ڱ���ʱ��
 typedef struct __Reg
 {
			uint8_t 	 cnt;
	    uint8_t    wait;
	    uint8_t    fullflag;
	    uint8_t    flashflag;
			uint16_t   time;
 }_Reg;
 APP_EXT _Reg Reg;

 
 //ģ����Ϣ
 APP_EXT uint8_t IT2_INFO[8];

//========================================================================================================= 

//=========================================================================================================
//����Ӳ����ض���=========================================================================================
 //===============================================================================================
 //���ڴ���
 //���ջ������ṹ
 #define RX_MAX 200
 #define RX_IDLE 0
 #define RX_BUSY 1
 typedef struct __RxDat
 {
	uint8_t cnt;					//���ո���
	uint8_t index;					//����֡���к�
	uint8_t buf[RX_MAX];                            //���ջ�����
	uint8_t stu;					//״̬0-����;1-���ڽ���;2-�ȴ�����
 }_RxDat;

 APP_EXT _RxDat RxDat;

 //���ͻ������ṹ
 #define TX_MAX 100
 #define TX_IDLE 0
 #define TX_BUSY 1
 typedef struct __TxDat
 {
	uint8_t cnt;					//���ո���
	uint8_t index;					//����֡���к�
	uint8_t f_tx;
	uint8_t buf[TX_MAX];            		//���ջ�����
	uint8_t stu;					//״̬0-����;1-���ڽ���;2-���ڴ���
	//uint8_t isoffline;				//�Ƿ�����
 }_TxDat;

 APP_EXT _TxDat TxDat;

 //=====================================================================================================
 //�������߼��
 typedef struct __BusStu
 {
			uint8_t 	WaitForSend;
			uint8_t     	SelfSend;
			uint8_t 	time;
			uint8_t 	interval;
    	uint8_t     	randtime;
    	uint16_t     	distime;
    	uint8_t         rxok;				//2016.3.5 add
	    uint8_t   timeout;					//���ڳ�ʱ��ʱ
 }_BusStu;
 APP_EXT _BusStu Bus;
 
 //====================================================================================================
 //���ڱ���ʱ��
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
 #define SEND_TIME_MAX            600  //ÿ100mS�ݼ�����������ʱ�����10S
 #define IR_DAT_MAX               100
 #define IR_TYPE_DAT              0x00
 #define IR_TYPE_KEY              0x40
 #define IR_TYPE_DEL              0x80
 //���������·�����
 typedef struct __Ir
 {
			uint8_t   addr;							//���ͨ����ַ
	    uint8_t   flag;							//��ǣ�=1��ʾ�յ����ߺ������ݣ������·�
	    uint8_t   wait;							//�ȴ�����װ��
	    uint8_t   TgAddr[4];					//54��ң������ַ
	    uint8_t   FrameNum;					//���������ܵ�֡����
	    uint8_t   serial;						//����֡���к�
	    uint8_t   cmd_key;					//��Ӧ��ֵ 0xfe 0xff Ϊ����ָ��
	    uint8_t   key_mode;					//����ģʽ
	    uint8_t   type_len;					//bit7 bit6��ʾ����;bit5-bit0ʵ�����ݳ���
	    uint8_t   curps;						//��ǰ���͵���ʼ��ַ
	    uint8_t   curtype;					//��ǰ���͵���ң�������ͻ��ֵ
	    uint16_t  sendtime;					//����ʱ��
	    uint8_t   sendmode;					//����ģʽ 0-���з��ͣ����Ӧ��;1-����-Ӧ��-����
			uint8_t 	typelen;					//�������ݳ���
			uint8_t   typebuf[IR_DAT_MAX];			//�������ݻ���
			uint8_t 	keylen;						//�������ݳ���
			uint8_t 	keybuf[IR_DAT_MAX];			//�������ݻ���
	    uint8_t   typechm;					//���������ۼӺ�
	    uint8_t   keychm;						//���������ۼӺ�
 }_Ir;
 APP_EXT _Ir Ir;
 //=====================================================================================================
 //485���ͻ�����У����߷��ͻ���
 APP_EXT QueueBUF RS485_Queue,WirelessSend_Queue;
 
//==================================================================================================================
//��ʱ����ṹ
//��Ҫ4����ʱ��
typedef struct __BUS_DELAY_ITEM
{
	uint8_t flag;			//ʹ�ñ��
	uint8_t index;		//������ʱ���͵�֡����
	uint8_t dat;		  //����ģʽ
}_BUS_DELAY_ITEM;  

typedef struct __BUS_DELAY_TABLE
{
	uint8_t cnt;			
	_BUS_DELAY_ITEM item[CHL_NUM];
}_BUS_DELAY_TABLE;

APP_EXT _BUS_DELAY_TABLE DlyTable;		//


//================================================================================================
//============================���¿�����Ҫ�޸�====================================================

//================================================================================================
//=================================================================================================
//============================����ģʽ����=========================================================
    #define DEBUG			

//=================================================================================================
//=================================================================================================
//============================�����õ�����=========================================================
//=================================================================================================   
  
     
    #define LED_OFF_FOREVER    		0XFF	
    #define LED_ON_FOREVER   			0XFF
    #define LED_LOOP_FOREVER  		0XFF
         
    #define LEARN_DLY_TIME      	8				//ѧϰʱ��8��
    #define ENCODE_DLY_TIME     	3				//����ʱ��2��
  
    #define TRUE 									1
    #define FALSE									0
    #define NULL    							0
		#define ALL                   0xff
		
	  //=================================================================================	
		//����Э��֡��ʽ
		#define ACK                   0x00
		#define MTD										0x10
		#define DTM                   0x11														//ң���������ϱ�
		#define DTM1                  0x12                            //��ת����ң���������ϱ�

		#define MTD_IR_SETUP					0x40														//�������豸-����
		#define MTD_WL_SETUP					0x41                            //����������-����
		#define DTM_IR_SETUP_FINISH   0X42														//�������
		#define DTM_WL_SETUP_FINISH		0x43														//�������	
		#define MTD_IR_DEL            0x44
		
			


//=================================================================================================
//=================================================================================================
//=================================================================================================  


//=================================================================================================
//=================================================================================================
//============================���ݽṹ�ͱ�������===================================================
//=================================================================================================   





#endif
//================================================================================================= 
//=======================================================================================
