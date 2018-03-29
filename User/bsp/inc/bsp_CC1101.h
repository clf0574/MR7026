#ifndef  __CC1101_H__
#define  __CC1101_H__

#define 	LOW     									0
#define 	HIGH    									1
#define		INT8U											unsigned char
#define		INT16U										uint16_t

//#define MaxRxCnt    								1
#define MIN_LENGTH               		16

//extern unsigned char RxBuf[MaxRxCnt][MIN_LENGTH];
//extern unsigned char RxBuf[MIN_LENGTH];
//extern unsigned char RxCnt;			//数组计数

//***************** CC1100接口 *******************************


 //GDO0
 #define RCC_GDO0                  RCC_APB2Periph_GPIOB
 #define GPIO_GDO0_PORT            GPIOB    
 #define GPIO_GDO0_PIN             GPIO_Pin_0
 //CCS
 #define RCC_CCS                   RCC_APB2Periph_GPIOA
 #define GPIO_CCS_PORT             GPIOA    
 #define GPIO_CCS_PIN              GPIO_Pin_4
 //CSCLK
 #define RCC_CSCLK                 RCC_APB2Periph_GPIOA
 #define GPIO_CSCLK_PORT           GPIOA    
 #define GPIO_CSCLK_PIN            GPIO_Pin_5
 //CMOSI
 #define RCC_CMOSI                RCC_APB2Periph_GPIOA
 #define GPIO_CMOSI_PORT          GPIOA    
 #define GPIO_CMOSI_PIN           GPIO_Pin_6
 //CMISO
 #define RCC_CMISO                RCC_APB2Periph_GPIOA
 #define GPIO_CMISO_PORT          GPIOA    
 #define GPIO_CMISO_PIN           GPIO_Pin_7
        



//-------------------------------------------------------------------------------------------------------
// RF_SETTINGS is a data structure which contains all relevant CCxxx0 registers
typedef struct S_RF_SETTINGS{
    INT8U FSCTRL2;		//自已加的
    INT8U FSCTRL1;   	// Frequency synthesizer control.
    INT8U FSCTRL0;   	// Frequency synthesizer control.
    INT8U FREQ2;     	// Frequency control word, high INT8U.
    INT8U FREQ1;     	// Frequency control word, middle INT8U.
    INT8U FREQ0;     	// Frequency control word, low INT8U.
    INT8U MDMCFG4;   	// Modem configuration.
    INT8U MDMCFG3;   	// Modem configuration.
    INT8U MDMCFG2;   	// Modem configuration.
    INT8U MDMCFG1;   	// Modem configuration.
    INT8U MDMCFG0;   	// Modem configuration.
    INT8U CHANNR;    	// Channel number.
    INT8U DEVIATN;   	// Modem deviation setting (when FSK modulation is enabled).
    INT8U FREND1;    	// Front end RX configuration.
    INT8U FREND0;    	// Front end RX configuration.
    //INT8U MCSM1;//加
    INT8U MCSM0;     	// Main Radio Control State Machine configuration.
    INT8U FOCCFG;    	// Frequency Offset Compensation Configuration.
    INT8U BSCFG;     	// Bit synchronization Configuration.
    INT8U AGCCTRL2;  	// AGC control.
	INT8U AGCCTRL1;  	// AGC control.
    INT8U AGCCTRL0;  	// AGC control.
    INT8U FSCAL3;    	// Frequency synthesizer calibration.
    INT8U FSCAL2;    	// Frequency synthesizer calibration.
	INT8U FSCAL1;    	// Frequency synthesizer calibration.
    INT8U FSCAL0;    	// Frequency synthesizer calibration.
    INT8U FSTEST;    	// Frequency synthesizer calibration control
    INT8U TEST2;     	// Various test settings.
    INT8U TEST1;     	// Various test settings.
    INT8U TEST0;     	// Various test settings.
    INT8U IOCFG2;    	// GDO2 output pin configuration
    INT8U IOCFG0;    	// GDO0 output pin configuration
    INT8U PKTCTRL1;  	// Packet automation control.
    INT8U PKTCTRL0;  	// Packet automation control.
    INT8U ADDR;      	// Device address.
    INT8U PKTLEN;    	// Packet length.
} RF_SETTINGS;



