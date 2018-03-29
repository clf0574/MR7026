
//======================================================================================================
//包含头文件
#include <includes.h>


//======================================================================================================

//======================================================================================================
/* 按键口对应的RCC时钟 */
#define RCC_ALL_A7139 	(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB)

#define GPIO_PORT_SCS   GPIOA
#define GPIO_PIN_SCS	GPIO_Pin_4

#define GPIO_PORT_SCK   GPIOA
#define GPIO_PIN_SCK	GPIO_Pin_5

#define GPIO_PORT_SDIO   GPIOA
#define GPIO_PIN_SDIO	 GPIO_Pin_7

#define GPIO_PORT_CKO   GPIOB
#define GPIO_PIN_CKO	GPIO_Pin_1

#define GPIO_PORT_GIO1   GPIOB
#define GPIO_PIN_GIO1	 GPIO_Pin_0

#define GPIO_PORT_GIO2   GPIOA
#define GPIO_PIN_GIO2	 GPIO_Pin_6

//========================================================================================================
//#define A7139_SDIO     P1_4
//#define A7139_SCLK     P1_3
//#define A7139_SCS       P1_2
//#define A7139_GIO1     P1_5

//#define A7139_SDIO_R P1_4
//#define A7139_GIO2_R P1_5
//========================================================================================================
//底层函数定义
#define Write_SDIO_HI()    GPIO_SetBits(GPIO_PORT_SDIO, GPIO_PIN_SDIO)
#define Write_SDIO_LO()    GPIO_ResetBits(GPIO_PORT_SDIO, GPIO_PIN_SDIO)

#define Read_GIO2()        GPIO_ReadInputDataBit(GPIO_PORT_GIO2,GPIO_PIN_GIO2)
#define Read_SDIO()        GPIO_ReadInputDataBit(GPIO_PORT_SDIO,GPIO_PIN_SDIO)

#define Write_SCS_LO()     GPIO_ResetBits(GPIO_PORT_SCS, GPIO_PIN_SCS)
#define Write_SCS_HI()     GPIO_SetBits(GPIO_PORT_SCS, GPIO_PIN_SCS)

#define Write_SCK_HI()     GPIO_SetBits(GPIO_PORT_SCK, GPIO_PIN_SCK)
#define Write_SCK_LO()     GPIO_ResetBits(GPIO_PORT_SCK, GPIO_PIN_SCK)

//#define Write_SDIO_HI()    A7139_SDIO=1
//#define Write_SDIO_LO()    A7139_SDIO=0

//#define Read_GIO2()        A7139_GIO2_R
//#define Read_SDIO()        A7139_SDIO_R

//#define Write_SCS_LO()     A7139_SCS=0
//#define Write_SCS_HI()     A7139_SCS=1

//#define Write_SCK_HI()     A7139_SCLK=1
//#define Write_SCK_LO()     A7139_SCLK=0

//=========================================================================================================


//=========================================================================================================
//=========================================================================================================
//定义变量
static uint8_t  init_err=0;
static uint8_t  WaitForTime=0;
static uint8_t  WaitFlag=0;
unsigned int RSSI;
//========================================================================================
//函数声明
void Set_SDIO_Direction(uint8_t val);
void Err_State(void);
void StrobeCMD(uint8_t cmd);
void ByteSend(uint8_t src);
uint8_t ByteRead(void);
void A7129_WriteReg(uint8_t address, uint16_t dataWord);
uint16_t A7129_ReadReg(uint8_t address);
void A7129_WritePageA(uint8_t address, uint16_t dataWord);
uint16_t A7129_ReadPageA(uint8_t address);
void A7129_WritePageB(uint8_t address, uint16_t dataWord);
uint16_t A7129_ReadPageB(uint8_t address);
void A7129_Config(void);
void A7129_WriteFIFO(uint8_t *buf,uint8_t length);
void A7129_WriteID(void);
void A7139_Delay10us(uint16_t dly);
void My_NOP(void);

//以下用于扩频和无线唤醒
#ifdef USE_DSSS_MODE
void RCOSC_Cal(void);
void WOR_enable_by_preamble(void);
void WOR_enable_by_sync(void);
void WOR_enable_by_RSSI_carrier(void);
void WOT_enable(void);
void RSSI_measurement(void);
void DSSS_MODE(void);
#endif
////=================================================================================

//=================================================================================

//=================================================================================
#ifdef USE_DSSS_MODE
#define R10k  1
#define R50k  2
#define R100k 3

#define A7139_rate  R100k

const Uint8 ID_Tab[8]={0xd3,0x91,0xd3,0x91,0xC7,0x33,0x45,0xE7};   //ID code

