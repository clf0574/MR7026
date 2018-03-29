


#include "bsp.h"

// ���忪����
#define ON						1
#define OFF						0

#define  NOP()	__nop()


//ָ��
#define WRITE_BURST     		0x40//����д��
#define READ_SINGLE     		0x80//��
#define READ_BURST      		0xC0//������
#define BYTES_IN_RXFIFO         0x7F//���ջ���������Ч�ֽ���
#define CRC_OK                  0x80//CRCУ��ͨ��λ��־

//��С֡����
//#define MIN_LENGTH               16

//#define MaxRxCnt    10

//unsigned char RxBuf[MaxRxCnt][MIN_LENGTH];
//unsigned char RxBuf[MIN_LENGTH];
//unsigned char RxCnt;			//�������

void InitSPI(void);
unsigned char SpiReadStatus(unsigned char addr);
unsigned char SPITxRxByte(unsigned char xdata);
void ResetCC1100(void);
void PowerUpResetCC1100(void);
void SPIWriteReg(unsigned char addr, unsigned char value) ;
void SPIWriteBurstReg(unsigned char addr, unsigned char *buffer, unsigned char count) ;
void SPIStrobe(unsigned char strobe) ;
unsigned char SPIReadReg(unsigned char addr) ;
void SPIReadBurstReg(unsigned char addr, unsigned char *buffer, unsigned char count) ;
void RfWriteRfSettings(void);

//void RfSendPacket(INT8U *txBuffer, INT8U size) ;

void SetRxMode(void);
//unsigned char RfReceivePacket(unsigned char *rxBuffer, unsigned char length) ;
//unsigned char RfReceivePacket(void); 

void Delay2us(void);
void Delayx10us(uint8_t n);



GPIO_InitTypeDef GPIO_InitStructure;

//***************���๦�ʲ������ÿ���ϸ�ο�DATACC1100Ӣ���ĵ��е�48-49ҳ�Ĳ�����******************
//INT8U PaTabel[8] = {0x04 ,0x04 ,0x04 ,0x04 ,0x04 ,0x04 ,0x04 ,0x04};  //-30dBm   ������С
//INT8U PaTabel[8] = {0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60};  //0dBm
//unsigned char PaTabel[8] = {0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0};   //10dBm     �������
unsigned char PaTabel[8] = {0xC3 ,0xC3 ,0xC3 ,0xC3 ,0xC3 ,0xC3 ,0xC3 ,0xC3};   //10dBm     �������
//********************************************

//------------------------------------------------------------------------------------------------------
// Chipcon
// Product = CC1100
// Chip version = E
// Crystal accuracy = 40 ppm
// X-tal frequency = 26 MHz
// RF output power = 0 dBm
// RX filterbandwidth = 540.000000 kHz
// Deviation = 0.000000 
// Datarate = 250.000000 kbps
// Modulation = (7) MSK
// Manchester enable = (0) Manchester disabled
// RF Frequency = 433.000000 MHz
// Channel spacing = 199.951172 kHz
// Channel number = 0
// Optimization = Sensitivity
// Sync mode = (3) 30/32 sync word bits detected
// Format of RX/TX data = (0) Normal mode, use FIFOs for RX and TX
// CRC operation = (1) CRC calculation in TX and CRC check in RX enabled
// Forward Error Correction = (0) FEC disabled
// Length configuration = (1) Variable length packets, packet length configured by the first received byte after sync word.
// Packetlength = 255
// Preamble count = (2)  4 bytes
// Append status = 1
// Address check = (0) No address check
// FIFO autoflush = 0
// Device address = 0
// GDO0 signal selection = ( 6) Asserts when sync word has been sent / received, and de-asserts at the end of the packet
// GDO2 signal selection = (11) Serial Clock


/*  clf del 2014.3.30
RF_SETTINGS rfSettings = {
	0x00,
    0x06,//0x0B,   // FSCTRL1   Frequency synthesizer control.
    0x00,   // FSCTRL0   Frequency synthesizer control.
    0x10,   // FREQ2     Frequency control word, high byte.
    0xA7,//0xAA,//,   // FREQ1     Frequency control word, middle byte.
    0x62,//0x55,//   // FREQ0     Frequency control word, low byte.
    0xc8,//0x8A,   //2d MDMCFG4   Modem configuration.
    0x93,//0x01,   //3B MDMCFG3   Modem configuration.
    0x1b,//0x73,   // MDMCFG2   Modem configuration.
    0x22,//0xA2,   // 22  MDMCFG1   Modem configuration.
    0xF8,   // MDMCFG0   Modem configuration.

    0x00,   // CHANNR    Channel number.
    0x34,//0x00,   // DEVIATN   Modem deviation setting (when FSK modulation is enabled).
    0x56,//0xB6,   // FREND1    Front end RX configuration.
    0x17,   //10 FREND0    Front end RX configuration.
    
    //0x3e,	//MCSM1��
    0x18,   // MCSM0     Main Radio Control State Machine configuration.
    0x16,//0x1D,   // FOCCFG    Frequency Offset Compensation Configuration.
    0x6c,//0x1C,   // BSCFG     Bit synchronization Configuration.
    0x43,//0xC7,   // AGCCTRL2  AGC control.
    0x40,//0x00,   // AGCCTRL1  AGC control.
    0x91,//0xB2,   // AGCCTRL0  AGC control.

    0xA9,//0xEA,   // FSCAL3    Frequency synthesizer calibration.
    0x2A,//0x0A,   // FSCAL2    Frequency synthesizer calibration.
    0x00,   // FSCAL1    Frequency synthesizer calibration.
    0x11,   // FSCAL0    Frequency synthesizer calibration.
    0x59,   // FSTEST    Frequency synthesizer calibration.
    0x81,//0x88,   // TEST2     Various test settings.
    0x35,//0x31,   // TEST1     Various test settings.
    0x0B,   // TEST0     Various test settings.
    0x0B,   // IOCFG2    GDO2 output pin configuration.
    0x06,   // IOCFG0D   GDO0 output pin configuration. Refer to SmartRF?Studio User Manual for detailed pseudo register explanation.

    0x04,   // PKTCTRL1  Packet automation control.
    0x45,   // 05   PKTCTRL0  Packet automation control.
    0x00,   // ADDR      Device address.
    0xff    // PKTLEN    Packet length.
};
*/

