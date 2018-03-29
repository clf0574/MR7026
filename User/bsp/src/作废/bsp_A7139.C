
//======================================================================================================
//包含头文件
#include "includes.h"
#include "bsp.h"
#include "A7129reg.h"
//#include "A7139reg.h"

//======================================================================================================
//管脚定义

/* 按键口对应的RCC时钟 */
#define RCC_ALL_A7139 	(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB)

#define GPIO_PORT_SCS   GPIOA
#define GPIO_PIN_SCS	GPIO_Pin_4

#define GPIO_PORT_SCK   GPIOA
#define GPIO_PIN_SCK	GPIO_Pin_5

#define GPIO_PORT_SDIO   GPIOA
#define GPIO_PIN_SDIO	GPIO_Pin_7

#define GPIO_PORT_CKO   GPIOB
#define GPIO_PIN_CKO	GPIO_Pin_1

#define GPIO_PORT_GIO1   GPIOB
#define GPIO_PIN_GIO1	GPIO_Pin_0

#define GPIO_PORT_GIO2   GPIOA
#define GPIO_PIN_GIO2	GPIO_Pin_6



void Delay100us(uint8_t n);
void Delay10us(uint8_t n);



//#define TRIS_SCS  TRISC4
//#define SCS 		PORTCbits.RC4 		//SPI SCS
//#define TRIS_SCK  TRISC6
//#define SCK 		PORTCbits.RC6 		//SPI SCK
//#define TRIS_SDIO TRISC3
//#define SDIO 		PORTCbits.RC3 		//SPI SDIO
//#define TRIS_CKO  TRISB1
//#define CKO 		PORTBbits.RB1 		//CKO
//#define TRIS_GIO1  TRISB0
//#define GIO1 		PORTBbits.RB0 		//GIO1
//#define TRIS_GIO2  TRISC5
//#define GIO2 		PORTCbits.RC5 		//GIO2  输入
//========================================================================================
//变量声明
/*********************************************************************
**  Constant Declaration
*********************************************************************/
#define TIMEOUT     100			//100ms
#define t0hrel      1000		//1ms

// #define Uint8  uint8_t 
// #define Uint16 uint16_t
// #define Uint32 uint32_t
/*********************************************************************
**  Global Variable Declaration
*********************************************************************/
uint8_t    timer;
uint8_t    TimeoutFlag;
uint16_t   RxCnt;
uint32_t   Err_ByteCnt;
uint32_t   Err_BitCnt;
uint16_t   TimerCnt0;
uint8_t    *Uartptr;
uint8_t    UartSendCnt;
uint8_t    CmdBuf[11];
uint8_t    tmpbuf[64];

static uint8_t  init_err=0;

const uint8_t BitCount_Tab[16]={0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4};
//const uint8_t ID_Tab[8]={0xd3,0x91,0xd3,0x91,0xC7,0x33,0x45,0xE7};   //ID code
const uint8_t ID_Tab[4]={0xd3,0x91,0xd3,0x91};
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




