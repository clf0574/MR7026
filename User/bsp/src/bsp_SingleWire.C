//======================Copyright (c)=====================================
// 文件名称: Base_SingleWire.C
// 功能描述:
//
// 修改记录:
//           2017-06-21, PiaoHong 创建文件
//
//
//========================================================================

#ifndef _BASE_SINGLEWIRE_C
#define _BASE_SINGLEWIRE_C

#include "includes.h"
//= Start ================================================================
//
//DS28E01在存储上和DS2431是一样的。都是1024位。读写方式也是相同的。
//
//采用一线制通讯方式。
//
//在时序上，控制0电平的时间是很重要的。不能少于资料规定的MIN值。
//

#define bool  uint8_t
#define uint8 uint8_t
#define uint16 uint16_t
#define false 0
#define true  1
#define HIGH  1
#define LOW   0
#define Base_Interrupt_Enable()  __set_PRIMASK(0)
#define Base_Interrupt_Disable() __set_PRIMASK(1)

#define GPIO_PORT_DS    GPIOA
#define GPIO_PIN_DS     GPIO_Pin_3
#define RCC_ALL_DS 	    (RCC_APB2Periph_GPIOA )

#define DSE_State()     GPIO_ReadInputDataBit(GPIO_PORT_DS,GPIO_PIN_DS)

#define DQHIGH()    DSE_Out(HIGH)
#define DQLOW()    DSE_Out(LOW)






const uint8 dscrc_table[] =
{
    0, 94, 188, 226, 97, 63, 221, 131, 194, 156, 126, 32, 163, 253, 31, 65,
    157, 195, 33, 127, 252, 162, 64, 30, 95,  1, 227, 189, 62, 96, 130, 220,
    35, 125, 159, 193, 66, 28, 254, 160, 225, 191, 93,  3, 128, 222, 60, 98,
    190, 224,  2, 92, 223, 129, 99, 61, 124, 34, 192, 158, 29, 67, 161, 255,
    70, 24, 250, 164, 39, 121, 155, 197, 132, 218, 56, 102, 229, 187, 89,  7,
    219, 133, 103, 57, 186, 228,  6, 88, 25, 71, 165, 251, 120, 38, 196, 154,
    101, 59, 217, 135,  4, 90, 184, 230, 167, 249, 27, 69, 198, 152, 122, 36,
    248, 166, 68, 26, 153, 199, 37, 123, 58, 100, 134, 216, 91,  5, 231, 185,
    140, 210, 48, 110, 237, 179, 81, 15, 78, 16, 242, 172, 47, 113, 147, 205,
    17, 79, 173, 243, 112, 46, 204, 146, 211, 141, 111, 49, 178, 236, 14, 80,
    175, 241, 19, 77, 206, 144, 114, 44, 109, 51, 209, 143, 12, 82, 176, 238,
    50, 108, 142, 208, 83, 13, 239, 177, 240, 174, 76, 18, 145, 207, 45, 115,
    202, 148, 118, 40, 171, 245, 23, 73,  8, 86, 180, 234, 105, 55, 213, 139,
    87,  9, 235, 181, 54, 104, 138, 212, 149, 203, 41, 119, 244, 170, 72, 22,
    233, 183, 85, 11, 136, 214, 52, 106, 43, 117, 151, 201, 74, 20, 246, 168,
    116, 42, 200, 150, 21, 75, 169, 247, 182, 232, 10, 84, 215, 137, 107, 53
};
static short oddparity[16] = { 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0 };

//uint8 DS28E11_Buf[32];
uint8 CRC8;
unsigned int CRC16;
//-------------------------------------------------------------------------------------------

void DSE_InOut(uint8_t st)
{	
	
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 打开GPIO时钟 */
	RCC_APB2PeriphClockCmd(RCC_ALL_DS, ENABLE);
	
	if(st==false)
	{
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		/* 设为输出口 */
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	/* IO口最大速度 */
			GPIO_InitStructure.GPIO_Pin = GPIO_PIN_DS;
			GPIO_Init(GPIO_PORT_DS, &GPIO_InitStructure);
	}
	else
	{
		  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	// 输入浮空模式 		
			GPIO_InitStructure.GPIO_Pin = GPIO_PIN_DS;
			GPIO_Init(GPIO_PORT_DS, &GPIO_InitStructure);
	}
}

void DSE_Out(uint8_t st)
{
	if(st==HIGH)
	{
			GPIO_PORT_DS->BSRR=GPIO_PIN_DS;
	}
	else
	{
			GPIO_PORT_DS->BRR=GPIO_PIN_DS;
	}
}

/* CRC calculation
// Input parameter:
   1.uint8 x :   data ready for crc
// Returns:
   1.uint8 : crc output

*/