/*
RF_SETTINGS rfSettings = {
	0x00,
    0x06,//0x0B,   // FSCTRL1   Frequency synthesizer control.
    0x00,   // FSCTRL0   Frequency synthesizer control.
    0x10,   // FREQ2     Frequency control word, high byte.
    0xA7,//0xAA,//,   // FREQ1     Frequency control word, middle byte.
    0x62,//0x55,//   // FREQ0     Frequency control word, low byte.
    0xc8,//0x8A,   //2d MDMCFG4   Modem configuration.
    0x93,//0x01,   //3B MDMCFG3   Modem configuration.
    0x1b,//0x73,   // MDMCFG2   Modem configuration.
    0x22,//0xA2,   // 22  MDMCFG1   Modem configuration.
    0xF8,   // MDMCFG0   Modem configuration.

    0x00,   // CHANNR    Channel number.
    0x34,//0x00,   // DEVIATN   Modem deviation setting (when FSK modulation is enabled).
    0x56,//0xB6,   // FREND1    Front end RX configuration.
    0x17,   //10 FREND0    Front end RX configuration.
    0x18,   // MCSM0     Main Radio Control State Machine configuration.
    0x16,//0x1D,   // FOCCFG    Frequency Offset Compensation Configuration.
    0x6c,//0x1C,   // BSCFG     Bit synchronization Configuration.
    0x43,//0xC7,   // AGCCTRL2  AGC control.
    0x40,//0x00,   // AGCCTRL1  AGC control.
    0x91,//0xB2,   // AGCCTRL0  AGC control.

    0xA9,//0xEA,   // FSCAL3    Frequency synthesizer calibration.
    0x2A,//0x0A,   // FSCAL2    Frequency synthesizer calibration.
    0x00,   // FSCAL1    Frequency synthesizer calibration.
    0x11,   // FSCAL0    Frequency synthesizer calibration.
    0x59,   // FSTEST    Frequency synthesizer calibration.
    0x81,//0x88,   // TEST2     Various test settings.
    0x35,//0x31,   // TEST1     Various test settings.
    0x0B,   // TEST0     Various test settings.
    0x0B,   // IOCFG2    GDO2 output pin configuration.
    //0x06,   //����0x07 IOCFG0D   GDO0 output pin configuration. Refer to SmartRF?Studio User Manual for detailed pseudo register explanation.
	0x07,//����0x07 IOCFG0D   GDO0 output pin configuration. Refer to SmartRF?Stu
	
    0x04,   // PKTCTRL1  Packet automation control.
    0x45,   // 05   PKTCTRL0  Packet automation control.
    0x00,   // ADDR      Device address.
    0xff    // PKTLEN    Packet length.
};
*/
//**********************************************
//��������void RESET_CC1100(void)
//���룺��
//�������
//������������λCC1100
//**************************************************
void ResetCC1100(void)
{
    unsigned int i=0;

	//CCS = 0;      // CSn �� ��оƬѡ�񣬵͵�ƽ��Ч��
	GPIO_ResetBits(GPIO_CCS_PORT,GPIO_CCS_PIN);
					
	//while(CMISO)//�� CSn ��ͣ� �ڿ�ʼת��ͷ�ֽ�֮ǰ��
	while(GPIO_ReadInputDataBit(GPIO_CMISO_PORT,GPIO_CMISO_PIN))
	{
		i++;
		if(i>20000)
		  break;
	}
			      //MCU����ȴ���ֱ�� SO�ű��			
    SPITxRxByte(CCxxx0_SRES); 	//д�븴λ����
	i=0;
	//while(CMISO)
	while(GPIO_ReadInputDataBit(GPIO_CMISO_PORT,GPIO_CMISO_PIN))
	{
		i++;
		if(i>20000)
		  break;
	}
	 
    //CCS = 1; 	//ת������	
	GPIO_SetBits(GPIO_CCS_PORT,GPIO_CCS_PIN);
}
//************************************************
//��������void POWER_UP_RESET_CC1100(void) 
//���룺��
//�������
//�����������ϵ縴λCC1100
//**********************************************
//ʱ���CC1100Ӣ�İ�43ҳ
void PowerUpResetCC1100(void) 
{
	//CSCLK = 1;		//to avoid potential problems 
	GPIO_SetBits(GPIO_CSCLK_PORT,GPIO_CSCLK_PIN);
						
	//CMISO = 0;		//with pin control mode
    GPIO_ResetBits(GPIO_CMISO_PORT,GPIO_CMISO_PIN);

	//CCS = 1; 
	GPIO_SetBits(GPIO_CCS_PORT,GPIO_CCS_PIN);

	//Delay2us();
	Delayx10us(1);
	 
	//CCS = 0; 
	 GPIO_ResetBits(GPIO_CCS_PORT,GPIO_CCS_PIN);

	//Delay2us();
	Delayx10us(1);

	//CCS = 1;
	GPIO_SetBits(GPIO_CCS_PORT,GPIO_CCS_PIN); 

	Delayx10us(5);	//Hold CSn high for at least 40��s  
					//relative to pulling CSn low 
	ResetCC1100();  //��λCC1100
}

