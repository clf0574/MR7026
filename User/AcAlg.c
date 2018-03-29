
//=================================================================================================
//����⣬��868M���߻���������
//2016.12.7 clf
//==================================================================================================
//#include <main.h>
//#include "AcAlg.h"
//#include <stdio.h>

#include "includes.h"




//=================================================================================================
//#ifdef _DEBUG 
//#define _CRTDBG_MAP_ALLOC  
//#include <stdlib.h>  
//#include <crtdbg.h>
//#endif 

//==================================================================================================
//�����������ػ����ַ�������
#define STRINGTRANSMIT
//#define ARRAYTRANSMIT
//==================================================================================================


//==================================================================================================
//�������
//����1������ң��������
//����2�����밴������
//����3���������Ƶ��
//����4�������������
uint8_t Ir_Decode(uint8_t *indat1,uint8_t *indat2,uint16_t *freq,uint16_t *outdat,uint16_t *outnum,uint8_t *repeat) 
//uint8_t Ir_Decode(void)
{
#if 0
	uint16_t out[1024], retNum;//����������飬Ԫ�ظ���
	uint8_t frameCount = 1;
    	uint32_t pulse;
    	uint16_t i = 0;
	uint16_t j = 0;
	
	#ifdef ARRAYTRANSMIT
	//ң��������     ÿ��ң������Ψһ�� ����ֻ�贫���豸һ��
	uint8_t params[] = {0x00,0x24,0x0E,0xD8,0x01,0x00,0x00,0x00,0x00,0x10,0x00,0x10,0x00,0x10,0x00,
			  0x31,0x01,0x00,0x00,0x02,0x00,0x05,0x1E,0x09,0x00,0x00,0x10,0x03,0xCA,0x00,
			  0x84,0x00,0x42,0x09,0x08,0x00,0x10,0x05,0x34,0x00,0x84,0x00,0x42,0x09,0x10,
			  0x00,0x10,0x05,0x34,0x00,0x84,0x00,0x42,0x00};
	//��������   ÿ����һ�δ��͸��豸
	uint8_t bytes[] ={0x00,0x00,0x11,0xDA,0x27,0x00,0xC5,0x00,0x00,0xD7,0x11,0xDA,0x27,0x00,0x42,
			0x00,0x00,0x54,0x11,0xDA,0x27,0x00,0x00,0x49,0x28,0x00,0xAF,0x00,0x00,0x06,
			0x60,0x00,0x00,0xC1,0x80,0x00,0xD9}; 
			
	//�����յ���ң�ز��������ɽ����õ�ң����ϵ��
	create(params,sizeof(params));
	//�����յ��İ�������������ɺ�����巢�͵�����
	enc(bytes, sizeof(bytes), out,(sizeof(out) / (sizeof(unsigned short))), &retNum, &frameCount);
	#endif 
#endif
//=====================================================================================================
//ʵ�ʳ���ʹ��
	create(indat1,sizeof(indat1));
	enc(indat2, sizeof(indat2), outdat,500, outnum ,repeat);
  *freq=getFrequency();

//===================================================================================================

//#ifdef STRINGTRANSMIT//�����ַ���
//	//ң�������� 
//	UBYTE params[200]={0};//ע�����������뱣��
//	//��������
//	UBYTE bytes[200]={0};//ע�����������뱣��
//	//ң�������� �ַ�����   ÿ��ң������Ψһ�� ����ֻ�贫���豸һ��
//	UBYTE params_str[] = {"00240ED80100000000100010001000310100000200051E0900001003CA00840042090800100534008400420910001005340084004200"};
//	//���������ַ�����  ÿ����һ�δ��͸��豸
//	UBYTE bytes_str[] ={"000011DA2700C50000D711DA27004200005411DA270000492800AF000006600000C18000D9"}; 
//
//	for(i=0;i<sizeof(params_str)-1;i++){
//		if(params_str[i]>='A'){
//		params[i]=params_str[i]-'7';
//			}
//		else {
//			params[i]=params_str[i]-'0';
//			}
//	}
//	for(i=0;i<(sizeof(params_str)-1)/2;i++){
//			params[i]=(params[2*i]<<4)+params[2*i+1];
//	}
//	
//	for(i=0;i<sizeof(bytes_str)-1;i++){
//	if(bytes_str[i]>='A'){
//		bytes[i]=bytes_str[i]-'7';
//			}
//		else {
//			bytes[i]=bytes_str[i]-'0';
//			}
//	}
//	for(i=0;i<(sizeof(bytes_str)-1)/2;i++){
// 		bytes[i]=(bytes[2*i]<<4)+bytes[2*i+1];
//	}
//	create(params,(sizeof(params_str)-1)/2);
//	if ((enc(bytes, (sizeof(bytes_str)-1)/2, out,(sizeof(out) / (sizeof(unsigned short))),		
//		 &retNum, &frameCount))!=0){
//		 printf("enc error\n");
//
//		 	return 0;
//		}
//#endif 	
//=================================================================================================
#if 0

	//printf("retNum=%d\n", retNum);
	//printf("getFrequency=%d\n", getFrequency());//Ƶ��
	//printf("frameCount=%d\n", frameCount);
	
	
	//һ֡������ͨ��out���鷵�أ���λΪ(1000000 /Ƶ��)���ظ���֡��ͨ��frameCount���أ����Ҫ�õ������ĸߵ͵�ƽʱ������(��λus)ʹ��������롣
	pulse = (double) 1000000 / getFrequency();
	
	
	//for(i = 0;i < frameCount;i++){//�ظ�����
	//	for (j = 0; j < retNum; j++) {
	//	printf("%02X,", (int) (out[j]));//�õ�out����
	//	}
	//}
	
	//printf("\n");
	//printf("\n");
	
	for(i = 0;i < frameCount;i++){//�ظ�����
		for (j = 0; j < retNum; j++) {
		printf("%d,", (int) (out[j] * pulse));//�õ�ʱ�����е�λus
		}

	}
#endif

//==================================================================================================
	//printf("\n");
//	#ifdef _DEBUG
//	_CrtDumpMemoryLeaks();
//	#endif
//===================================================================================================
	return 0;
}