const uint16_t A7129Config[]=		//868MHz, 10kbps (IFBW = 100KHz, Fdev = 37.5KHz), Crystal=12.8MHz
{
	0x1EC4,		//SYSTEM CLOCK register,
	0x0A43,		//PLL1 register,
	0xd3b8,//0xD405,		//PLL2 register,	866.001MHz
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


const uint16_t A7129Config_PageB[]=   //868MHz, 10kbps (IFBW = 100KHz, Fdev = 37.5KHz), Crystal=12.8MHz
{
	0x8307,		//TX2 register,0x8337,0x8313,0x8307
	0x8312,		//IF1 register, 	Enable Auto-IF, IF=200KHz
	0x0000,		//IF2 register,
	0x0005,		//ACK register,03
	0x0000		//ART register,
};





//const uint16_t A7129Config[]=		//868MHz, 10kbps (IFBW = 100KHz, Fdev = 37.5KHz), Crystal=12.8MHz
//{
//	0x1EC4,		//SYSTEM CLOCK register,
//	0x0A43,		//PLL1 register,
//	0xD405,		//PLL2 register,	866.001MHz
//	0x0000,		//PLL3 register,
//	0x1A20,		//PLL4 register,
//	0x0024,		//PLL5 register,
//	0x0000,		//PLL6 register,
//	0x0013,		//CRYSTAL register,
//	0x0000,		//PAGEA,
//	0x0000,		//PAGEB,
//	0x18D4,		//RX1 register, 	IFBW=100KHz	
//	0x7009,		//RX2 register, 	by preamble
//	0x4400,		//ADC register,	   	
//	0x0800,		//PIN CONTROL register,		Use Strobe CMD
//	0x4C45,		//CALIBRATION register,
//	0x20C0		//MODE CONTROL register, 	Use FIFO mode
//};

//const uint16_t A7129Config_PageA[]=   //868MHz, 10kbps (IFBW = 100KHz, Fdev = 37.5KHz), Crystal=12.8MHz
//{
//	0xF341,		//TX1 register, 	Fdev = 37.5kHz
//	0x0000,		//WOR1 register,
//	0xF800,		//WOR2 register,
//	0x1107,		//RFI register, 	Enable Tx Ramp up/down  
//	0x9170,		//PM register,		CST=1
//	0x0201,		//RTH register,
//	0x400F,		//AGC1 register,	
//	0x6AC0,		//AGC2 register, 
//	0xC041,		//GIO register, 	
//	0xD381,		//CKO register
//	0x0004,		//VCB register,
//	0x0343,		//CHG1 register, 	860MHz
//	0x0044,		//CHG2 register, 	870MHz
//	0x003F,		//FIFO register, 	FEP=63+1=64bytes
//	
//	0x1507,		//CODE register, 	Preamble=4bytes, ID=4bytes
//	0x0000		//WCAL register,
//};

//const uint16_t A7129Config_PageB[]=   //868MHz, 10kbps (IFBW = 100KHz, Fdev = 37.5KHz), Crystal=12.8MHz
//{
//	0x8337,		//TX2 register,
//	0x8312,		//IF1 register, 	Enable Auto-IF, IF=200KHz
//	0x0000,		//IF2 register,
//	0x0000,		//ACK register,
//	0x0000		//ART register,
//};


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
void NOP(void);

/*********************************************************************
** initRF
*********************************************************************/

//=========================================================================================
//初始化模块
uint8_t bsp_InitRF(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//initial pin
	RCC_APB2PeriphClockCmd(RCC_ALL_A7139, ENABLE);
	
	//==================================================================================
	//片选信号
	//TRIS_SCS=0;
	//SCS = 1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	/* 设为输出口 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO口最大速度 */	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_SCS;
	GPIO_Init(GPIO_PORT_SCS, &GPIO_InitStructure);		//片选信号
	GPIO_SetBits(GPIO_PORT_SCS,GPIO_PIN_SCS);
	 
        //=================================================================================
        //时钟信号
    	//TRIS_SCK=0;
        //SCK = 0;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	/* 设为输出口 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO口最大速度 */	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_SCK;
	GPIO_Init(GPIO_PORT_SCK, &GPIO_InitStructure);		//
	GPIO_ResetBits(GPIO_PORT_SCK,GPIO_PIN_SCK);
	
	//=================================================================================
	//输入输出口	
    	//TRIS_SDIO=0;
    	//SDIO= 1;
    	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	/* 设为输出口 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO口最大速度 */	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_SDIO;
	GPIO_Init(GPIO_PORT_SDIO, &GPIO_InitStructure);		//
	GPIO_SetBits(GPIO_PORT_SDIO,GPIO_PIN_SDIO);
	
	//=================================================================================
	//无线模块时钟输出，这里不用	
    	//TRIS_CKO=0;
    	//CKO	= 1;

     	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	/* 设为输出口 */
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO口最大速度 */	
 	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_CKO;
 	GPIO_Init(GPIO_PORT_CKO, &GPIO_InitStructure);		//
	
	//================================================================================
	//通用管脚GIO1	
    	//TRIS_GIO1=1;
    	//GIO1= 1;
    	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	/* 设为输出口 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO口最大速度 */	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_GIO1;
	GPIO_Init(GPIO_PORT_GIO1, &GPIO_InitStructure);		//
	
	//=================================================================================
	//通用管脚GIO2
    	//TRIS_GIO2=1;
    	//GIO2= 1;
    	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	/* 设为输出口 */
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO口最大速度 */	
			GPIO_InitStructure.GPIO_Pin = GPIO_PIN_GIO2;
			GPIO_Init(GPIO_PORT_GIO2, &GPIO_InitStructure);		//
    	
    	//=================================================================================
      init_err=0;																				//初始化错误标记 //clf add 2016.1.14
			
     	StrobeCMD(CMD_RF_RST);	//reset A7129 chip
    	A7129_Config();			//config A7129 chip
    	Delay100us(8);			//delay 800us for crystal stabilized
    	A7129_WriteID();		//write ID code
    	A7129_Cal();			//IF and VCO calibration
			StrobeCMD(CMD_STBY);
			StrobeCMD(CMD_RX);
			Delay10us(1);
			//trobeCMD(CMD_TX);
	  
		  return init_err;                                 //clf add 2016.1.14
}
//===============================================================================================
//无线模块 发送
void RF_TX(uint8_t *buf,uint8_t length)
{
	uint32_t yy=0;	
	//uint8_t tempbuf[65],i;
	
	StrobeCMD(CMD_STBY);	
	StrobeCMD(CMD_TFR);
	
//	//clf add 2017.3.17
//	tempbuf[0]=length;
//	for(i=0;i<length;i++)tempbuf[i+1]=buf[i];				
//	A7129_WriteFIFO(tempbuf,length+1); 	//write data to TX FIFO
	
	A7129_WriteFIFO(buf,length); 
	
	
	
  StrobeCMD(CMD_TX);
	Delay10us(1);
	
	
	
	//while(GIO2)
	yy=0;
	while(1==GPIO_ReadInputDataBit(GPIO_PORT_GIO2,GPIO_PIN_GIO2))
	{
		 yy++;
		 Delay100us(1);
		 if(yy>30)break;	//测试用，暂时屏蔽
	}
	
	
	
// 	yy=0;
// 	while(0==GPIO_ReadInputDataBit(GPIO_PORT_GIO2,GPIO_PIN_GIO2))
// 	{
// 		 yy++;
// 		 Delay100us(1);
// 		 if(yy>90000)break;	//测试用，暂时屏蔽
// 	}
	
	StrobeCMD(CMD_STBY);
	StrobeCMD(CMD_RX);
	
	Delay10us(1);
}

//==============================================================================================
//无线模块 接收
uint8_t RF_RX(uint8_t *buf)
{
	uint8_t cnt,i,j;
	
	//if(GIO2==0)
	if(0==GPIO_ReadInputDataBit(GPIO_PORT_GIO2,GPIO_PIN_GIO2))
	{
		 cnt=RxPacket(buf);
		 NOP();
		 NOP();
		 //bsp_InitRF();//CLF ADD 2015.12.02
		 //===========================================================================
		 //数据处理,帧截取
		 if(cnt==0x10)
		 {
					return 1;						//0x10为ID系统标准帧长度
		 }
     else 
		 {
					for(j=0;j<cnt;j++)
			    {
						  //if((*(buf+j)==0x10)&&(j<0x10))
							if((*(buf+j)==0x10)&&(j>=0x10))   //clf 2016.1.9 ADD
							{
									for(i=0;i<0x10;i++)
								  {
											*(buf+i)=*(buf+i+j);
									}
									return 1;
							}
					}
		 }
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
    GPIO_ResetBits(GPIO_PORT_SCS,GPIO_PIN_SCS);
    
    for(i=0;i<8; i++)
    {
        if(cmd & 0x80)
        {
            //SDIO = 1;
            GPIO_SetBits(GPIO_PORT_SDIO,GPIO_PIN_SDIO);
        }
        else
        {
            //SDIO = 0;
            GPIO_ResetBits(GPIO_PORT_SDIO,GPIO_PIN_SDIO);
        }
        NOP();
        //SCK=1;
        GPIO_SetBits(GPIO_PORT_SCK,GPIO_PIN_SCK);
        
        NOP();
        //SCK=0;
        GPIO_ResetBits(GPIO_PORT_SCK,GPIO_PIN_SCK);
        
        cmd<<=1;
    }
    //SCS=1;
    GPIO_SetBits(GPIO_PORT_SCS,GPIO_PIN_SCS);
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
             GPIO_SetBits(GPIO_PORT_SDIO,GPIO_PIN_SDIO);
        }
        else
        {
            //SDIO = 0;
            GPIO_ResetBits(GPIO_PORT_SDIO,GPIO_PIN_SDIO);
        }

	NOP();
        //SCK=1;
        GPIO_SetBits(GPIO_PORT_SCK,GPIO_PIN_SCK);
        NOP();
        //SCK=0;
        GPIO_ResetBits(GPIO_PORT_SCK,GPIO_PIN_SCK);
         
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
	        if(0!=GPIO_ReadInputDataBit(GPIO_PORT_SDIO,GPIO_PIN_SDIO))
	       	{
	            tmp = (tmp << 1) | 0x01;
	        }
	        else
	        {
	            tmp = tmp << 1;
	        }

	        //SCK=1;
	        GPIO_SetBits(GPIO_PORT_SCK,GPIO_PIN_SCK);
	         
	        NOP();
	        //SCK=0;
	        GPIO_ResetBits(GPIO_PORT_SCK,GPIO_PIN_SCK);
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
    GPIO_ResetBits(GPIO_PORT_SCS,GPIO_PIN_SCS);
    
    address |= CMD_Reg_W;
    for(i=0; i<8; i++)
    {
        if(address & 0x80)
        {
            //SDIO = 1;
             GPIO_SetBits(GPIO_PORT_SDIO,GPIO_PIN_SDIO);
        }
        else
        {
            //SDIO = 0;
             GPIO_ResetBits(GPIO_PORT_SDIO,GPIO_PIN_SDIO);
        }

        //SCK=1;
        GPIO_SetBits(GPIO_PORT_SCK,GPIO_PIN_SCK);
        NOP(); 
        //SCK=0;
        GPIO_ResetBits(GPIO_PORT_SCK,GPIO_PIN_SCK);
         
        address<<=1;
    }
    NOP();

    //send data word
    for(i=0; i<16; i++)
    {
        if(dataWord & 0x8000)
        {
            //SDIO = 1;
            GPIO_SetBits(GPIO_PORT_SDIO,GPIO_PIN_SDIO);
        }
        else
        {
            //SDIO = 0;
             GPIO_ResetBits(GPIO_PORT_SDIO,GPIO_PIN_SDIO);
        }

        //SCK=1;
        GPIO_SetBits(GPIO_PORT_SCK,GPIO_PIN_SCK);
         
        NOP();
        //SCK=0;
        GPIO_ResetBits(GPIO_PORT_SCK,GPIO_PIN_SCK);
        dataWord<<=1;
    }
    //SCS=1;
    GPIO_SetBits(GPIO_PORT_SCS,GPIO_PIN_SCS);
}

//=============================================================================
//A7129_ReadReg
uint16_t A7129_ReadReg(uint8_t address)
{
    uint8_t i;
    uint16_t tmp=0;

    //SCS=0;
    GPIO_ResetBits(GPIO_PORT_SCS,GPIO_PIN_SCS);
    
    address |= CMD_Reg_R;
    for(i=0; i<8; i++)
    {
        if(address & 0x80)
        {
            //SDIO = 1;
            GPIO_SetBits(GPIO_PORT_SDIO,GPIO_PIN_SDIO);
        }
        else
        {
            //SDIO = 0;
            GPIO_ResetBits(GPIO_PORT_SDIO,GPIO_PIN_SDIO);
        }

				NOP(); 
        //SCK=1;
        GPIO_SetBits(GPIO_PORT_SCK,GPIO_PIN_SCK);
        
        NOP();
        //SCK=0;
        GPIO_ResetBits(GPIO_PORT_SCK,GPIO_PIN_SCK);
        
        address<<=1;
    }
    NOP();
    
    //read data code
    //TRIS_SDIO = 1;		//SDIO pull high
    Set_SDIO_Direction(1);
    
    for(i=0; i<16; i++)
    {
        //if(SDIO)
        if(0!=GPIO_ReadInputDataBit(GPIO_PORT_SDIO,GPIO_PIN_SDIO))
        {
            tmp = (tmp << 1) | 0x01;
        }
        else
        {
            tmp = tmp << 1;
        }

        //SCK=1;
        GPIO_SetBits(GPIO_PORT_SCK,GPIO_PIN_SCK);
        NOP();
        //SCK=0;
        GPIO_ResetBits(GPIO_PORT_SCK,GPIO_PIN_SCK);
         
    }
    //SCS=1;
     GPIO_SetBits(GPIO_PORT_SCS,GPIO_PIN_SCS);
     
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
	GPIO_ResetBits(GPIO_PORT_SCS,GPIO_PIN_SCS);
	
	ByteSend(CMD_ID_W);
	for(i=0; i<4; i++)
	{
		ByteSend(ID_Tab[i]);
	}
	
	//SCS=1;
	GPIO_SetBits(GPIO_PORT_SCS,GPIO_PIN_SCS);

	//SCS=0;
	GPIO_ResetBits(GPIO_PORT_SCS,GPIO_PIN_SCS);
	
	ByteSend(CMD_ID_R);
	d1=ByteRead();
	d2=ByteRead();
	d3=ByteRead();
	d4=ByteRead();
	
	//SCS=1;
	GPIO_SetBits(GPIO_PORT_SCS,GPIO_PIN_SCS);
	
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

    	//IF calibration procedure @STB state
			A7129_WriteReg(MODE_REG, A7129Config[MODE_REG] | 0x0802);			//IF Filter & VCO Current Calibration
	    Delay10us(1);			//clf test
	    dly=5000;
    	do{
        	tmp = A7129_ReadReg(MODE_REG);
				  Delay10us(1);dly--;if(dly==0)break;			//clf add 2016.1.13
    	}while(tmp & 0x0802);
	
			//A7129_WriteReg(CALIBRATION_REG,0x5a5a);		//test clf 
			//Delay100us(1);														//test clf
			
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
				  Delay10us(1);dly--;if(dly==0)break;	         //clf add 2016.1.13
    	}while(tmp & 0x1000);
			A7129_WriteReg(ADC_REG, A7129Config[ADC_REG]);


    	//VCO calibration procedure @STB state
			A7129_WriteReg(PLL1_REG, A7129Config[PLL1_REG]);
			A7129_WriteReg(PLL2_REG, A7129Config[PLL2_REG]);
			A7129_WriteReg(MODE_REG, A7129Config[MODE_REG] | 0x0004);		//VCO Band Calibration
			dly=5000;
			do{
					tmp = A7129_ReadReg(MODE_REG);
				  Delay10us(1);dly--;if(dly==0)break;	
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
        GPIO_ResetBits(GPIO_PORT_SCS,GPIO_PIN_SCS);
        
	ByteSend(CMD_FIFO_W);			//TX FIFO write command
        ByteSend(length);   			//clf   add
	for(i=0;i<length;i++)
	{		
		ByteSend(buf[i]);
	}
	//SCS=1;
	GPIO_SetBits(GPIO_PORT_SCS,GPIO_PIN_SCS);
	
}

//=======================================================================================
//RxPacket
uint8_t RxPacket(uint8_t *buf)
{
	    uint8_t i;
	    //uint8_t recv;
	    uint8_t tmp;

	    //RxCnt++;

	    StrobeCMD(CMD_RFR);		//RX FIFO address pointer reset

    	//SCS=0;
    	GPIO_ResetBits(GPIO_PORT_SCS,GPIO_PIN_SCS);
    	    
	    ByteSend(CMD_FIFO_R);		//RX FIFO read command
	    tmp =ByteRead();
	    if(tmp>=(RF_DATA_BUF_MAX_LEN-1))
			{
						//Set_GPIO_SCS();
				    GPIO_SetBits(GPIO_PORT_SCS,GPIO_PIN_SCS);
						StrobeCMD(CMD_STBY);////////
				    StrobeCMD(CMD_RX);
						return 0;
      }
	    for(i=0; i <tmp; i++)																		//此处要当心，缓冲区要开的大点
	    {
					buf[i] = ByteRead();
	    }
			
			//StrobeCMD(CMD_RFR);	//clf add 2015.12.02
			bsp_InitRF();
			
	    //SCS=1;
	    GPIO_SetBits(GPIO_PORT_SCS,GPIO_PIN_SCS);
	    
	    return tmp;			//返回长度
/*
	for(i=0; i<64; i++)
	{
		recv = tmpbuf[i];
		tmp = recv ^ PN9_Tab[i];
		if(tmp!=0)
		{
			Err_ByteCnt++;
			Err_BitCnt += (BitCount_Tab[tmp>>4] + BitCount_Tab[tmp & 0x0F]);
		}
    }
*/    
}




//=====================================================================================================
///**********************************************************
//* Delay10us @12MHz
//**********************************************************/
//void Delay10us(Uint8 n)
//{
//    Uint8 i;
//	
//	while(n--)
//	{
//        for(i=0; i<12; i++);
//	}
//}
//
///**********************************************************
//* Delay100us @12MHz
//**********************************************************/
//void Delay100us(Uint8 n)
//{
//    Uint8 i;
//
//    while(n--)
//	{
//        for(i=0; i<120; i++);
//	}
//}
//
///**********************************************************
//* Delay1ms @12MHz
//**********************************************************/
//void Delay1ms(Uint8 n)
//{
//    Uint8 i, j;
//
//    while(n--)
//    {
//        for(i=0; i<10; i++)
//            for(j=0; j<245; j++);
//    }
//}
//
///**********************************************************
//* Delay10ms @12MHz
//**********************************************************/
//void Delay10ms(Uint8 n)
//{
//    Uint8 i, j;
//
//    while(n--)
//    {
//        for(i=0; i<100; i++)
//            for(j=0; j<245; j++);
//    }
//}
//========================================================================================
//管脚输出方式改变
//参数：0-输出，1-输入
void Set_SDIO_Direction(uint8_t val)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	if(val==0)
	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		/* 设为输出口 */
	}
	else
	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		//设置为输入口，悬空输入
	}

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO口最大速度 */	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_SDIO;
	GPIO_Init(GPIO_PORT_SDIO, &GPIO_InitStructure);
}
//=======================================================================================
//延时一个指令
void NOP(void)
{
	uint8_t dly;
	for(dly=0;dly<10;dly++);	
}

void Delay10us(uint8_t n)
{
		uint8_t i,j;
	  while(n>0)
		{
			  n--;
				j=100;
				while(j>0)
				{
						j--;
						for(i=0;i<100;i++);
				}
	  }
}

void Delay100us(uint8_t n)
{
	  while(n>0)
		{
				n--;
				Delay10us(10);
	  }
}