//------------------------------------------------------------------------------------------------------
// CC2500/CC1100 STROBE, CONTROL AND STATUS REGSITER
#define CCxxx0_IOCFG2       0x00        // GDO2 output pin configuration
#define CCxxx0_IOCFG1       0x01        // GDO1 output pin configuration
#define CCxxx0_IOCFG0       0x02        // GDO0 output pin configuration
#define CCxxx0_FIFOTHR      0x03        // RX FIFO and TX FIFO thresholds
#define CCxxx0_SYNC1        0x04        // Sync word, high INT8U
#define CCxxx0_SYNC0        0x05        // Sync word, low INT8U
#define CCxxx0_PKTLEN       0x06        // Packet length
#define CCxxx0_PKTCTRL1     0x07        // Packet automation control
#define CCxxx0_PKTCTRL0     0x08        // Packet automation control
#define CCxxx0_ADDR         0x09        // Device address
#define CCxxx0_CHANNR       0x0A        // Channel number
#define CCxxx0_FSCTRL1      0x0B        // Frequency synthesizer control
#define CCxxx0_FSCTRL0      0x0C        // Frequency synthesizer control
#define CCxxx0_FREQ2        0x0D        // Frequency control word, high INT8U
#define CCxxx0_FREQ1        0x0E        // Frequency control word, middle INT8U
#define CCxxx0_FREQ0        0x0F        // Frequency control word, low INT8U
#define CCxxx0_MDMCFG4      0x10        // Modem configuration
#define CCxxx0_MDMCFG3      0x11        // Modem configuration
#define CCxxx0_MDMCFG2      0x12        // Modem configuration
#define CCxxx0_MDMCFG1      0x13        // Modem configuration
#define CCxxx0_MDMCFG0      0x14        // Modem configuration
#define CCxxx0_DEVIATN      0x15        // Modem deviation setting
#define CCxxx0_MCSM2        0x16        // Main Radio Control State Machine configuration
#define CCxxx0_MCSM1        0x17        // Main Radio Control State Machine configuration
#define CCxxx0_MCSM0        0x18        // Main Radio Control State Machine configuration
#define CCxxx0_FOCCFG       0x19        // Frequency Offset Compensation configuration
#define CCxxx0_BSCFG        0x1A        // Bit Synchronization configuration
#define CCxxx0_AGCCTRL2     0x1B        // AGC control
#define CCxxx0_AGCCTRL1     0x1C        // AGC control
#define CCxxx0_AGCCTRL0     0x1D        // AGC control
#define CCxxx0_WOREVT1      0x1E        // High INT8U Event 0 timeout
#define CCxxx0_WOREVT0      0x1F        // Low INT8U Event 0 timeout
#define CCxxx0_WORCTRL      0x20        // Wake On Radio control
#define CCxxx0_FREND1       0x21        // Front end RX configuration
#define CCxxx0_FREND0       0x22        // Front end TX configuration
#define CCxxx0_FSCAL3       0x23        // Frequency synthesizer calibration
#define CCxxx0_FSCAL2       0x24        // Frequency synthesizer calibration
#define CCxxx0_FSCAL1       0x25        // Frequency synthesizer calibration
#define CCxxx0_FSCAL0       0x26        // Frequency synthesizer calibration
#define CCxxx0_RCCTRL1      0x27        // RC oscillator configuration
#define CCxxx0_RCCTRL0      0x28        // RC oscillator configuration
#define CCxxx0_FSTEST       0x29        // Frequency synthesizer calibration control
#define CCxxx0_PTEST        0x2A        // Production test
#define CCxxx0_AGCTEST      0x2B        // AGC test
#define CCxxx0_TEST2        0x2C        // Various test settings
#define CCxxx0_TEST1        0x2D        // Various test settings
#define CCxxx0_TEST0        0x2E        // Various test settings

// Strobe commands
#define CCxxx0_SRES         0x30        // Reset chip.
#define CCxxx0_SFSTXON      0x31        // Enable and calibrate frequency synthesizer (if MCSM0.FS_AUTOCAL=1).
                                        // If in RX/TX: Go to a wait state where only the synthesizer is
                                        // running (for quick RX / TX turnaround).
#define CCxxx0_SXOFF        0x32        // Turn off crystal oscillator.
#define CCxxx0_SCAL         0x33        // Calibrate frequency synthesizer and turn it off
                                        // (enables quick start).
#define CCxxx0_SRX          0x34        // Enable RX. Perform calibration first if coming from IDLE and
                                        // MCSM0.FS_AUTOCAL=1.
#define CCxxx0_STX          0x35        // In IDLE state: Enable TX. Perform calibration first if
                                        // MCSM0.FS_AUTOCAL=1. If in RX state and CCA is enabled:
                                        // Only go to TX if channel is clear.
#define CCxxx0_SIDLE        0x36        // Exit RX / TX, turn off frequency synthesizer and exit
                                        // Wake-On-Radio mode if applicable.