unsigned char SPITxRxByte(unsigned char xdata)
{
	INT8U i,temp;
	temp = 0;
	
	//CSCLK = 0;
	GPIO_ResetBits(GPIO_CSCLK_PORT,GPIO_CSCLK_PIN);

	for(i=0; i<8; i++)
	{
		if(xdata & 0x80)  //��xdataд��
		{
			//CMOSI = 1;
			GPIO_SetBits(GPIO_CMOSI_PORT,GPIO_CMOSI_PIN);
		}
		else 
		{
			//CMOSI = 0;
			GPIO_ResetBits(GPIO_CMOSI_PORT,GPIO_CMOSI_PIN);
		}
		xdata <<= 1;

		//CSCLK = 1; 
		GPIO_SetBits(GPIO_CSCLK_PORT,GPIO_CSCLK_PIN);

		NOP();
    	NOP();
		NOP();
		NOP();
    	    	
		temp <<= 1;
		//if(CMISO)temp++;    //����temp
		if(GPIO_ReadInputDataBit(GPIO_CMISO_PORT,GPIO_CMISO_PIN))temp++;

		//CSCLK = 0;
		GPIO_ResetBits(GPIO_CSCLK_PORT,GPIO_CSCLK_PIN);

		NOP();
		NOP();
		NOP();
		NOP();
		
	}
	return temp;	
}

//*****************************************************************************************
//��������void SPIWriteReg(INT8U addr, INT8U value)
//���룺��ַ��������
//�������
//����������SPIд�Ĵ���
//*****************************************************************************************
void SPIWriteReg(unsigned char addr, unsigned char value) 
{
    unsigned int i=0;
    //CCS = 0;
	GPIO_ResetBits(GPIO_CCS_PORT,GPIO_CCS_PIN);
    //while (CMISO)
	while(GPIO_ReadInputDataBit(GPIO_CMISO_PORT,GPIO_CMISO_PIN))
	{
		i++;
		if(i>20000)
		  break;
	}
    SPITxRxByte(addr);		//д��ַ
    SPITxRxByte(value);		//д������
    //CCS = 1;
	GPIO_SetBits(GPIO_CCS_PORT,GPIO_CCS_PIN); 
}

//*****************************************************************************************
//��������void SPIWriteBurstReg(INT8U addr, INT8U *buffer, INT8U count)
//���룺��ַ��д�뻺������д�����
//�������
//����������SPI����д���üĴ���
//*****************************************************************************************
void SPIWriteBurstReg(unsigned char addr, unsigned char *buffer, unsigned char count) 
{
    unsigned int i=0;
    unsigned char temp;
	temp = addr | WRITE_BURST;
    //CCS = 0;
	GPIO_ResetBits(GPIO_CCS_PORT,GPIO_CCS_PIN);
	i=0;
    //while (CMISO)
	while(GPIO_ReadInputDataBit(GPIO_CMISO_PORT,GPIO_CMISO_PIN))
	{
		i++;
		if(i>20000)
		  break;
	}
    SPITxRxByte(temp);
    for(i = 0; i < count; i++)
 	{
        SPITxRxByte(buffer[i]);
    }
    //CCS = 1;
	GPIO_SetBits(GPIO_CCS_PORT,GPIO_CCS_PIN); 
}

//*****************************************************************************************
//��������void SpiStrobe(INT8U strobe)
//���룺����
//�������
//����������SPIд����
//*****************************************************************************************
void SPIStrobe(unsigned char strobe) 
{
    unsigned int i=0;

    //CCS = 0;
	GPIO_ResetBits(GPIO_CCS_PORT,GPIO_CCS_PIN);
    //while (CMISO)
	while(GPIO_ReadInputDataBit(GPIO_CMISO_PORT,GPIO_CMISO_PIN))
	{
		i++;
		if(i>20000)
		  break;
	}
    SPITxRxByte(strobe);		//д������
    //CCS = 1;
	GPIO_SetBits(GPIO_CCS_PORT,GPIO_CCS_PIN); 
    
	//CMOSI = 0;
	GPIO_ResetBits(GPIO_CMOSI_PORT,GPIO_CMOSI_PIN);
}

