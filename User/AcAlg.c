
//=================================================================================================
//红外库，有868M无线或蓝牙下载
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
//采用数组下载还是字符串下载
#define STRINGTRANSMIT
//#define ARRAYTRANSMIT
//==================================================================================================


//==================================================================================================
//解码程序
//参数1：输入遥控器参数
//参数2：输入按键参数
//参数3：输出波形频率
//参数4：输出波形数据
uint8_t Ir_Decode(uint8_t *indat1,uint8_t *indat2,uint16_t *freq,uint16_t *outdat,uint16_t *outnum,uint8_t *repeat) 
//uint8_t Ir_Decode(void)
{
#if 0
	uint16_t out[1024], retNum;//脉冲个数数组，元素个数
	uint8_t frameCount = 1;
    	uint32_t pulse;
    	uint16_t i = 0;
	uint16_t j = 0;
	
	#ifdef ARRAYTRANSMIT
	//遥控器参数     每套遥控器是唯一的 所以只需传给设备一次
	uint8_t params[] = {0x00,0x24,0x0E,0xD8,0x01,0x00,0x00,0x00,0x00,0x10,0x00,0x10,0x00,0x10,0x00,
			  0x31,0x01,0x00,0x00,0x02,0x00,0x05,0x1E,0x09,0x00,0x00,0x10,0x03,0xCA,0x00,
			  0x84,0x00,0x42,0x09,0x08,0x00,0x10,0x05,0x34,0x00,0x84,0x00,0x42,0x09,0x10,
			  0x00,0x10,0x05,0x34,0x00,0x84,0x00,0x42,0x00};
	//按键参数   每按键一次传送给设备
	uint8_t bytes[] ={0x00,0x00,0x11,0xDA,0x27,0x00,0xC5,0x00,0x00,0xD7,0x11,0xDA,0x27,0x00,0x42,
			0x00,0x00,0x54,0x11,0xDA,0x27,0x00,0x00,0x49,0x28,0x00,0xAF,0x00,0x00,0x06,
			0x60,0x00,0x00,0xC1,0x80,0x00,0xD9}; 
			
	//根据收到的遥控参数，生成解码用的遥控器系列
	create(params,sizeof(params));
	//根据收到的按键参数，解码成红外具体发送的数组
	enc(bytes, sizeof(bytes), out,(sizeof(out) / (sizeof(unsigned short))), &retNum, &frameCount);
	#endif 
#endif
//=====================================================================================================
//实际程序使用
	create(indat1,sizeof(indat1));
	enc(indat2, sizeof(indat2), outdat,500, outnum ,repeat);
  *freq=getFrequency();

//===================================================================================================

//#ifdef STRINGTRANSMIT//解析字符串
//	//遥控器参数 
//	UBYTE params[200]={0};//注意这个数组必须保留
//	//按键参数
//	UBYTE bytes[200]={0};//注意这个数组必须保留
//	//遥控器参数 字符串，   每套遥控器是唯一的 所以只需传给设备一次
//	UBYTE params_str[] = {"00240ED80100000000100010001000310100000200051E0900001003CA00840042090800100534008400420910001005340084004200"};
//	//按键参数字符串，  每按键一次传送给设备
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
	//printf("getFrequency=%d\n", getFrequency());//频率
	//printf("frameCount=%d\n", frameCount);
	
	
	//一帧的数据通过out数组返回，单位为(1000000 /频率)，重复的帧数通过frameCount返回，如果要得到完整的高低电平时间序列(单位us)使用下面代码。
	pulse = (double) 1000000 / getFrequency();
	
	
	//for(i = 0;i < frameCount;i++){//重复发送
	//	for (j = 0; j < retNum; j++) {
	//	printf("%02X,", (int) (out[j]));//得到out数组
	//	}
	//}
	
	//printf("\n");
	//printf("\n");
	
	for(i = 0;i < frameCount;i++){//重复发送
		for (j = 0; j < retNum; j++) {
		printf("%d,", (int) (out[j] * pulse));//得到时间序列单位us
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