const Uint16  RCOSC_TGNUM=395;	//TGNUM=195 for Crystal=12.8M & IFBW=50KHz
const Uint16  A7129Config[]=  //868MHz, 10kbps (IFBW = 50KHz, Fdev = 18.75KHz), Crystal=12.8MHz
{
#if A7139_rate== R10k
    0x1221,     //SYSTEM CLOCK register,
#elif A7139_rate== R50k
	0x0221,				//100K带宽 50k速率
#elif A7139_rate== R100k
	0x0021,
#endif
    0x0A43,     //PLL1 register,
	
		//====================================================================>>>>>>>>>>
    //0xd405,     //PLL2 register,    866.2MHz
	  0xda05,     //PLL2 register,    866.5MHz
	  //====================================================================<<<<<<<<<<
    0x0000,     //PLL3 register,
    0x1A20,     //PLL4 register,
    0x0024,     //PLL5 register,
    0x0000,     //PLL6 register,
    0x0015,     //CRYSTAL register,
    0x0000,     //PAGEA,
    0x0000,     //PAGEB,
    0x18D4,     //RX1 register,     IFBW=50KHz
//	0x18d0,		//  IFBW=50kHz
    0x7009,     //RX2 register,     by preamble
    0x4000,     //ADC register,     
    0x0800,     //PIN CONTROL register,     Use Strobe CMD
    0x4C45,     //CALIBRATION register,
    0x20C0       //MODE CONTROL register,    Use FIFO mode
};
const Uint16  A7129Config_PageA[]=   //868MHz, 10kbps (IFBW = 50KHz, Fdev = 18.75KHz), Crystal=12.8MHz
{
    0xF706,     //TX1 register,     Fdev = 37.5kHz
    0x0000,     //WOR1 register,
    0xF800,     //WOR2 register,
    0x1107,     //RFI register,     Enable Tx Ramp up/down  
    0x0170,     //PM register,      CST=1
    0x0402,     //RTH register,
    0x400F,     //AGC1 register,    
    0x2AC0,     //AGC2 register, 
    0x0041,			//0xc041,           //GIO register,     GIO2=WTR, GIO1=FSYNC //clf mod 20161027
    0xDB81,     //CKO register
    0x0064,     //VCB register,
    0x1343,     //CHG1 register,    860MHz
    0x0044,     //CHG2 register,    870MHz
    0x0020,     //FIFO register,    FEP=63+1=64bytes
//	0x0010,
    0x1507,     //CODE register,    Preamble=4bytes, ID=4bytes
    0x8000      //WCAL register,
};

const Uint16  A7129Config_PageB[]=   //868MHz, 10kbps (IFBW = 50KHz, Fdev = 18.75KHz), Crystal=12.8MHz
{
//    0x035F,		//TX2 register,035f 	
	0x0307,			//0x0304, 20161109
	0x8400,		//IF1 register, 	Enable Auto-IF, IF=100KHz
//	0x8200,
	0x0000,		//IF2 register,
	0x0000,		//ACK register,
	0x0000,		//ART register,
	0x0000,		//SYN register,
//	0x0000,		//SYN register,
	0x0000,		//ACKFIFO register,
//	0xc4ff,		//DSSS register,
	0X0000,
	0x0000,		//CCM1 register,
	0x0000,		//CCM2 register,
	0x0000,		//CCM3 register,
	0x0000,		//CCM4 register,
	0x0000,		//CCM5 register,
	0x0000,		//RF_Cmd register,
	0x0000,		//Btn_Cmd register,
//	0x6200,		//DSSS2 register,
	0x0000,		//DSSS2 register,
	0x0000,		//DSSS3 register,
	0x0000,		//CCM6 register,
	0x0000,		//CDET register,
//	0x0400,		//DC_SHIFT register,
	0x0000,		//DC_SHIFT register,
	0x0000		//RCOSC register,
};

//=================================================================================
#else
const uint8_t BitCount_Tab[16]={0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4};
const uint8_t ID_Tab[8]={0xd3,0x91,0xd3,0x91,0xC7,0x33,0x45,0xE7};   //ID code
const uint8_t PN9_Tab[]=
{   0xFF,0x83,0xDF,0x17,0x32,0x09,0x4E,0xD1,
    0xE7,0xCD,0x8A,0x91,0xC6,0xD5,0xC4,0xC4,
    0x40,0x21,0x18,0x4E,0x55,0x86,0xF4,0xDC,
    0x8A,0x15,0xA7,0xEC,0x92,0xDF,0x93,0x53,
    0x30,0x18,0xCA,0x34,0xBF,0xA2,0xC7,0x59,
    0x67,0x8F,0xBA,0x0D,0x6D,0xD8,0x2D,0x7D,
    0x54,0x0A,0x57,0x97,0x70,0x39,0xD2,0x7A,
    0xEA,0x24,0x33,0x85,0xED,0x9A,0x1D,0xE0
};	// This table are 64bytes PN9 pseudo random code.
//===================================================================================

//===================================================================================
const uint16_t A7129Config[]=		//868MHz, 10kbps (IFBW = 100KHz, Fdev = 37.5KHz), Crystal=12.8MHz
{
	0x1EC4,		//SYSTEM CLOCK register,
	0x0A43,		//PLL1 register,
	0xd3b8,		//0xD405,		//PLL2 register,	866.001MHz
	0x0000,		//PLL3 register,
	0x1A20,		//PLL4 register,
	0x0024,		//PLL5 register,
	0x0000,		//PLL6 register,
	0x0013,		//CRYSTAL register,
	0x0000,		//PAGEA,
	0x0000,		//PAGEB,
	0x18D4,		//RX1 register, 	IFBW=100KHz	
	0x7009,		//RX2 register, 	by preamble
	0x4400,		//ADC register,	   	
	0x0800,		//PIN CONTROL register,		Use Strobe CMD
	0x4C45,		//CALIBRATION register,
	0x20C0		//MODE CONTROL register, 	Use FIFO mode

};
//====================================================================================
//====================================================================================
const uint16_t A7129Config_PageA[]=   //868MHz, 10kbps (IFBW = 100KHz, Fdev = 37.5KHz), Crystal=12.8MHz
{
	0xF341,		//TX1 register, 	Fdev = 37.5kHz
	0x0000,		//WOR1 register,
	0xF800,		//WOR2 register,
	0x1107,		//RFI register, 	Enable Tx Ramp up/down  
	0x9170,		//PM register,		CST=1
	0x0201,		//RTH register,
	0x400F,		//AGC1 register,	
	0x6AC0,		//AGC2 register, 
	0xC041,		//GIO register, 	GIO2=WTR, GIO1=FSYNC
	0xD381,		//CKO register
	0x0004,		//VCB register,
	0x0343,		//CHG1 register, 	860MHz
	0x0044,		//CHG2 register, 	870MHz
	0x003F,		//FIFO register, 	FEP=63+1=64bytes
	0x1507,		//CODE register, 	Preamble=4bytes, ID=4bytes
	0x0000		//WCAL register,
			
};
//=======================================================================================
//=======================================================================================
const uint16_t A7129Config_PageB[]=   //868MHz, 10kbps (IFBW = 100KHz, Fdev = 37.5KHz), Crystal=12.8MHz
{
	0x8307,		//TX2 register,0x8337,0x8313,0x8307
	0x8312,		//IF1 register, 	Enable Auto-IF, IF=200KHz
	0x0000,		//IF2 register,
	0x0000,//0x0005,		//ACK register,03
	0x0000		//ART register,
	
//		0x0307,
//		0x8400,		//IF1 register, 	Enable Auto-IF, IF=100KHz
//		0x0000,		//IF2 register,
//		0x0000,		//ACK register,
//		0x0000,		//ART register,
};
#endif
//=======================================================================================
//=======================================================================================


