/*
*********************************************************************************************************
*
*	模块名称 : 加密芯片运算
*	文件名称 : bsp_24e11
*	版    本 : V1.0
*	说    明 : 
*	修改记录 :
*		版本号  日期       作者    说明
*		
*
*
*********************************************************************************************************
*/

#define  BSP_24E11_MODULE

#include <bsp.h>





#define WriteMemory_Function     0x55
#define ReadMemory_Function      0xF0
#define WriteBlock_Protection    0xC3
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
#define Read_Protection 0x80
#define Write_Protection 0x40
#define EMPROM_Emulation 0x20
#define Authentication_Protection 0x10
#define Segment0 0x00
#define Segment1 0x01
#define Page0 0x00
#define Page1 0x01
#define Block0 0x00
#define Block1 0x01
#define Block2 0x10
#define Block3 0x11
#define ANONYMOUS (bool)1
/* SHA-256 Algorithm Definition */
#define SHA_256_INITIAL_LENGTH    8


//============================================================================================
// #define unsigned char 		uint8_t
// #define unsigned int  		uint16_t
// #define unsigned long 		uint32_t
// #define unsigned short    uint32_t
#define uchar unsigned char
#define ulong unsigned long
#define ushort unsigned int
#define bool   unsigned char
//=============================================================================================

//管脚定义
#define RCC_ALL_DS 	(RCC_APB2Periph_GPIOA )
#define GPIO_PORT_DS   GPIOA
#define GPIO_PIN_DS	GPIO_Pin_3

#define One_Wire				GPIOA
#define One_Wire_IO     GPIO_Pin_3

#define One_Wire_Mask   0x0008

#define OUTPUTDATA      0
#define INPUTDATA       1



unsigned long SHA_256_Initial[] = 
{
   0x6a09e667,
   0xbb67ae85,
   0x3c6ef372,
   0xa54ff53a,
   0x510e527f,
   0x9b05688c,
   0x1f83d9ab,
   0x5be0cd19
};

unsigned long SHA_CONSTANTS[] =  
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

unsigned char dscrc_table[] = 
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

static unsigned char SECRET[32];
unsigned long a32, b32, c32, d32, e32, f32, g32, h32; // SHA working variables
unsigned long W32[16];                                        // SHA message schedule
unsigned long H32[8];                                         // last SHA result variables
unsigned int reverse_endian=1; 
unsigned int sha_debug=0;
unsigned long  SHAVM_Hash[5];                         // Hash buffer, in wrong order for Dallas MAC
unsigned char SHAVM_Message[64];


unsigned char CRC8,blockindex,segmentindex,pageindex;
unsigned long CRC16;
unsigned char RomID_Buffer[8],Secret_256bit[32],Personality_Value[2],MAC_Computer_Datainput[120],workbuffer[32];
bool BoolValue;



//======================================================================================================
//函数定义
void bsp_Init28E11Var(void);
void bsp_Init28E11Hard(void);
void bsp_Init28E11(void);
void One_Wire_IO_Direction(uint8_t t); 

unsigned char Read_RomID(unsigned char *RomID);
unsigned char ReadBlockStatus(unsigned char BlockNum,bool PersonalityByteIndicator ,unsigned char *BlockStatus);
unsigned char ow_reset(void);
unsigned char read_bit(void);
void write_bit(char bitval);
unsigned char dowcrc(unsigned char x);
void write_byte(char val);
void  CalculateNextSecret256(unsigned char* binding, unsigned char* partial, int page_num, unsigned char* manid);
unsigned char ComputeAndLock_Secret(unsigned char PageNum, bool LockEnable);
unsigned char ComputeAndReadPageMAC(bool Anonymous_Mode,unsigned char PageNum,unsigned char *MAC_Value);
void ComputeMAC256(unsigned char *MT, short length, unsigned char *MAC);
void ComputeSHA256(unsigned char* message,short length,ushort skipconst,ushort reverse,unsigned char* digest);
void Delay_us(unsigned long t);
unsigned short docrc16(unsigned short data);
unsigned char dowcrc(unsigned char x);
unsigned char Load_and_LockSecret(bool SecretLock_Enable);
unsigned char read_byte(void);
unsigned char   ReadMemory(unsigned char Segment, unsigned char Page,unsigned char *Receive);
//unsigned char ReadWriteScratchpad(bool ReadMode, unsigned char *Buffer);
void set_secret(unsigned char *secret);
unsigned char SettingAndLock_Secret(unsigned char *secret, bool LockEnable);
void sha256_hashblock(unsigned char* message, ushort lastblock);
ulong sha_bigsigma256_0(ulong x);
ulong sha_bigsigma256_1(ulong x);
unsigned long sha_ch(unsigned long x, ulong y, ulong z);
void sha_copy32(ulong* p1, ulong* p2, ushort length);
void sha_copyWordsToBytes32(ulong* input, unsigned char* output, ushort numwords);
unsigned long sha_getW(int index);
ulong sha_littlesigma256_0(ulong x);
ulong sha_littlesigma256_1(ulong x);
ulong sha_maj(ulong x, ulong y, ulong z);
void sha_prepareSchedule(unsigned char* message);
ulong sha_rotr_32(ulong val, ushort r);
ulong sha_shr_32(ulong val, ushort r);
void sha_writeResult(ushort reverse, uchar* outpointer);
bool VerifyMAC256(unsigned char * MT, short length, unsigned char* compare_MAC);
unsigned char WriteBlockProtection(unsigned char Block ,unsigned char ProtectOption);
//unsigned char   WriteMemory(unsigned char Segment, unsigned char Page,unsigned char *Buffer);

