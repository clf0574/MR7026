#ifndef _AC_ALG_H_ 
#define _AC_ALG_H_ 


uint8_t Ir_Decode(uint8_t *indat1,uint8_t *indat2,uint16_t *freq,uint16_t *outdat,uint16_t *outnum,uint8_t *repeat);
void create(uint8_t *params, uint16_t len);
uint16_t getFrequency(void);
int enc(uint8_t *in, uint16_t inNum, uint16_t *out, uint16_t outMaxNum,	uint16_t *retNum, uint8_t *frameCount);

#if 0

#include <string.h>

typedef unsigned char UBYTE;

//数据初始化
void create(UBYTE *params, unsigned short len);

//得到频率
int getFrequency(void);

//数据解析, in 按键参数, inNum in中有效字节数, out 调用方准备好的返回高低电平缓冲区（单位：脉冲个数），
// outMaxNum out允许存放的最大元素个数(out指向的可用内存空间/sizeof(unsigned short))，
// retNum 编码完成后在out 内存空间中实际写了多少个unsigned short元素，
// frameCount 通过out返回的高低电平数组（一帧）需要重复的次数
int enc(UBYTE *in, unsigned short inNum, unsigned short *out, unsigned short outMaxNum,
		unsigned short *retNum, UBYTE *frameCount);

#endif


#endif

