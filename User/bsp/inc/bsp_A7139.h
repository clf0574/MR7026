#ifndef  __A7139_H__
#define  __A7139_H__

#include "Extern_defin.h"

#define RF_DATA_BUF_MAX_LEN					64
#define RF_DATA_BUF_MIN_LEN     		16



//=====================================================================
uint8_t bsp_InitRF(uint8_t st);
void A7129_Cal(void);
void A7129_WriteFIFO(uint8_t *buf,uint8_t length);
uint8_t RxPacket(uint8_t *buf);
void RF_TX(uint8_t *buf,uint8_t length);
void RF_TX1(uint8_t *buf,uint8_t length);//ÎÞÏß»½ÐÑ
uint8_t RF_RX(uint8_t *buf,uint8_t *plen);
void Rf_Power_En(uint8_t stu);
uint8_t RF_WaitForSend(void);

void StrobeCMD(uint8_t cmd);
//=====================================================================
#define USE_CONFIG_A7159
//=====================================================================
#ifdef USE_CONFIG_A7129
	#define SYSTEMCLOCK_REG 	0x00
	#define PLL1_REG 			0x01
	#define PLL2_REG 			0x02
	#define PLL3_REG 			0x03
	#define PLL4_REG			0x04
	#define PLL5_REG			0x05
	#define PLL6_REG			0x06
	#define CRYSTAL_REG			0x07
	#define PAGEA_REG	  		0x08
	#define PAGEB_REG			0x09
	#define RX1_REG  			0x0A
	#define RX2_REG  			0x0B
	#define ADC_REG  			0x0C
	#define PIN_REG 			0x0D
	#define CALIBRATION_REG  	0x0E
	#define MODE_REG  			0x0F
	
	#define TX1_PAGEA           0x00
	#define WOR1_PAGEA          0x01
	#define WOR2_PAGEA          0x02
	#define RFI_PAGEA           0x03
	#define PM_PAGEA            0x04
	#define RTH_PAGEA           0x05
	#define AGC1_PAGEA          0x06
	#define AGC2_PAGEA          0x07
	#define GIO_PAGEA           0x08
	#define CKO_PAGEA           0x09
	#define VCB_PAGEA           0x0A
	#define CHG1_PAGEA          0x0B
	#define CHG2_PAGEA          0x0C
	#define FIFO_PAGEA			0x0D
	#define CODE_PAGEA			0x0E
	#define WCAL_PAGEA			0x0F
	
	#define TX2_PAGEB			0x00
	#define	IF1_PAGEB			0x01
	#define IF2_PAGEB			0x02
	#define	ACK_PAGEB			0x03
	#define	ART_PAGEB			0x04
	
	
	#define CMD_Reg_W			0x00	//000x,xxxx	control register write
	#define CMD_Reg_R			0x80	//100x,xxxx	control register read
	#define CMD_ID_W			0x20	//001x,xxxx	ID write
	#define CMD_ID_R			0xA0	//101x,xxxx	ID Read
	#define CMD_FIFO_W			0x40	//010x,xxxx	TX FIFO Write
	#define CMD_FIFO_R			0xC0	//110x,xxxx	RX FIFO Read
	
	#define CMD_RF_RST			0x70	//x111,xxxx	RF reset		//a7129
	//#define CMD_RF_RST			0xFF	//1111,1111	RF reset    //a7139
	
	#define CMD_TFR				0x60	//0110,xxxx	TX FIFO address pointrt reset
	#define CMD_RFR				0xE0	//1110,xxxx	RX FIFO address pointer reset
	
	#define CMD_SLEEP			0x10	//0001,0000	SLEEP mode
	#define CMD_IDLE			0x12	//0001,0010	IDLE mode
	#define CMD_STBY			0x14	//0001,0100	Standby mode
	#define CMD_PLL				0x16	//0001,0110	PLL mode
	#define CMD_RX				0x18	//0001,1000	RX mode
	#define CMD_TX				0x1A	//0001,1010	TX mode
	//#define CMD_DEEP_SLEEP	0x1C	//0001,1100 Deep Sleep mode(tri-state)
	#define CMD_DEEP_SLEEP		0x1F	//0001,1111 Deep Sleep mode(pull-high)

