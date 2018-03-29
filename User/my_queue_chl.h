

#ifndef _MY_QUEUE_H
 #define _MY_QUEUE_H

#define QUEUE_BUF_SIZE				       12
#define QUEUE_BUF_SSIZE							 8
#define BUFFER_SIZE     			       200	//���Ϊ64

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

//16ͨ��
#define USE_CHL_0  0x0001									//�㲥ͨ��

#define USE_CHL_1  0x0002									//ͨ��1
#define USE_CHL_2  0x0004									//ͨ��2
#define USE_CHL_3  0x0008									//ͨ��3
#define USE_CHL_4  0x0010									//ͨ��4

#define USE_CHL_5  0x0020									//ͨ��5
#define USE_CHL_6  0x0040									//ͨ��6
#define USE_CHL_7  0x0080									//ͨ��7
#define USE_CHL_8  0x0100									//ͨ��8

#define USE_CHL_9   0x0200									//ͨ��9
#define USE_CHL_10  0x0400									//ͨ��10
#define USE_CHL_11  0x0800									//ͨ��11
#define USE_CHL_12  0x1000									//ͨ��12

#define USE_CHL_13  0x2000									//ͨ��13
#define USE_CHL_14  0x4000									//ͨ��14
#define USE_CHL_15  0x8000									//ͨ��15





//=========================================================================================
typedef struct __Frame
{
	unsigned char buf[BUFFER_SIZE]; 	//������
	unsigned char cnt;			//�������ֽ���         								 //Ӧ��
	unsigned char flag;                     //�Ƿ���Ч���
	unsigned char resend;                   //�ط�����
	unsigned char offline;                  //�Ƿ����߷���
	unsigned char rand;											//ȡ�����
	unsigned char ackwait;						//�ȴ�Ӧ��
}_Frame;

typedef struct __Buffer
{
	//unsigned char front;	 			//ͷָ��
	//unsigned char  rear;	 			//βָ��
	unsigned char index;					//��ѯָ��
	unsigned int count;	 					//������
	_Frame  Data[QUEUE_BUF_SIZE];			//������
}QueueBUF;




void InitQueueBUF(QueueBUF *queueBuf); 						//��ʼ������
unsigned char IsQueueBufEmpty(QueueBUF *queueBuf); 				//�����Ƿ�Ϊ�գ�1��ʾΪ��
unsigned char IsQueueBufFull(QueueBUF *queueBuf);	 			//��������1��ʾ��
unsigned char EnQueueBuf(QueueBUF *queueBuf,unsigned char channel);  		//���
unsigned char DeQueueBuf(QueueBUF *queueBuf,unsigned char channel);		//����
_Frame* GetQueue_cycle(QueueBUF *queueBuf);					//��ѭ��ȡ����

_Frame* GetQueue_InQ(QueueBUF *queueBuf,unsigned char channel);			//��ȡ���ݽ�ջָ��
_Frame* GetQueue_OutQ(QueueBUF *queueBuf,unsigned char *pindex);					//��ȡ���ݳ�ջָ��


#endif
