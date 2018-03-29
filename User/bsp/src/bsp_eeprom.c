//=========================================================================================================
//=========================================================================================================
#define  BSP_UART_MODULE
//#include <bsp.h>
#include <includes.h>


//===================================================================================================
//EPROM-24C01~24C2048 系列读写=======================================================================
//功能描述：读写串行EEPROM(适用24C01~24C2048)
//输入：MCU地址，EEP地址，读写字节数,24的控制字
//输出：错误标志，0--正确，1--错误
//版本说明：V1.1
//注:(1)写数据时不用考虑跨页问题(已有处理)
//   (2)控制字与24的硬件连接有关
//      例:24的1,2,3,4脚都与地连接
//      则:写24的控制字=0xa0,读24的控制字=0xa1
//----------------------------------------------

//根据使用的24确定
#define PAGESIZE 					128      		//页大小      24C02=8    24C512=128个字节
#define EEPALLBYTE 				0xffff	   		//总字节数    24C02=256  24C512=65536
#define Control         	0xa0
static  unsigned char 		EEP_flag;    			//0--运行正确,1--运行错误

//管脚定义在外部文件定义

 #define PORT_I2C            	GPIOB  
 #define GPIO_IIC_PORT          GPIOB  
 #define SDA             	GPIO_Pin_7
 #define SCL             	GPIO_Pin_6
 #define WP               GPIO_Pin_5


//static void eep_IO(void);
static void delay_nop(void);
static void eep_delay10ms(void);
void I2C_SDA_IO_SET(unsigned char io_set);
static void I2C_Start(void);
static void I2C_Stop(void);
static void I2C_RecAck(void);
static void I2C_SendAck(void);
static void I2C_NoAck(void);
static void I2C_wbyte(unsigned char wbyte);
static unsigned char I2C_rbyte(void);

//IO口配置
void bsp_EepromInit(void)
{  
  GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = SCL;          			//24C02 SCL
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(PORT_I2C, &GPIO_InitStructure); 

  GPIO_InitStructure.GPIO_Pin = SDA;          			//24C02 SDA 作为输出
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(PORT_I2C, &GPIO_InitStructure); 
	

	GPIO_InitStructure.GPIO_Pin = WP;          			//24C02 SDA 作为输出
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(PORT_I2C, &GPIO_InitStructure); 
	
#ifndef HT8201
	//使能特殊功能和禁止JTAG脚
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 	
	DBGMCU->CR = DBGMCU->CR & ~((uint32_t)1<<5);							//关闭TRACESWO信号跟踪
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);   //禁止JTAG脚
#endif
	
	I2C_WP_SET(1);

}

//为了兼容大多数24CXX，至少延时2uS 与晶振有关,定义成静态函数
static void delay_nop(void)     
{ 
   uint8_t i=10; //i=10延时1.5us//这里可以优化速度 ，经测试最低到5还能写入
   while(i--); 
}

//延时10mS 与晶振有关,定义成静态函数
static void eep_delay10ms(void)   
{ 
	unsigned int t1,t2;
	t1=1000;
	while(t1--)
	{
		t2=100;
		while(t2--);
	}
 	//OSDelayxmS(5);
}

//SDA管脚方向设置
void I2C_SDA_IO_SET(unsigned char io_set) //SDA引脚输入输出设置
{
  GPIO_InitTypeDef GPIO_InitStructure;
  if(io_set==0)
  {
  	GPIO_InitStructure.GPIO_Pin = SDA;          //24C02 SDA 作为输出
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO_Mode_Out_OD;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(PORT_I2C, &GPIO_InitStructure); 
  }
  else if(io_set==1)
  {
  	GPIO_InitStructure.GPIO_Pin = SDA;          //24C02 SDA 作为输入
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //上拉输入
  	GPIO_Init(PORT_I2C, &GPIO_InitStructure); 
  }
  else
  {;}
}
//设置是否允许写
void I2C_WP_SET(uint8_t io_set) //SDA引脚输入输出设置
{
  if(io_set!=0)
  {
		GPIO_SetBits(GPIO_IIC_PORT,WP); 
  }
  else
  {
		GPIO_ResetBits(GPIO_IIC_PORT,WP); 
  }
}

