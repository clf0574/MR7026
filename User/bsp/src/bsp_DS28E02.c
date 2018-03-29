//=========================================================================================================
//=========================================================================================================

//#include <ioCC2540.h>
//#include<stdio.h>
//#include<stdlib.h>
//#include<math.h>
//#include "Define.h"
//#include "bsp_DS28E02.h"

#include <bsp.h>
/***********************************************************************************/

//管脚定义

	#define RCC_ALL_DS 	(RCC_APB2Periph_GPIOA )
	#define GPIO_PORT_DS   GPIOA
  #define GPIO_PIN_DS	GPIO_Pin_3
	
	
	#define ONEWIRETRIS_OUT()    OW_Direct(0)//{P0DIR |= 0x10;}
	#define ONEWIRETRIS_IN()     OW_Direct(1)//{ P0DIR &= ~(0x10);}

	//#define ONEWIRE            		P0_4
	#define ONEWIRE_HI()						GPIO_PORT_DS->BSRR = GPIO_PIN_DS
	#define ONEWIRE_LO()						GPIO_PORT_DS->BRR = GPIO_PIN_DS	
	#define ONEWIRE_R()        			GPIO_ReadInputDataBit(GPIO_PORT_DS,GPIO_PIN_DS)//P0_4

/************************************************************************************/
/*
**-----------------------------------------File Info--------------------------------
** Last modified Date:  2013-8-1                                                  
** Descriptions: DS28E15 Authentication Function Testing Code
**----------------------------------------------------------------------------------
** Created By:  	Yang.xiaofei                                                      
** Created Date:	2013-8-1                                                       
**----------------------------------------------------------------------------------
** Modified by: 	Yang.xiaofei                                                      
** Modified date:	2013-8-1                                                        
** Version:		V1.0.0                                                                 
** Descriptions:                                                                        
***********************************************************************************/  

//=======================================================================================
////20171016,单片机为CC2541，晶振32M
////输入100，实际延时60us
//#define OW10uS			9//17
//#define OW12uS			12//20
//#define OW5uS			2//8
//#define OW8uS			5//13
//#define OW60uS			100
////#define OW200uS         	334
//#define OW100uS         	166

//#define OW_RST_TMSP             100
//#define OW_RST_WAIT             100


//#define OW_R_DLY		OW10uS       //实际需5uS~15uS
//#define OW_R_WAIT       	OW10uS	//实际小于15uS,从机决定
//#define OW_R_FINISH     	OW10uS
//#define OW_W1_DLY       	OW60uS      //实际需60uS~120us
//#define OW_W0_DLY       	OW10uS	//实际1uS~15uS 

//20171016,单片机为STM32F100C8，晶振72M
#define OW10uS			18
//#define OW12uS			20
//#define OW5uS			  15
//#define OW8uS			  15
#define OW60uS			110
#define OW100uS     185

#define OW_RST_TMSP             OW10uS
#define OW_RST_WAIT             OW10uS


#define OW_R_DLY					OW10uS       //实际需5uS~15uS
#define OW_R_WAIT       	OW10uS	//实际小于15uS,从机决定
#define OW_R_FINISH     	OW10uS
#define OW_W1_DLY       	OW60uS  //实际需60uS~120us
#define OW_W0_DLY       	OW10uS	//实际1uS~15uS   
//==================================================================================
//宏定义
//只使用最小定制应用  
#define USE_CUSTOM_ONLY	

#ifndef USE_CUSTOM_ONLY		
	//是否使用单片机计算SHA-256
	#define USE_MCU_SHA256  
#endif   
//
//==================================================================================


//================================================================================== 
#define WriteMemory_Function     	0x55
#define ReadMemory_Function      	0xF0
#define WriteBlock_Protection    	0xC3
#define AuthenticatedWriteBlockProtection_Command 0xCC
#define ReadProtection_Command       0x80
#define WriteProtection_Command      0x40
#define EPromProtection_Command      0x20
#define Authentication_Command       0x10
#define ReadStaus_Command            0xAA
#define ReadWriteScratchPad_Command  0x0F
#define LoadAndLockSecret_Command    0x33
#define ComputeAndLockSecret_Command 0x3C
#define ComputeAndReadPageMAC_Command 0xA5
#define AuthenticatedWriteMemory_Command 0x5A
#define Read_Protection 	0x80
#define Write_Protection 	0x40
#define EMPROM_Emulation 	0x20
#define Authentication_Protection 0x10
#define Segment0 		0x00
#define Segment1 		0x01
#define Page0 			0x00
#define Page1 			0x01
#define Block0 			0x00
#define Block1 			0x01
#define Block2 			0x10
#define Block3 			0x11
#define ANONYMOUS 		(bool)1
/* SHA-256 Algorithm Definition */
#define SHA_256_INITIAL_LENGTH    8

//clf add
#define OUTPUTDATA 0
#define INPUTDATA  1

//=====>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#ifdef USE_MCU_SHA256
unsigned long int SHA_256_Initial[] = 
{
   0x6a09e667,
   0xbb67ae85,
   0x3c6ef372,
   0xa54ff53a,
   0x510e527f,
   0x9b05688c,
   0x1f83d9ab,
   0x5be0cd19,
};

unsigned long int SHA_CONSTANTS[] =  
{
  0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
  0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
  0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
  0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
  0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
  0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
  0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
  0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
  0xca273ece, 0xd186b8c7, 0xeada7dd6, 0xf57d4f7f, 0x06f067aa, 0x0a637dc5, 0x113f9804, 0x1b710b35,
  0x28db77f5, 0x32caab7b, 0x3c9ebe0a, 0x431d67c4, 0x4cc5d4be, 0x597f299c, 0x5fcb6fab, 0x6c44198c
};

unsigned long a32, b32, c32, d32, e32, f32, g32, h32; // SHA working variables
unsigned long W32[16]; //ulong W32[16];                                        // SHA message schedule
unsigned long H32[8];//ulong H32[8];                                         // last SHA result variables
unsigned long  SHAVM_Hash[5];                         // Hash buffer, in wrong order for Dallas MAC
unsigned char SHAVM_Message[64];
#endif
//<<<<<<<<<<<<<<<=================================================================================

const unsigned char dscrc_table[] = 
{
        0, 94,188,226, 97, 63,221,131,194,156,126, 32,163,253, 31, 65,
      157,195, 33,127,252,162, 64, 30, 95,  1,227,189, 62, 96,130,220,
       35,125,159,193, 66, 28,254,160,225,191, 93,  3,128,222, 60, 98,
      190,224,  2, 92,223,129, 99, 61,124, 34,192,158, 29, 67,161,255,
       70, 24,250,164, 39,121,155,197,132,218, 56,102,229,187, 89,  7,
      219,133,103, 57,186,228,  6, 88, 25, 71,165,251,120, 38,196,154,
      101, 59,217,135,  4, 90,184,230,167,249, 27, 69,198,152,122, 36,
      248,166, 68, 26,153,199, 37,123, 58,100,134,216, 91,  5,231,185,
      140,210, 48,110,237,179, 81, 15, 78, 16,242,172, 47,113,147,205,
       17, 79,173,243,112, 46,204,146,211,141,111, 49,178,236, 14, 80,
      175,241, 19, 77,206,144,114, 44,109, 51,209,143, 12, 82,176,238,
       50,108,142,208, 83, 13,239,177,240,174, 76, 18,145,207, 45,115,
      202,148,118, 40,171,245, 23, 73,  8, 86,180,234,105, 55,213,139,
       87,  9,235,181, 54,104,138,212,149,203, 41,119,244,170, 72, 22,
      233,183, 85, 11,136,214, 52,106, 43,117,151,201, 74, 20,246,168,
      116, 42,200,150, 21, 75,169,247,182,232, 10, 84,215,137,107, 53
};
static short oddparity[16] = { 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0 };

//<<<<<<<<<<<<<<===========================================================================



//unsigned char DS28E11_Buf[32];
unsigned char CRC8;
unsigned int CRC16;

//===============================================================================




//=====>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#ifndef USE_CUSTOM_ONLY	
static unsigned char SECRET[32];
int reverse_endian=1; 
int sha_debug=0;

//unsigned char   WriteMemory(unsigned char Segment, unsigned char Page,unsigned char *Buffer);
//unsigned char   ReadMemory(unsigned char Segment, unsigned char Page,unsigned char *Receive);
unsigned char WriteBlockProtection(unsigned char Block ,unsigned char ProtectOption);
//unsigned char AuthenticatedWriteBlockProtection(unsigned char Block ,unsigned char ProtectOption);
unsigned char Auth_WB_Protection(unsigned char Block ,unsigned char ProtectOption);
unsigned char Load_and_LockSecret(bool SecretLock_Enable);
unsigned char SettingAndLock_Secret(unsigned char *secret, bool LockEnable);
unsigned char ComputeAndLock_Secret(unsigned char PageNum, bool LockEnable);
unsigned char AuthenticatedWriteMemory(unsigned char Segment ,unsigned char Page, unsigned char *Buffer,unsigned char *MAC);
unsigned char AuthenticatedWriteNewData(unsigned char Segment ,unsigned char Page, unsigned char *Bufferdata);
void set_secret(unsigned char *secret);
void ComputeMAC256(unsigned char *MT, short length, unsigned char *MAC);
bool VerifyMAC256(unsigned char * MT, short length, unsigned char* compare_MAC);
void  CalculateNextSecret256(unsigned char* binding, unsigned char* partial, int page_num, unsigned char* manid);
#endif
//<<<<<<<<<<<<<<============================================================================


