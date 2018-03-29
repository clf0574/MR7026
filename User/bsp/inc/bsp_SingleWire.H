//======================Copyright (c)=====================================
// �ļ�����: Base_SingleWire.H
// ��������:
//
// �޸ļ�¼:
//           2017-06-21, PiaoHong �����ļ�
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
#define READ_ROM    0x33 //��ȡ����ROM
#define MATCH_ROM   0x55 //ƥ������ROM
#define SKIP_ROM    0xCC //����ROMƥ��

GLOBAL void SingleWire_Init(void);

GLOBAL uint8 OWReadRom(uint8 *id);

uint8 ANON_READ_AuthPage(uint8 *Receive_buffer, uint8* Scratchpad, uint8 PageNum);
void Secret_Verify(uint8 *pdat, uint8 len);

//= end   ================================================================
#undef GLOBAL

#endif //_BASE_SINGLEWIRE_H