//*****************************************************************************************
//��������INT8U halSpiReadReg(INT8U addr)
//���룺��ַ
//������üĴ�����������
//����������SPI���Ĵ���
//*****************************************************************************************
unsigned char SPIReadReg(unsigned char addr) 
{
    unsigned int i=0;
	unsigned char temp, value;
    temp = addr | READ_SINGLE;//���Ĵ�������
	//CCS = 0;
	GPIO_ResetBits(GPIO_CCS_PORT,GPIO_CCS_PIN);
	//while (CMISO)
	while(GPIO_ReadInputDataBit(GPIO_CMISO_PORT,GPIO_CMISO_PIN))
	{
		i++;
		if(i>20000)
		  break;
	}
	SPITxRxByte(temp);  //���Ͷ�����������
	value = SPITxRxByte(0);
	//CCS = 1;
	GPIO_SetBits(GPIO_CCS_PORT,GPIO_CCS_PIN); 
	return value;
}

//*****************************************************************************************
//��������void halSpiReadBurstReg(INT8U addr, INT8U *buffer, INT8U count)
//���룺��ַ���������ݺ��ݴ�Ļ��������������ø���
//�������
//����������SPI����д���üĴ���
//*****************************************************************************************
void SPIReadBurstReg(unsigned char addr, unsigned char *buffer, unsigned char count) 
{
    unsigned int i=0;
    unsigned char temp;
	temp = addr | READ_BURST;		//д��Ҫ�������üĴ�����ַ�Ͷ�����
    //CCS = 0;
	GPIO_ResetBits(GPIO_CCS_PORT,GPIO_CCS_PIN);
    //while (CMISO)
	while(GPIO_ReadInputDataBit(GPIO_CMISO_PORT,GPIO_CMISO_PIN))
	{
		i++;
		if(i>20000)
		  break;
	}
	SPITxRxByte(temp);   
    for (i = 0; i < count; i++) 
	{
        buffer[i] = SPITxRxByte(0);
    }
    //CCS = 1;
	GPIO_SetBits(GPIO_CCS_PORT,GPIO_CCS_PIN);
}


//*****************************************************************************************
//��������INT8U halSpiReadReg(INT8U addr)
//���룺��ַ
//�������״̬�Ĵ�����ǰֵ
//����������SPI��״̬�Ĵ���
//*****************************************************************************************
unsigned char SPIReadStatus(unsigned char addr) 
{
    unsigned int j=0;
    unsigned char value,temp;
	temp = addr | READ_BURST;		//д��Ҫ����״̬�Ĵ����ĵ�ַͬʱд�������
    //CCS = 0;
	GPIO_ResetBits(GPIO_CCS_PORT,GPIO_CCS_PIN);
    //while (CMISO)
	while(GPIO_ReadInputDataBit(GPIO_CMISO_PORT,GPIO_CMISO_PIN))
	{
		j++;
		if(j>20000)
		  break;
	}
    SPITxRxByte(temp);
	value = SPITxRxByte(0);
	//CCS = 1;
	GPIO_SetBits(GPIO_CCS_PORT,GPIO_CCS_PIN);
	return value;
}

//*****************************************************************************************
//��������void halRfWriteRfSettings(RF_SETTINGS *pRfSettings)
//���룺��
//�������
//��������������CC1100�ļĴ���
//*****************************************************************************************