//启动
static void I2C_Start(void)   //启动总线
{ 
   //clf add 
   //GPIO_ResetBits(GPIO_IIC_PORT, SCL);
   //delay_nop();

   //SDA=1;
   GPIO_SetBits(GPIO_IIC_PORT,SDA);
   delay_nop();
   //SCL=1;
   GPIO_SetBits(GPIO_IIC_PORT,SCL);
   delay_nop();
   //SDA=0;
    GPIO_ResetBits(GPIO_IIC_PORT, SDA);
   delay_nop();
   //SCL=0;
   GPIO_ResetBits(GPIO_IIC_PORT, SCL);
   delay_nop();

}

//停止
static void I2C_Stop(void)    //停止总线
{  
   //SCL=0;
   GPIO_ResetBits(GPIO_IIC_PORT,SCL);
   delay_nop();
   //SDA=0;
   GPIO_ResetBits(GPIO_IIC_PORT, SDA);
   delay_nop();
   //SCL=1;
   GPIO_SetBits(GPIO_IIC_PORT,SCL);
   delay_nop();
   //SDA=1;
   GPIO_SetBits(GPIO_IIC_PORT,SDA);
   delay_nop();

   //SCL=0;
   //GPIO_ResetBits(GPIO_IIC_PORT,SCL);
   //delay_nop();

}

//主机写后，等待从机应答信号
static void I2C_RecAck(void)  
{
  //SCL=0;  
  GPIO_ResetBits(GPIO_IIC_PORT,SCL);    //主机拉低SCL
  delay_nop();  

  I2C_SDA_IO_SET(1);                    //设数据线为输入
  delay_nop();

  //SCL=1;
  delay_nop();                          //等待从机操作，0-成功1-失败
  
  GPIO_SetBits(GPIO_IIC_PORT,SCL);      //拉高SCL，禁止SDA变化
  delay_nop(); 

  //EEP_flag=SDA;
  EEP_flag=GPIO_ReadInputDataBit(GPIO_IIC_PORT,SDA);        //读SDA状态，此状态有从机决定

  //SCL=0;
  GPIO_ResetBits(GPIO_IIC_PORT,SCL);    //拉低SCL，允许主机写入SDA
  delay_nop();

  //SDAoutput();
  I2C_SDA_IO_SET(0);       //设数据线为输出
  delay_nop();
}


//发送应答信号，用于连续读 
static void I2C_SendAck(void)  
{
 	//SDA=0;
	GPIO_ResetBits(GPIO_IIC_PORT,SDA);
 	delay_nop();

 	//SCL=1;
	GPIO_SetBits(GPIO_IIC_PORT,SCL);
 	delay_nop();

 	//SCL=0;
	GPIO_ResetBits(GPIO_IIC_PORT,SCL);
 	delay_nop();
}
static void I2C_NoAck(void)    //不发送应答信号，用于停止读 
{
    	//SDA=1;
	GPIO_SetBits(GPIO_IIC_PORT,SDA);
 	delay_nop();
 	//SCL=1;
	GPIO_SetBits(GPIO_IIC_PORT,SCL);
 	delay_nop();
 	//SCL=0;
	GPIO_ResetBits(GPIO_IIC_PORT,SCL);
    	delay_nop();
}    
//写一字节到EEPROM
static void I2C_wbyte(unsigned char wbyte)      
{
	unsigned char i=8;
 	
	delay_nop();
	DISABLE_INT();//CPU_IntDis(); //关中断
 	for(;i>0;i--)
  	{
		//SCL=0;
	    GPIO_ResetBits(GPIO_IIC_PORT,SCL);

   		if(wbyte&0x80)
		{
			//SDA=1;
			GPIO_SetBits(GPIO_IIC_PORT,SDA);
		}
  		else
		{
			//SDA=0;
			GPIO_ResetBits(GPIO_IIC_PORT,SDA);
		}   
   		delay_nop();
   		//SCL=1;
		GPIO_SetBits(GPIO_IIC_PORT,SCL);
   		wbyte<<=1;
   		delay_nop();
  }
	ENABLE_INT();//CPU_IntEn();  //开中断
} 