//内部函数定义
//void One_Wire_IO_Direction(unsigned char d);
//void GPIO_ResetBits(void);
//void GPIO_SetBits(void);
//unsigned char GPIO_ReadBits(void);
void OW_Delay_us(unsigned char dly);
void OW_Delay_ms(unsigned char dly);


unsigned char ow_reset(void);	
unsigned char read_bit(void);
void write_bit(char bitval);
unsigned char read_byte(void);
void write_byte(char val);
unsigned char dowcrc(unsigned char x);
unsigned short docrc16(unsigned short data);
//unsigned char Read_RomID(unsigned char *RomID);
unsigned long sha_ch(unsigned long x, unsigned long y, unsigned long z);
unsigned long sha_maj(unsigned long x, unsigned long y, unsigned long z);
unsigned long sha_rotr_32(unsigned long val, unsigned int r);
unsigned long sha_shr_32(unsigned long val, unsigned int r);
unsigned long sha_bigsigma256_0(unsigned long x);
unsigned long sha_bigsigma256_1(unsigned long x);
unsigned long sha_littlesigma256_0(unsigned long x);
unsigned long sha_littlesigma256_1(unsigned long x);
void sha_copy32(unsigned long* p1, unsigned long* p2, unsigned int length);
void sha_copyWordsToBytes32(unsigned long* input, unsigned char* output, unsigned int numwords);
void sha_writeResult(unsigned int reverse, unsigned char* outpointer);
unsigned long sha_getW(int index);
void sha_prepareSchedule(unsigned char* message);
void sha256_hashblock(unsigned char* message, unsigned int lastblock);
void ComputeSHA256(unsigned char* message, int length, unsigned int skipconst, unsigned int reverse, unsigned char* digest);



void bsp_DS28E02_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_ALL_DS, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		/* 设为输出口 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO口最大速度 */
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_DS;
	GPIO_Init(GPIO_PORT_DS, &GPIO_InitStructure);
	ONEWIRE_HI();
	
//	//===================================================================
//	//测试时序
//	while(1)
//	{
//		
//			ONEWIRE_HI();
//			OW_Delay_us(OW100uS);
//	    ONEWIRE_LO();
//			OW_Delay_us(OW100uS);
//	}
	//====================================================================
}

void OW_Direct(uint8_t st)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	if(st==0)
	{
		
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		/* 设为输出口 */
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO口最大速度 */
				GPIO_InitStructure.GPIO_Pin = GPIO_PIN_DS;
				GPIO_Init(GPIO_PORT_DS, &GPIO_InitStructure);
	}
	else
	{
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		/* 设为输出口 */
					GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO口最大速度 */
		    GPIO_InitStructure.GPIO_Pin = GPIO_PIN_DS;
				GPIO_Init(GPIO_PORT_DS, &GPIO_InitStructure);
	}
}

 void OW_Delay_us(unsigned char dly)		//此函数必须精确到1us,且不能被中断打断
 {
	 uint8_t i;
 	//用示波器测定
 	while(dly>0)                           //20171016 ,输入100，实际延时60uS
 	{
 		dly=dly-1;
		for(i=0;i<3;i++);
 	}
 }
 void OW_Delay_ms(unsigned char dly)		//此函数必须精确到1us,且不能被中断打断
 {
 	//用示波器测定
 	while(dly--)
 	{
        OW_Delay_us(OW100uS);OW_Delay_us(OW100uS);OW_Delay_us(OW100uS);OW_Delay_us(OW100uS);OW_Delay_us(OW100uS);
        OW_Delay_us(OW100uS);OW_Delay_us(OW100uS);OW_Delay_us(OW100uS);OW_Delay_us(OW100uS);OW_Delay_us(OW100uS);
 	}
 }

//-------------------------------------------------------------------------------------------
/*one wire reset
// Input parameter: None               
// Returns: if Presence  :  0x00 
            if No device :  0x01
*/
unsigned char ow_reset(void)
{
			unsigned char presence;
        ONEWIRETRIS_OUT();//ONEWIRETRIS=0;//One_Wire_IO_Direction(OUTPUTDATA);         	// set 1-Wire Data Pin to output
        ONEWIRE_LO();//GPIO_ResetBits();//GPIO_ResetBits(One_Wire, One_Wire_IO);       // leave 1-Wire Data Pin low for 60us
        
        //需要精确延时500uS
				OW_Delay_us(OW100uS);OW_Delay_us(OW100uS);OW_Delay_us(OW100uS);OW_Delay_us(OW100uS);OW_Delay_us(OW100uS);//CLF MOD 2017.3.14
				OW_Delay_us(OW100uS);//480us-640us
				
				ONEWIRE_HI();//GPIO_SetBits();//GPIO_SetBits(One_Wire, One_Wire_IO);         // allow line to return high

        //OW_Delay_us(OW_RST_TMSP);			//等待电阻充电上升
				//OW_Delay_us(OW60uS);	

        ONEWIRETRIS_IN();//ONEWIRETRIS=1;//One_Wire_IO_Direction(INPUTDATA);          // set 1-Wire Data pin to input mode   

        //OW_Delay_us(OW_RST_WAIT);//OW_Delay_us(OW8uS);                                 // wait for presence
				OW_Delay_us(OW100uS);
	
        presence=(unsigned char)ONEWIRE_R();//presence=GPIO_ReadBits();//presence= ((One_Wire->IDR)>>8)&(0x40);

        OW_Delay_us(OW100uS);OW_Delay_us(OW100uS);OW_Delay_us(OW100uS);//OW_Delay_us(OW10uS); //CLF MOD

				return(presence);                            // presence signal returned
} 


//------------------------------------------------------------------------------------------- 
/* read bit funtion by 1-wire communication 
// Input parameter: None 
// Returns: Reading bits status on 1-wire line
*/
unsigned char read_bit(void)
{
	unsigned char vamm;
        ONEWIRETRIS_OUT();//ONEWIRETRIS=0;//One_Wire_IO_Direction(OUTPUTDATA);                       
        ONEWIRE_LO();//GPIO_ResetBits();//GPIO_ResetBits(One_Wire, One_Wire_IO);
        OW_Delay_us(OW_R_DLY);     //CLF MOD 2017.3.14
        ONEWIRETRIS_IN();//ONEWIRETRIS=1;//One_Wire_IO_Direction(INPUTDATA);
	OW_Delay_us(OW_R_WAIT);    //clf add 2017.3.14
        vamm=(unsigned char)ONEWIRE_R();//vamm=GPIO_ReadBits();//vamm=((One_Wire->IDR)>>8)&(0x40);
       //OW_Delay_us(OW5uS);                                 //waiting for Reading slot over
        //OW_Delay_us(OW_R_FINISH);					//clf mod 2017.3.14
				OW_Delay_us(OW60uS);	
	return(vamm);                                // return value of 1-wire data pin
	
	
}


 
//------------------------------------------------------------------------------------------- 
/* write bit funtion by 1-wire communication
// Input parameter: 
   1.char bitval: specify writing bit value
// Returns: None
*/

void write_bit(char bitval)
{
        ONEWIRETRIS_OUT();//ONEWIRETRIS=0;//One_Wire_IO_Direction(OUTPUTDATA);

        //ONEWIRE=0;//GPIO_ResetBits();//GPIO_ResetBits(One_Wire, One_Wire_IO);
        //OW_Delay_us(2);//NOP();//OW_Delay_us(1);                                //waiting for 1 us//2017.3.14MOD
        //if(bitval!=0)
        //{
        //	ONEWIRE=1;//GPIO_SetBits();//GPIO_SetBits(One_Wire, One_Wire_IO);
        //}		
        //OW_Delay_us(OW12uS);                              // waiting for 12us   

				ONEWIRE_LO();//GPIO_ResetBits();//GPIO_ResetBits(One_Wire, One_Wire_IO);
        OW_Delay_us(OW_W0_DLY);//NOP();//OW_Delay_us(1);                                //waiting for 1 us//2017.3.14MOD
        if(bitval!=0)
        {
        	ONEWIRE_HI();//GPIO_SetBits();//GPIO_SetBits(One_Wire, One_Wire_IO);
        }		
        OW_Delay_us(OW_W1_DLY);//OW_Delay_us(OW12uS);                              // waiting for 12us   
   
        ONEWIRE_HI();//GPIO_SetBits();//GPIO_SetBits(One_Wire, One_Wire_IO);       // restore 1-wire data pin to high     
}



//------------------------------------------------------------------------------------------- 
/* read  byte by 1-wire communication
// Input parameter: None
// Returns: 
   1.unsigned char : reading 1 byte data by 1-wire bus
*/
unsigned char read_byte(void)
{
	unsigned char i;
	unsigned char value = 0;
	for (i = 0; i < 8; i++)
		{
			if(read_bit()) value |= 0x01<<i;      // reads byte in, one byte at a time and then shifts it left
			
		}
	return(value);
}