//=========================================================================================
//初始化模块
uint8_t bsp_InitRF(uint8_t st)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	//initial pin
	RCC_APB2PeriphClockCmd(RCC_ALL_A7139, ENABLE);
	
	//==================================================================================
	//TRIS_SCS=0;
	//SCS = 1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	/* ????? */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO????? */	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_SCS;
	GPIO_Init(GPIO_PORT_SCS, &GPIO_InitStructure);		//????
	GPIO_SetBits(GPIO_PORT_SCS,GPIO_PIN_SCS);
	 //=================================================================================
   //TRIS_SCK=0;
   //SCK = 0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	/* ????? */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO????? */	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_SCK;
	GPIO_Init(GPIO_PORT_SCK, &GPIO_InitStructure);		//
	GPIO_ResetBits(GPIO_PORT_SCK,GPIO_PIN_SCK);
	//=================================================================================
  //TRIS_SDIO=0;
  //SDIO= 1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	/* ????? */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO????? */	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_SDIO;
	GPIO_Init(GPIO_PORT_SDIO, &GPIO_InitStructure);		//
	GPIO_SetBits(GPIO_PORT_SDIO,GPIO_PIN_SDIO);
	//=================================================================================
  //TRIS_CKO=0;
  //CKO	= 1;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	/* ????? */
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO????? */	
// 	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_CKO;
// 	GPIO_Init(GPIO_PORT_CKO, &GPIO_InitStructure);		//
	//================================================================================
  //TRIS_GIO1=1;
  //GIO1= 1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	/* ????? */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO????? */	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_GIO1;
	GPIO_Init(GPIO_PORT_GIO1, &GPIO_InitStructure);		//
	//=================================================================================
  //TRIS_GIO2=1;
  //GIO2= 1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	/* ????? */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO????? */	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_GIO2;
	GPIO_Init(GPIO_PORT_GIO2, &GPIO_InitStructure);		//
	//=====================================================================================
	Write_SDIO_HI();
	Write_SCS_HI();
	//=====================================================================================
	//开始初始化
  init_err=0;									//初始化错误标记 //clf add 2016.1.14
	
	if(st!=0)
	{
				A7139_Delay10us(80);	
				StrobeCMD(CMD_RF_RST);			//reset A7129 chip
				A7139_Delay10us(80);	
				A7129_Config();							//config A7129 chip
				A7139_Delay10us(80);				//delay 800us for crystal stabilized
				A7129_WriteID();						//write ID code
				A7129_Cal();								//IF and VCO calibration
				StrobeCMD(CMD_STBY);
				StrobeCMD(CMD_RX);
				A7139_Delay10us(1);

				#ifdef USE_DSSS_MODE
				DSSS_MODE();
				#endif
	}  
  return init_err;                                 //clf add 2016.1.14
}

//========================================================================================
//管脚输出方式改变
//参数：0-输出，1-输入
void Set_SDIO_Direction(uint8_t val)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	if(val==0)
	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		/* ????? */
	}
	else
	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//??????,????
	}

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO????? */	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_SDIO;
	GPIO_Init(GPIO_PORT_SDIO, &GPIO_InitStructure);
}

//===============================================================================================
//无线唤醒
void RF_TX1(uint8_t *buf,uint8_t length)
{
				unsigned int yy=0;	
				unsigned int i;

				StrobeCMD(CMD_STBY);								
				A7129_WritePageA(FIFO_PAGEA,1);			
				StrobeCMD(CMD_TFR);
				for(i=0;i<550;i++)			//150MS
				//for(i=0;i<1815;i++)			//500MS
				{
					IWDG_Feed();
	        A7129_WriteFIFO(buf,1);
					StrobeCMD(CMD_TX);
					A7139_Delay10us(1);
					yy=0;
					while(1==GPIO_ReadInputDataBit(GPIO_PORT_GIO2,GPIO_PIN_GIO2))
					{
						 yy++;
						 if(yy>0x65500)break;
					}
				}
				StrobeCMD(CMD_STBY);
				A7139_Delay10us(10);
}
//===============================================================================================
//无线模块 发送
void RF_TX(uint8_t *buf,uint8_t length)
{
	uint16_t yy;
	
	StrobeCMD(CMD_STBY);		
	A7129_WritePageA(FIFO_PAGEA,length);//clf ?20171129
	StrobeCMD(CMD_TFR);
	A7129_WriteFIFO(buf,length); 	//write data to TX FIFO
  StrobeCMD(CMD_TX);
	A7139_Delay10us(10);
	
	
	//========================================================================
	#if 0
	WaitForTime=50;
	WaitFlag=0x5a;
	#else
	while(1==Read_GIO2()) 
	{
		 yy++;
		 A7139_Delay10us(10);
		 if(yy>50000)break;	//测试用，暂时屏蔽
	}
	StrobeCMD(CMD_STBY);
	StrobeCMD(CMD_RX);
	A7139_Delay10us(1);
  #endif
	//=========================================================================	
}

