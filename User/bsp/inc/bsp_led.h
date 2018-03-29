
//============================================================================================
#ifndef  __LED_H__
#define  __LED_H__

#define LOOP 0XFF
#define ON   1
#define OFF  0

void bsp_InitLed(void);
void Led_set(unsigned char index,unsigned char ON_t,unsigned char OFF_t,unsigned char cnt,unsigned char type);
void led_run(void);
//void Led_IO_SET(void);
uint8_t bsp_IsLedOn(uint8_t _no);
void bsp_LedToggle(uint8_t _no);
void bsp_LedOff(uint8_t _no);
void bsp_LedOn(uint8_t _no);

#endif
//=========================