//从EEPROM读1字节数据返回
static unsigned char I2C_rbyte(void) 
{
	unsigned char i=8;
 	unsigned char rbyte=0;
	unsigned char tmp;
    
 	//SDAinput();                  //设数据线为输入
	I2C_SDA_IO_SET(1); 

 	while(i--)
   	{
		//SCL=1;
		GPIO_SetBits(GPIO_IIC_PORT,SCL);
    	delay_nop();
    	//rbyte=(rbyte<<1)|SDA;
		tmp=GPIO_ReadInputDataBit(GPIO_IIC_PORT,SDA);
		if(tmp!=0)tmp=1;
		rbyte=(rbyte<<1)|tmp;

    	//SCL=0;
		GPIO_ResetBits(GPIO_IIC_PORT,SCL);

    	delay_nop();
    }
    //SDAoutput();
   	I2C_SDA_IO_SET(0); 
 
    return(rbyte);
}



//读写n字节数据
//数据地址由p_data确定,EEPROM地址由eep_addr确定 
//数据数量由n确定,读写由Control确定

//读多字节
u8 WriteDataToFlash(uint16_t eep_addr,uint8_t* p_data,uint16_t n)
{
	 unsigned int j=3;//,i=0,;
  
  	while(1)   
    {
     	I2C_Start();
     	I2C_wbyte(Control&0xfe);
     	I2C_RecAck();
     	if(EEP_flag)
		{
        	break;
		}

     	if(EEPALLBYTE>256)          		 	//有16位地址吗?
        {
			I2C_wbyte(eep_addr/256);  			//16位地址
        	I2C_RecAck();
        	if(EEP_flag)
			{
           		break;
			}
        }

     	I2C_wbyte(eep_addr);          			//8位地址
     	I2C_RecAck();
     	if(EEP_flag)
		{
        	break;
		}

		while(n>0)
        {
           	I2C_wbyte(*p_data);
           	I2C_RecAck();
			n--;
           	if(EEP_flag)
			{
              break;
			}
           	p_data++;
           	eep_addr++;
           	if((eep_addr%PAGESIZE==0))   //跨页
            {
				I2C_Stop();
              	eep_delay10ms();
              	break;
            }
         }

        if(n==0)
        {
			I2C_Stop();
           	eep_delay10ms();
           	//break;
			return(EEP_flag);
        }
        else if(EEP_flag)
        {
			if(j--==0)      //允许重试3次
			{
             	//break;
				return(EEP_flag);
			}
        }
    }
 return(EEP_flag);

}


//bit RW24CXX(unsigned char *p_data,unsigned int eep_addr,
//            unsigned int n,unsigned char Control)
u8 ReadDataFromFlash(uint16_t eep_addr,uint8_t* p_data,uint16_t n)
{ 
	//unsigned int j=3;//,i=0;
	 
 	 while(1)   //
    {
     	I2C_Start();
     	I2C_wbyte(Control&0xfe);
     	I2C_RecAck();
     	if(EEP_flag)
		{
        	break;
		}
     	if(EEPALLBYTE>256)           //有16位地址吗?
       	{
			I2C_wbyte(eep_addr/256);  //16位地址
        	I2C_RecAck();
        	if(EEP_flag)
			{
           		break;
			}
        }

     	I2C_wbyte(eep_addr);          //8位地址
     	I2C_RecAck();
     	if(EEP_flag)
		{
        	break;
		}

       //读-----------
	    I2C_Start();
        I2C_wbyte(Control|0X01);	 //0XA1读，0xa0写

        I2C_RecAck();
        if(EEP_flag)
		{
           break;
		}
        while(n>0)
        {
			*p_data++=I2C_rbyte();
			n--;
           	if(n>0)
			{
              I2C_SendAck();
			}
           	else
            {
				I2C_NoAck();
              	I2C_Stop();
                 break;
            }
          }

		//==========================================
		if(n==0)
        {
           	//break;
			 return(EEP_flag);
        }
       }
 return(EEP_flag);
}