void memcpy_clf(unsigned char *p1, unsigned char *p2, unsigned char len);
void memset_clf(unsigned char *p1, unsigned char dat, unsigned char len);

/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************/
void 	TEST_28E11(uint8_t *pDatIn,uint8_t *pDatOut)
{
    unsigned char NewPageData[32] = {   0x11,0x24,0x4d,0x2a,0x43,0x29,0x4d,0x2a,
                                        0x22,0x25,0x62,0x21,0x43,0x39,0x4e,0x2b,
                                        0x33,0x26,0x4d,0x2a,0x43,0x09,0x4d,0x5e,
                                        0x44,0x27,0x5d,0x20,0x03,0x29,0x0d,0x43,
                                    };
    
    unsigned char Newupdata[8]={ 0x12,0x34,0x56,0x78,0x90,0x12,0x34,0x56,};
		
    unsigned char GetPageData[32],PageNumber=0,i;
		
		unsigned char EEPROM[8],Block_Status[2];
		unsigned char Scratch_Writebuffer[8],Scratch_Readbuffer[8],MAC_Read_Value[32];
		unsigned char Verify_outcome;
		
		
    bsp_Init28E11();

  
   //while(1)
   {
     
						/*test how to read 64bits ROM ID */
						Read_RomID(RomID_Buffer);
						
						/*test how to read MAN_ID_L, MAN_ID_H value ,which will be used in MAC Computation */
		        //位于用户数据区，用于制造商信息，16个字节
						ReadBlockStatus(0,(bool) 1 ,Personality_Value);
						
						/*test how to read and write EEPROM   Segment0-7;*/
		        //用户数据读写
		        //用户数据分2个pages(32个字节),每个page分4个blocks(8个字节)，分8个segment(4个字节)，
						if( ReadMemory(Segment0, Page0,EEPROM)==0xaa)
						{ 
								for( i=0 ; i <8 ;i++)
								WriteMemory(i, Page0,&NewPageData[4*i]);
						}
												
						/*test how to set specified Block Protecting Mode and read Block protection status */
						//读取 用户数据区 每区的状态
					  if(ReadBlockStatus(Block0,(bool)0,Block_Status)==0xaa)
					  {
								WriteBlockProtection(Block0 ,Authentication_Protection);
					  }


						 /*test how to read and write  Scratchpad buffer */
						//读取中间结果缓冲区
						 if(ReadWriteScratchpad((bool)0, Scratch_Writebuffer)==0xaa)
								ReadWriteScratchpad((bool)1, Scratch_Readbuffer);
						 
						 /*test how to load 256bit basic secret */
						 //设置状态保密数据
						 SettingAndLock_Secret(Secret_256bit, (bool)0);
						 
						 /* test how to use Computer and Lock Secret command  */
						 //计算并锁定密文
						 ComputeAndLock_Secret(Page1, (bool)0);
						 
							
						 /*test how to Computer and read SHA-256 MAC according to specified  page num, ROM ID ,256bits secret etc. */
						 //输入一串数据(存于指定page)和ROM_ID(如果ROM_ID不用，用0XFF代替),最后一个参数为输出的MAC值(哈希值)
						 ComputeAndReadPageMAC(ANONYMOUS,Page0,MAC_Read_Value);
	

//=============================================================================================================>>>>>>>						 
						 /*test how to compare MAC value between host's computation and DS28E15's computation */
						 //用于主机发过来的MAC值和DS28E15计算产生的MAC值比较					
						 // 1. firstly read MAC Value computed by DS28E15 according to Scratch value, secret ,page data ,rom id etc.
									SettingAndLock_Secret(Secret_256bit, (bool)0);									
									ReadWriteScratchpad((bool)0, Scratch_Writebuffer);
									ComputeAndReadPageMAC((bool) 0,Page0,MAC_Read_Value);
							 
						// 2. get MAC value computed by Host according to expected secret, specified EEPROM data,ROM ID etc.
								 if( ReadMemory(0, 0,MAC_Computer_Datainput)==0xaa) 
								{
							 
									ReadWriteScratchpad((bool)0, Scratch_Writebuffer); 
									memcpy_clf(&MAC_Computer_Datainput[32],Scratch_Writebuffer,32);
									memcpy_clf(&MAC_Computer_Datainput[96],RomID_Buffer,8);
									memcpy_clf(&MAC_Computer_Datainput[104],&Personality_Value[2],2);
									MAC_Computer_Datainput[106]=0x00;
								
							// 3. compare MAC value between host's computation and DS28E15's computation; if matchs, Verify_outcome=0x01; 
								 Verify_outcome=VerifyMAC256(MAC_Computer_Datainput, 119, MAC_Read_Value);
							 
								 }
//======================================================================================================<<<<<<<<<<<


//==================================================================================================================		
#if 0								 
							/* test how to update EEPROM data which is alreday in authenticated protection */							
							//1. Set Block0 in Authenticated protection 
							if(ReadBlockStatus(Block0,(bool)0 ,Block_Status)==0xaa)
							WriteBlockProtection(Block0 ,Authentication_Protection);						
							//2. WriteMemory should be failure because Block0 is in authenticated protection 
							for( i=0 ; i <4 ;i++)
							WriteMemory(i, Page0,&NewPageData[4*i]);				
							//3. Using Authenticate write EEPROM  command to update EEPROM  should be OK, provided MAC is right 
							AuthenticatedWriteNewData(Segment0 ,Page0, Newupdata);
#endif 
//==================================================================================================================						

//==================================================================================================================	
#if 0							
							 /* test how to complete  Authenticate Protection Setting*/
							 //1. Set block0 read protection should be failure,beacuse Block0 is already Authenticated protection
							 if(ReadBlockStatus(Block0,0 ,Block_Status)==0xaa)
								WriteBlockProtection(Block0 ,Read_Protection);							 
							 //2. Using AuthenticatedWriteBlockProtection to set Block0 reading protection  should be success provided MAC is right 
							 AuthenticatedWriteBlockProtection(Block0 ,Read_Protection);
#endif
//===================================================================================================================      
   }


}


