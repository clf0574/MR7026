
//============================================================================================
#ifndef _main_h
#define _main_h

	#ifdef MAIN_VARIABLE
		#define APP_EXT   
	#else
		#define APP_EXT extern       
	#endif
 
 //===========================================================================================
 //2017.5.12 Ôö¼Ó³¤°´ÉèÖÃ¼ü£¬Çå³ı±¾Éí±í¸ñ£¬²¢·¢ËÍÉ¾³ı54¼üÒ£¿ØÆ÷ËùÒÔÅäÖÃµÄÖ¸Áî
   
 //============================================================================================  
 
 #include "my_queue_chl.h"
 #define Uint8 uint8_t
 #define Uint16 uint16_t	
 
 //============================================================================================ 
 //============================================================================================
 //È¡µÃÎ¨Ò»ĞòÁĞºÅ·½Ê½
 //#define USE_NEWREGISTER
 //#define GET_SERIAL_FROM_CHIP_ID
 #define GET_SERIAL_FROM_DS28E02
 //#define USE_3MATCH								//Á¬Ğø3´ÎÊÕµ½£¬±íÊ¾ÊÕµ½¶ÔÂëĞÅºÅ¡£·ñÔòÎª1´Î
 
 //ÒÔÏÂÁ½¸ö¶şÑ¡Ò»
 #define USE_A7139
 //#define USE_CC1101
 //¶¨ÒåÊÇ·ñÊ¹ÓÃÀ©Æµ
 #define USE_DSSS_MODE
 
 //ÒÔÏÂÁ½¸ö¶şÑ¡Ò»,Ê¹ÓÃÊ±×¢ÒâÒª¼ÓÔØ¶ÔÓ¦µÄ.cÎÄ¼ş,²¢ÇÒÔÚbsp.hÖĞÑ¡Ôñ¶ÔÓ¦µÄ.hÎÄ¼ş
 //#define USE_DS28E02
 #define USE_DS28E11
 //#define USE_LOCK_BY_DS28E11
 
 //Ê¹ÓÃ´®¿Ú²âÊÔ¹¦ÄÜ
 //#define USE_SERIAL_DEBUG
 //============================================================================================
 //===========================================================================================================
 //×ª·¢Æ÷ ×ª·¢±í½á¹¹
 //×ª·¢±íĞ´Èë£¬ÊÇ°´ÌõÄ¿Ğ´ÈëµÄ£¬ĞŞ¸ÄÕâÀïĞèĞŞ¸ÄWrite_Ex_Tableº¯Êı
 //×ª·¢±íÍ¨µÀ£¬»ò¿É×ª»»Éè±¸ÊıÄ¿
 #define EX_TABLE_NUM         8 
 #define CHL_NUM              EX_TABLE_NUM
 //============================================================================================
 //Ğ­Òé¶¨Òå 
 #define IT1         	        0 
 #define IT2 	 	       				2
 #define FREE1 		            1 
 
 
 //IT1Ğ­ÒéÖĞÊı¾İ¸ñÊ½
 #define S_FM			0
 #define S_SEH			1
 #define S_TADD		2
 #define S_SEL			3
 #define S_ID			4
 #define S_CMD		5
 #define S_DATLEN	6
 #define S_DAT  	7
 //ÎŞÏßÖ¡¸ñÊ½Î»ÖÃ
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
 //×ª·¢Æ÷ÔÚ¶ÅÑÇIT2µÄÈ«¾Ö¶¨Òå
 #define DEV_IT2_VER     100
 
 //==============================================================================================
 //Éè±¸(×ª·¢Æ÷)ÔËĞĞ×´Ì¬ ×¢²áÌ¬ ÉèÖÃÌ¬£¨¶ÔÂë»òÑ§Ï°£©£¬Õı³£ÔËĞĞÌ¬
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

 //#define REG_START_ADDR         0x30			//¼Ä´æÆ÷
 //#define REG_MODE1_ADDR         0x2f                    //ÊÇ·ñÍ¸´«(Ä¬ÈÏÎªÍ¸´«£¬Ö÷¶¯ÉÏ±¨,»òÕß¼Ä´æÆ÷¶ÁÈ¡)
 
 //==============================================================================================
 //Ä¬ÈÏÍø¹ØµØÖ·,ÓÃÓÚITÏµÍ³£¬¶Ô¿ª·Å485Ğ­Òé²»´æÔÚ
 #define Master_Address 	0x00  //Íø¹ØµØÖ·
 
 //==============================================================================================
 //³ÌĞò·µ»Ø×´Ì¬£¬´íÎó£¬ÕıÈ·£¬ºöÔ½
 #define RE_ERR 0xFF				//0xff
 #define RE_OK  RE_ERR-1			//0xfe
 #define RE_CANCEL RE_OK-1			//0xfd
 //³ÌĞò²ÎÊı ÕıÈ· ´íÎó
 #define OPT_ERR RE_CANCEL-1			//0xfc
 #define OPT_OK  OPT_ERR-1			//0xfb
 #define OPT_CANCEL OPT_OK-1
  #define OPT_ERR_SETUP		0xf9 
 
  #define OPT_SETKEY              0xf0
 
 //È«¾Ö±äÁ¿£¬´íÎó±ê¼Ç
 #define ERR_EEPROM       0x80
 #define ERR_WIRELESS     0x40
 #define ERR_UART         0x20
 #define ERR_POWER        0x10                         //µçÑ¹´íÎó£¬µÍÑ¹
 //=================================================================================================
 //Ïà¶ÔÎŞÏßÉè±¸À´Ëµ
 #define EX_DEV_MOTOR_R    0x02
 #define EX_DEV_MOTOR_E    0x04
 #define EX_DEV_BLIND_R    0x12
 #define EX_DEV_BLIND_E    0x11
 #define EX_DEV_SOCKET     0x21
 #define EX_DEV_LIGHT      0x31
 #define EX_DEV_LIGHT_ADJ  0x33
 
 
 #define EX_DEV_MEDIA			 0xd0
 
 //´«¸ĞÆ÷Àà
 #define EX_DEV_SEN        0xe0
 #define EX_DEV_WSR    		 0xe0
 #define EX_DEV_WIND    	 0xe1
 #define EX_DEV_SUN     	 0xe2
 #define EX_DEV_RAIN       0xe3
  #define EX_DEV_SUN1      0xe4 //ÊıÖµÁÁ

 
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
 //ÒÔÏÂÈ«¾Ö±äÁ¿¶¨Òå
 //Éè±¸Êı¾İ½á¹¹
 typedef struct __Dev
 {
  //¹«ÓÃ²¿·Ö
	uint8_t   profile;									//Ê¹ÓÃµÄ485Ğ­Òé
	uint8_t   flag;											//bit0-µÍÑ¹±ê¼Ç bit1-ÊÇ·ñĞ´EPROM±ê¼Ç bit2-×ª·¢±í±¸·İ±ê¼Ç
	uint8_t   addr[2];									//Éè±¸µØÖ·,ITĞ­ÒéÓÃaddr[0],¿ª·Å485Ğ­Òéaddr[0]-ID_L,addr[1]-ID_H
	uint16_t  type;										  //Éè±¸ÀàĞÍ
  uint8_t   stu;										  //Éè±¸¹¤×÷×´Ì¬
	uint8_t   rand;										  //Ëæ»úÊı
	uint8_t   serial;                   //·¢ËÍĞòÁĞºÅ
	uint8_t   power_time;								//ÉÏµçÊ±¼ä
	uint8_t   wireless_busy;						//ÎŞÏßÃ¦£
	uint8_t   learn_time;
	uint8_t   encode_time;							//
	uint8_t   mode;									    ///Í¨Ñ¶Ä£Ê½£¬Í¸´«»ò»º³å
	uint8_t   err;									    //´íÎó×´Ì¬
	uint16_t  sync;	
 }_Dev;
 APP_EXT _Dev Dev;
 
 typedef struct __Exchange
 {
		uint8_t    type;								        								//Í¨ÓÃËæÉíÌù£¬È«¹¦ÄÜËæÉíÌù£¬µç»ú£¬ÕÕ¶È´«¸ĞÆ÷µÈ
		uint8_t    addr;																				//ÆäÊµ¾ÍÊÇÍ¨µÀºÅ
		uint8_t    stu;    																			//µç»úÕıÏò»ò·´Ïò µÈĞÅÏ¢                    						
		uint8_t    wl_addr[3];
		uint8_t    channel;		
		uint8_t    wltype;																			//¶ÔÓ¦ÎŞÏßÀàĞÍ,ÓĞÎŞÏßÉè±¸Ö÷¶¯ÉÏ±¨
		uint8_t    key;																					//¶ÔÓ¦ÎŞÏßÉè±¸µÄµÄ°´¼üÖµ
		//ÒÔÏÂ¼ÓÃÜÊ±ÓÃµ½
    uint16_t   sync;                                  			//Í¬²½×Ö
	  uint8_t    secret;
 }_Exchange;

 typedef struct _Ex_Table
 {
		uint8_t    index;
	  uint8_t    addr[4];																			//ÎŞÏßÖ÷»úµØÖ·
		uint8_t    reg[9];																			//Í¨µÀ×¢²á×´Ì¬
	  uint8_t    SyncErr[EX_TABLE_NUM];
	  uint16_t   SyncNum[EX_TABLE_NUM];
		uint8_t    stu[EX_TABLE_NUM];					        					//×îºóÒ»¸öÃüÁî×´Ì¬
		uint8_t    rtdat[EX_TABLE_NUM];	                        //·µ»ØÊı¾İ,ÎŞĞ§0xff,0-100ÓĞĞ§£¬±íÊ¾µÆ¿ª¹ØºÍµç»úĞĞ³Ì		
		_Exchange  Exchange[EX_TABLE_NUM];	                    //×ª·¢Æ÷Í¨µÀÊı¾İ-Ò»¹²8Í¨µÀ
 }_Ex_Table;

 APP_EXT _Exchange Exchangebuf;							        				//×ª·¢Æ÷ÁÙÊ±Êı¾İ
 APP_EXT _Ex_Table Ex_Table;
 
 
 //===============================================================================================
 //===============================================================================================
 //ÒÔÏÂÎªËø´¦Àí
 #define LOCK_ITEMNUM    16
 #define LOCK_KEYNUM     8
 #define LOCK_ALLNUM     80
 
 //×Ü×Ö½Ú4+5+8+4+16+16+16=9+12+48=21+48=69
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
	  uint16_t   stu;																							//ËøµÄ×´Ì¬
	  uint16_t    Serail[LOCK_ITEMNUM];
	  uint8_t    IdType[LOCK_ITEMNUM];
	  uint8_t    UserType[LOCK_ITEMNUM];
 }_LockItem;

 typedef struct __Lock
 {
		uint8_t    index;
	  uint8_t    report;
	  uint8_t    setkeytime;
	  uint8_t    randkey[8];																	//8¸öËæ»úÃÜÂë
	  uint8_t    SyncErr;
	  uint8_t    waittime;										//µÈ´ıÊ±¼ä
		_LockItem  item;	                   		
 }_Lock;
 
 APP_EXT _Lock lock;
 APP_EXT _LockItem LockItem;
 
 //===============================================================================================
 //³¡¾°Êı¾İ½á¹¹-ÓÃÓÚ×ª·¢Æ÷
 //¹²Ğè×Ö½Ú (16*50)*8=6400¸ö×Ö½Ú
  typedef struct __S_item
 {
		uint8_t    flag;												//Ê¹ÓÃ±ê¼Ç
		uint16_t   id;													//³¡¾°ºÅ
		uint8_t    chl;                 				//Í¨µÀºÅ    
		uint8_t    cmd;
        uint8_t    len;
		uint8_t    dat[5];	        						//Êı¾İ	
		uint16_t   dlytime;											//ÑÓÊ±Ê±¼ä	
        uint8_t    sum;	 
 }_S_item;
 
 typedef struct __S_Dev
 {
		uint8_t    num;                 				//³¡¾°¸öÊı
		_S_item    item[50];            				//Ò»¸öÉè±¸ ×Ü¹²ÓĞ50¸ö³¡¾°
       					                   													        		
 }_S_Dev;
 
 APP_EXT _S_Dev Scene[EX_TABLE_NUM];										//Ò»¸ö×ª·¢Æ÷ ×î¶àÓĞ8¸öÉè±¸
 