//==============================================================================================
//每10mS进来检测一次
uint8_t RF_WaitForSend(void)
{
 	
	if(WaitFlag==0)return 0;
	
	if(WaitForTime>0)WaitForTime--;
	if((WaitForTime==0)||(0==Read_GIO2()))
	{
		WaitFlag=0;
		Rf_Power_En(0);
	}
	
	return 1;
	
}
//==============================================================================================
//无线模块 接收
uint8_t RF_RX(uint8_t *buf,uint8_t *plen)
{
  uint8_t cnt;
	if(0==Read_GIO2()) 
	{
		 cnt=RxPacket(buf);
		 My_NOP();
		 *plen=cnt;		 
     return 1;
	}	 
	return 0;
}

//==============================================================================================
//错误状态
void Err_State(void)
{
    //ERR display
    //Error Proc...
    //...
    //while(1);							//clf add 
    init_err=0x55;
		  
}
//==============================================================================================
// Strobe Command
void StrobeCMD(uint8_t cmd)
{
    uint8_t i;

    //SCS=0;
    Write_SCS_LO(); 
    
    for(i=0;i<8; i++)
    {
        if(cmd & 0x80)
        {
            //SDIO = 1;
            Write_SDIO_HI(); 
        }
        else
        {
            //SDIO = 0;
            Write_SDIO_LO(); 
        }
        My_NOP();
        //SCK=1;
        Write_SCK_HI();      
        My_NOP();
        //SCK=0;
        Write_SCK_LO();        
        cmd<<=1;
    }
    //SCS=1;
    Write_SCS_HI(); 
}

//=================================================================================================
//发送一个字节
void ByteSend(uint8_t src)
{
    uint8_t i;

    for(i=0; i<8; i++)
    {
        if(src & 0x80)
        {
            //SDIO = 1;
             Write_SDIO_HI(); 
        }
        else
        {
            //SDIO = 0;
            Write_SDIO_LO(); 
        }

	My_NOP();
        //SCK=1;
        Write_SCK_HI(); 
        My_NOP();
        //SCK=0;
        Write_SCK_LO(); 
        src<<=1;
    }
}

//================================================================================================
//读字节
uint8_t ByteRead(void)
{
	uint8_t i,tmp=0;

    	//TRIS_SDIO = 1;
    	Set_SDIO_Direction(1);
    	
    	for(i=0; i<8; i++)
    	{
	        //if(SDIO)
	        if(0!=Read_SDIO())
	       {
	            tmp = (tmp << 1) | 0x01;
	        }
	        else
	        {
	            tmp = tmp << 1;
	        }

	        //SCK=1;
	        Write_SCK_HI();  
	        My_NOP();
	        //SCK=0;
	        Write_SCK_LO(); 
    	}
	//TRIS_SDIO = 0;
	Set_SDIO_Direction(0);
    	return tmp;
}

//==============================================================================================
//A7129_WriteReg
void A7129_WriteReg(uint8_t address, uint16_t dataWord)
{
    uint8_t i;

    //SCS=0;
    Write_SCS_LO(); 
    address |= CMD_Reg_W;
    for(i=0; i<8; i++)
    {
        if(address & 0x80)
        {
            //SDIO = 1;
             Write_SDIO_HI(); 
        }
        else
        {
            //SDIO = 0;
             Write_SDIO_LO(); 
        }

        //SCK=1;
        Write_SCK_HI(); 
        My_NOP(); 
        //SCK=0;
        Write_SCK_LO(); 
        address<<=1;
    }
    My_NOP();

    //send data word
    for(i=0; i<16; i++)
    {
        if(dataWord & 0x8000)
        {
            //SDIO = 1;
            Write_SDIO_HI(); 
        }
        else
        {
            //SDIO = 0;
            Write_SDIO_LO(); 
        }

        //SCK=1;
        Write_SCK_HI(); 
         
        My_NOP();
        //SCK=0;
        Write_SCK_LO(); 
        dataWord<<=1;
    }
    //SCS=1;
    Write_SCS_HI(); 
}

//=============================================================================
//A7129_ReadReg
uint16_t A7129_ReadReg(uint8_t address)
{
    uint8_t i;
    uint16_t tmp=0;

    //SCS=0;
    Write_SCS_LO(); 
    
    address |= CMD_Reg_R;
    for(i=0; i<8; i++)
    {
        if(address & 0x80)
        {
            //SDIO = 1;
            Write_SDIO_HI(); 
        }
        else
        {
            //SDIO = 0;
            Write_SDIO_LO(); 
        }

	My_NOP(); 
        //SCK=1;
        Write_SCK_HI(); 
        My_NOP();
        //SCK=0;
        Write_SCK_LO(); 
        address<<=1;
    }
    My_NOP();
    
    //read data code
    //TRIS_SDIO = 1;		//SDIO pull high
    Set_SDIO_Direction(1);
    
    for(i=0; i<16; i++)
    {
        //if(SDIO)
        if(0!=Read_SDIO())
        {
            tmp = (tmp << 1) | 0x01;
        }
        else
        {
            tmp = tmp << 1;
        }

        //SCK=1;
        Write_SCK_HI(); 
        My_NOP();
        //SCK=0;
        Write_SCK_LO(); 
         
    }
    //SCS=1;
    Write_SCS_HI(); 
     
    //TRIS_SDIO = 0;
    Set_SDIO_Direction(0); 
    return tmp;
}