#endif
//===================================================================
#ifdef USE_CONFIG_A7139
	#define SYSTEMCLOCK_REG 	0x00
	#define PLL1_REG 			0x01
	#define PLL2_REG 			0x02
	#define PLL3_REG 			0x03
	#define PLL4_REG			0x04
	#define PLL5_REG			0x05
	#define PLL6_REG			0x06
	#define CRYSTAL_REG			0x07
	#define PAGEA_REG	  		0x08
	#define PAGEB_REG			0x09
	#define RX1_REG  			0x0A
	#define RX2_REG  			0x0B
	#define ADC_REG  			0x0C
	#define PIN_REG 			0x0D
	#define CALIBRATION_REG  	0x0E
	#define MODE_REG  			0x0F
	
	#define TX1_PAGEA           0x00
	#define WOR1_PAGEA          0x01
	#define WOR2_PAGEA          0x02
	#define RFI_PAGEA           0x03
	#define PM_PAGEA            0x04
	#define RTH_PAGEA           0x05
	#define AGC1_PAGEA          0x06
	#define AGC2_PAGEA          0x07
	#define GIO_PAGEA           0x08
	#define CKO_PAGEA           0x09
	#define VCB_PAGEA           0x0A
	#define CHG1_PAGEA          0x0B
	#define CHG2_PAGEA          0x0C
	#define FIFO_PAGEA			0x0D
	#define CODE_PAGEA			0x0E
	#define WCAL_PAGEA			0x0F
	
	#define TX2_PAGEB			0x00
	#define	IF1_PAGEB			0x01
	#define IF2_PAGEB			0x02
	#define	ACK_PAGEB			0x03
	#define	ART_PAGEB			0x04
	
	
	#define CMD_Reg_W			0x00	//000x,xxxx	control register write
	#define CMD_Reg_R			0x80	//100x,xxxx	control register read
	#define CMD_ID_W			0x20	//001x,xxxx	ID write
	#define CMD_ID_R			0xA0	//101x,xxxx	ID Read
	#define CMD_FIFO_W			0x40	//010x,xxxx	TX FIFO Write
	#define CMD_FIFO_R			0xC0	//110x,xxxx	RX FIFO Read
	#define CMD_RF_RST			0xFF	//1111,1111	RF reset
	#define CMD_TFR				0x60	//0110,xxxx	TX FIFO address pointrt reset
	#define CMD_RFR				0xE0	//1110,xxxx	RX FIFO address pointer reset
	
	#define CMD_SLEEP			0x10	//0001,0000	SLEEP mode
	#define CMD_IDLE			0x12	//0001,0010	IDLE mode
	#define CMD_STBY			0x14	//0001,0100	Standby mode
	#define CMD_PLL				0x16	//0001,0110	PLL mode
	#define CMD_RX				0x18	//0001,1000	RX mode
	#define CMD_TX				0x1A	//0001,1010	TX mode
	//#define CMD_DEEP_SLEEP	0x1C	//0001,1100 Deep Sleep mode(tri-state)
	#define CMD_DEEP_SLEEP		0x1F	//0001,1111 Deep Sleep mode(pull-high)
#endif

//====================================================================
#ifdef USE_CONFIG_A7159
        #define SYSTEMCLOCK_REG 	0x00
        #define PLL1_REG 			0x01
        #define PLL2_REG 			0x02
        #define PLL3_REG 			0x03
        #define PLL4_REG			0x04
        #define PLL5_REG			0x05
        #define PLL6_REG			0x06
        #define CRYSTAL_REG			0x07
        #define PAGEA_REG	  		0x08
        #define PAGEB_REG			0x09
        #define RX1_REG  			0x0A
        #define RX2_REG  			0x0B
        #define ADC_REG  			0x0C
        #define PIN_REG 			0x0D
        #define CALIBRATION_REG  	0x0E
        #define MODE_REG  			0x0F

        #define TX1_PAGEA           0x00
        #define WOR1_PAGEA          0x01
        #define WOR2_PAGEA          0x02
        #define RFI_PAGEA           0x03
        #define PM_PAGEA            0x04
        #define RTH_PAGEA           0x05
        #define AGC1_PAGEA          0x06
        #define AGC2_PAGEA          0x07
        #define GIO_PAGEA           0x08
        #define CKO_PAGEA           0x09
        #define VCB_PAGEA           0x0A
        #define CHG1_PAGEA          0x0B
        #define CHG2_PAGEA          0x0C
        #define FIFO_PAGEA			0x0D
        #define CODE_PAGEA			0x0E
        #define WCAL_PAGEA			0x0F

        #define TX2_PAGEB			0x00
        #define	IF1_PAGEB			0x01
        #define IF2_PAGEB			0x02
        #define	ACK_PAGEB			0x03
        #define	ART_PAGEB			0x04
        #define SYN_PAGEB			0x05
        #define ACKFIFO_PAGEB		0x06
        #define DSSS_PAGEB			0x07
        #define CCM1_PAGEB			0x08
        #define CCM2_PAGEB			0x09
        #define CCM3_PAGEB			0x0A
        #define CCM4_PAGEB			0x0B
        #define CCM5_PAGEB			0x0C
        #define RF_Cmd_PAGEB		0x0D
        #define Btn_Cmd_PAGEB		0x0E
        #define DSSS2_PAGEB			0x0F
        #define DSSS3_PAGEB			0x10
        #define CCM6_PAGEB			0x11
        #define CDET_PAGEB			0x12
        #define DC_SHIFT_PAGEB		0x13
        #define RCOSC_PAGEB			0x14


        #define CMD_Reg_W			0x00	//000x,xxxx	control register write
        #define CMD_Reg_R			0x80	//100x,xxxx	control register read
        #define CMD_ID_W			0x20	//001x,xxxx	ID write
        #define CMD_ID_R			0xA0	//101x,xxxx	ID Read
        #define CMD_FIFO_W			0x40	//010x,xxxx	TX FIFO Write
        #define CMD_FIFO_R			0xC0	//110x,xxxx	RX FIFO Read
        #define CMD_RF_RST			0xFF	//x111,xxxx	RF reset
				
        #define CMD_TFR				0x60	//0110,xxxx	TX FIFO address pointrt reset
        #define CMD_RFR				0xE0	//1110,xxxx	RX FIFO address pointer reset

        #define CMD_SLEEP			0x10	//0001,0000	SLEEP mode
        #define CMD_IDLE			0x12	//0001,0010	IDLE mode
        #define CMD_STBY			0x14	//0001,0100	Standby mode
        #define CMD_PLL				0x16	//0001,0110	PLL mode
        #define CMD_RX				0x18	//0001,1000	RX mode
        #define CMD_TX				0x1A	//0001,1010	TX mode
                                //#define CMD_DEEP_SLEEP	0x1C	//0001,1100 Deep Sleep mode(tri-state)
        #define CMD_DEEP_SLEEP		0x1F	//0001,1111 Deep Sleep mode(pull-high)

#endif
//====================================================================


#endif