/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/

void bsp_Init28E11(void)
{
	bsp_Init28E11Hard();		/* 初始化按键硬件 */
	bsp_Init28E11Var();		/* 初始化按键变量 */
}


/*
*********************************************************************************************************
*	函 数 名: bsp_Init28E11Hard
*	功能说明: 初始化按键硬件
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_Init28E11Hard(void)
{
	/* 配置按键采样口为浮空输入*/
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_ALL_DS, ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_DS; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;						//上拉输入
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
  GPIO_Init(GPIO_PORT_DS, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	函 数 名: bsp_Init28E11Var
*	功能说明: 初始化按键变量
*	形    参：strName : 例程名称字符串
*			  strDate : 例程发布日期
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_Init28E11Var(void)
{
    unsigned char i,buf[8];
	
	  for(i=0;i<4;i++)buf[i]=0x00;
		for( i=0 ; i <8 ;i++)
	  {
								WriteMemory(i, Page0,&buf[0]);
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
        One_Wire_IO_Direction(OUTPUTDATA);         	// set 1-Wire Data Pin to output
       	GPIO_ResetBits(One_Wire, One_Wire_IO);       	// leave 1-Wire Data Pin low for 60us
        Delay_us(60);
				GPIO_SetBits(One_Wire, One_Wire_IO);         	// allow line to return high
        One_Wire_IO_Direction(INPUTDATA);          	// set 1-Wire Data pin to input mode   
        Delay_us(8);                                 	// wait for presence
	
        //presence= ((One_Wire->IDR)>>8)&(0x40);
	      presence= (unsigned char)((One_Wire->IDR)&(One_Wire_Mask));	
        Delay_us(10);
//===============================================>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//以下测试用	
//	      One_Wire_IO_Direction(OUTPUTDATA); 
//	      while(1)
//				{
//						GPIO_ResetBits(One_Wire, One_Wire_IO); 
//					  Delay_us(1);
//					  GPIO_SetBits(One_Wire, One_Wire_IO); 
//					  Delay_us(1);
//				}
//<<<<<<<<<<<<<<<========================================================================				
				
				return(presence);                            	// presence signal returned
} 

void One_Wire_IO_Direction(uint8_t t)
{
				GPIO_InitTypeDef GPIO_InitStructure;

				RCC_APB2PeriphClockCmd(RCC_ALL_DS, ENABLE);
	
				if(t==OUTPUTDATA)
				{
						GPIO_InitStructure.GPIO_Pin = GPIO_PIN_DS; 
						GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;						//输出
						GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
				}
				else
				{
						GPIO_InitStructure.GPIO_Pin = GPIO_PIN_DS; 
						GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;						//上拉输入
						GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
				}
				
				GPIO_Init(GPIO_PORT_DS, &GPIO_InitStructure);
}

//------------------------------------------------------------------------------------------- 
/* read bit funtion by 1-wire communication
// Input parameter: None 
// Returns: Reading bits status on 1-wire line
*/
unsigned char read_bit(void)
{
				unsigned char vamm;
        //One_Wire_IO_Direction(OUTPUTDATA); 
				GPIOA->CRL&=0XFFFF0FFF; 
				GPIOA->CRL|=0X00003000;//PA3 推挽输出
	
        GPIO_ResetBits(One_Wire, One_Wire_IO);
	
        //One_Wire_IO_Direction(INPUTDATA);
	
	      Delay_us(1);
	      GPIOA->CRL&=0XFFFF0FFF; 
				GPIOA->CRL|=0X00004000;//PA3 悬空输入
	
        //vamm=((One_Wire->IDR)>>8)&(0x40);
	      vamm= (unsigned char)((One_Wire->IDR)&(One_Wire_Mask));	
				Delay_us(5);                                 //waiting for Reading slot over
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
        One_Wire_IO_Direction(OUTPUTDATA);
        GPIO_ResetBits(One_Wire, One_Wire_IO);
        Delay_us(1);                                //waiting for 1 us

        if(bitval!=0)
        GPIO_SetBits(One_Wire, One_Wire_IO);
        Delay_us(12);                              // waiting for 12us      
        GPIO_SetBits(One_Wire, One_Wire_IO);       // restore 1-wire data pin to high  
     
				Delay_us(10); 
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


unsigned char Read_RomID(unsigned char *RomID)
{
   unsigned char i;
   if ( (ow_reset())!=0 ) return 0;
   write_byte(0x33);
   Delay_us(10);
   for(i = 0;i < 8;i++)
   {
      RomID[i] = read_byte();
   }
  
  
    /*CRC8 Check */
    CRC8=0;
    for(i=0;i<8;i++)
    	dowcrc(RomID[i]);     //if Receiving No  Error ,CRC =0;
    if(CRC8!=0) return 0;
    else return 1; 
   
  
}



//--------------------------------------------------------------------------
/*Write 4-byte to EEPROM memory of DS28E15
This command is applicable only to memory blocks that do not require master authentication (authentication
protection not activated). Additionally, the memory block must not be write protected.
// Input parameter: 
   1.unsigned char Segment : Specify Writing Segment Number  ;DS28E15 EEPROM include 2 Pages ,Each Page 32Bytes ;  ？？？？64Bytes 
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

unsigned char   WriteMemory(unsigned char Segment, unsigned char Page,unsigned char *Buffer)
{
    
   unsigned char pbuf[20],cnt=0,i,outcome;
  
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
       Delay_us(10000);
       
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

unsigned char   ReadMemory(unsigned char Segment, unsigned char Page,unsigned char *Receive)
{
  unsigned char pbuf[20],cnt=0,i;
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
        return 0xFF;
        return 0xAA; 
              
}

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
       Delay_us(10000);
       
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

unsigned char AuthenticatedWriteBlockProtection(unsigned char Block ,unsigned char ProtectOption)

{
  
     unsigned char pbuf[20],cnt=0,i,outcome,Mac_value[32],status[4];
     blockindex=Block;
     ReadBlockStatus(blockindex,(bool)0, status);
     memcpy_clf(&MAC_Computer_Datainput[32],RomID_Buffer,32);
     memcpy_clf(&MAC_Computer_Datainput[40],&Personality_Value[2],2);
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
  
     
  
       Delay_us(3000);                                   //Wait for 1 times for SHA-256 Calculation; 
  
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
       Delay_us(10000);
  
       outcome=read_byte();
       ow_reset();
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
    unsigned char pbuf[20],cnt=0,i;
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------
/* To read or write 32 Bytes Scratchpad buffer.To ensure the integrity of the scratchpad data the master can rely on the CRC that
the DS28E15 generates and transmits after the scratchpad is filled to capacity
// Input parameter: 
   1. bool ReadMode : specify to read or write scratchpad buffer
   2. unsigned char *Buffer : Data buffer ; If read scratchpad ,Buffer is receiving data. If write scratchpad ,Buffer is data ready for writing.
// Returns: AAh = success
            FFh = CRC Error
           
*/

unsigned char ReadWriteScratchpad(bool ReadMode, unsigned char *Buffer)
{
    unsigned char pbuf[50]/*pbuf[20]*/,cnt=0,i;
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
       Delay_us(10000);
       
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
       Delay_us(6000);                                   //Wait for 2 times for SHA-256 Calculation; 
       for(i=0 ;i<10;i++)                               //Waiting for 100ms for Secreting Loading
       Delay_us(10000);
  
       outcome=read_byte();
       ow_reset();
       return outcome;
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
  
    unsigned char pbuf[50],cnt=0,i,outcome;
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
      
      Delay_us(6000);                                   //Wait for 2 times for SHA-256 Calculation;  
  
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
      
      Delay_us(3000);                                   //Wait for 1 times for SHA-256 Calculation; 
  
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
       Delay_us(10000); 
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
  unsigned char buffer[32],Mac_value[32],newdata[4];
  segmentindex=Segment;pageindex=Page;
  memcpy_clf(newdata,Bufferdata,4);
  ReadMemory(Segment, Page,buffer);
  memcpy_clf(&MAC_Computer_Datainput[32],RomID_Buffer,32);
  memcpy_clf(&MAC_Computer_Datainput[40],&Personality_Value[2],2);
  MAC_Computer_Datainput[42]=Page&0x01;
  memcpy(&(MAC_Computer_Datainput[44]),buffer,4);
  memcpy(&MAC_Computer_Datainput[48],newdata,4);
  ComputeMAC256(MAC_Computer_Datainput, 55, Mac_value);
  return(AuthenticatedWriteMemory(segmentindex,segmentindex,newdata,Mac_value));
  
  
}


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
     memcpy_clf(&MT[64], &Secret_256bit[0], 32);

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
      memcpy_clf(&MT[0], &Secret_256bit[0], 32);

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
   memset_clf(MT,0,128);

   // insert page data
   memcpy_clf(&MT[0],binding,32);

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
         memcpy_clf(workbuffer, message, bytes_per_block);
         length -= bytes_per_block;
      }
      else if (length==bytes_per_block)
      {
         memcpy_clf(workbuffer, message, length);
         length = 0;
      }
      else // length is less than number of bytes in a block
      {
         memcpy_clf(workbuffer, message, length);
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

//延时函数 for(dly=0;dly<9;dly++);
//输入60uS,实际为64us
//输入8uS,实际为9uS
//输入10uS,实际为11.5uS
//输入5uS,时间为5.8uS
//输入2uS,实际为1.5uS
//这里改为for(dly=0;dly<8;dly++);

void Delay_us(unsigned long t)
{
		uint8_t dly;
	  while(t)
		{
			  //for(dly=0;dly<36;dly++);
			  //for(dly=0;dly<9;dly++);
			  for(dly=0;dly<8;dly++);
				t--;
		}
}

void memcpy_clf(unsigned char *p1, unsigned char *p2, unsigned char len)
{
		while(len>0)
		{
				*p1++=*p2++;
			  len--;
		}
}

void memset_clf(unsigned char *p1, unsigned char dat, unsigned char len)
{
		while(len>0)
		{
				*p1++=dat;
			  len--;
		}
}




























/***************************** ************************************************************************/