//------------------------------------------------------------------------------------------- 
/* write  byte by 1-wire communication
// Input parameter: 
   1.char val :   data ready for writing 
// Returns: None
   
*/
void write_byte(char val)
{
	unsigned char i;
	unsigned char temp; 

	for (i = 0; i < 8; i++) // writes byte, one bit at a time
		{
			temp = val>>i; // shifts val right ‘i’ spaces
			temp &= 0x01; // copy that bit to temp
			write_bit(temp); // write bit in temp into
		}

}


//------------------------------------------------------------------------------------------- 
/* CRC calculation
// Input parameter: 
   1.unsigned char x :   data ready for crc
// Returns: 
   1.unsigned char : crc output 
   
*/

unsigned char dowcrc(unsigned char x)
{
   CRC8 = dscrc_table[CRC8 ^ x];
   return CRC8;
}

/*--------------------------------------------------------------------------
 * Calculate a new CRC16 from the input data shorteger.  Return the current
 * CRC16 and also update the global variable CRC16.
// Input parameter: 
   1.unsigned short data:   data ready for 16bitcrc
// Returns: 
   1.unsigned short : 16bit crc output 
*/

unsigned short docrc16(unsigned short data)
{
   data = (data ^ (CRC16 & 0xff)) & 0xff;
   CRC16 >>= 8;

   if (oddparity[data & 0xf] ^ oddparity[data >> 4])
     CRC16 ^= 0xc001;

   data <<= 6;
   CRC16   ^= data;
   data <<= 1;
   CRC16   ^= data;

   return CRC16;
}


//--------------------------------------------------------------------------
/*Read the 64-bit ROM ID of DS28E15
// Input parameter: 
   1.RomID  :64Bits RomID Receiving Buffer 
                    
// Returns: 0x01 = success ,RomID CRC check is right ,RomID Sotred in RomID Buffer
            0x00 = failure , maybe on_reset() Error ,or CRC check Error;
*/


//取得ROM_ID,用于模块唯一标记
unsigned char Read_RomID(unsigned char *RomID)
{
   unsigned char i;  
   
   
		 if ( (ow_reset())!=0 ) return 0;
	
		 write_byte(0x33);
		 OW_Delay_us(OW100uS);//OW_Delay_us(OW10uS);//CLF 2017.3.14//20171016,yuanxian 100
		 for(i = 0;i < 8;i++)
		 {
						//DS28E11_Buf[i] = read_byte();
						RomID[i]= read_byte();
		 }
  
    /*CRC8 Check */
    CRC8=0;
    for(i=0;i<8;i++)
    {
    	//dowcrc(DS28E11_Buf[i]);     //if Receiving No  Error ,CRC =0;
		dowcrc(RomID[i]);
    }
    if(CRC8!=0) return 0;
    else return 1; 
}

//================================================================================================================
//20170314增加 来自美信杨工
//2017.3.14
unsigned char ANON_READ_AuthPage(unsigned char *Receive_buffer, unsigned char* Scratchpad, unsigned char PageNum)
{
 		unsigned char pbuf[60],cnt;
        unsigned char /*Challenge[8],*/i;
        
        
		//for(i=0;i<8;i++)Challenge[i]=0x01;		

		/* write Challenge */
    	ow_reset();                 //reset 1-wire bus and detect response pulse
    	write_byte(0xCC);          //SKIP ROM command
        cnt=0;
    	pbuf[cnt++]=0x0F;          //Function Command, Write challenge to scratchpad
    	pbuf[cnt++]=PageNum<<5;
    	pbuf[cnt++]=PageNum>>3;

        pbuf[cnt++]=0xff;
        pbuf[cnt++]=0xff;
        for(i=0;i<5;i++) pbuf[cnt++]=Scratchpad[i];
        pbuf[cnt++]=0xff;
        for(i=0;i<11;i++) write_byte(pbuf[i]);

      	/* Read CRC */
        pbuf[cnt++] = read_byte();
        pbuf[cnt++] = read_byte();

     	/* Verify CRC */
        CRC16 = 0;
        for (i = 0; i < cnt; i++) docrc16(pbuf[i]);


      	// return result of inverted CRC
      	if( CRC16 != 0xB001) return 0;

     	/****************************************************************/
     	/* Read Anonymous Auth Page MAC */
        ow_reset();
    	write_byte(0xCC);
        cnt=0;
    	pbuf[cnt++]=0xCC;          //Function Command, Anony Auth MAC
    	pbuf[cnt++]=PageNum<<5;
    	pbuf[cnt++]=PageNum>>3;
        for(i=0;i<3;i++) write_byte(pbuf[i]);

    	for(i = 0;i < 35;i++)      //Master receive 32 Page data, oxFF, 2Bytes CRC
        {
          pbuf[cnt++] = read_byte();
        }
 
    	/* Verify CRC */
        CRC16 = 0;
        for (i = 0; i < cnt; i++) docrc16(pbuf[i]);
        if( CRC16 != 0xB001)   return 0;  //not 0 because that the calculating result is CRC16 and the reading result is inverted CRC16


		OW_Delay_ms(5);//Delay_Ms(2000);                   //waiting for finishing SHA-1 algorithm
        cnt=0;
		for(i = 0;i < 22;i++)             //Receive Auth MAC 20Bytes ,2 CRC Bytes
        {
          pbuf[cnt++] = read_byte();
        }
    	/* Verify CRC */
        CRC16 = 0;
        for (i = 0; i < cnt; i++) docrc16(pbuf[i]);
        if( CRC16 != 0xB001)  return 0;  //not 0 because that the calculating result is CRC16 and the reading result is inverted CRC16

        /*copy Auth Page MAC to Receive_buffer*/
        //memcpy(Receive_buffer, pbuf, 32);
		for(i=0;i<20;i++)Receive_buffer[i]=pbuf[i];
        return 1;


}



//=========>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#ifndef USE_CUSTOM_ONLY	


//--------------------------------------------------------------------------
/*Write 4-byte to EEPROM memory of DS28E15
This command is applicable only to memory blocks that do not require master authentication (authentication
protection not activated). Additionally, the memory block must not be write protected.
// Input parameter: 
   1.unsigned char Segment : Specify Writing Segment Number  ;DS28E15 EEPROM include 2 Pages ,Each Page 32Bytes ; 
                    Each Pages include 4 Block ; every Block 16Bytes ;
                    Eevry Block include 4 Segment ;every Segment include 4 Bytes;
                    DS28E15 writing according to Segment alignment, 4 Bytes every writing cycle.
   2.unsigned char Page: specify Writing Page Number;
   3.unsigned char *Buffer: Data Buffer writing to DS28E15 EEPROM
          
// Returns: AAh = success
            55h = The command failed because master authentication is required.
            33h = The command failed because the memory block is write protected.
            53h = The command failed because the memory block is write protected and requires master authentication.
            FFh = CRC Error
*/