//===============================================================================================
//====================================================================================
//°´¼üÅäÖÃ±í¸ñ
#define KEY_NUM   8																			//×ª·¢Æ÷ÅäÖÃÎªÒ£¿ØÆ÷Ê±£¬Ã¿Ò»Â·Ò£¿ØÆ÷×î¶à4¼ü
#define KEY_ITEM_NUM  16																//Ğ´Èë¶Á³öµÄÒ»Ö¡³¤¶È
typedef struct __PANNEL_ITEM
{
	uint8_t irflagbuf[8];																	//±ê¼Ç£¬±íÊ¾ÊÇ·ñÊ¹ÓÃ
	uint8_t  onoffbuf[7];
	//uint8_t type;																				//ÀàĞÍ Éè±¸ ³¡¾° ĞòÁĞ ·´Ïò
	//uint8_t tg_id;																			//Ä¿±êµØÖ·
	//uint8_t datalen;																		//Êı¾İ×Ö½Ú³¤¶È
	//uint8_t buf[10];																		//Êı¾İ
}_PANNEL_ITEM;  

typedef struct __PANNEL_TABLE
{
	uint8_t cnt;		//ÒÑÅäÍ¨µÀ×ÜÊı
	_PANNEL_ITEM item[KEY_NUM];
}_PANNEL_TABLE;

