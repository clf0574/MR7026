

#ifndef _MY_QUEUE_H
 #define _MY_QUEUE_H

#define QUEUE_BUF_SIZE				       12
#define QUEUE_BUF_SSIZE							 8
#define BUFFER_SIZE     			       200	//最大为64

#define RANDOM											 QUEUE_BUF_SSIZE+1												


#ifndef TRUE 
	#define TRUE 					1
#endif

#ifndef FALSE
	#define FALSE					0
#endif

#ifndef NULL
	#define NULL    			0
#endif

//16通道
#define USE_CHL_0  0x0001									//广播通道

#define USE_CHL_1  0x0002									//通道1
#define USE_CHL_2  0x0004									//通道2
#define USE_CHL_3  0x0008									//通道3
#define USE_CHL_4  0x0010									//通道4

#define USE_CHL_5  0x0020									//通道5
#define USE_CHL_6  0x0040									//通道6
#define USE_CHL_7  0x0080									//通道7
#define USE_CHL_8  0x0100									//通道8

#define USE_CHL_9   0x0200									//通道9
#define USE_CHL_10  0x0400									//通道10
#define USE_CHL_11  0x0800									//通道11
#define USE_CHL_12  0x1000									//通道12

#define USE_CHL_13  0x2000									//通道13
#define USE_CHL_14  0x4000									//通道14
#define USE_CHL_15  0x8000									//通道15





//=========================================================================================
typedef struct __Frame
{
	unsigned char buf[BUFFER_SIZE]; 	//缓冲区
	unsigned char cnt;			//缓冲区字节数         								 //应答
	unsigned char flag;                     //是否有效标记
	unsigned char resend;                   //重发次数
	unsigned char offline;                  //是否离线发送
	unsigned char rand;											//取随机数
	unsigned char ackwait;						//等待应答
}_Frame;

typedef struct __Buffer
{
	//unsigned char front;	 			//头指针
	//unsigned char  rear;	 			//尾指针
	unsigned char index;					//轮询指针
	unsigned int count;	 					//计数器
	_Frame  Data[QUEUE_BUF_SIZE];			//数据区
}QueueBUF;




void InitQueueBUF(QueueBUF *queueBuf); 						//初始化队列
unsigned char IsQueueBufEmpty(QueueBUF *queueBuf); 				//队列是否为空，1表示为空
unsigned char IsQueueBufFull(QueueBUF *queueBuf);	 			//队列满？1表示满
unsigned char EnQueueBuf(QueueBUF *queueBuf,unsigned char channel);  		//入队
unsigned char DeQueueBuf(QueueBUF *queueBuf,unsigned char channel);		//出队
_Frame* GetQueue_cycle(QueueBUF *queueBuf);					//按循环取队列

_Frame* GetQueue_InQ(QueueBUF *queueBuf,unsigned char channel);			//获取数据进栈指针
_Frame* GetQueue_OutQ(QueueBUF *queueBuf,unsigned char *pindex);					//获取数据出栈指针


#endif