void RfWriteRfSettings(void) 
{
/*
	SPIWriteReg(CCxxx0_FSCTRL0,  rfSettings.FSCTRL2);//���Ѽӵ�
    // Write register settings
    SPIWriteReg(CCxxx0_FSCTRL1,  rfSettings.FSCTRL1);
    SPIWriteReg(CCxxx0_FSCTRL0,  rfSettings.FSCTRL0);
    SPIWriteReg(CCxxx0_FREQ2,    rfSettings.FREQ2);
    SPIWriteReg(CCxxx0_FREQ1,    rfSettings.FREQ1);
    SPIWriteReg(CCxxx0_FREQ0,    rfSettings.FREQ0);
    SPIWriteReg(CCxxx0_MDMCFG4,  rfSettings.MDMCFG4);
    SPIWriteReg(CCxxx0_MDMCFG3,  rfSettings.MDMCFG3);
    SPIWriteReg(CCxxx0_MDMCFG2,  rfSettings.MDMCFG2);
    SPIWriteReg(CCxxx0_MDMCFG1,  rfSettings.MDMCFG1);
    SPIWriteReg(CCxxx0_MDMCFG0,  rfSettings.MDMCFG0);
    SPIWriteReg(CCxxx0_CHANNR,   rfSettings.CHANNR);
    SPIWriteReg(CCxxx0_DEVIATN,  rfSettings.DEVIATN);
    SPIWriteReg(CCxxx0_FREND1,   rfSettings.FREND1);
    SPIWriteReg(CCxxx0_FREND0,   rfSettings.FREND0);
    //SPIWriteReg(CCxxx0_MCSM1,    rfSettings.MCSM1 );//��
    SPIWriteReg(CCxxx0_MCSM0 ,   rfSettings.MCSM0 );
    SPIWriteReg(CCxxx0_FOCCFG,   rfSettings.FOCCFG);
    SPIWriteReg(CCxxx0_BSCFG,    rfSettings.BSCFG);
    SPIWriteReg(CCxxx0_AGCCTRL2, rfSettings.AGCCTRL2);
	SPIWriteReg(CCxxx0_AGCCTRL1, rfSettings.AGCCTRL1);
    SPIWriteReg(CCxxx0_AGCCTRL0, rfSettings.AGCCTRL0);
    SPIWriteReg(CCxxx0_FSCAL3,   rfSettings.FSCAL3);
	SPIWriteReg(CCxxx0_FSCAL2,   rfSettings.FSCAL2);
	SPIWriteReg(CCxxx0_FSCAL1,   rfSettings.FSCAL1);
    SPIWriteReg(CCxxx0_FSCAL0,   rfSettings.FSCAL0);
    SPIWriteReg(CCxxx0_FSTEST,   rfSettings.FSTEST);
    SPIWriteReg(CCxxx0_TEST2,    rfSettings.TEST2);
    SPIWriteReg(CCxxx0_TEST1,    rfSettings.TEST1);
    SPIWriteReg(CCxxx0_TEST0,    rfSettings.TEST0);
    SPIWriteReg(CCxxx0_IOCFG2,   rfSettings.IOCFG2);
    SPIWriteReg(CCxxx0_IOCFG0,   rfSettings.IOCFG0);    
    SPIWriteReg(CCxxx0_PKTCTRL1, rfSettings.PKTCTRL1);
    SPIWriteReg(CCxxx0_PKTCTRL0, rfSettings.PKTCTRL0);
    SPIWriteReg(CCxxx0_ADDR,     rfSettings.ADDR);
    SPIWriteReg(CCxxx0_PKTLEN,   rfSettings.PKTLEN);
*/

	halRfWriteReg(IOCFG0,0x06);
	
	halRfWriteReg(PKTCTRL0,0x05);
	
	
	
	//halRfWriteReg(CHANNR,0x05);         //20170424������
	halRfWriteReg(CHANNR,0x01);
	
	
	halRfWriteReg(FSCTRL1,0x08);
	halRfWriteReg(FREQ2,0x21);
	halRfWriteReg(FREQ1,0x62);
	halRfWriteReg(FREQ0,0x76);
	halRfWriteReg(MDMCFG4,0xC7);
	halRfWriteReg(MDMCFG3,0x83);
	halRfWriteReg(MDMCFG2,0x93);
	halRfWriteReg(DEVIATN,0x40);
	halRfWriteReg(MCSM0,0x18);
	halRfWriteReg(FOCCFG,0x16);
	halRfWriteReg(AGCCTRL2,0x43);
	halRfWriteReg(WORCTRL,0xFB);
	halRfWriteReg(FSCAL3,0xE9);
	halRfWriteReg(FSCAL2,0x2A);
	halRfWriteReg(FSCAL1,0x00);
	halRfWriteReg(FSCAL0,0x1F);
	halRfWriteReg(TEST2,0x81);
	halRfWriteReg(TEST1,0x35);
	halRfWriteReg(TEST0,0x09);

	//SPIWriteReg(CCxxx0_IOCFG2,   0x0e);
	SPIWriteReg(CCxxx0_IOCFG2,   0x0b);
	SPIWriteReg(CCxxx0_FSCTRL0,  0x00);
	SPIWriteReg(CCxxx0_BSCFG,    0x6c);
	SPIWriteReg(CCxxx0_FSTEST,   0x59);
	SPIWriteReg(CCxxx0_PKTCTRL1, 0x04);
	//SPIWriteReg(CCxxx0_ADDR,     0x00);
    SPIWriteReg(CCxxx0_PKTLEN,   0xff); 
	halRfWriteReg(AGCCTRL1,0x41);
	halRfWriteReg(AGCCTRL0,0xb2);
}
//*****************************************************************************************
//��������void halRfSendPacket(INT8U *txBuffer, INT8U size)
//���룺���͵Ļ��������������ݸ���
//�������
//����������CC1100����һ������
//*****************************************************************************************
void RfSendPacket(INT8U *txBuffer, INT8U size) 
{
unsigned int i=0;
	//SPIStrobe(CCxxx0_SRX);	// �������״̬(CCA)	ADD
	SPIWriteReg(CCxxx0_TXFIFO, size);
    SPIWriteBurstReg(CCxxx0_TXFIFO, txBuffer, size);	//д��Ҫ���͵�����

 	SPIStrobe(CCxxx0_SIDLE);	//����֮ǰ�������,SUB
    SPIStrobe(CCxxx0_STX);		//���뷢��ģʽ��������

    // Wait for GDO0 to be set -> sync transmitted
	// Delayx10ms(10);
	i=0;
	//while (!CGDO0)
	//while(0==GPIO_ReadInputDataBit(GPIO_GDO0_PORT,GPIO_GDO0_PIN))
    while(!GPIO_ReadInputDataBit(GPIO_GDO0_PORT,GPIO_GDO0_PIN))
	{
	    Delay2us();
		if(i++>50000)break;
	}
    // Wait for GDO0 to be cleared -> end of packet
	i=0;
 	//while (CGDO0)
	//while(0!=GPIO_ReadInputDataBit(GPIO_GDO0_PORT,GPIO_GDO0_PIN))
    while(GPIO_ReadInputDataBit(GPIO_GDO0_PORT,GPIO_GDO0_PIN))
	{
	    Delay2us();
		if(i++>50000)break;
	}
	// Delayx10us(10);
	SPIStrobe(CCxxx0_SFTX);
	//SPIStrobe(CCxxx0_SFRX);		//��ϴ���ջ�����
    //SPIStrobe(CCxxx0_SRX);		//�������״̬
	//SPIStrobe(CCxxx0_SIDLE);//add
}