uint8 dowcrc(uint8 x)
{
    CRC8 = dscrc_table[CRC8 ^ x];
    return CRC8;
}



/*--------------------------------------------------------------------------
 * Calculate a new CRC16 from the input data shorteger.  Return the current
 * CRC16 and also update the global variable CRC16.
// Input parameter:
   1.uint16 data:   data ready for 16bitcrc
// Returns:
   1.uint16 : 16bit crc output
*/

uint16 docrc16(uint16 data)
{
    data = (data ^(CRC16 & 0xff)) & 0xff;
    CRC16 >>= 8;

    if (oddparity[data & 0xf] ^ oddparity[data >> 4])
        CRC16 ^= 0xc001;

    data <<= 6;

    CRC16   ^= data;

    data <<= 1;

    CRC16   ^= data;

    return CRC16;
}


//--------------------------------------------------------------
// 函数名称: Get_CRC8
// 功能描述: 得到CRC8校验值 X8+X5+X4+1
// 输入参数: 无
// 返回参数: 无
// 修改记录:
//           2013-05-07, PiaoHong 创建函数
//---------------------------------------------------------------
static uint8 Get_CRC8(uint8 *pBuff, uint16 len)
{
    uint8 crc = 0, i, j;

    for (i = 0; i < len ; i++)
    {
        crc = crc ^ *pBuff++;

        for (j = 0; j < 8; j++)
        {
            if
            (crc & 0x01) crc = (crc >> 1) ^ 0x8C;
            else
                crc >>= 1;
        }
    }

    return crc;
}
//====================================================
#define OSC 16
#if OSC ==6
void Base_Delay_1us(void)
{
   __nop();
//    __nop();
//    __nop();
//   __nop();
//     __nop();
//    __nop();
//    __nop();
 //   __nop();
//    __nop();
 //   __nop();
 //   __nop();
    

   
   // __nop();
    //__nop();
    //__nop();
    //__nop();
    //__nop();
//    __nop();
    //__nop();
}

void Base_Delay_5us(unsigned int us)
{
    while(us--)
    {
	//			__nop();
        Base_Delay_1us();
 //       Base_Delay_1us();
 //       Base_Delay_1us();
 //       Base_Delay_1us();
 //       Base_Delay_1us();
    }
}
void Base_Delay_50us(void)
{
		Base_Delay_5us(64);
//		Base_Delay_1us();
//		Base_Delay_1us();
}	
void Base_Delay_60us(void)
{
	  Base_Delay_5us(79);
//		Base_Delay_1us();
//		Base_Delay_1us();
		
}
void Base_Delay_45us(void)
{
		Base_Delay_5us(58);
//		Base_Delay_1us();
}	
void Base_Delay_240us(void)
{
		Base_Delay_5us(350);
//		Base_Delay_1us();
}	
void Base_Delay_720us(void)
{
		Base_Delay_5us(1050);
//		Base_Delay_1us();
}	
void Base_Delay_10us(void)
{
		Base_Delay_5us(1);
		Base_Delay_1us();
		Base_Delay_1us();
		__nop();
    __nop();
    __nop();
    __nop();
//		 __nop();
//	 __nop();
}	
void Base_Delay_5_5us(void)		//5us
{
    Base_Delay_1us();
}
#elif OSC == 16
void Base_Delay_1us(void)
{
   __nop();
    __nop();
    __nop();
   __nop();
     __nop();
    __nop();
    __nop();
    __nop();
    __nop();
 //   __nop();
 //   __nop();
    

   
   // __nop();
    //__nop();
    //__nop();
    //__nop();
    //__nop();
//    __nop();
    //__nop();
}

void Base_Delay_5us(uint16 us)
{
    while(us--)
    {
        Base_Delay_1us();
        Base_Delay_1us();
        Base_Delay_1us();
        Base_Delay_1us();
        Base_Delay_1us();
    }
}
void Base_Delay_50us(void)
{
		Base_Delay_5us(10);
		Base_Delay_1us();
		Base_Delay_1us();
}	
void Base_Delay_60us(void)
{
	  Base_Delay_5us(12);
		Base_Delay_1us();
		Base_Delay_1us();
		
}
void Base_Delay_45us(void)
{
		Base_Delay_5us(9);
		Base_Delay_1us();
}	
void Base_Delay_240us(void)
{
		Base_Delay_5us(52);
//		Base_Delay_1us();
}	
void Base_Delay_720us(void)
{
		Base_Delay_5us(157);
//		Base_Delay_1us();
}	
void Base_Delay_10us(void)
{
		Base_Delay_5us(1);
		Base_Delay_1us();
		Base_Delay_1us();
		__nop();
    __nop();
    __nop();
    __nop();
		 __nop();
	 __nop();
}	
void Base_Delay_5_5us(void)		//5us
{
		Base_Delay_1us();
    Base_Delay_1us();
    Base_Delay_1us();
//    Base_Delay_1us();
//    Base_Delay_1us();
}
#endif