//============================================================================================
//A7129_WritePageA
void A7129_WritePageA(uint8_t address, uint16_t dataWord)
{
	    uint16_t tmp;
	
	    tmp = address;
	    tmp = ((tmp << 12) | A7129Config[CRYSTAL_REG]);
	    A7129_WriteReg(CRYSTAL_REG, tmp);
	    A7129_WriteReg(PAGEA_REG, dataWord);
}

//===========================================================================================
//A7129_ReadPageA
uint16_t A7129_ReadPageA(uint8_t address)
{
	    uint16_t tmp;
	
	    tmp = address;
	    tmp = ((tmp << 12) | A7129Config[CRYSTAL_REG]);
	    A7129_WriteReg(CRYSTAL_REG, tmp);
	    tmp = A7129_ReadReg(PAGEA_REG);
	    return tmp;
}

//===========================================================================================
//A7129_WritePageB
void A7129_WritePageB(uint8_t address, uint16_t dataWord)
{
	    uint16_t tmp;
	
	    tmp = address;
	    tmp = ((tmp << 7) | A7129Config[CRYSTAL_REG]);
	    A7129_WriteReg(CRYSTAL_REG, tmp);
	    A7129_WriteReg(PAGEB_REG, dataWord);
}

//===========================================================================================
//A7129_ReadPageB
uint16_t A7129_ReadPageB(uint8_t address)
{
	    uint16_t tmp;
	
	    tmp = address;
	    tmp = ((tmp << 7) | A7129Config[CRYSTAL_REG]);
	    A7129_WriteReg(CRYSTAL_REG, tmp);
	    tmp = A7129_ReadReg(PAGEB_REG);
	    return tmp;
}
//===========================================================================================
//A7129_Config
void A7129_Config(void)
{
	uint8_t i;
	uint16_t tmp;

    	for(i=0; i<8; i++)
    	{
        	A7129_WriteReg(i, A7129Config[i]);
        }

	for(i=10; i<16; i++)
	{
        	A7129_WriteReg(i, A7129Config[i]);
        }

    	for(i=0; i<16; i++)
    	{
        	A7129_WritePageA(i, A7129Config_PageA[i]);
        }

	for(i=0; i<5; i++)
	{
        	A7129_WritePageB(i, A7129Config_PageB[i]);
  }

	//for check        
	tmp = A7129_ReadReg(SYSTEMCLOCK_REG);
	if(tmp != A7129Config[SYSTEMCLOCK_REG])
	{
		Err_State();	
	}

}

//===================================================================================================
//WriteID
void A7129_WriteID(void)
{
	uint8_t i;
	uint8_t d1, d2, d3, d4;

	//SCS=0;
	Write_SCS_LO(); 
	ByteSend(CMD_ID_W);
	for(i=0; i<4; i++)
	{
		ByteSend(ID_Tab[i]);
	}
	
	//SCS=1;
  	Write_SCS_HI(); 
	//SCS=0;
	Write_SCS_LO(); 
	ByteSend(CMD_ID_R);
	d1=ByteRead();
	d2=ByteRead();
	d3=ByteRead();
	d4=ByteRead();
	
	//SCS=1;
	Write_SCS_HI(); 
	
	if((d1!=ID_Tab[0]) || (d2!=ID_Tab[1]) || (d3!=ID_Tab[2]) || (d4!=ID_Tab[3]))
	{
	       Err_State();
	 }
}

//=================================================================================================
// A7129_Cal
void A7129_Cal(void)
{
	uint8_t fb,fcd, fbcf;							//IF Filter
	uint8_t vb,vbcf;									//VCO Current
	uint8_t vcb, vccf;								//VCO Band
	uint16_t tmp;
	uint16_t dly=0;
	
	//===============================================================================================
	//防止编译警告
	fb=fb;
	fcd=fcd;
	vcb=vcb;
	vb=vb;
	//===============================================================================================
	A7129_WriteReg(MODE_REG, A7129Config[MODE_REG] | 0x0802);			//IF Filter & VCO Current Calibration
	A7139_Delay10us(1);			//clf test
	dly=5000;
    	do{
        	tmp = A7129_ReadReg(MODE_REG);
		A7139_Delay10us(1);dly--;if(dly==0)break;			//clf add 2016.1.13
    	}while(tmp & 0x0802);

    	//for check(IF Filter)
    	tmp = A7129_ReadReg(CALIBRATION_REG);
    	fb = tmp & 0x0F;
	fcd = (tmp>>11) & 0x1F;
    	fbcf = (tmp>>4) & 0x01;
    	if(fbcf)
    	{
        	Err_State();
    	}

	//for check(VCO Current)
    	tmp = A7129_ReadPageA(VCB_PAGEA);
	vcb = tmp & 0x0F;
	vccf = (tmp>>4) & 0x01;
	if(vccf)
	{
        	Err_State();
    	}
    
    
    	//RSSI Calibration procedure @STB state
	A7129_WriteReg(ADC_REG, 0x4C00);									//set ADC average=64
    	A7129_WriteReg(MODE_REG, A7129Config[MODE_REG] | 0x1000);			//RSSI Calibration
	dly=5000;
    	do{
        	tmp = A7129_ReadReg(MODE_REG);
		A7139_Delay10us(1);dly--;if(dly==0)break;	         //clf add 2016.1.13
    	}while(tmp & 0x1000);
	A7129_WriteReg(ADC_REG, A7129Config[ADC_REG]);


    	//VCO calibration procedure @STB state
	A7129_WriteReg(PLL1_REG, A7129Config[PLL1_REG]);
	A7129_WriteReg(PLL2_REG, A7129Config[PLL2_REG]);
	A7129_WriteReg(MODE_REG, A7129Config[MODE_REG] | 0x0004);		//VCO Band Calibration
	dly=5000;
	do{
		tmp = A7129_ReadReg(MODE_REG);
		A7139_Delay10us(1);dly--;if(dly==0)break;	
	}while(tmp & 0x0004);

	//for check(VCO Band)
	tmp = A7129_ReadReg(CALIBRATION_REG);
	vb = (tmp >>5) & 0x07;
	vbcf = (tmp >>8) & 0x01;
	if(vbcf)
	{
		Err_State();
	}
}