#define CCxxx0_SAFC         0x37        // Perform AFC adjustment of the frequency synthesizer
#define CCxxx0_SWOR         0x38        // Start automatic RX polling sequence (Wake-on-Radio)
#define CCxxx0_SPWD         0x39        // Enter power down mode when CSn goes high.
#define CCxxx0_SFRX         0x3A        // Flush the RX FIFO buffer.
#define CCxxx0_SFTX         0x3B        // Flush the TX FIFO buffer.
#define CCxxx0_SWORRST      0x3C        // Reset real time clock.
#define CCxxx0_SNOP         0x3D        // No operation. May be used to pad strobe commands to two
                                        // INT8Us for simpler software.

#define CCxxx0_PARTNUM      0x30
#define CCxxx0_VERSION      0x31
#define CCxxx0_FREQEST      0x32
#define CCxxx0_LQI          0x33
#define CCxxx0_RSSI         0x34
#define CCxxx0_MARCSTATE    0x35
#define CCxxx0_WORTIME1     0x36
#define CCxxx0_WORTIME0     0x37
#define CCxxx0_PKTSTATUS    0x38
#define CCxxx0_VCO_VC_DAC   0x39
#define CCxxx0_TXBYTES      0x3A
#define CCxxx0_RXBYTES      0x3B

#define CCxxx0_PATABLE      0x3E
#define CCxxx0_TXFIFO       0x3F
#define CCxxx0_RXFIFO       0x3F


#define  halRfWriteReg  SPIWriteReg
#define IOCFG2       0x00        // GDO2 output pin configuration
#define IOCFG1       0x01        // GDO1 output pin configuration
#define IOCFG0       0x02        // GDO0 output pin configuration
#define FIFOTHR      0x03        // RX FIFO and TX FIFO thresholds
#define SYNC1        0x04        // Sync word, high INT8U
#define SYNC0        0x05        // Sync word, low INT8U
#define PKTLEN       0x06        // Packet length
#define PKTCTRL1     0x07        // Packet automation control
#define PKTCTRL0     0x08        // Packet automation control
#define ADDR         0x09        // Device address
#define CHANNR       0x0A        // Channel number
#define FSCTRL1      0x0B        // Frequency synthesizer control
#define FSCTRL0      0x0C        // Frequency synthesizer control
#define FREQ2        0x0D        // Frequency control word, high INT8U
#define FREQ1        0x0E        // Frequency control word, middle INT8U
#define FREQ0        0x0F        // Frequency control word, low INT8U
#define MDMCFG4      0x10        // Modem configuration
#define MDMCFG3      0x11        // Modem configuration
#define MDMCFG2      0x12        // Modem configuration
#define MDMCFG1      0x13        // Modem configuration
#define MDMCFG0      0x14        // Modem configuration
#define DEVIATN      0x15        // Modem deviation setting
#define MCSM2        0x16        // Main Radio Control State Machine configuration
#define MCSM1        0x17        // Main Radio Control State Machine configuration
#define MCSM0        0x18        // Main Radio Control State Machine configuration
#define FOCCFG       0x19        // Frequency Offset Compensation configuration
#define BSCFG        0x1A        // Bit Synchronization configuration
#define AGCCTRL2     0x1B        // AGC control
#define AGCCTRL1     0x1C        // AGC control
#define AGCCTRL0     0x1D        // AGC control
#define WOREVT1      0x1E        // High INT8U Event 0 timeout
#define WOREVT0      0x1F        // Low INT8U Event 0 timeout
#define WORCTRL      0x20        // Wake On Radio control
#define FREND1       0x21        // Front end RX configuration
#define FREND0       0x22        // Front end TX configuration
#define FSCAL3       0x23        // Frequency synthesizer calibration
#define FSCAL2       0x24        // Frequency synthesizer calibration
#define FSCAL1       0x25        // Frequency synthesizer calibration
#define FSCAL0       0x26        // Frequency synthesizer calibration
#define RCCTRL1      0x27        // RC oscillator configuration
#define RCCTRL0      0x28        // RC oscillator configuration
#define FSTEST       0x29        // Frequency synthesizer calibration control
#define PTEST        0x2A        // Production test
#define AGCTEST      0x2B        // AGC test
#define TEST2        0x2C        // Various test settings
#define TEST1        0x2D        // Various test settings
#define TEST0        0x2E        // Various test settings


//extern unsigned char RfReceivePacket(unsigned char *rxBuffer, unsigned char length);
//extern unsigned char RfReceivePacket(unsigned char *rxBuffer, unsigned char *length);

//void InitCC1101(void);
void bsp_InitCC1101(void);

void RfSendPacket(INT8U *txBuffer, INT8U size);
//unsigned char RfReceivePacket(void);
unsigned char RfReceivePacket(unsigned char *rxBuffer,unsigned char *len);


void SpiSleep(void);
void SPIStrobe(unsigned char strobe);
void CC1100WakeUp(void);
void InitWorMode(void);


// PATABLE (0 dBm output power)

#endif