APP_EXT _PANNEL_TABLE Pannel;						//×ª·¢Æ÷ÓĞ8¸öÍ¨µÀ£¬Ã¿Í¨µÀµÄÒ£¿ØÆ÷ Ïàµ±ÓÚÒ»¸öÃæ°å

//=======================================================================================================
//×¢²á±¨ÎÄËù´øµÄËùÓĞÍ¨µÀĞÅÏ¢
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
//ÑÓÊ±´¦Àí½á¹¹
//ĞèÒª4¸ö¶¨Ê±Æ÷
typedef struct __DELAY_ITEM
{
		uint8_t    flag;												//Ê¹ÓÃ±ê¼Ç
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
 //ÓÃÓÚ±¾µØÊ±¼ä
 typedef struct __Reg
 {
			uint8_t 	 cnt;
	    uint8_t    wait;
	    uint8_t    fullflag;
	    uint8_t    flashflag;
			uint16_t   time;
 }_Reg;
 APP_EXT _Reg Reg;

 
 //Ä£¿éĞÅÏ¢
 APP_EXT uint8_t IT2_INFO[8];

//========================================================================================================= 

//=========================================================================================================
//ÒÔÏÂÓ²¼şÏà¹Ø¶¨Òå=========================================================================================
 //===============================================================================================
 //ÓÃÓÚ´®¿Ú
 //½ÓÊÕ»º³åÇø½á¹¹
 #define RX_MAX 200
 #define RX_IDLE 0
 #define RX_BUSY 1
 typedef struct __RxDat
 {
	uint8_t cnt;					//½ÓÊÕ¸öÊı
	uint8_t index;					//½ÓÊÕÖ¡ĞòÁĞºÅ
	uint8_t buf[RX_MAX];                            //½ÓÊÕ»º³åÇø
	uint8_t stu;					//×´Ì¬0-¿ÕÏĞ;1-ÕıÔÚ½ÓÊÕ;2-µÈ´ı·¢ËÍ
 }_RxDat;

 APP_EXT _RxDat RxDat;

 //·¢ËÍ»º³åÇø½á¹¹
 #define TX_MAX 100
 #define TX_IDLE 0
 #define TX_BUSY 1
 typedef struct __TxDat
 {
	uint8_t cnt;					//½ÓÊÕ¸öÊı
	uint8_t index;					//½ÓÊÕÖ¡ĞòÁĞºÅ
	uint8_t f_tx;
	uint8_t buf[TX_MAX];            		//½ÓÊÕ»º³åÇø
	uint8_t stu;					//×´Ì¬0-¿ÕÏĞ;1-ÕıÔÚ½ÓÊÕ;2-ÕıÔÚ´¦Àí
	//uint8_t isoffline;				//ÊÇ·ñÀëÏß
 }_TxDat;

 APP_EXT _TxDat TxDat;

 //=====================================================================================================
 //ÓÃÓÚ×ÜÏß¼ì²â
 typedef struct __BusStu
 {
			uint8_t 	WaitForSend;
			uint8_t     	SelfSend;
			uint8_t 	time;
			uint8_t 	interval;
    	uint8_t     	randtime;
    	uint16_t     	distime;
    	uint8_t         rxok;				//2016.3.5 add
	    uint8_t   timeout;					//ÓÃÓÚ³¬Ê±¼ÆÊ±
 }_BusStu;
 APP_EXT _BusStu Bus;
 
 //====================================================================================================
 //ÓÃÓÚ±¾µØÊ±¼ä
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
 #define SEND_TIME_MAX            600  //Ã¿100mSµİ¼õ£¬·¢ËÍÔÊĞíÊ±¼ä×î´ó10S
 #define IR_DAT_MAX               100
 #define IR_TYPE_DAT              0x00
 #define IR_TYPE_KEY              0x40
 #define IR_TYPE_DEL              0x80
 //ºìÍâÅäÖÃÏÂ·¢Êı¾İ
 typedef struct __Ir
 {
			uint8_t   addr;							//±í¸ñÍ¨µÀµØÖ·
	    uint8_t   flag;							//±ê¼Ç£¬=1±íÊ¾ÊÕµ½×ÜÏßºìÍâÊı¾İ£¬ÕıÔÚÏÂ·¢
	    uint8_t   wait;							//µÈ´ı·¢ËÍ×°Èë
	    uint8_t   TgAddr[4];					//54¼üÒ£¿ØÆ÷µØÖ·
	    uint8_t   FrameNum;					//ÉèÖÃÊı¾İ×ÜµÄÖ¡¸öÊı
	    uint8_t   serial;						//·¢ËÍÖ¡ĞòÁĞºÅ
	    uint8_t   cmd_key;					//¶ÔÓ¦¼üÖµ 0xfe 0xff Îª²âÊÔÖ¸Áî
	    uint8_t   key_mode;					//°´¼üÄ£Ê½
	    uint8_t   type_len;					//bit7 bit6±íÊ¾ÀàĞÍ;bit5-bit0Êµ¼ÊÊı¾İ³¤¶È
	    uint8_t   curps;						//µ±Ç°·¢ËÍµÄÆğÊ¼µØÖ·
	    uint8_t   curtype;					//µ±Ç°·¢ËÍµÄÊÇÒ£¿ØÆ÷ÀàĞÍ»ò¼üÖµ
	    uint16_t  sendtime;					//·¢ËÍÊ±¼ä
	    uint8_t   sendmode;					//·¢ËÍÄ£Ê½ 0-¼¯ÖĞ·¢ËÍ£¬×îºóÓ¦´ğ;1-·¢ËÍ-Ó¦´ğ-·¢ËÍ
			uint8_t 	typelen;					//ÀàĞÍÊı¾İ³¤¶È
			uint8_t   typebuf[IR_DAT_MAX];			//ÀàĞÍÊı¾İ»º´æ
			uint8_t 	keylen;						//°´¼üÊı¾İ³¤¶È
			uint8_t 	keybuf[IR_DAT_MAX];			//°´¼üÊı¾İ»º´æ
	    uint8_t   typechm;					//ÀàĞÍÊı¾İÀÛ¼ÓºÍ
	    uint8_t   keychm;						//°´¼üÊı¾İÀÛ¼ÓºÍ
 }_Ir;
 APP_EXT _Ir Ir;
 //=====================================================================================================
 //485·¢ËÍ»º³å¶ÓÁĞ£¬ÎŞÏß·¢ËÍ»º³å
 APP_EXT QueueBUF RS485_Queue,WirelessSend_Queue;
 
//==================================================================================================================
//ÑÓÊ±´¦Àí½á¹¹
//ĞèÒª4¸ö¶¨Ê±Æ÷
typedef struct __BUS_DELAY_ITEM
{
	uint8_t flag;			//Ê¹ÓÃ±ê¼Ç
	uint8_t index;		//ÓÃÓÚÑÓÊ±·¢ËÍµÄÖ¡ÀàĞÍ
	uint8_t dat;		  //°´¼üÄ£Ê½
}_BUS_DELAY_ITEM;  

typedef struct __BUS_DELAY_TABLE
{
	uint8_t cnt;			
	_BUS_DELAY_ITEM item[CHL_NUM];
}_BUS_DELAY_TABLE;

APP_EXT _BUS_DELAY_TABLE DlyTable;		//


//================================================================================================
//============================ÒÔÏÂ¿ÉÄÜĞèÒªĞŞ¸Ä====================================================

//================================================================================================
//=================================================================================================
//============================µ÷ÊÔÄ£Ê½½øÈë=========================================================
    #define DEBUG			

//=================================================================================================
//=================================================================================================
//============================³ÌĞòÓÃµ½³£Á¿=========================================================
//=================================================================================================   
  
     
    #define LED_OFF_FOREVER    		0XFF	
    #define LED_ON_FOREVER   			0XFF
    #define LED_LOOP_FOREVER  		0XFF
         
    #define LEARN_DLY_TIME      	8				//Ñ§Ï°Ê±¼ä8Ãë
    #define ENCODE_DLY_TIME     	3				//¶ÔÂëÊ±¼ä2Ãë
  
    #define TRUE 									1
    #define FALSE									0
    #define NULL    							0
		#define ALL                   0xff
		
	  //=================================================================================	
		//ÎŞÏßĞ­ÒéÖ¡¸ñÊ½
		#define ACK                   0x00
		#define MTD										0x10
		#define DTM                   0x11														//Ò£¿ØÆ÷Ö÷¶¯ÉÏ±¨
		#define DTM1                  0x12                            //Ğı×ª±àÂëÒ£¿ØÆ÷Ö÷¶¯ÉÏ±¨

		#define MTD_IR_SETUP					0x40														//Ö÷»úµ½Éè±¸-ºìÍâ
		#define MTD_WL_SETUP					0x41                            //Ö÷»úµ½ÉèÖÃ-ÎŞÏß
		#define DTM_IR_SETUP_FINISH   0X42														//ÉèÖÃÍê³É
		#define DTM_WL_SETUP_FINISH		0x43														//ÉèÖÃÍê³É	
		#define MTD_IR_DEL            0x44
		
			


//=================================================================================================
//=================================================================================================
//=================================================================================================  


//=================================================================================================
//=================================================================================================
//============================Êı¾İ½á¹¹ºÍ±äÁ¿¶¨Òå===================================================
//=================================================================================================   





#endif
//================================================================================================= 
//=======================================================================================