//================================================================================================
// A7129_WriteFIFO
//void A7129_WriteFIFO(void)
void A7129_WriteFIFO(uint8_t *buf,uint8_t length)
{
	uint8_t i;

	
	StrobeCMD(CMD_TFR);			//TX FIFO address pointer reset

        //SCS=0;
  	Write_SCS_LO(); 
	ByteSend(CMD_FIFO_W);			//TX FIFO write command
	
	//__set_PRIMASK(1);			//关中断
	
       ByteSend(length);   			//clf   add
	for(i=0;i<length;i++)
	{		
		ByteSend(buf[i]);
	}
	
	//__set_PRIMASK(0);	//开中断
	
	//SCS=1;
	Write_SCS_HI(); 
}

//=======================================================================================
//RxPacket
uint8_t RxPacket(uint8_t *buf)
{
	    uint8_t i;
	    uint8_t tmp;
	
	    StrobeCMD(CMD_RFR);		//RX FIFO address pointer reset
    	//SCS=0;
    	Write_SCS_LO(); 
    	   
			//StrobeCMD(CMD_RFR);		//RX FIFO address pointer reset  //clf mod 20171201
	    ByteSend(CMD_FIFO_R);		//RX FIFO read command
	    tmp =ByteRead();
	
	    //数据过长
	    if(tmp>=(RF_DATA_BUF_MAX_LEN-1))
	    {
					StrobeCMD(CMD_STBY);
					StrobeCMD(CMD_RX);
					//Set_GPIO_SCS();
					//Write_SCS_HI(); 
					return 0;
      }
			
			
			if(tmp==0)return 0;
				
	    for(i=0; i<tmp; i++)																		//此处要当心，缓冲区要开的大点
	    {
					buf[i] = ByteRead();
	    }
			
	    //bsp_InitRF(1);
			
			StrobeCMD(CMD_RF_RST);
			A7139_Delay10us(80);	
			//StrobeCMD(CMD_STBY);
			StrobeCMD(CMD_RX);
			
	    //SCS=1;
	    Write_SCS_HI(); 
	    
	    return tmp;			//返回长度 
}

//========================================================================================
void A7139_Delay10us(uint16_t dly)
{
	uint16_t td;
	
	td=dly;
	while(td>0)
	{
	    td--;
	    My_NOP();
	}
}

//==========================================================================================
void Rf_Power_En(uint8_t stu)
{

}

//========================================================================================
void My_NOP(void)
{
	uint8_t dly;
	for(dly=0;dly<10;dly++);	
}


//=======================================================================================
//以下用于扩频和无线唤醒
#ifdef USE_DSSS_MODE
/*********************************************************************
** RC Oscillator Calibration
*********************************************************************/
void RCOSC_Cal(void)
{
        Uint16 tmp;
	Uint16 TGNUM;  
	
	StrobeCMD(CMD_STBY);
	
	TGNUM = RCOSC_TGNUM;
	A7129_WritePageB(CCM1_PAGEB, (TGNUM<<4));

	A7129_WritePageA(WOR2_PAGEA, A7129Config_PageA[WOR2_PAGEA] | 0x0011);		//enable RC OSC and RCOT=1

	while(1)
	{
		A7129_WritePageA(WCAL_PAGEA, A7129Config_PageA[WCAL_PAGEA] | 0x0001);	//set ENCAL=1 to start RC OSC CAL
		do{
			tmp = A7129_ReadPageA(WCAL_PAGEA);
		}while(tmp & 0x0001);
			
		tmp = (A7129_ReadPageA(WCAL_PAGEA) & 0x1FFF);		//read NUMLH[11:0]
		tmp >>= 1;	
		
		if((tmp > (TGNUM-10)) && (tmp < (TGNUM+10)))		//NUMLH[11:0]=TGNUM+-10
		{
			break;
		}
	}
}