//====================================================
//总线上器件复位
bool SingleWire_Reset(void)
{
    bool ret = false;

    Base_Interrupt_Disable();

    DSE_InOut(false);   //置成输出
    DSE_Out(LOW);       //拉低720us
    Base_Delay_720us();//720
    DSE_InOut(true);    //置成输入
    Base_Delay_50us();
    //等10us

    if (DSE_State() == LOW)
    {   //为低，初始化完成
        Base_Delay_240us();
        DSE_InOut(false);
        DQHIGH();
        ret = true;
    }
    else
    {   //初始化失败
        Base_Delay_240us();
        DSE_InOut(false);
        DQHIGH();
        ret = false;
    }

    Base_Delay_10us();

    Base_Interrupt_Enable();
    return ret;
}


//发送一位数据，恢复时间10us
void OWWriteBit(uint8 bit)
{
    Base_Interrupt_Disable();
    DSE_InOut(false);
//    DQHIGH();
//    tickDelay(G);  //命令间隔
    DQLOW();
    Base_Delay_5_5us();

    if (bit)
    {
        DQHIGH();
        Base_Delay_60us();
    }
    else
    {
        Base_Delay_60us();
        DQHIGH();
    }

    Base_Delay_5_5us();

    Base_Interrupt_Enable();
}

uint8 OWReadBit(void)
{
    Base_Interrupt_Disable();
    DSE_InOut(false);

    DQLOW();
    Base_Delay_5_5us();
    DQHIGH();
    DSE_InOut(true);
    Base_Delay_5_5us();

    if (DSE_State())
    {
        Base_Delay_45us();
        DSE_InOut(false);
        DQHIGH();
        Base_Interrupt_Enable();
        Base_Delay_5_5us();
        return 1;
    }
    else
    {
        Base_Delay_45us();
        DSE_InOut(false);
        DQHIGH();
        Base_Interrupt_Enable();
        Base_Delay_5_5us();
        return 0;
    }
}


void OWWriteByte(uint8 value)
{
    uint8 i;

    for (i = 0;i < 8;i++)
    {
        OWWriteBit(value & 0x01);
        value >>= 1;
    }
}

uint8 OWReadByte(void)
{
    uint8 i, result = 0;

    for (i = 0;i < 8;i++)
    {
        result >>= 1;

        if (OWReadBit())result |= 0x80;
    }

    return result;
}

//=========================================================
//================================================================================================================
//20170314?? ??????
//2017.3.14
uint8 ANON_READ_AuthPage(uint8 *Receive_buffer, uint8* Scratchpad, uint8 PageNum)
{
    uint8 pbuf[60], cnt;
    uint8 /*Challenge[8],*/i;

    /* write Challenge */
    SingleWire_Reset();                 //reset 1-wire bus and detect response pulse
    OWWriteByte(0xCC);          //SKIP ROM command
    cnt = 0;
    pbuf[cnt++] = 0x0F;        //Function Command, Write challenge to scratchpad
    pbuf[cnt++] = PageNum << 5;
    pbuf[cnt++] = PageNum >> 3;

    pbuf[cnt++] = 0xff;
    pbuf[cnt++] = 0xff;

    for (i = 0;i < 5;i++) pbuf[cnt++] = Scratchpad[i];

    pbuf[cnt++] = 0xff;

    for (i = 0;i < 11;i++) OWWriteByte(pbuf[i]);

    /* Read CRC */
    pbuf[cnt++] = OWReadByte();

    pbuf[cnt++] = OWReadByte();

    /* Verify CRC */
    CRC16 = 0;

    for (i = 0; i < cnt; i++) docrc16(pbuf[i]);


    // return result of inverted CRC
    if (CRC16 != 0xB001) return 0;

    /****************************************************************/
    /* Read Anonymous Auth Page MAC */
    SingleWire_Reset();

    OWWriteByte(0xCC);

    cnt = 0;

    pbuf[cnt++] = 0xCC;        //Function Command, Anony Auth MAC

    pbuf[cnt++] = PageNum << 5;

    pbuf[cnt++] = PageNum >> 3;

    for (i = 0;i < 3;i++) OWWriteByte(pbuf[i]);

    for (i = 0;i < 35;i++)     //Master receive 32 Page data, oxFF, 2Bytes CRC
    {
        pbuf[cnt++] = OWReadByte();
    }

    /* Verify CRC */
    CRC16 = 0;

    for (i = 0; i < cnt; i++) docrc16(pbuf[i]);

    if (CRC16 != 0xB001)   return 0;  //not 0 because that the calculating result is CRC16 and the reading result is inverted CRC16


//		OW_Delay_ms(5);//Delay_Ms(2000);                   //waiting for finishing SHA-1 algorithm
    Base_Delay_5us(5000 / 5);

    cnt = 0;

    for (i = 0;i < 22;i++)            //Receive Auth MAC 20Bytes ,2 CRC Bytes
    {
        pbuf[cnt++] = OWReadByte();
//					Base_Delay_5us(5/5);
    }

    /* Verify CRC */
    CRC16 = 0;

    for (i = 0; i < cnt; i++)
        docrc16(pbuf[i]);

    if (CRC16 != 0xB001)
        return 0;  //not 0 because that the calculating result is CRC16 and the reading result is inverted CRC16

    /*copy Auth Page MAC to Receive_buffer*/
    //memcpy(Receive_buffer, pbuf, 32);
    for (i = 0;i < 20;i++)Receive_buffer[i] = pbuf[i];

    return 1;
}

