//=========================================================================================================
//=========================================================================================================
#define  BSP_UART_MODULE
//#include <bsp.h>
#include <includes.h>


//===================================================================================================
//EPROM-24C01~24C2048 ϵ�ж�д=======================================================================
//������������д����EEPROM(����24C01~24C2048)
//���룺MCU��ַ��EEP��ַ����д�ֽ���,24�Ŀ�����
//����������־��0--��ȷ��1--����
//�汾˵����V1.1
//ע:(1)д����ʱ���ÿ��ǿ�ҳ����(���д���)
//   (2)��������24��Ӳ�������й�
//      ��:24��1,2,3,4�Ŷ��������
//      ��:д24�Ŀ�����=0xa0,��24�Ŀ�����=0xa1
//----------------------------------------------

//����ʹ�õ�24ȷ��
#define PAGESIZE 					128      		//ҳ��С      24C02=8    24C512=128���ֽ�
#define EEPALLBYTE 				0xffff	   		//���ֽ���    24C02=256  24C512=65536
#define Control         	0xa0
static  unsigned char 		EEP_flag;    			//0--������ȷ,1--���д���

//�ܽŶ������ⲿ�ļ�����

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

//IO������
void bsp_EepromInit(void)
{  
  GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = SCL;          			//24C02 SCL
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(PORT_I2C, &GPIO_InitStructure); 

  GPIO_InitStructure.GPIO_Pin = SDA;          			//24C02 SDA ��Ϊ���
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(PORT_I2C, &GPIO_InitStructure); 
	

	GPIO_InitStructure.GPIO_Pin = WP;          			//24C02 SDA ��Ϊ���
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(PORT_I2C, &GPIO_InitStructure); 
	
#ifndef HT8201
	//ʹ�����⹦�ܺͽ�ֹJTAG��
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 	
	DBGMCU->CR = DBGMCU->CR & ~((uint32_t)1<<5);							//�ر�TRACESWO�źŸ���
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);   //��ֹJTAG��
#endif
	
	I2C_WP_SET(1);

}

//Ϊ�˼��ݴ����24CXX��������ʱ2uS �뾧���й�,����ɾ�̬����
static void delay_nop(void)     
{ 
   uint8_t i=10; //i=10��ʱ1.5us//��������Ż��ٶ� ����������͵�5����д��
   while(i--); 
}

//��ʱ10mS �뾧���й�,����ɾ�̬����
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

//SDA�ܽŷ�������
void I2C_SDA_IO_SET(unsigned char io_set) //SDA���������������
{
  GPIO_InitTypeDef GPIO_InitStructure;
  if(io_set==0)
  {
  	GPIO_InitStructure.GPIO_Pin = SDA;          //24C02 SDA ��Ϊ���
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO_Mode_Out_OD;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(PORT_I2C, &GPIO_InitStructure); 
  }
  else if(io_set==1)
  {
  	GPIO_InitStructure.GPIO_Pin = SDA;          //24C02 SDA ��Ϊ����
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //��������
  	GPIO_Init(PORT_I2C, &GPIO_InitStructure); 
  }
  else
  {;}
}
//�����Ƿ�����д
void I2C_WP_SET(uint8_t io_set) //SDA���������������
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

//����
static void I2C_Start(void)   //��������
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

//ֹͣ
static void I2C_Stop(void)    //ֹͣ����
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

//����д�󣬵ȴ��ӻ�Ӧ���ź�
static void I2C_RecAck(void)  
{
  //SCL=0;  
  GPIO_ResetBits(GPIO_IIC_PORT,SCL);    //��������SCL
  delay_nop();  

  I2C_SDA_IO_SET(1);                    //��������Ϊ����
  delay_nop();

  //SCL=1;
  delay_nop();                          //�ȴ��ӻ�������0-�ɹ�1-ʧ��
  
  GPIO_SetBits(GPIO_IIC_PORT,SCL);      //����SCL����ֹSDA�仯
  delay_nop(); 

  //EEP_flag=SDA;
  EEP_flag=GPIO_ReadInputDataBit(GPIO_IIC_PORT,SDA);        //��SDA״̬����״̬�дӻ�����

  //SCL=0;
  GPIO_ResetBits(GPIO_IIC_PORT,SCL);    //����SCL����������д��SDA
  delay_nop();

  //SDAoutput();
  I2C_SDA_IO_SET(0);       //��������Ϊ���
  delay_nop();
}


//����Ӧ���źţ����������� 
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
static void I2C_NoAck(void)    //������Ӧ���źţ�����ֹͣ�� 
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
//дһ�ֽڵ�EEPROM
static void I2C_wbyte(unsigned char wbyte)      
{
	unsigned char i=8;
 	
	delay_nop();
	DISABLE_INT();//CPU_IntDis(); //���ж�
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
	ENABLE_INT();//CPU_IntEn();  //���ж�
} 

