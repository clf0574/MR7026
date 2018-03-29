#ifndef _EEPROM_H
	#define _EEPROM_H

//==========================================================================================================
//==========================================================================================================
//==========================================================================================================
//==========================================================================================================
//==========================================================================================================
//===============以下APP1应用，杜亚开放485协议==============================================================
//==========================================================================================================
  //AT24C512=64*1024个字节，地址 0x0000----0xffff       
     
	//================================================================
	//===============================================================
	#define USED_FLAG              0x5b
	#define TYPE_EE_ADDR_USED      0
	#define TYPE_EE_ADDR_ADDR0     1
	#define TYPE_EE_ADDR_ADDR1     2
	#define TYPE_EE_ADDR_MODE      3
	#define TYPE_EE_ADDR_PROFILE   4
	#define TYPE_EE_ADDR_INFO      5 
	#define TYPE_EE_ADDR_TYPES     6
	#define TYPE_EE_ADDR_TYPESH    7
	
	//产品型号  
	#define EE_DEV_INFO                     0x0010  
  #define EE_DEV_INFO_0                   0x0010            //H    
	#define EE_DEV_INFO_1                   0x0011            //T
	#define EE_DEV_INFO_2                   0x0012            //7
	#define EE_DEV_INFO_3                   0x0013            //2
	#define EE_DEV_INFO_4                   0x0014            //0                                                                                                    
  #define EE_DEV_INFO_5                   0x0015            //3
	#define EE_DEV_INFO_6                   0x0016            ///
	#define EE_DEV_INFO_7                   0x0017            //1
	#define EE_DEV_INFO_8                   0x0018            //5 
	#define EE_DEV_INFO_9                   0x0019            //.
	#define EE_DEV_INFO_10                  0x001A            //1
	#define EE_DEV_INFO_11                  0x001B            //1
	#define EE_DEV_INFO_12                  0x001C            //. 
	#define EE_DEV_INFO_13                  0x001D            //2
	#define EE_DEV_INFO_14                  0x001E            //0
	#define EE_DEV_INFO_15                  0x001F            //
	
	//设备信息
	#define EE_DEV_USED               			0x0020 
	#define EE_DEV_FLAG0             				0x0020  	  //使用标记
	#define EE_DEV_FLAG1            				0x0021  	  //使用标记
	#define EE_DEV_FLAG2             				0x0022  	  //使用标记
	
	#define EE_DEV_ADDR0             				0x0023  	  //地址0
	#define EE_DEV_ADDR0_0             	0x0023  	  //地址0
	#define EE_DEV_ADDR0_1            	0x0024  	  //地址0
	#define EE_DEV_ADDR0_2            	0x0025  	  //地址0
	
	#define EE_DEV_ADDR1             		0x0026  	  //地址1
	#define EE_DEV_ADDR1_0             	0x0026  	  //地址1
	#define EE_DEV_ADDR1_1            	0x0027  	  //地址1
	#define EE_DEV_ADDR1_2            	0x0028  	  //地址1
	
	//是否透传，主动上报--只用于开放485协议	
	#define EE_DEV_MODE            	  	0x0029  	  //模式
	#define EE_DEV_MODE0            		0x0029  	  //模式
	#define EE_DEV_MODE1            		0x002a  	  //模式
	#define EE_DEV_MODE2            		0x002b  	  //模式
	
	#define EE_DEV_PROFILE            	0x002c  	  //协议
	#define EE_DEV_PROFILE0            	0x002c  	  //协议
	#define EE_DEV_PROFILE1            	0x002d  	  //协议
	#define EE_DEV_PROFILE2            	0x002e  	  //协议
	
	#define EE_DEV_TYPES                0x002f     
	#define EE_DEV_TYPES0               0x002f   
	#define EE_DEV_TYPES1               0x0030	
	#define EE_DEV_TYPES2               0x0031   
	
	#define EE_DEV_TYPESH               0x0032     
	#define EE_DEV_TYPESH0              0x0032   
	#define EE_DEV_TYPESH1              0x0033	
	#define EE_DEV_TYPESH2              0x0034 
	
	#define EE_DEV_KEY                  0x0060
	#define EE_DEV_KEY1                 0x0060
	#define EE_DEV_KEY2                 0x0070
	
	//=======================================================================
  //转发器存贮空间

	#define EX_TABLE_ADDR   						0x100				//
	#define EX_TABLE_ADDR_BCK    				0x200
  #define EX_ITEM_NUM           			32					       //32*8=256
	
	//========================================================================
	//锁的存贮空间
	#define LOCK_TABLE_ADDR   						0x400				//
	#define LOCK_TABLE_ADDR_BCK    				0x600
  #define LOCK_ITEM_NUM           			32					       //32*16=512
  //========================================================================
	//面板-遥控器存贮空间
	#define EE_PANNEL_ADDR           		0x0800
	#define EE_PANNEL_ADDR_BCK       		0x0c00
	
	//========================================================================
	//场景存贮空间
	#define DEV_NUM                         8
	#define DEV_SCENE_NUM				    				50
	#define SCENE_ITEM_NUM                  16
	#define DEV_SCENE_SIZE                  800
	#define SCENE_BCK_OFFSET                0x4000
	//=========================================================================
	//设备0
	#define EE_DEV_SCENE_ADDR               0x1000
	//=========================================================================
	
	//=========================================================================
	//场景备份
    //设备0
	#define EE_DEV_SCENE_ADDR_BCK           0x5000
	
	
	//=====================================================================
	//以下0x5000开始============================================================
	
	
	
#endif