//用数据FData填充从eep_addr开始的区域
u8 FillDataToFlash(uint8_t FData,uint16_t eep_addr,uint16_t n)//,unsigned char Control)
{
	unsigned char j=3;
 	while(1)   //
    {
     I2C_Start();
     I2C_wbyte(Control&0xfe); 				//0xa0写，0xa1读
     I2C_RecAck();
     if(EEP_flag)
	 {
        break;
	 }
     if(EEPALLBYTE>256)           			//有16位地址吗?
     {
	 	I2C_wbyte(eep_addr/256);  			//16位地址
        I2C_RecAck();
        if(EEP_flag)
		{
           break;
		}
      }

     I2C_wbyte(eep_addr);          			//8位地址
     I2C_RecAck();
     if(EEP_flag)
	 {
        break;
	 }

     while(n>0)
     {
	    
        I2C_wbyte(FData);
        I2C_RecAck();

		n--;

        if(EEP_flag)
		{
           break;
		}
        eep_addr++;

        if((eep_addr%PAGESIZE==0))   //跨页
        {
			I2C_Stop();
           	eep_delay10ms();
           	break;
         }
      }

     if(n==0)
     {
	 		I2C_Stop();
        	eep_delay10ms();
        	//break;
			return(EEP_flag);
     }
     else if(EEP_flag)
     {
	 		if(j--==0)      //允许重试3次
           	//break;
			return(EEP_flag);
     }
    }
 return(EEP_flag);
}



//先把原来的数据搬走(用copy),再插入新数据
u8 InsertDataToFlash(uint8_t *p_data,uint16_t eep_addr,uint16_t n)//,unsigned char Control)
{
  	return 0;
}
//先删除,再把数据搬过来(用copy)
u8 DeleteDataToFlash(uint16_t eep_addr,uint16_t n)//,unsigned char Control)
{
	FillDataToFlash(0xff,eep_addr,n);//,Control);

	return 1;
}

//复制
unsigned char CopyDataInFlash(uint16_t SSAddres,uint16_t DSAddres,uint16_t n)
{
	unsigned char buff[8];  //每次复制的数量由数组决定
 	unsigned char i=8;
 	while(n>0)
    {
		if(n>=8)
      	{
			i=8;
       		n-=8;
      	}
    else
    {
		i=n;
       	n=0;
    }

    //RW24CXX(buffer,SSAddres,i,Control|0x01);
	ReadDataFromFlash(SSAddres,buff,i);
    //RW24CXX(buffer,DSAddres,i,Control&0xfe);
	I2C_WP_SET(0);
	WriteDataToFlash(DSAddres,buff,i);
	I2C_WP_SET(1);	

    if(EEP_flag)
	{
       break;
	}
    SSAddres+=8;
    DSAddres+=8;
   }
 return(EEP_flag);
}


//=========================================================================================================
//数据存贮，有原先的FLASH改为EEPROM========================================================================

//测试程序
void TestWRFlash(void)
{  
	 static u8 buf[150],buf1[150];

	 u8 i;//,j;

	 for(i=0;i<150;i++)
	 {
	 	buf[i]=0xcf;
		buf1[i]=0x35;
	 }

	 I2C_WP_SET(0);
	 WriteDataToFlash(0x80,buf,150);
   I2C_WP_SET(1);
	 ReadDataFromFlash(0x80,buf1,150);

} 
//清除所有FLASH
void EraseAllFlash(void)
{

}

//清除单个FLASH扇区，每区设定为4K ,根据具体应用修改
void EraseOneFlash(uint8_t i)
{
       unsigned int tmp;
	   if(i<16)
	   {
			tmp=1024*4*i;
			FillDataToFlash(0xff,tmp,1024*4-1);
	   }
}