/*********************************************************************
** WOR_enable_by_preamble
*********************************************************************/
void WOR_enable_by_preamble(void)
{
	RCOSC_Cal();     	//RC Oscillator Calibration
	
	A7129_WritePageA(GIO_PAGEA, (A7129Config_PageA[GIO_PAGEA] & 0xF000) | 0x004D);	//GIO1=PMDO, GIO2=WTR
	//A7159_WritePageA(GIO_PAGEA, (A7159Config_PageA[GIO_PAGEA] & 0xF000) | 0x0051);	//GIO1=TWWS, GIO2=WTR

	//setup WOR Sleep time and Rx time
	//WOR Active Period = (WOR_AC[8:0]+1) x (1/32768), (30.5us ~ 15.6ms).
	//WOR Sleep  Period = (WOR_SL[9:0]+1) x (32/4096), (7.8ms ~ 7.99s).
	A7129_WritePageA(WOR1_PAGEA, 0x0005);		//WOR_SL[9:0]=5   : WOR Sleep  Period ~ 46.875ms 
	A7129_WritePageB(ACKFIFO_PAGEB, 0x8000);	//WOR_AC[8:0]=256 : WOR Active Period ~ 7.84ms
	
	//WOR_HOLD : WOR hold RX setting when preamble/sync/carrier detected ok.
	//			[0]: No hold. [1]: Hold RX.
	//WOR_SEL: TWWS=1 setting. 
	//			[0]:RX valid packet.  [1]: preamble/sync/carrier  detected ok.
	//WOR_RST: TWWS=1 width setting. 
	//			[0]: pulse, reset by EOP. [1]: hold high, reset by strobe command.
	A7129_WritePageB(DSSS_PAGEB, A7129Config_PageB[DSSS_PAGEB] & ~0x0800);	//WOR_SEL=0
	A7129_WritePageB(DSSS2_PAGEB, A7129Config_PageB[DSSS2_PAGEB] | 0x0003);	//WOR_HOLD=1, WOR_RST=1
	
	A7129_WritePageA(WOR2_PAGEA, A7129Config_PageA[WOR2_PAGEA] | 0x0031);	//enable RC OSC & WOR by preamble
	
	A7129_WriteReg(MODE_REG, A7129Config[MODE_REG] | 0x0200);				//WORE=1 to enable WOR function
	
	//CLF MOD 20161027
	while(Read_GIO2()==0);		//GIO1=PMDO : Stay in WOR mode until receiving preamble(preamble ok)
	//while(GIO1==0);		//GIO1=TWWS	: Stay in WOR mode until receiving valid packet. (WOR_SEL=0)
}

extern void RSSI_measurement(void);
/*********************************************************************
** WOR_enable_by_sync
*********************************************************************/
void WOR_enable_by_sync(void)
{
	RCOSC_Cal();     	//RC Oscillator Calibration
	
	A7129_WritePageA(GIO_PAGEA, (A7129Config_PageA[GIO_PAGEA] & 0xF000) | 0x0045);	//GIO1=FSYNC, GIO2=WTR
	//A7159_WritePageA(GIO_PAGEA, (A7159Config_PageA[GIO_PAGEA] & 0xF000) | 0x0051);	//GIO1=TWWS, GIO2=WTR

	//setup WOR Sleep time and Rx time
	//WOR Active Period = (WOR_AC[8:0]+1) x (1/32768), (30.5us ~ 15.6ms).
	//WOR Sleep  Period = (WOR_SL[9:0]+1) x (32/4096), (7.8ms ~ 7.99s).
	A7129_WritePageA(WOR1_PAGEA, 0xFC05);		//WOR_SL[9:0]=5   : WOR Sleep  Period ~ 46.875ms 
	A7129_WritePageB(ACKFIFO_PAGEB, 0xE000);	//WOR_AC[8:0]=256 : WOR Active Period ~ 7.84ms
	
	//WOR_HOLD : WOR hold RX setting when preamble/sync/carrier detected ok.
	//			[0]: No hold. [1]: Hold RX.
	//WOR_SEL: TWWS=1 setting. 
	//			[0]:RX valid packet.  [1]: preamble/sync/carrier  detected ok.
	//WOR_RST: TWWS=1 width setting. 
	//			[0]: pulse, reset by EOP. [1]: hold high, reset by strobe command.
	A7129_WritePageB(DSSS_PAGEB, A7129Config_PageB[DSSS_PAGEB] & ~0x0800);	//WOR_SEL=0
	A7129_WritePageB(DSSS2_PAGEB, A7129Config_PageB[DSSS2_PAGEB] | 0x0003);	//WOR_HOLD=1, WOR_RST=1
	
	A7129_WritePageA(WOR2_PAGEA, A7129Config_PageA[WOR2_PAGEA] | 0x0011);	//enable RC OSC & WOR by sync
	
	A7129_WriteReg(MODE_REG, A7129Config[MODE_REG] | 0x0200);				//WORE=1 to enable WOR function
	
//	while(GIO1==0);		//GIO1=FSYNC: Stay in WOR mode until receiving ID code(sync ok)
	My_NOP();
	//while(GIO1==1);
//	RSSI_measurement();
	//RxPacket(xxyy);
	My_NOP();
	My_NOP();
	//while(GIO1==0);		//GIO1=TWWS	: Stay in WOR mode until receiving valid packet. (WOR_SEL=0)
}