void SetRxMode(void)
{
    SPIStrobe(CCxxx0_SRX);		//�������״̬
}


//unsigned char RfReceivePacket(void) 
//{
//    unsigned char status[2];
//    unsigned char packetLength;
//	unsigned char i;  // �������Ҫ����datarate��length������
//	i=MIN_LENGTH*4;
//		while (CGDO0)
//		{
//			Delayx10us(2);
//			--i;
//			if(i<1)
//			    return 0;	    
//		}	
//    if((SPIReadStatus(CCxxx0_RXBYTES) & BYTES_IN_RXFIFO)) // ����ӵ��ֽ�����Ϊ0
//		{
//        packetLength = SPIReadReg(CCxxx0_RXFIFO);		// ������һ���ֽڣ����ֽ�Ϊ��֡���ݳ���
//        if(packetLength >= MIN_LENGTH) 					// �����Ҫ����Ч���ݳ���С�ڵ��ڽ��յ������ݰ��ĳ���
//				{			
////						if(packetLength == MIN_LENGTH)length = MIN_LENGTH;	// ��׼֡			
////            SPIReadBurstReg(CCxxx0_RXFIFO, rxBuffer, length); 	// �������н��յ�������
//             //SPIReadBurstReg(CCxxx0_RXFIFO, RxBuf[RxCnt],MIN_LENGTH);
//             SPIReadBurstReg(CCxxx0_RXFIFO, RxBuf,MIN_LENGTH);
//			//RxCnt++;
//			//if(RxCnt==MaxRxCnt)RxCnt=0;
//			//*length = packetLength;							// �ѽ������ݳ��ȵ��޸�Ϊ��ǰ���ݵĳ���
//             // Read the 2 appended status bytes (status[0] = RSSI, status[1] = LQI)
//            SPIReadBurstReg(CCxxx0_RXFIFO, status, 2); 	// ����CRCУ��λ
//						SPIStrobe(CCxxx0_SFRX);						// ��ϴ���ջ�����
//						SPIStrobe(CCxxx0_SIDLE);					// add
//						SPIStrobe(CCxxx0_SRX);						// �������״̬
//			//return (status[1] & CRC_OK);				// ���У��ɹ����ؽ��ճɹ�
//        			return (CRC_OK);
//        }
//		else 
//		{
//            //*length = packetLength;
//            SPIStrobe(CCxxx0_SFRX);		//��ϴ���ջ�����
//            //SPIStrobe(CCxxx0_SRX);		//�������״̬
//            //SPIStrobe(CCxxx0_SIDLE);//add
//            return 0;
//        }
//    } 
//		else
//		{
//		//SPIStrobe(CCxxx0_SIDLE);//add
//	 		return 0;
// 		}	
//}