//==========================================================================================================
//=====================================以下程序不使用=======================================================
#if 0
////==========================================================================================================
////程序区分60K,EPROM4K,两套协议共用同一个存贮区，当协议切换时存贮区全部清零
//
///* Private typedef -----------------------------------------------------------*/
//typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
//
///* Private define ------------------------------------------------------------*/
//#define StartAddr  ((u32)0x0800e000)
//#define EndAddr    ((u32)0x0800ffff)
//u32 *p=(u32 *)0x0800f000;
//
////u32 *p,pt=0;
//
//#define PageNum  4
//
//volatile FLASH_Status FLASHStatus;							//读写状态，在 库函数stm32f10x_flash.h中实现
//
//
//
//
////=============================================================================================================
////最多一次能写64个字节，8个字，如需增加，修改buf缓存
//void WriteDataToFlash(u16 addr,u8* pDat,u16 nbyte)
//{
//      u32 adr;
//      u16 cnt,offset,t1;						
//	  u8 tmp,tmp1,i;
//	  
//	  typedef union{
//         u32 buf1[16];
//         u8 buf2[64];
//      }T_U;
//      T_U rt;  
//	  
//	  //p=&pt;
//
//	  offset=addr&0xfffc;				//如果写入的字节不对齐，则先强制对齐
//	  tmp=addr%4;
//	  t1=nbyte+tmp;
//	  cnt=t1/4;
//	  tmp1=t1%4;if(tmp1!=0)cnt=cnt+1;
//
//	  //p=StartAddr+(u32)offset;
//	  p=p+offset;
//
//	  //先保存数据
//	  i=0;
//	  while(i<cnt) 
//  	  {
//	 	  rt.buf1[i]=*p;p++;i++;
//      }
//
//	  //填充数据
//	  for(i=0;i<nbyte;i++)
//	  {
//		   rt.buf2[i+tmp]=*pDat++;
//	  }
//	  //重新写入数据
//
//	  FLASH_Unlock();
//  	  FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
//	
//	  i=0;
//	  adr=StartAddr+ offset;
//	  while((i < cnt) && (FLASHStatus == FLASH_COMPLETE))
//  	  {
//    		FLASHStatus = FLASH_ProgramWord(adr, rt.buf1[i]);
//    		adr = adr + 4;
//			i++;
//  	  }
//	  FLASH_Lock();
//}
//
////==========================================================================================================
////最多能读64个字节，16个字
//void ReadDataFromFlash(u16 addr,u8* pDat,u16 nbyte)
//{
//      u16 cnt,offset,t1;						
//	  u8 tmp,tmp1,i;
//	  
//	  typedef union{
//         u32 buf1[16];
//         u8 buf2[64];
//      }T_U;
//      T_U rt;  
//
//	  //p=&pt;
//	  offset=addr&0xffffc;				//如果读出的字节不对齐，则先强制对齐
//	  tmp=addr%4;
//	  t1=nbyte+tmp;
//	  cnt=t1/4;
//	  tmp1=t1%4;if(tmp1!=0)cnt=cnt+1;
//
//	  //p=StartAddr+(u32)offset;
//	  p=p+offset;
//
//	  //先保存数据
//	  i=0;
//	  while(i<cnt) 
//  	  {
//	 	  rt.buf1[i]=*p;p++;i++;
//      }
//	  pDat=&rt.buf2[0];
//}
//
////===========================================================================================================
////清除所有flash存贮区
//#define  FLASH_PAGE_SIZE 1024
//void EraseAllFlash(void)
//{
//	u8 i;
//	for(i= 0; (i <= PageNum) && (FLASHStatus == FLASH_COMPLETE); i++)
//    {
//      FLASHStatus = FLASH_ErasePage(StartAddr + (FLASH_PAGE_SIZE * i));
//    }
//}
////=============================================================================================================
////清除指定扇区存贮区(1k)
//void EraseOneFlash(unsigned char i)
//{
//	if(i>3)return ;
//
//	FLASHStatus = FLASH_ErasePage(StartAddr + (FLASH_PAGE_SIZE * i));
//}

#endif
//===========================================================================================================