//写入DS28E11的内部EEPROM,共32个字节,一个Segment有4个字节
unsigned char   WriteMemory(unsigned char Segment, unsigned char Page,unsigned char *Buffer)
{
    
   unsigned char pbuf[32],cnt=0,i,outcome;
    if ( (ow_reset())!=0 ) return 0;
    
    write_byte(0xCC);		                   //Skip ROM command
    /* construct a packet to send */
    pbuf[cnt++] = WriteMemory_Function;            // write WriteMemory_Function command
    pbuf[cnt++] = ((Segment<<5)|Page)&(0xE1);      // Segment and Page ,Assure Bit4~Bit1:0000
    	        

    /* Send Data */
    for (i = 0; i < cnt; i++)  write_byte(pbuf[i]);
    
    /* Receive CRC */
      pbuf[cnt++]=read_byte();
      pbuf[cnt++]=read_byte();

   /* calculate the CRC over this part */
      CRC16 = 0;
      for (i = 0; i < cnt; i++) docrc16(pbuf[i]);

     /* return result of inverted CRC */
      if (CRC16 != 0xB001)                     //not 0 because that the calculating result is CRC16 and the reading result is inverted CRC16
       return 0xFF;
       
      cnt=0;                                       
      /* Send Memory Data */
      for (cnt = 0; cnt < 4; cnt++) 
      {  pbuf[cnt]=Buffer[cnt];
         write_byte(Buffer[cnt]);
      }
      /* Receive CRC */
      pbuf[cnt++]=read_byte();
      pbuf[cnt++]=read_byte();
      
      /* calculate the CRC over this part */
      CRC16 = 0;
      for (i = 0; i < cnt; i++) docrc16(pbuf[i]);

     /* return result of inverted CRC */
      if (CRC16 != 0xB001)                      //not 0 because that the calculating result is CRC16 and the reading result is inverted CRC16
        return 0xFF;
      
     /* Send Release Byte */                    // waiting for EEPROM programming 
       write_byte(0xAA); 
       OW_Delay_ms(10);//OW_Delay_us(10000);
       
       outcome=read_byte(); 
       ow_reset();
       return outcome;
         
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------
/*Reading  DS28E15 EEPROM Data From Specified starting Segment  to last Segment in Specified Page.
The Read Memory command is used to read the user memory. The page number and segment number specify where
the reading begins.After the last byte of the page is read, the DS28E15 transmits a CRC of the page data for the master to verify the data integrity.
// Input parameter: 
1. unsigned char Segment : specify the location within the selected memory page at which the reading begins
2. unsigned char Page : specifies the memory page at which the reading begins.
                        Valid memory page numbers are 0 (page 0) and 1 (page 1).
3. unsigned char *Receive : Receiving Data buffer                      
// Returns:  AAh = success
             FFh = CRC Error
*/

//读DS28E11内部EPROM,一个Segment有4个字节
unsigned char   ReadMemory(unsigned char Segment, unsigned char Page,unsigned char *Receive)
{
  unsigned char pbuf[40],cnt=0,i;
  if ( (ow_reset())!=0 ) return 0;
  
    write_byte(0xCC);		                   //Skip ROM command
    /* construct a packet to send */
    pbuf[cnt++] = ReadMemory_Function;            // write WriteMemory_Function command
    pbuf[cnt++] = ((Segment<<5)|Page)&(0xE1);      // Segment and Page ,Assure Bit4~Bit1:0000
    	        

    /* Send Data */
    for (i = 0; i < cnt; i++)  write_byte(pbuf[i]);
    
    /* Receive CRC */
      pbuf[cnt++]=read_byte();
      pbuf[cnt++]=read_byte();
  
      /* calculate the CRC over this part */
      CRC16 = 0;
      for (i = 0; i < cnt; i++) docrc16(pbuf[i]);

     /* return result of inverted CRC */
      if (CRC16 != 0xB001)                      //not 0 because that the calculating result is CRC16 and the reading result is inverted CRC16
        return 0xFF;
  
      /* Receive DS28E15 EEPROM Data */  
      for (cnt=0, i=Segment*4 ; i<32 ;i++,cnt++)
      { 
        Receive[cnt]= read_byte();
        pbuf[cnt]= Receive[cnt];
      }
       
      
     /* Receive CRC */
      pbuf[cnt++]=read_byte();
      pbuf[cnt++]=read_byte();
      
         /* calculate the CRC over this part */
      CRC16 = 0;
      for (i = 0; i < cnt; i++) docrc16(pbuf[i]);
       
      ow_reset();
     /* return result of inverted CRC */
      if (CRC16 != 0xB001)                      //not 0 because that the calculating result is CRC16 and the reading result is inverted CRC16
      {
      	  return 0xFF;
      }
      
      return 0xAA; 
              
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------
/* To read or write 32 Bytes Scratchpad buffer.To ensure the integrity of the scratchpad data the master can rely on the CRC that
the DS28E15 generates and transmits after the scratchpad is filled to capacity
// Input parameter: 
   1. bool ReadMode : specify to read or write scratchpad buffer
   2. unsigned char *Buffer : Data buffer ; If read scratchpad ,Buffer is receiving data. If write scratchpad ,Buffer is data ready for writing.
// Returns: AAh = success
            FFh = CRC Error
           
*/
//把数据写入或者读出 缓冲区
unsigned char ReadWriteScratchpad(bool ReadMode, unsigned char *Buffer)
{
    unsigned char pbuf[40],cnt=0,i;
    if ( (ow_reset())!=0 ) return 0;


    write_byte(0xCC);		                           //Skip ROM command
     
    
 /* construct a packet to send */
    pbuf[cnt++] = ReadWriteScratchPad_Command;            //  ReadWriteScratchpad command
    pbuf[cnt++]= ReadMode? 0x0F:0x00;
 
    /* Send Data */
    for (i = 0; i < cnt; i++)  write_byte(pbuf[i]);
    
     /* Receive CRC */
      pbuf[cnt++]=read_byte();
      pbuf[cnt++]=read_byte();
    
    /* calculate the CRC over this part */
      CRC16 = 0;
      for (i = 0; i < cnt; i++) docrc16(pbuf[i]);   
      if (CRC16 != 0xB001)                            //not 0 because that the calculating result is CRC16 and the reading result is inverted CRC16
      return 0xFF;
      
      if(ReadMode)
      {
        for (i=0 ,cnt=0 ; i<32 ; cnt++,i++)
        {
         Buffer[cnt]=read_byte();
         pbuf[i]=Buffer[cnt];
        }
      }
      else        
      {
          for (i=0 ,cnt=0 ; i<32 ; cnt++,i++)
         {
           write_byte(Buffer[i]);
           pbuf[cnt]=Buffer[i];
         }
       }  
         /* Receive CRC */
      pbuf[cnt++]=read_byte();
      pbuf[cnt++]=read_byte();
      
      
      ow_reset();
         /* calculate the CRC over this part */
      CRC16 = 0;
      for (i = 0; i < cnt; i++) docrc16(pbuf[i]);  
      if (CRC16 != 0xB001)                            //not 0 because that the calculating result is CRC16 and the reading result is inverted CRC16
      return 0xFF;
      return 0xAA;
     
  
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------
/*
The Compute and Read Page MAC command is used to authenticate the DS28E15 to the master. The master computes
the MAC from the same data and the expected secret. If both MACs are identical, the device is confirmed authentic
within the application.
// Input parameter: 
   1.bool Anonymous_Mode: specify whether the device’s ROM ID is used for the MAC computation.
                          If anonymous mode , replacing the ROM ID with FFh Bytes when MAC computation
   2.unsigned char PageNum:specifies the memory page that is to be used for the MAC computation.
                           Valid memory page numbers are 0 (page 0) and 1 (page 1).
   3.unsigned char *MAC_Value: receiving data buffer for 32 Bytes Computation MAC values
                     
// Returns: AAh = success
            FFh = CRC Error
            
*/

unsigned char ComputeAndReadPageMAC(bool Anonymous_Mode,unsigned char PageNum,unsigned char *MAC_Value)
{
  
    unsigned char pbuf[40],cnt=0,i,outcome;
    if ( (ow_reset())!=0 ) return 0;

    write_byte(0xCC);		                                       //Skip ROM command  
    
        /* construct a packet to send */
    pbuf[cnt++] = ComputeAndReadPageMAC_Command;                        // Read Memory Block Status or PersonalityByteindicator
    pbuf[cnt++]= Anonymous_Mode? (PageNum|0xE0):(PageNum);   
    
     /* Send Data */
    for (i = 0; i < cnt; i++)  write_byte(pbuf[i]);
    
      /* Receive CRC */
    pbuf[cnt++]=read_byte();
    pbuf[cnt++]=read_byte();
    
      /* calculate the CRC over this part */
      CRC16 = 0;
      for (i = 0; i < cnt; i++) docrc16(pbuf[i]);   
      if (CRC16 != 0xB001)                            //not 0 because that the calculating result is CRC16 and the reading result is inverted CRC16
      return 0xFF;
      
      OW_Delay_ms(6);//OW_Delay_us(6000);                                   //Wait for 2 times for SHA-256 Calculation;  
  
      outcome=read_byte();
      
      for (i = 0,cnt=0 ; i < 32; i++,cnt++)
      {
        MAC_Value[i]=read_byte();
        pbuf[cnt]=MAC_Value[i];
      }
      
      /* Receive CRC */
      pbuf[cnt++]=read_byte();
      pbuf[cnt++]=read_byte();
      
       /* calculate the CRC over this part */
      CRC16 = 0;
      for (i = 0; i < cnt; i++) docrc16(pbuf[i]);   
      if (CRC16 != 0xB001)                            //not 0 because that the calculating result is CRC16 and the reading result is inverted CRC16
      return 0xFF;
      return outcome;      
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------
/*  Read BlockStatus 
The Read Block Status command is used to read the memory block protection settings and to read the device’s personality
bytes. The parameter byte determines whether the command reads the block protection or the device personality.
// Input parameter: 
   1.unsigned char BlockNum : specify starting Block number for protection status reading . DS28E15 include 4 Block, every block include 16Bytes.
   2.bool PersonalityByteIndicator : specify reading memory block protection settings or  reading the device’s personality   
   3.unsigned char *BlockStatus : storing data buffer for Block protection setting or device's personality.
// Returns: AAh = success
            FFh = CRC Error
           
*/
unsigned char ReadBlockStatus(unsigned char BlockNum,bool PersonalityByteIndicator ,unsigned char *BlockStatus)
{
      unsigned char pbuf[40],cnt=0,i;
      if ( (ow_reset())!=0 ) return 0;

      write_byte(0xCC);		                            //Skip ROM command
     
    
 	  /* construct a packet to send */
      pbuf[cnt++] = ReadStaus_Command;                        // Read Memory Block Status or PersonalityByteindicator
      pbuf[cnt++]=PersonalityByteIndicator? 0xE0:(BlockNum&0x03);

    
      /* Send Data */
      for (i = 0; i < cnt; i++)  write_byte(pbuf[i]);
    
      /* Receive CRC */
      pbuf[cnt++]=read_byte();
      pbuf[cnt++]=read_byte();
    
    /* calculate the CRC over this part */
      CRC16 = 0;
      for (i = 0; i < cnt; i++) docrc16(pbuf[i]);   
      if (CRC16 != 0xB001)                            //not 0 because that the calculating result is CRC16 and the reading result is inverted CRC16
      return 0xFF;
      
      if(PersonalityByteIndicator)
      {
        
        	  for (cnt =0, i=0; i<4 ; i++,cnt++)
              {
                BlockStatus[cnt]=read_byte();
                pbuf[cnt]=BlockStatus[cnt];  
              } 
          
      }
      else
      {
       
           	  for (cnt =0, i=BlockNum; i<4 ; i++,cnt++)
              {
                BlockStatus[cnt]=read_byte();
                pbuf[cnt]=BlockStatus[cnt];  
              } 
      
      }
      /* Receive CRC */
      pbuf[cnt++]=read_byte();
      pbuf[cnt++]=read_byte();
  	  /* calculate the CRC over this part */
      CRC16 = 0;
      for (i = 0; i < cnt; i++) docrc16(pbuf[i]);   
      if (CRC16 != 0xB001)                           //not 0 because that the calculating result is CRC16 and the reading result is inverted CRC16
       return 0xFF;
       ow_reset();
       return 0xAA;  
}



#ifndef USE_CUSTOM_ONLY	
//-------------------------------------------------------------------------------------------------------------------------------------------------------
/* Set Block Protection According to protection mode parameter 
The Write Block Protection command is used to set the protection of user memory blocks. 
The parameter byte specifies the desired protection modes.Once set, a protection cannot be reset.
// Input parameter: 
   1.unsigned char Block : specify Block number for EEPROM protection . DS28E15 include 4 Block, every block include 16Bytes.EEPROM protection apply to specified Block.
   2.unsigned char ProtectOption:  Blcok protection modes include :1. Read Protection; 2.Write Protection; 3.EPROM Emulation Mode (EM);4.Authentication Protection;                
// Returns: AAh = success
            FFh = CRC Error
            55h = Authentication Mac No Match 
*/

//写入Block保护，28E11有2个Block
unsigned char WriteBlockProtection(unsigned char Block ,unsigned char ProtectOption)
{
    unsigned char pbuf[20],cnt=0,i,outcome;
    if ( (ow_reset())!=0 ) return 0;
    write_byte(0xCC);		                     //Skip ROM command
     
    
 /* construct a packet to send */
    pbuf[cnt++] = WriteBlock_Protection;            // write WriteMemory_Function command
    pbuf[cnt++] = ProtectOption|Block;              // Segment and Page ,Assure Bit4~Bit1:0000
    	        

    /* Send Data */
    for (i = 0; i < cnt; i++)  write_byte(pbuf[i]);
    
    /* Receive CRC */
      pbuf[cnt++]=read_byte();
      pbuf[cnt++]=read_byte();
    
    /* calculate the CRC over this part */
      CRC16 = 0;
      for (i = 0; i < cnt; i++) docrc16(pbuf[i]);   
      if (CRC16 != 0xB001)                            //not 0 because that the calculating result is CRC16 and the reading result is inverted CRC16
       return 0xFF;
          
    /* Send Release Byte */                           // waiting for DS28E15 EEPROM programming           
       write_byte(0xAA); 
       OW_Delay_ms(10);//OW_Delay_us(10000);
       
       outcome=read_byte(); 
       ow_reset();
       return outcome;
  
  
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------
 /* Setting certain protection mode to Block which is already in authentication protection .
The Authenticated Write Block Protection command is used to modify the protection of a user memory block that is
already authentication protected.
// Input parameter: 
   1.unsigned char Block : specify Block number for EEPROM protection . DS28E15 include 4 Block, every block include 16Bytes.EEPROM protection apply to specified Block.
   2.unsigned char ProtectOption:  Blcok protection modes include :1. Read Protection; 2.Write Protection; 3.EPROM Emulation Mode (EM);4.Authentication Protection;                
// Returns: AAh = success
            FFh = CRC Error
            55h = Authentication Mac No Match 

*/ 

//校验写保护功能，对的话，写入
//unsigned char AuthenticatedWriteBlockProtection(unsigned char Block ,unsigned char ProtectOption)
unsigned char Auth_WB_Protection(unsigned char Block ,unsigned char ProtectOption)
{
     unsigned char blockindex;
     unsigned char pbuf[20],cnt=0,i,outcome,Mac_value[32],status[4];
     blockindex=Block;
     ReadBlockStatus(blockindex,(bool)0, status);
     memcpy(&MAC_Computer_Datainput[32],RomID_Buffer,32);
     memcpy(&MAC_Computer_Datainput[40],&Personality_Value[2],2);
     MAC_Computer_Datainput[42]=Block&0x03;
     
     for(i=0 ;i <4 ; i++)
     {
       
       MAC_Computer_Datainput[44+i]=((*status>>(4+i))&0x01)==0x01? 0x01:0x00;
     }
     for(i=0 ;i <4 ; i++)
     {
       
       MAC_Computer_Datainput[48+i]=((ProtectOption>>(4+i))&0x01)==0x01? 0x01:0x00;
     }
     
    ComputeMAC256(MAC_Computer_Datainput, 55, Mac_value);
  
    if ( (ow_reset())!=0 ) return 0;
    write_byte(0xCC);		                                         //Skip ROM command
  
  /* construct a packet to send */
    pbuf[cnt++] = AuthenticatedWriteBlockProtection_Command;             // write WriteMemory_Function command
    pbuf[cnt++] = ProtectOption|Block;                                   // Segment and Page ,Assure Bit4~Bit1:0000
    	        

    /* Send Data */
    for (i = 0; i < cnt; i++)  write_byte(pbuf[i]);
    
    /* Receive CRC */
      pbuf[cnt++]=read_byte();
      pbuf[cnt++]=read_byte();
    
    /* calculate the CRC over this part */
      CRC16 = 0;
      for (i = 0; i < cnt; i++) docrc16(pbuf[i]);   
      if (CRC16 != 0xB001)                            //not 0 because that the calculating result is CRC16 and the reading result is inverted CRC16
       return 0xFF;
  
     
  
      OW_Delay_ms(3);//OW_Delay_us(3000);                                   //Wait for 1 times for SHA-256 Calculation; 
  
      for(i = 0 ,cnt = 0; i < 32 ;i++, cnt++)
      { 
        write_byte(Mac_value[i]);
        pbuf[cnt]=Mac_value[i];
      }
       /* Receive CRC */
       pbuf[cnt++]=read_byte();
       pbuf[cnt++]=read_byte();
       
          /* calculate the CRC over this part */
      CRC16 = 0;
      for (i = 0; i < cnt; i++) docrc16(pbuf[i]);   
      if (CRC16 != 0xB001)                            //not 0 because that the calculating result is CRC16 and the reading result is inverted CRC16
      return 0xFF;
      
      outcome=read_byte();
      if(outcome!=0xAA)
      return outcome;
      
      
        /* Send Release Byte */
       write_byte(0xAA); 
       for(i=0 ;i<10;i++)                               //Waiting for 100ms for Secreting Loading
       OW_Delay_ms(10);//OW_Delay_us(10000);
  
       outcome=read_byte();
       ow_reset();
       return outcome;
   
}






//-------------------------------------------------------------------------------------------------------------------------------------------------------
/*
The Load and Lock Secret command is used to install a predefined secret when the DS28E15 is first set up for an
application. This command takes the data in the scratchpad and copies it to the secret memory.
// Input parameter: 
   1. bool SecretLock_Enable : specify whether Lock secret after Loading the secret to Secret memory
// Returns: AAh = success
            FFh = CRC Error
            55h = The command failed because the secret was already locked.
*/

unsigned char Load_and_LockSecret(bool SecretLock_Enable)

{
  
   unsigned char pbuf[20],cnt=0,i,outcome;
   
     if ( (ow_reset())!=0 ) return 0;
    write_byte(0xCC);		                           //Skip ROM command
  
   /* construct a packet to send */
    pbuf[cnt++] = LoadAndLockSecret_Command;                        // Read Memory Block Status or PersonalityByteindicator
    pbuf[cnt++]= SecretLock_Enable? 0xE0:0x00;
    
   /* Send Data */
    for (i = 0; i < cnt; i++)  write_byte(pbuf[i]);
    
    
    
   /* Receive CRC */
    pbuf[cnt++]=read_byte();
    pbuf[cnt++]=read_byte();
    
    /* calculate the CRC over this part */
      CRC16 = 0;
      for (i = 0; i < cnt; i++) docrc16(pbuf[i]);   
      if (CRC16 != 0xB001)                            //not 0 because that the calculating result is CRC16 and the reading result is inverted CRC16
      return 0xFF;
    
    
     /* Send Release Byte */
       write_byte(0xAA); 
       for(i=0 ;i<10;i++)                           //Waiting for 100ms for Secreting Loading
       OW_Delay_ms(10);//OW_Delay_us(10000);
       
       outcome=read_byte();
       ow_reset();
       return outcome;
 
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------
/*
The Seting and Lock Secret command is used to install specified secret in application .
// Input parameter: 
   1.unsigned char *secret : Secret buffer ready for loading 
   2.bool LockEnable : specify whether locking secret after loading completion
// Returns: AAh = success
            FFh = CRC Error
            55h = The command failed because the secret was already locked.

*/

unsigned char SettingAndLock_Secret(unsigned char *secret, bool LockEnable)
{   
    bool BoolValue;

    BoolValue= LockEnable;
    if(ReadWriteScratchpad((bool)0,secret)!=0xAA)
      return 0xff;
    return Load_and_LockSecret(BoolValue);
     
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------
/*
The Compute and Lock Secret command is used to install a computed, device specific secret when the DS28E15 is
set up for an application. To increase the security level, one can use this command multiple times, each time with a
different partial secret written to the scratchpad data. The initial secret must be loaded, but not locked, before Compute
and Lock Secret can be executed.
// Input parameter: 
   1.unsigned char PageNum :specifies the memory page that is to be used for the MAC computation.
                            Valid memory page numbers are 0 (page 0) and 1 (page 1).
   2.bool LockEnable:  specify whether the secret is to be write protected (locked) after it is copied to the secret memory
                     
// Returns: AAh = success
            FFh = CRC Error
            55h = The command failed because the secret was already locked.

*/

unsigned char ComputeAndLock_Secret(unsigned char PageNum, bool LockEnable)

{
    unsigned char pbuf[20],cnt=0,i,outcome;
    if ( (ow_reset())!=0 ) return 0;
    write_byte(0xCC);		                                       //Skip ROM command
    
       /* construct a packet to send */
    pbuf[cnt++] = ComputeAndLockSecret_Command;                        // Read Memory Block Status or PersonalityByteindicator
    pbuf[cnt++]= LockEnable? (PageNum|0xE0):(PageNum);
    
     /* Send Data */
    for (i = 0; i < cnt; i++)  write_byte(pbuf[i]);
    
      /* Receive CRC */
    pbuf[cnt++]=read_byte();
    pbuf[cnt++]=read_byte();
    
    /* calculate the CRC over this part */
      CRC16 = 0;
      for (i = 0; i < cnt; i++) docrc16(pbuf[i]);   
      if (CRC16 != 0xB001)                            //not 0 because that the calculating result is CRC16 and the reading result is inverted CRC16
      return 0xFF;
      
       /* Send Release Byte */
       write_byte(0xAA); 
       OW_Delay_ms(6);//OW_Delay_us(6000);                                   //Wait for 2 times for SHA-256 Calculation; 
       for(i=0 ;i<10;i++)                               //Waiting for 100ms for Secreting Loading
       OW_Delay_ms(10);//OW_Delay_us(10000);
  
       outcome=read_byte();
       ow_reset();
       return outcome;
 }




//-------------------------------------------------------------------------------------------------------------------------------------------------------
/*
The Authenticated Write Memory command is used to program one or more contiguous 4 byte segments of a memory
block that requires master authentication. The parameter byte specifies the page and segment number where the writing
begins.
// Input parameter: 
   1.unsigned char Segment:specify the location within the selected memory page where the writing begins.
   2.unsigned char Page:specify the memory page that is to be written to. Valid memory page numbers are 0 (page 0) and 1 (page 1).
   3.unsigned char *Buffer: Data buffer ready for writing specified EEPROM
   4.unsigned char *MAC: MAC value that is computed from the xpected secret, the DS28E15’s ROM ID, the number of the selected segment, 
                         the number of the selected memory page,the manufacturer ID, the currently valid segment data and the new segment data;
// Returns: AAh = success
            FFh = CRC Error
            55h = The command failed because the MAC provided is not valid.
            33h = The command failed because the block is write protected.
*/
unsigned char AuthenticatedWriteMemory(unsigned char Segment ,unsigned char Page, unsigned char *Buffer,unsigned char *MAC)
{
  
    unsigned char pbuf[20],cnt=0,i,outcome;
    if ( (ow_reset())!=0 ) return 0;
    write_byte(0xCC);		                                       //Skip ROM command  
    
    
        /* construct a packet to send */
    pbuf[cnt++] = AuthenticatedWriteMemory_Command;                        // Read Memory Block Status or PersonalityByteindicator
    pbuf[cnt++]= ((Segment<<5)|Page)&0xE1;   
    
       /* Send Data */
    for (i = 0; i < cnt; i++)  write_byte(pbuf[i]);
    
       /* Receive CRC */
    pbuf[cnt++]=read_byte();
    pbuf[cnt++]=read_byte();
    
    /* calculate the CRC over this part */
      CRC16 = 0;
      for (i = 0; i < cnt; i++) docrc16(pbuf[i]);   
      if (CRC16 != 0xB001)                            //not 0 because that the calculating result is CRC16 and the reading result is inverted CRC16
      return 0xFF;
      
      for(i = 0 ,cnt = 0; i < 4 ;i++, cnt++)
      { 
        write_byte(Buffer[i]);
        pbuf[cnt]=Buffer[i];
      }
      
      /* Receive CRC */
       pbuf[cnt++]=read_byte();
       pbuf[cnt++]=read_byte();
       
          /* calculate the CRC over this part */
      CRC16 = 0;
      for (i = 0; i < cnt; i++) docrc16(pbuf[i]);   
      if (CRC16 != 0xB001)                            //not 0 because that the calculating result is CRC16 and the reading result is inverted CRC16
      return 0xFF;
      
      OW_Delay_ms(3);//OW_Delay_us(3000);                                   //Wait for 1 times for SHA-256 Calculation; 
  
      for(i = 0 ,cnt = 0; i < 32 ;i++, cnt++)
      { 
        write_byte(MAC[i]);
        pbuf[cnt]=MAC[i];
      }
       /* Receive CRC */
       pbuf[cnt++]=read_byte();
       pbuf[cnt++]=read_byte();
       
      /* calculate the CRC over this part */
      CRC16 = 0;
      for (i = 0; i < cnt; i++) docrc16(pbuf[i]);   
      if (CRC16 != 0xB001)                            //not 0 because that the calculating result is CRC16 and the reading result is inverted CRC16
      return 0xFF;
      
      outcome=read_byte();
      if(outcome!=0xAA)
      return outcome;
      
       /* Send Release Byte */
       write_byte(0xAA); 
       OW_Delay_ms(10);//OW_Delay_us(10000); 
       outcome=read_byte();
       ow_reset();
       return outcome;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------
/* 
 The Authenticated Write New Data command is used to program one or more contiguous 4 byte segments of a memory
block that requires master authentication in application.
// Input parameter: 
   1.unsigned char Segment:specify the location within the selected memory page where the writing begins.
   2.unsigned char Page:specify the memory page that is to be written to. Valid memory page numbers are 0 (page 0) and 1 (page 1).
   3.unsigned char *Bufferdata: Data buffer ready for writing specified EEPROM
// Returns: AAh = success
            FFh = CRC Error
            55h = The command failed because the MAC provided is not valid.
            33h = The command failed because the block is write protected.
*/

unsigned char AuthenticatedWriteNewData(unsigned char Segment ,unsigned char Page, unsigned char *Bufferdata)
{
  unsigned char segmentindex,pageindex;
	
  unsigned char buffer[32],Mac_value[32],newdata[4];
  segmentindex=Segment;pageindex=Page;
  memcpy(newdata,Bufferdata,4);
  ReadMemory(Segment, Page,buffer);
  memcpy(&MAC_Computer_Datainput[32],RomID_Buffer,32);
  memcpy(&MAC_Computer_Datainput[40],&Personality_Value[2],2);
  MAC_Computer_Datainput[42]=Page&0x01;
  memcpy(&(MAC_Computer_Datainput[44]),buffer,4);
  memcpy(&MAC_Computer_Datainput[48],newdata,4);
  ComputeMAC256(MAC_Computer_Datainput, 55, Mac_value);
  return(AuthenticatedWriteMemory(segmentindex,segmentindex,newdata,Mac_value));
  
}

#endif

//===========================================================================================
//以下用于单片机计算SHA256
#ifdef USE_MCU_SHA256

//----------------------------------------------------------------------
/* 
Seting secret buffer for SHA-256 Algrorithm
// Input parameter: 
   1.unsigned char *secret : Data buffer for secret seting;
// Returns: None;
*/
void set_secret(unsigned char *secret)
{ 
   unsigned char i;

   for (i = 0; i < 32; i++)
      SECRET[i] = secret[i];   
}



//----------------------------------------------------------------------
/*  Computes SHA-256 given the MT digest buffer after first iserting
    the secret at the correct location in the array defined by all Maxim
    devices. Since part of the input is secret it is called a Message
    Autnentication Code (MAC).
//  Input Parameter: 
    1.unsigned char *MT :buffer containing the message digest
    2.short length:Length of block to digest
    3.unsigned char *MAC:result MAC in byte order used by 1-Wire device
// Returns: None;    
*/
void ComputeMAC256(unsigned char *MT, short length, unsigned char *MAC)
{
   unsigned char i,j;  
   unsigned char tmp[4]; 

   // check for two block format
   if (length == 119)
   {
      // insert secret
     memcpy(&MT[64], &Secret_256bit[0], 32);

      // change to little endian for A1 devices
      if (reverse_endian)
      {
         for (i = 0; i < 108; i+=4)
         {
            for (j = 0; j < 4; j++)
               tmp[3 - j] = MT[i + j];
      
            for (j = 0; j < 4; j++)
               MT[i + j] = tmp[j];
         }
      }
   
      ComputeSHA256(MT, 119, TRUE, TRUE, MAC);
   }
   // one block format
   else
   {
      // insert secret
      memcpy(&MT[0], &Secret_256bit[0], 32);

      // change to little endian for A1 devices
      if (reverse_endian)
      {
         for (i = 0; i < 56; i+=4)
         {
            for (j = 0; j < 4; j++)
               tmp[3 - j] = MT[i + j];
      
            for (j = 0; j < 4; j++)
               MT[i + j] = tmp[j];
         }
      }

      ComputeSHA256(MT, 55, TRUE, TRUE, MAC);
   }


}


//----------------------------------------------------------------------
/*Computes SHA-256 given the MT digest buffer after first iserting
  the secret at the correct location in the array defined by all Maxim
  devices. Since part of the input is secret it is called a Message
  Autnentication Code (MAC).
//  Input Parameter: 
    1.unsigned char * MT: buffer containing the message digest 
    2.short length:Length of block to digest
    3.unsigned char* compare_MAC : MAC in byte order used by 1-Wire device to compare
                                    with calculate MAC.        
*/
bool VerifyMAC256(unsigned char * MT, short length, unsigned char* compare_MAC)
{
   unsigned char calc_mac[32];
   int i;

   // calculate the MAC
   ComputeMAC256(MT, length, calc_mac);

   // Compare calculated mac with one read from device
   for (i = 0; i < 32; i++)
   {
      if (compare_MAC[i] != calc_mac[i])
         return (bool)0;
   }
   return (bool)1;
}


/* Performs a Compute Next SHA-256 calculation given the provided 32-bytes
   of binding data and 8 byte partial secret. The first 8 bytes of the
   resulting MAC is set as the new secret.
//  Input Parameter:  
    1. unsigned char *binding: 32 byte buffer containing the binding data
    2. unsigned char *partial: 8 byte buffer with new partial secret
    3. int page_nim: page number that the compute is calculated on 
    4. unsigned char *manid: manufacturer ID
   Globals used : SECRET used in calculation and set to new secret
   Returns: TRUE if compute successful
            FALSE failed to do compute
*/
void  CalculateNextSecret256(unsigned char* binding, unsigned char* partial, int page_num, unsigned char* manid)
{
   unsigned char MT[128];
   unsigned char MAC[64];

   // clear 
   memset(MT,0,128);

   // insert page data
   memcpy(&MT[0],binding,32);

   // insert challenge
   memcpy(&MT[32],partial,32);

   // insert ROM number or FF
   memcpy(&MT[96],RomID_Buffer,8);

   MT[106] = page_num;
   MT[105] = manid[0];
   MT[104] = manid[1];

   ComputeMAC256(MT, 119, MAC);

   // set the new secret to the first 32 bytes of MAC
   set_secret(MAC);

}


//----------------------------------------------------------------------
// SHA-256 support function
//
unsigned long sha_ch(unsigned long x, ulong y, ulong z)
{
   return (x & y) ^ ((~x) & z);
}

//----------------------------------------------------------------------
// SHA-256 support function
//
ulong sha_maj(ulong x, ulong y, ulong z)
{
   ulong temp = x & y;
   temp ^= (x & z);
   temp ^= (y & z);
   return temp;  //(x & y) ^ (x & z) ^ (y & z);
}

//----------------------------------------------------------------------
// SHA-256 support function
//
ulong sha_rotr_32(ulong val, ushort r)
{
   val = val & 0xFFFFFFFFL;
   return ((val >> r) | (val << (32 - r))) & 0xFFFFFFFFL;
}

//----------------------------------------------------------------------
// SHA-256 support function
//
ulong sha_shr_32(ulong val, ushort r)
{
   val = val & 0xFFFFFFFFL;
   return val >> r;
}

//----------------------------------------------------------------------
// SHA-256 support function
//
ulong sha_bigsigma256_0(ulong x)
{
   return sha_rotr_32(x,2) ^ sha_rotr_32(x,13) ^ sha_rotr_32(x,22);
}

//----------------------------------------------------------------------
// SHA-256 support function
//
ulong sha_bigsigma256_1(ulong x)
{
   return sha_rotr_32(x,6) ^ sha_rotr_32(x,11) ^ sha_rotr_32(x,25);
}

//----------------------------------------------------------------------
// SHA-256 support function
//
ulong sha_littlesigma256_0(ulong x)
{
   return sha_rotr_32(x,7) ^ sha_rotr_32(x,18) ^ sha_shr_32(x,3);
}

//----------------------------------------------------------------------
// SHA-256 support function
//
ulong sha_littlesigma256_1(ulong x)
{
   return sha_rotr_32(x,17) ^ sha_rotr_32(x,19) ^ sha_shr_32(x,10);
}

//----------------------------------------------------------------------
// SHA-256 support function
//
void sha_copy32(ulong* p1, ulong* p2, ushort length)
{
   while (length > 0)
   {
      *p2++ = *p1++;
      length--;
   }
}

//----------------------------------------------------------------------
// SHA-256 support function
//
void sha_copyWordsToBytes32(ulong* input, unsigned char* output, ushort numwords)
{
    ulong temp;
    ushort i;

    for (i=0;i<numwords;i++)
    {
        temp = *input++;
        *output++ = (uchar)(temp >> 24);
        *output++ = (uchar)(temp >> 16);
        *output++ = (uchar)(temp >> 8);
        *output++ = (uchar)(temp);
    }
}

//----------------------------------------------------------------------
// SHA-256 support function
//
void sha_writeResult(ushort reverse, uchar* outpointer)
{
   unsigned char i,tmp;;
   sha_copyWordsToBytes32(H32, outpointer, 8); 

   if (reverse)
   {
      for (i = 0; i < 16; i++)
      {  
         tmp = outpointer[i];
         outpointer[i] = outpointer[31-i];
         outpointer[31-i] = tmp;
      }
   }

}

//----------------------------------------------------------------------
// SHA-256 support function
//
unsigned long sha_getW(int index)
{
   unsigned long newW;
   if (index < 16)
   {
      return W32[index];
   }

   newW = sha_littlesigma256_1(W32[(index-2)&0x0f]) + 
            W32[(index-7)&0x0f] + 
          sha_littlesigma256_0(W32[(index-15)&0x0f]) + 
            W32[(index-16)&0x0f];
   W32[index & 0x0f] = newW & 0xFFFFFFFFL;  // just in case...

   return newW;
}

//----------------------------------------------------------------------
// Prepair the block for hashing
//
void sha_prepareSchedule(unsigned char* message)
{
   // we need to copy the initial message into the 16 W registers
   unsigned char i,j;
   unsigned long temp;
   for (i = 0; i < 16; i++)
   {
      temp = 0;
      for (j = 0; j < 4;j++)
      {
         temp = temp << 8;
         temp = temp | (*message & 0xff);
         message++;
      }

      W32[i] = temp;
   }
}

//----------------------------------------------------------------------
// Hash a single block of data. 
//
void sha256_hashblock(unsigned char* message, ushort lastblock)
{
   ushort sha1counter = 0;
   ushort sha1functionselect = 0;
   ushort i;
   unsigned long nodeT1, nodeT2;

   unsigned long Wt, Kt;

   // chunk the original message into the working schedule
   sha_prepareSchedule(message);

   a32 = H32[0];
   b32 = H32[1];
   c32 = H32[2];
   d32 = H32[3];
   e32 = H32[4];
   f32 = H32[5];
   g32 = H32[6];
   h32 = H32[7];

   // rounds
   for (i = 0; i < 64; i++)
   {
      Wt = sha_getW(i);
      Kt = SHA_CONSTANTS[i]; 

      nodeT1 = (h32 + sha_bigsigma256_1(e32) + sha_ch(e32,f32,g32) + Kt + Wt); // & 0xFFFFFFFFL;
      nodeT2 = (sha_bigsigma256_0(a32) + sha_maj(a32,b32,c32)); // & 0xFFFFFFFFL;
      h32 = g32;
      g32 = f32;
      f32 = e32;
      e32 = d32 + nodeT1;
      d32 = c32;
      c32 = b32;
      b32 = a32;
      a32 = nodeT1 + nodeT2;

      sha1counter++;
      if (sha1counter==20)
      {
         sha1functionselect++;
         sha1counter = 0;
      }			

   }

   if (!lastblock)
   {
      // now fix up our H array
      H32[0] += a32;
      H32[1] += b32;
      H32[2] += c32;
      H32[3] += d32;
      H32[4] += e32;
      H32[5] += f32;
      H32[6] += g32;
      H32[7] += h32;
   }
   else
   {
      // now fix up our H array
      H32[0] = a32;
      H32[1] = b32;
      H32[2] = c32;
      H32[3] = d32;
      H32[4] = e32;
      H32[5] = f32;
      H32[6] = g32;
      H32[7] = h32;
   }
}

//----------------------------------------------------------------------
// Computes SHA-256 given the data block 'message' with no padding. 
// The result is returned in 'digest'.   
//
// 'message'  - buffer containing the message 
// 'skipconst' - skip adding constant on last block (skipconst=1)
// 'reverse' - reverse order of digest (reverse=1, MSWord first, LSByte first)
// 'digest'   - result hash digest in byte order used by 1-Wire device
//
void ComputeSHA256(unsigned char* message, short length, ushort skipconst, ushort reverse, unsigned char* digest)
{
   ushort bytes_per_block;
   ushort nonpaddedlength;
   ushort numblocks;
   ushort i,j;
   ulong bitlength;
   ushort markerwritten;
   ushort lastblock;

   ushort wordsize = 32;

  

   // if wordsize is 32 bits, we need 512 bit blocks.  else 1024 bit blocks.
   // that means 16 words are in one message.
   bytes_per_block = 16 * (wordsize / 8);
   // 1 byte for the '80' that follows the message, 8 or 16 bytes of length
   nonpaddedlength = length + 1 + (wordsize/4);
   numblocks = nonpaddedlength / bytes_per_block;
   if ((nonpaddedlength % bytes_per_block) != 0) 
   {
      // then there is some remainder we need to pad
      numblocks++;
   }

   sha_copy32(SHA_256_Initial, H32, SHA_256_INITIAL_LENGTH); 

   bitlength = 8 * length;
   markerwritten = 0;
   // 'length' is our number of bytes remaining.
   for (i = 0; i < numblocks; i++)
   {
      if (length > bytes_per_block)
      {
         memcpy(workbuffer, message, bytes_per_block);
         length -= bytes_per_block;
      }
      else if (length==bytes_per_block)
      {
         memcpy(workbuffer, message, length);
         length = 0;
      }
      else // length is less than number of bytes in a block
      {
         memcpy(workbuffer, message, length);
         // message is now used for temporary space
         message = workbuffer + length;     
         if (markerwritten == 0)
         {
            *message++ = 0x80;
            length++;
         }

         while (length < bytes_per_block)
         {
            // this loop is inserting padding, in this case all zeroes
            *message++ = 0;
            length++;
         }
         length = 0;
         // signify that we have already written the 80h
         markerwritten = 1;
      }

      // on the last block, put the bit length at the very end
      lastblock = (i == (numblocks - 1));
      if (lastblock)
      {
         // point at the last byte in the block
         message = workbuffer + bytes_per_block - 1;
         for (j = 0; j < wordsize/4; j++)
         {
            *message-- = (uchar)bitlength;
            bitlength = bitlength >> 8;
         }
      }

      // SHA in software 
      sha256_hashblock(workbuffer, (ushort)(lastblock && skipconst));
      message += bytes_per_block;
   }

   sha_writeResult(reverse, digest);

  
}

#endif
//以上单片机计算HSA256
//=============================================================================





/*******************************************************************************
* Function Name  : main.
* Description    : Main routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
//void main(void)
void DS28E11_Cal(void)
{
////	unsigned char ret;
////
//////    unsigned char NewPageData[32] = {   0x11,0x24,0x4d,0x2a,0x43,0x29,0x4d,0x2a,
//////                                        0x22,0x25,0x62,0x21,0x43,0x39,0x4e,0x2b,
//////                                        0x33,0x26,0x4d,0x2a,0x43,0x09,0x4d,0x5e,
//////                                        0x44,0x27,0x5d,0x20,0x03,0x29,0x0d,0x43,
//////                                    };
//////    
//////    unsigned char Newupdata[8]={ 0x12,0x34,0x56,0x78,0x90,0x12,0x34,0x56,};
//////    unsigned char GetPageData[32],PageNumber=0,i;
//////    System_Init();  
//////    Peripheral_Init();
//////    Variable_Init();
////
////
////  
//////   while(1)
////   {
//// 
//// 
////#if 0    
////    /*test how to read 64bits ROM ID */
////    Read_RomID(RomID_Buffer);
////    
////    /*test how to read MAN_ID_L, MAN_ID_H value ,which will be used in MAC Computation */
////    ReadBlockStatus(0,(bool) 1 ,Personality_Value);
////    
////    /*test how to read and write EEPROM   Segment0-7;*/
////    if( ReadMemory(Segment0, Page0,EEPROM)==0xaa)
////    { 
////      for( i=0 ; i <8 ;i++)
////      WriteMemory(i, Page0,&NewPageData[4*i]);
////    }
////    
////#endif
////    
////#ifndef USE_CUSTOM_ONLY   
////    
////    /*test how to set specified Block Protecting Mode and read Block protection status */
////     if(ReadBlockStatus(Block0,0 ,Block_Status)==0xaa)
////     {
////       WriteBlockProtection(Block0 ,Authentication_Protection);
////     }
////#endif
////
////
////#if 0
////     /*test how to read and write  Scratchpad buffer */
////     if(ReadWriteScratchpad((bool)0, Scratch_Writebuffer)==0xaa)
////        ReadWriteScratchpad((bool)1, Scratch_Readbuffer);
////#endif
////
////        
////#ifndef USE_CUSTOM_ONLY            
////     /*test how to load 256bit basic secret */
////       SettingAndLock_Secret(Secret_256bit, (bool)0);
////     
////     /* test how to use Computer and Lock Secret command  */
////      ComputeAndLock_Secret(Page1, (bool)0);
////      
////      /*test how to Computer and read SHA-256 MAC according to specified  page num, ROM ID ,256bits secret etc. */
////     ComputeAndReadPageMAC(ANONYMOUS,Page0,MAC_Read_Value);
////#endif
////
//////在此应用中，只需把收到的随机数，放入DS28E11缓冲区，28E11的密码已经在工厂制造时放入，不使用ROM_ID,内部EEPROM的内容为0xff
////     
////     /*test how to compare MAC value between host's computation and DS28E15's computation */
////    
////       // 1. firstly read MAC Value computed by DS28E15 according to Scratch value, secret ,page data ,rom id etc.
////
////#if 0     
////       //已经在工厂制造时写入  
////          SettingAndLock_Secret(Secret_256bit, (bool)0);
////#endif    
////  
////      //==============================================================================================================
////      //以下使用      
////	  //计算随机数放入DS28E11_Buf
////          //ReadWriteScratchpad((bool)0, Scratch_Writebuffer);
////          ReadWriteScratchpad((bool)0, DS28E11_Buf);
////           
////          //第一个参数为1，则不使用内部ROM_ID   
////          //ComputeAndReadPageMAC((bool) 0,Page0,MAC_Read_Value);
////          ret=ComputeAndReadPageMAC((bool)1,Page0,DS28E11_Buf);
////          
////          //得到的MAC在DS28E11_Buf中，共32个字节
////          
////      //以上使用
////      //==============================================================================================================
////
////#if 0       
////      // 2. get MAC value computed by Host according to expected secret, specified EEPROM data,ROM ID etc.
////         if( ReadMemory(0, 0,MAC_Computer_Datainput)==0xaa) 
////        {
////       
////          ReadWriteScratchpad((bool)0, Scratch_Writebuffer); 
////          memcpy(&MAC_Computer_Datainput[32],Scratch_Writebuffer,32);
////          memcpy(&MAC_Computer_Datainput[96],RomID_Buffer,8);
////          memcpy(&MAC_Computer_Datainput[104],&Personality_Value[2],2);
////          MAC_Computer_Datainput[106]=0x00;
////        
////      // 3. compare MAC value between host's computation and DS28E15's computation; if matchs, Verify_outcome=0x01; 
////         Verify_outcome=VerifyMAC256(MAC_Computer_Datainput, 119, MAC_Read_Value);
////       
////	}
////#endif
////
////        
////        
//// #ifndef USE_CUSTOM_ONLY      
////     
////      /* test how to update EEPROM data which is alreday in authenticated protection */
////      
////      //1. Set Block0 in Authenticated protection 
////      if(ReadBlockStatus(Block0,0 ,Block_Status)==0xaa)
////      WriteBlockProtection(Block0 ,Authentication_Protection);
////      
////      //2. WriteMemory should be failure because Block0 is in authenticated protection 
////      for( i=0 ; i <4 ;i++)
////      WriteMemory(i, Page0,&NewPageData[4*i]);
////      
////      
////      //3. Using Authenticate write EEPROM  command to update EEPROM  should be OK, provided MAC is right 
////       AuthenticatedWriteNewData(Segment0 ,Page0, Newupdata);
////       
////       
////       /* test how to complete  Authenticate Protection Setting*/
////
////       //1. Set block0 read protection should be failure,beacuse Block0 is already Authenticated protection
////        if(ReadBlockStatus(Block0,0 ,Block_Status)==0xaa)
////        WriteBlockProtection(Block0 ,Read_Protection);
////       
////       //2. Using AuthenticatedWriteBlockProtection to set Block0 reading protection  should be success provided MAC is right 
////       //AuthenticatedWriteBlockProtection(Block0 ,Read_Protection);
////       Auth_WB_Protection(Block0 ,Read_Protection);
////
////#endif
////
////   }

}
#endif
//<<<<<<<<<<<<<<<<<==============================================================================

