unsigned char RfReceivePacket(unsigned char *rxBuffer,unsigned char *len)
{
	  unsigned char status[2];
    unsigned char packetLength;
	
	  static uint8_t flag_recv=0;
	
//	  unsigned char i;  // �������Ҫ����datarate��length������
//	  i=MIN_LENGTH*4;
	  //while (CGDO0)
// 	  while(1==GPIO_ReadInputDataBit(GPIO_GDO0_PORT,GPIO_GDO0_PIN))
// 		{
// 			Delayx10us(200);
// 			--i;
// 			if(i<1)return 0;	    
// 		}	
	
	
	
//	  //===================================================================
	  if(1==GPIO_ReadInputDataBit(GPIO_GDO0_PORT,GPIO_GDO0_PIN))
		{
			 flag_recv++;
			 return 2;
			 //if(flag_recv<5)return 2;		 
		}
		else
	  {
			 if(flag_recv==0)return 0;
		}
		flag_recv=0;
//	  //====================================================================
		
		
    if((SPIReadStatus(CCxxx0_RXBYTES) & BYTES_IN_RXFIFO)) // ����ӵ��ֽ�����Ϊ0
		{
        packetLength = SPIReadReg(CCxxx0_RXFIFO);		// ������һ���ֽڣ����ֽ�Ϊ��֡���ݳ���
        if(packetLength >= MIN_LENGTH) 					// �����Ҫ����Ч���ݳ���С�ڵ��ڽ��յ������ݰ��ĳ���
				{			
						//if(packetLength == MIN_LENGTH)length = MIN_LENGTH;	// ��׼֡	
					
            SPIReadBurstReg(CCxxx0_RXFIFO, rxBuffer,MIN_LENGTH); 	 // �������н��յ�������
			
            //SPIReadBurstReg(CCxxx0_RXFIFO, RxBuf[RxCnt],MIN_LENGTH);

					  *len=packetLength;
			      //*length = packetLength;														// �ѽ������ݳ��ȵ��޸�Ϊ��ǰ���ݵĳ���
            // Read the 2 appended status bytes (status[0] = RSSI, status[1] = LQI)
					
            SPIReadBurstReg(CCxxx0_RXFIFO, status, 2); 					// ����CRCУ��λ
						SPIStrobe(CCxxx0_SFRX);															// ��ϴ���ջ�����
						SPIStrobe(CCxxx0_SIDLE);														// add
						SPIStrobe(CCxxx0_SRX);															// �������״̬
			       //return (status[1] & CRC_OK);											// ���У��ɹ����ؽ��ճɹ�
        		//return (CRC_OK);
					  return 1;
        }
		else 
		{
            //*length = packetLength;
            SPIStrobe(CCxxx0_SFRX);															//��ϴ���ջ�����
            //SPIStrobe(CCxxx0_SRX);														//�������״̬
            //SPIStrobe(CCxxx0_SIDLE);													//add
            return 0;
        }
    } 
		else
		{
		//SPIStrobe(CCxxx0_SIDLE);//add
	 		return 0;
 		}	
}


#if 0
unsigned char RfReceivePacket(void) 
{
    unsigned char status[2];
    unsigned char packetLength;
	unsigned char i;  // �������Ҫ����datarate��length������
	i=MIN_LENGTH*4;
	//while (CGDO0)
	while(1==GPIO_ReadInputDataBit(GPIO_GDO0_PORT,GPIO_GDO0_PIN))
		{
			Delayx10us(4);
			--i;
			if(i<1)
			    return 0;	    
		}	
    if((SPIReadStatus(CCxxx0_RXBYTES) & BYTES_IN_RXFIFO)) // ����ӵ��ֽ�����Ϊ0
		{
        packetLength = SPIReadReg(CCxxx0_RXFIFO);		// ������һ���ֽڣ����ֽ�Ϊ��֡���ݳ���
        if(packetLength >= MIN_LENGTH) 					// �����Ҫ����Ч���ݳ���С�ڵ��ڽ��յ������ݰ��ĳ���
				{			
//						if(packetLength == MIN_LENGTH)length = MIN_LENGTH;	// ��׼֡			
//            SPIReadBurstReg(CCxxx0_RXFIFO, rxBuffer, length); 	// �������н��յ�������

             			RxCnt++;
			if(RxCnt==MaxRxCnt)RxCnt=0;
			
             SPIReadBurstReg(CCxxx0_RXFIFO, RxBuf[RxCnt],MIN_LENGTH);

			//*length = packetLength;							// �ѽ������ݳ��ȵ��޸�Ϊ��ǰ���ݵĳ���
             // Read the 2 appended status bytes (status[0] = RSSI, status[1] = LQI)
            SPIReadBurstReg(CCxxx0_RXFIFO, status, 2); 	// ����CRCУ��λ
						SPIStrobe(CCxxx0_SFRX);						// ��ϴ���ջ�����
						SPIStrobe(CCxxx0_SIDLE);					// add
						SPIStrobe(CCxxx0_SRX);						// �������״̬
			//return (status[1] & CRC_OK);				// ���У��ɹ����ؽ��ճɹ�
        			return (CRC_OK);
        }
		else 
		{
            //*length = packetLength;
            SPIStrobe(CCxxx0_SFRX);		//��ϴ���ջ�����
            //SPIStrobe(CCxxx0_SRX);		//�������״̬
            //SPIStrobe(CCxxx0_SIDLE);//add
            return 0;
        }
    } 
		else
		{
		//SPIStrobe(CCxxx0_SIDLE);//add
	 		return 0;
 		}	
}
#endif

//*************************************************************************************** 
//��������INT8U CC1100_wakeup() 
//���룺�� 
//������� 
//�������ܣ�����cc1100 
//**************************************************************************************** 
void CC1100WakeUp(void) 
{ 
//	unsigned int n,m;
//	
//	//TRISC2 = 0;
//	TRISCbits.TRISC2 = 0;
//	CCS = 1; 	 
//	for(n=0;n<10;n++); 
//	CCS = 0; 
//	//for(n=0;n<30;n++) 
//	for(m=0;m<1000;m++); 
//	CCS = 1; 
//	for(n=0;n<10;n++);
	
} 
/*
**********************************************************************
************************** ����˯��ģʽ ******************************
**********************************************************************
*/
void SpiSleep(void)
{
	SPIStrobe(CCxxx0_SIDLE);
	SPIStrobe(CCxxx0_SPWD);
	//SPIStrobe(CCxxx0_SWOR);	
}

