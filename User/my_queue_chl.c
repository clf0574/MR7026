

#include <includes.h>

static unsigned int SetChannelBIT(unsigned int count,unsigned char channel);
static unsigned int ClrChannelBIT(unsigned int count,unsigned char channel);
static unsigned int FoundChannelBIT(unsigned int count,unsigned char channel);

//==============================================================================================
//
//初始化队列
void InitQueueBUF(QueueBUF *queueBuf)
{
	queueBuf->index=0;
	queueBuf->count=0;	
}

//队列是否为空
unsigned char IsQueueBufEmpty(QueueBUF *queueBuf)
{
	return queueBuf->count == 0;  /*队列无元素为空*/
}

//队列是否满
unsigned char IsQueueBufFull(QueueBUF *queueBuf)
{
	//return queueBuf->count == QUEUE_BUF_SIZE;
	return 0;
}


//获取队列进栈指针
_Frame* GetQueue_InQ(QueueBUF *queueBuf,unsigned char channel)
{
	if(channel>QUEUE_BUF_SIZE)
	{
		return NULL;
	}
	return &queueBuf->Data[channel];  	
}

//获取队列出栈指针
_Frame* GetQueue_OutQ(QueueBUF *queueBuf,unsigned char *pindex)
{
	unsigned char i;
	
	//队列为空，直接返回
	if(IsQueueBufEmpty(queueBuf))
	{
			return NULL;
	}
	
	for(i=0;i<QUEUE_BUF_SIZE;i++)
	{
			queueBuf->index++;
			if(queueBuf->index>=QUEUE_BUF_SIZE)queueBuf->index=0;
			
			if(0!=FoundChannelBIT(queueBuf->count,queueBuf->index))
			{
				*pindex=queueBuf->index;
				return &queueBuf->Data[queueBuf->index];
			}
			
		        /*
			if(queueBuf->Data[queueBuf->index].flag==1)
			{
				rt=queueBuf->index;
				queueBuf->index++;
				if(queueBuf->index>=QUEUE_BUF_SIZE)queueBuf->index=0;
				
				return &queueBuf->Data[rt];
			}
			else
			{
				queueBuf->index++;
				if(queueBuf->index>=QUEUE_BUF_SIZE)queueBuf->index=0;
			}
			*/
	}
	
	return NULL;	
}


//入队，TRUE表示成功
unsigned char EnQueueBuf(QueueBUF *queueBuf,unsigned char channel)
{
		queueBuf->count =SetChannelBIT(queueBuf->count,channel);
	  return TRUE;
}

//出队
unsigned char DeQueueBuf(QueueBUF *queueBuf,unsigned char channel)
{
		queueBuf->count =ClrChannelBIT(queueBuf->count,channel);	
		return TRUE;
	
}

static unsigned int SetChannelBIT(unsigned int count,unsigned char channel)
{
		if     (channel==0)    count |= USE_CHL_0;
	  else if(channel==1)    count |= USE_CHL_1;
	  else if(channel==2)    count |= USE_CHL_2;
	  else if(channel==3)    count |= USE_CHL_3;
	  else if(channel==4)    count |= USE_CHL_4;
	  else if(channel==5)    count |= USE_CHL_5;
	  else if(channel==6)    count |= USE_CHL_6;
	  else if(channel==7)    count |= USE_CHL_7;
	  else if(channel==8)    count |= USE_CHL_8;
	  else if(channel==9)    count |= USE_CHL_9;
	  else if(channel==10)   count |= USE_CHL_10;
	  else if(channel==11)   count |= USE_CHL_11;
	  else if(channel==12)   count |= USE_CHL_12;
	  else if(channel==13)   count |= USE_CHL_13;
	  else if(channel==14)   count |= USE_CHL_14;
	  else if(channel==15)   count |= USE_CHL_15;
	  else                   count = count ;
	
	  return count;
}

static unsigned int ClrChannelBIT(unsigned int count,unsigned char channel)
{
		if     (channel==0)    count &= ~USE_CHL_0;
	  else if(channel==1)    count &= ~USE_CHL_1;
	  else if(channel==2)    count &= ~USE_CHL_2;
	  else if(channel==3)    count &= ~USE_CHL_3;
	  else if(channel==4)    count &= ~USE_CHL_4;
	  else if(channel==5)    count &= ~USE_CHL_5;
	  else if(channel==6)    count &= ~USE_CHL_6;
	  else if(channel==7)    count &= ~USE_CHL_7;
	  else if(channel==8)    count &= ~USE_CHL_8;
	  else if(channel==9)    count &= ~USE_CHL_9;
	  else if(channel==10)   count &= ~USE_CHL_10;
	  else if(channel==11)   count &= ~USE_CHL_11;
	  else if(channel==12)   count &= ~USE_CHL_12;
	  else if(channel==13)   count &= ~USE_CHL_13;
	  else if(channel==14)   count &= ~USE_CHL_14;
	  else if(channel==15)   count &= ~USE_CHL_15;
	  else                   count = count ;
	
	  return count;
}

static unsigned int FoundChannelBIT(unsigned int count,unsigned char channel)
{
	unsigned int rt;
	
	if(channel==0)		rt=count&USE_CHL_0;
	else if(channel==1)	rt=count&USE_CHL_1;
	else if(channel==2)	rt=count&USE_CHL_2;
	else if(channel==3)	rt=count&USE_CHL_3;
	else if(channel==4)	rt=count&USE_CHL_4;
	else if(channel==5)	rt=count&USE_CHL_5;
	else if(channel==6)	rt=count&USE_CHL_6;
	else if(channel==7)	rt=count&USE_CHL_7;
	else if(channel==8)	rt=count&USE_CHL_8;
	else if(channel==9)	rt=count&USE_CHL_9;
	else if(channel==10)	rt=count&USE_CHL_10;
	else if(channel==11)	rt=count&USE_CHL_11;
	else if(channel==12)	rt=count&USE_CHL_12;
	else if(channel==13)	rt=count&USE_CHL_13;
	else if(channel==14)	rt=count&USE_CHL_14;
	else if(channel==15)	rt=count&USE_CHL_15;
	else                    rt=0;
	
	return rt;
}


