//======================Copyright (c)=====================================
// 文件名称: Base_SingleWire.H
// 功能描述:
//
// 修改记录:
//           2017-06-21, PiaoHong 创建文件
//
//
//========================================================================

#ifndef _BASE_SINGLEWIRE_H
#define _BASE_SINGLEWIRE_H

#ifdef _BASE_SINGLEWIRE_C
#define GLOBAL
#endif

#ifndef _BASE_SINGLEWIRE_C
#define GLOBAL extern
#endif
//= Start ================================================================
#define uint8 uint8_t
#define READ_ROM    0x33 //读取器件ROM
#define MATCH_ROM   0x55 //匹配器件ROM
#define SKIP_ROM    0xCC //忽略ROM匹配

GLOBAL void SingleWire_Init(void);

GLOBAL uint8 OWReadRom(uint8 *id);

uint8 ANON_READ_AuthPage(uint8 *Receive_buffer, uint8* Scratchpad, uint8 PageNum);
void Secret_Verify(uint8 *pdat, uint8 len);

//= end   ================================================================
#undef GLOBAL

#endif //_BASE_SINGLEWIRE_H