//��EEPROM��1�ֽ����ݷ���
static unsigned char I2C_rbyte(void) 
{
	unsigned char i=8;
 	unsigned char rbyte=0;
	unsigned char tmp;
    
 	//SDAinput();                  //��������Ϊ����
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



//��дn�ֽ�����
//���ݵ�ַ��p_dataȷ��,EEPROM��ַ��eep_addrȷ�� 
//����������nȷ��,��д��Controlȷ��

//�����ֽ�
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

     	if(EEPALLBYTE>256)          		 	//��16λ��ַ��?
        {
			I2C_wbyte(eep_addr/256);  			//16λ��ַ
        	I2C_RecAck();
        	if(EEP_flag)
			{
           		break;
			}
        }

     	I2C_wbyte(eep_addr);          			//8λ��ַ
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
           	if((eep_addr%PAGESIZE==0))   //��ҳ
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
			if(j--==0)      //��������3��
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
     	if(EEPALLBYTE>256)           //��16λ��ַ��?
       	{
			I2C_wbyte(eep_addr/256);  //16λ��ַ
        	I2C_RecAck();
        	if(EEP_flag)
			{
           		break;
			}
        }

     	I2C_wbyte(eep_addr);          //8λ��ַ
     	I2C_RecAck();
     	if(EEP_flag)
		{
        	break;
		}

       //��-----------
	    I2C_Start();
        I2C_wbyte(Control|0X01);	 //0XA1����0xa0д

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


//������FData����eep_addr��ʼ������
u8 FillDataToFlash(uint8_t FData,uint16_t eep_addr,uint16_t n)//,unsigned char Control)
{
	unsigned char j=3;
 	while(1)   //
    {
     I2C_Start();
     I2C_wbyte(Control&0xfe); 				//0xa0д��0xa1��
     I2C_RecAck();
     if(EEP_flag)
	 {
        break;
	 }
     if(EEPALLBYTE>256)           			//��16λ��ַ��?
     {
	 	I2C_wbyte(eep_addr/256);  			//16λ��ַ
        I2C_RecAck();
        if(EEP_flag)
		{
           break;
		}
      }

     I2C_wbyte(eep_addr);          			//8λ��ַ
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

        if((eep_addr%PAGESIZE==0))   //��ҳ
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
	 		if(j--==0)      //��������3��
           	//break;
			return(EEP_flag);
     }
    }
 return(EEP_flag);
}



//�Ȱ�ԭ�������ݰ���(��copy),�ٲ���������
u8 InsertDataToFlash(uint8_t *p_data,uint16_t eep_addr,uint16_t n)//,unsigned char Control)
{
  	return 0;
}
//��ɾ��,�ٰ����ݰ����(��copy)
u8 DeleteDataToFlash(uint16_t eep_addr,uint16_t n)//,unsigned char Control)
{
	FillDataToFlash(0xff,eep_addr,n);//,Control);

	return 1;
}

//����
unsigned char CopyDataInFlash(uint16_t SSAddres,uint16_t DSAddres,uint16_t n)
{
	unsigned char buff[8];  //ÿ�θ��Ƶ��������������
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
//���ݴ�������ԭ�ȵ�FLASH��ΪEEPROM========================================================================

//���Գ���
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
//�������FLASH
void EraseAllFlash(void)
{

}

//�������FLASH������ÿ���趨Ϊ4K ,���ݾ���Ӧ���޸�
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
//=====================================���³���ʹ��=======================================================
#if 0
////==========================================================================================================
////��������60K,EPROM4K,����Э�鹲��ͬһ������������Э���л�ʱ������ȫ������
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
//volatile FLASH_Status FLASHStatus;							//��д״̬���� �⺯��stm32f10x_flash.h��ʵ��
//
//
//
//
////=============================================================================================================
////���һ����д64���ֽڣ�8���֣��������ӣ��޸�buf����
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
//	  offset=addr&0xfffc;				//���д����ֽڲ����룬����ǿ�ƶ���
//	  tmp=addr%4;
//	  t1=nbyte+tmp;
//	  cnt=t1/4;
//	  tmp1=t1%4;if(tmp1!=0)cnt=cnt+1;
//
//	  //p=StartAddr+(u32)offset;
//	  p=p+offset;
//
//	  //�ȱ�������
//	  i=0;
//	  while(i<cnt) 
//  	  {
//	 	  rt.buf1[i]=*p;p++;i++;
//      }
//
//	  //�������
//	  for(i=0;i<nbyte;i++)
//	  {
//		   rt.buf2[i+tmp]=*pDat++;
//	  }
//	  //����д������
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
////����ܶ�64���ֽڣ�16����
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
//	  offset=addr&0xffffc;				//����������ֽڲ����룬����ǿ�ƶ���
//	  tmp=addr%4;
//	  t1=nbyte+tmp;
//	  cnt=t1/4;
//	  tmp1=t1%4;if(tmp1!=0)cnt=cnt+1;
//
//	  //p=StartAddr+(u32)offset;
//	  p=p+offset;
//
//	  //�ȱ�������
//	  i=0;
//	  while(i<cnt) 
//  	  {
//	 	  rt.buf1[i]=*p;p++;i++;
//      }
//	  pDat=&rt.buf2[0];
//}
//
////===========================================================================================================
////�������flash������
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
////���ָ������������(1k)
//void EraseOneFlash(unsigned char i)
//{
//	if(i>3)return ;
//
//	FLASHStatus = FLASH_ErasePage(StartAddr + (FLASH_PAGE_SIZE * i));
//}

#endif
//===========================================================================================================