/*********************************************************************
** WOR_enable_by_RSSI_carrier
*********************************************************************/
void WOR_enable_by_RSSI_carrier(void)
{
	RCOSC_Cal();     	//RC Oscillator Calibration
	
	A7129_WritePageA(GIO_PAGEA, (A7129Config_PageA[GIO_PAGEA] & 0xF000) | 0x0049);	//GIO1=CD, GIO2=WTR
	//A7159_WritePageA(GIO_PAGEA, (A7159Config_PageA[GIO_PAGEA] & 0xF000) | 0x0051);	//GIO1=TWWS, GIO2=WTR

	//setup WOR Sleep time and Rx time
	//WOR Active Period = (WOR_AC[8:0]+1) x (1/32768), (30.5us ~ 15.6ms).
	//WOR Sleep  Period = (WOR_SL[9:0]+1) x (32/4096), (7.8ms ~ 7.99s).
	A7129_WritePageA(WOR1_PAGEA, 0x0005);		//WOR_SL[9:0]=5   : WOR Sleep  Period ~ 46.875ms 
	A7129_WritePageB(ACKFIFO_PAGEB, 0x8000);	//WOR_AC[8:0]=256 : WOR Active Period ~ 7.84ms
	
	//WOR_HOLD : WOR hold RX setting when preamble/sync/carrier detected ok.
	//			[0]: No hold. [1]: Hold RX.
	//WOR_SEL: TWWS=1 setting. 
	//			[0]:RX valid packet.  [1]: preamble/sync/carrier  detected ok.
	//WOR_RST: TWWS=1 width setting. 
	//			[0]: pulse, reset by EOP. [1]: hold high, reset by strobe command.
	A7129_WritePageB(DSSS_PAGEB, A7129Config_PageB[DSSS_PAGEB] & ~0x0800);	//WOR_SEL=0
	A7129_WritePageB(DSSS2_PAGEB, A7129Config_PageB[DSSS2_PAGEB] | 0x0003);	//WOR_HOLD=1, WOR_RST=1
	
	A7129_WritePageA(WOR2_PAGEA, A7129Config_PageA[WOR2_PAGEA] | 0x0411);	//enable RC OSC & WOR by carrier
	A7129_WritePageA(RFI_PAGEA, A7129Config_PageA[RFI_PAGEA] & ~0x6000);	//select CDSEL=[0x] : RSSI Carrier Detect
	A7129_WriteReg(ADC_REG, A7129Config[ADC_REG] | 0x8064);					//ARSSI=1, RTH=100

	A7129_WriteReg(MODE_REG, A7129Config[MODE_REG] | 0x0200);				//WORE=1 to enable WOR function
	
	//CLF MOD 20161027 
	while(Read_GIO2()==0);		//GIO1=CD	: Stay in WOR mode until carrier signal strength is greater than the value set by RTH[7:0](carrier ok)
	//while(GIO1==0);		//GIO1=TWWS	: Stay in WOR mode until receiving valid packet. (WOR_SEL=0)	
}

/*********************************************************************
** WOT_enable
*********************************************************************/
void WOT_enable(void)
{
	RCOSC_Cal();     		//RC Oscillator Calibration
	
	A7129_WritePageA(GIO_PAGEA, (A7129Config_PageA[GIO_PAGEA] & 0xF000) | 0x0045);	//GIO1=FSYNC, GIO2=WTR

	//setup WOR Sleep time.
	//WOR Sleep  Period = (WOR_SL[9:0]+1) x (32/4096), (7.8ms ~ 7.99s).
	A7129_WritePageA(WOR1_PAGEA, 0x0005);		//WOR_SL[9:0]=5   : WOR Sleep  Period ~ 46.875ms 
	
//	A7129_WriteFIFO();		//write data to TX FIFO

	A7129_WriteReg(PIN_REG, A7129Config[PIN_REG] | 0x0400);		//WMODE=1 select WOT function
	
	A7129_WriteReg(MODE_REG, A7129Config[MODE_REG] | 0x0200);	//WORE=1 to enable WOT function
	
	while(1);
}	
//================================================================================
void RSSI_measurement(void)
{
	Uint16 tmp;

	StrobeCMD(CMD_STBY);
	
	A7129_WritePageA(GIO_PAGEA, (A7129Config_PageA[GIO_PAGEA] & 0xF000) | 0x0045);	//GIO1=FSYNC, GIO2=WTR
	
	A7129_WriteReg(ADC_REG, A7129Config[ADC_REG] | 0x8000);		//ARSSI=1
	
	StrobeCMD(CMD_RX);	//entry RX mode
	A7139_Delay10us(1);
	
	while(Read_GIO2()==0)		//Stay in WOR mode until receiving ID code(sync ok)
	{
		tmp = (A7129_ReadReg(ADC_REG) & 0x00FF);	//read RSSI value(environment RSSI)
	}
	tmp = (A7129_ReadReg(ADC_REG) & 0x00FF);		//read RSSI value(wanted signal RSSI)
	RSSI=tmp;
	My_NOP();
	My_NOP();
	My_NOP();
}
/*********************************************************************
** DSSS_MODE
*********************************************************************/
void DSSS_MODE(void)
{
	A7129_WritePageA(CODE_PAGEA, 0x5502);	//PML=3bytes, IDL=1bytes
	//DSSS mode : Preamble Length=3bytes
	//IDL[1:0]=[x0], x=don't care, ID Length=1byte
	//IDL[1:0]=[x1], x=don't care, ID Length=2bytes
	
	A7129_WritePageB(DC_SHIFT_PAGEB, A7129Config_PageB[DC_SHIFT_PAGEB] | 0x0400);	//DSSS mode : Preamble detect=2bytes
	A7129_WritePageB(DSSS_PAGEB, A7129Config_PageB[DSSS_PAGEB] | 0xC4FF);	//DSSS setting : ETHM=3, DSSS_SEL=1, FSYNC_D=255
	A7129_WritePageB(DSSS2_PAGEB, A7129Config_PageB[DSSS2_PAGEB] | 0x6200);	//TCNT_SEL=1, TCNT=34
	A7129_WritePageB(SYN_PAGEB, A7129Config_PageB[SYN_PAGEB] | 0x0400);		//enable DSSS=1
}
#endif