uint8  Crc8Check(uint8 *ptr, uint8 length)
{
    uint8 i;
    uint8 Crc8 = 0;

    while (length--)
    {
        //g(x)=x8+x5+x4+1
        for (i = 0x80; i; i >>= 1)
        {
            if ((Crc8&0x80) != 0)
            {
                Crc8 <<= 1;
                Crc8 ^= 0x31;
            }
            else
            {
                Crc8 <<= 1;
            }

            if (*ptr&i)
            {
                Crc8 ^= 0x31;
            }
        }

        ptr++;
    }

    return Crc8;
}

//uint8  SumCheck(uint8 *p, uint8 length)
//{
//    uint8  i;
//    uint8 sum = 0;

//    for (i = 0;i < length;i++)
//    {
//        sum += p[i];
//    }

//    return sum;
//}

void Secret_Verify(uint8 *pdat, uint8 len)
{
    uint8 rbuf[32], datbuf[10], i, cnt, ret;

    if (len >= 5)len = 5;

    for (i = 0;i < len;i++)datbuf[i] = *(pdat + i);

    for (i = len;i < 5;i++)datbuf[i] = 0x00;

    for (i = 0;i < 32;i++)rbuf[i] = 0x00;

    //????
    cnt = 3;

    while (cnt > 0)
    {
        Base_Interrupt_Disable();
        ret = ANON_READ_AuthPage(&rbuf[0], &datbuf[0], 0);
        Base_Interrupt_Enable();
        //if((ret!=0xff)&&(ret!=0x00))break;

        if (ret == 1)break;

        cnt--;
    }

    //???????
    *(pdat + 0) = Crc8Check(rbuf, 20);

    *(pdat + 1) = SumCheck(rbuf, 20);
}

//=========================================================
//常用的ROM命令有

#define READ_ROM  0x33 //读取器件ROM
#define MATCH_ROM  0x55 //匹配器件ROM
#define SKIP_ROM 0xCC //忽略ROM匹配
//用的最多的是SKIP_ROM命令，针对单一芯片操作。

//对应的函数代码如下：

//***************************************************
//读8位家族码;48位序列号;8位CRC码;读取成功返回0
//参数:   *id--读取的数据存放地址
//返回:0--操作成功；1--总线不可用;2--校验错误；
//***************************************************
uint8 OWReadRom(uint8 *id)
{
    uint8 i;

    if (SingleWire_Reset() == false)
        return(1);

    OWWriteByte(READ_ROM);    //写命令

    for (i = 0;i < 8;i++)
    {
        *id++ = OWReadByte();
    }

    if (Get_CRC8(id - 8, 8) != 0) //校验
        return(2);

    return(0);
}

uint8 OWTouchByte(uint8 val)
{
    return 0;
}

//***************************************************
//指定ROM芯片;成功返回0
//参数:   *id--读取的数据存放地址
//返回:0--操作成功；1--总线不可用;2--ROM不符；
//***************************************************
uint8 OWMatchRom(uint8 *id)
{
    uint8 i, tmp;

    if (SingleWire_Reset() == false)
        return(1);

    OWWriteByte(MATCH_ROM);    //写命令

    for (i = 0;i < 8;i++)
    {
        tmp = OWTouchByte(id[i]);

        if (tmp != id[i])return(2);
    }

    return(0);
}

//***************************************************
//忽略ROM芯片;成功返回0
//参数:   无
//返回:0--操作成功；1--总线不可用;
//***************************************************
uint8 OWSkipRom(void)
{
    if (SingleWire_Reset() == false)
        return(1);

    OWWriteByte(SKIP_ROM);        //写命令

    return (0);
}


void SingleWire_Init(void)
{
//    SetSpeed(true);
    DSE_InOut(false);
    DQHIGH();
}



//= end   ================================================================

#endif //_BASE_SINGLEWIRE_C