/*
**********************************************************************
****************************** ��ʼ�� ********************************
**********************************************************************
*/
//void InitCC1101(void)
void bsp_InitCC1101(void)
{
	InitSPI();	
	PowerUpResetCC1100();
	RfWriteRfSettings();
	SPIWriteBurstReg(CCxxx0_PATABLE, PaTabel, 8);
	SetRxMode();
}

/*
**********************************************************************
****************************** ���߼����ʼ�� ************************
**********************************************************************
*/
void InitWorMode(void)
{
	
  SPIWriteReg(CCxxx0_RCCTRL1,  0x41);        // RC oscillator configuration
	
  // Enable automatic initial calibration of RCosc.
  // Set T_event1 ~ 1.4 ms, enough for XOSC stabilize and FS calibration before RX.
  // Enable RC oscillator before starting with WOR (or else it will not wake up).
  //SPIWriteReg(CCxxx0_WORCTRL, 0x78);           // Not using AUTO_SYNC function.
  SPIWriteReg(CCxxx0_WORCTRL, 0x38);//�� WOR = 0XB8
  // Set Event0 timeout = 1 s (RX polling interval)
  // WOR_RES = 0
  // T_event0 = 750 / f_xosc * EVENT0 * 2^(5*WOR_RES) = 1 s,f_xosc = 27 MHz
  // =>  EVENT0 = 0x8CA0
  //SPIWriteReg(CCxxx0_WOREVT1, 0x8C);                // High byte Event0 timeout
  //SPIWriteReg(CCxxx0_WOREVT0, 0xA0);                // Low byte Event0 timeout.
  SPIWriteReg(CCxxx0_WOREVT1, 0x3C);                // High byte Event0 timeout
  SPIWriteReg(CCxxx0_WOREVT0, 0xCC);   
   
  //SPIWriteReg(CCxxx0_WOREVT1, 0x06);                // High byte Event0 timeout
  //SPIWriteReg(CCxxx0_WOREVT0, 0xC5);   //TEVENT0=49.9MS,RX6.25MS
  //SPIWriteReg(CCxxx0_WOREVT1, 0xad);  //  
  //SPIWriteReg(CCxxx0_WOREVT0, 0x56);  //
  
  // Setting Rx_timeout =2.596 ms.
  // MCSM2.RX_TIME = 000b
  // => Rx_timeout = EVENT0*C(RX_TIME, WOR_RES)
  SPIWriteReg(CCxxx0_MCSM2, 0x00);
  SPIWriteReg(CCxxx0_MCSM1,0x30); //���ö�ȡ�������IDLE̬ 
  // Enable automatic FS calibration when going from IDLE to RX/TX/FSTXON (in between EVENT0 and EVENT1)
  SPIWriteReg(CCxxx0_MCSM0, 0x18);
  
  SPIStrobe(CCxxx0_SIDLE);
  SPIStrobe(CCxxx0_SWORRST);
  SPIStrobe(CCxxx0_SWOR);  
}

//=================================================================

//==================================================================

void InitSPI(void)
{
//	TRIS_CGDO0 = 1; // ����GDO0 			
//   	TRIS_CCS = 0; // CSN ��� 
//  	CCS 	= 1;  	
//   	TRIS_CSCLK = 0; // SCLK���
//   	CSCLK 	= 0;     		   	
//	TRIS_CMISO = 1; // ����
//	CMISO	= 0;  
//   	TRIS_CMOSI= 0; // ���   	   
//	CMOSI	= 0;
 //====================================
	//GDO0
	RCC_APB2PeriphClockCmd(RCC_GDO0, ENABLE); 
  	GPIO_InitStructure.GPIO_Pin = GPIO_GDO0_PIN; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
  	GPIO_Init(GPIO_GDO0_PORT, &GPIO_InitStructure);
	//CCS
	RCC_APB2PeriphClockCmd(RCC_CCS, ENABLE); 
  	GPIO_InitStructure.GPIO_Pin = GPIO_CCS_PIN; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
  	GPIO_Init(GPIO_CCS_PORT, &GPIO_InitStructure); 
	//CSCLK
	RCC_APB2PeriphClockCmd(RCC_CSCLK, ENABLE); 
  	GPIO_InitStructure.GPIO_Pin = GPIO_CSCLK_PIN; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
  	GPIO_Init(GPIO_CSCLK_PORT, &GPIO_InitStructure); 
	//CMISO
	 RCC_APB2PeriphClockCmd(RCC_CMISO, ENABLE); 
  	GPIO_InitStructure.GPIO_Pin = GPIO_CMISO_PIN; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
  	GPIO_Init(GPIO_CMISO_PORT, &GPIO_InitStructure);
	//CMOSI
    RCC_APB2PeriphClockCmd(RCC_CMOSI, ENABLE); 
  	GPIO_InitStructure.GPIO_Pin = GPIO_CMOSI_PIN; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
  	GPIO_Init(GPIO_CMOSI_PORT, &GPIO_InitStructure); 
		
}

//========================================================================================
void Delay2us(void)
{
		uint8_t i,j;
	  j=100;
	  while(j>0)
		{
				j--;
			  for(i=0;i<50;i++);
		}
}
//=========================================================================================
//
void Delayx10us(uint8_t n)
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











