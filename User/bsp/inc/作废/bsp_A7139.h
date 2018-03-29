#ifndef  __A7139_H__
#define  __A7139_H__


#define RF_DATA_BUF_MAX_LEN			64


/*********************************************************************
**  I/O Declaration
*********************************************************************/


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
//#define GIO2 		PORTCbits.RC5 		//GIO2   ‰»Î





uint8_t bsp_InitRF(void);
void A7129_Cal(void);
void A7129_WriteFIFO(uint8_t *buf,uint8_t length);
uint8_t RxPacket(uint8_t *buf);
void RF_TX(uint8_t *buf,uint8_t length);
uint8_t RF_RX(uint8_t *buf);

#endif



