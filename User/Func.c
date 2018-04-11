//===============================================================================================
//===============================================================================================
#include <includes.h>


/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 
*	形    参：
*			 
*	返 回 值: 无
*********************************************************************************************************
*/
//设备初始化
void Dev_Init(void)
{
	uint8_t flag,err,i;
	uint8_t buf[12];
	#ifndef GET_SERIAL_FROM_CHIP_ID
	uint8_t *p;
  	#endif
	uint16_t ret;
	//===============================================================================================
	//读使用标记,如果第一次上电，初始化时间大概有3秒，所以看门狗不应该在此以前启用，以免不停复位
	flag=Read_Dev(TYPE_EE_ADDR_USED);
	if(flag!=USED_FLAG)																						//没有初始化
	{
		//开始初始化
		Write_Dev(0xff,TYPE_EE_ADDR_ADDR0);
		Write_Dev(0xff,TYPE_EE_ADDR_ADDR1);		
		//Write_Dev(DEV_MODE_PASS_THROUGH,TYPE_EE_ADDR_MODE);
		//Write_Dev(FREE1,TYPE_EE_ADDR_PROFILE);
		Write_Dev(0xb1,TYPE_EE_ADDR_TYPESH);
		Write_Dev(0x40,TYPE_EE_ADDR_TYPES);													//
    Init_Ex_Table(ALL);																					//初始化表格	
		Init_Pannel_Table();
		//Init_Scene_Table();																				//初始化场景表			
		//初始化锁表格
		Init_Lock_Table(ALL);
    Write_Dev(USED_FLAG,TYPE_EE_ADDR_USED);											//使用标记
	}
	
	//===============================================================================================
	//读出参数
	Dev.addr[0]=Read_Dev(TYPE_EE_ADDR_ADDR0);
	Dev.addr[1]=Read_Dev(TYPE_EE_ADDR_ADDR1);
	Dev.mode=Read_Dev(TYPE_EE_ADDR_MODE);
	//Dev.profile=Read_Dev(TYPE_EE_ADDR_PROFILE);
	//if(Dev.profile!=IT1)Dev.profile=FREE1;
	Dev.profile=IT1;		
	ret=Read_Dev(TYPE_EE_ADDR_TYPESH);
	ret=ret<<8;
	Dev.type=ret+Read_Dev(TYPE_EE_ADDR_TYPES);
		
  #ifdef GET_SERIAL_FROM_CHIP_ID
	for(i=0;i<8;i++)
	{
		IT2_INFO[i]=buf[i];
	}		
	IT2_INFO[6]=Crc8CheckSum(buf,12);  							//测试
	IT2_INFO[7]=Crc8CheckSum(&buf[8],4);
	#else
	ret=Read_RomID(&IT2_INFO[0]);
	#endif		
	//================================================================================================
	//读出转发表
	if((Dev.type&0xf000)==0xb000)
	{
				err=Read_Ex_Table();
				if(err==RE_ERR)Dev.err|=ERR_EEPROM;
				for(i=0;i<EX_TABLE_NUM;i++)
				{
					Ex_Table.reg[i]=0;
					Ex_Table.stu[i]=0;
					Ex_Table.rtdat[i]=0;
					Ex_Table.SyncNum[i]=0;
				}
				Ex_Table.index=0;
	}
	//如果是锁转发器，则读锁表
	else if((Dev.type&0xf000)==0x3000)
	{
				err=Read_Lock_Table();
		    for(i=0;i<8;i++)lock.randkey[i]=IT2_INFO[i]^Dev.rand;
				if(err==RE_ERR)Dev.err|=ERR_EEPROM;				
	}
	
	//==============================================================================================
	//读出按键表格
	Read_Pannel_Table();																		//读取表格
	//===============================================================================================
	//读场景表
	//for(i=0;i<EX_TABLE_NUM;i++)Read_Scene(i);
	//===============================================================================================
	//清空无线发送队列，清空485发送队列
	InitQueueBUF(&RS485_Queue);
	InitQueueBUF(&WirelessSend_Queue);
	//清空串口发送结构和接收结构
	for(i=0;i<RX_MAX;i++)RxDat.buf[i]=0;
	RxDat.cnt=0;
	for(i=0;i<TX_MAX;i++)TxDat.buf[i]=0;
	TxDat.cnt=0;	
	//================================================================================================
	//变量初始化
	Dev.stu=DEV_REGISTER;
	RegTimeClr();	
	Dev.err=0x00;
	Dev.power_time=0;
	Bus.distime=100;	
	Get_SerialNum(&buf[0]);														
	Dev.rand=Crc8CheckSum(buf,12);
	

	//无线主机地址
	Ex_Table.addr[0]=Crc8CheckSum(IT2_INFO,8);
	Ex_Table.addr[1]=Crc8CheckSum(IT2_INFO,12);
	Ex_Table.addr[2]=Crc8CheckSum(&IT2_INFO[4],8);		
	Ex_Table.addr[3]=0x01;
	
			
	//修改2017.3.30
	//源地址
	for(i=0;i<4;i++)Ir.TgAddr[i]=Ex_Table.addr[i];				//对应源地址
	for(i=0;i<CHL_NUM;i++)DlyTable.item[i].flag=0;
	
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 
*	形    参：
*			 
*	返 回 值: 无
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 
*	形    参：
*			 
*	返 回 值: 无
*********************************************************************************************************
*/
//自发自收检测
//uint8_t Check_SelfSend(void)
uint8_t Check_SelfSend(uint8_t *pt,uint8_t cnt)
{
		uint8_t i,flag;
    
		 flag=0;
		 if(cnt==RxDat.cnt)
		 {			 
		     flag=1;
			   for(i=0;i<RxDat.cnt;i++)
			   {
					  if(*(pt+i)!=RxDat.buf[i])
					  {
					   		flag=0;
							break;
					  }
			   }
		 }
		 else if(RxDat.cnt>cnt)
		 {			 
			   flag=1;
			   for(i=0;i<cnt;i++)
			   {
					  if(*(pt+i)!=RxDat.buf[i])
					  {
					   		flag=0;
							  break;
					  }
			   }
				 if(flag==1)
				 {
					  for(i=0;i<(30-cnt);i++)
					  {
						    RxDat.buf[i]=RxDat.buf[i+cnt];
						}
						RxDat.cnt=4;
						flag=2;
				 }
				 
		 }
		 else
		 {
				//Led_set(0,15,30,1,2);//test 2016.3.15
		 }
		 return flag;
}

/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 
*	形    参：
*			 
*	返 回 值: 无
*********************************************************************************************************
*/
//取随机数
uint8_t GetRandTime(void)
{
 	uint8_t buf[12],sum,i;
	Get_SerialNum(&buf[0]);
	sum=0;
	for(i=0;i<12;i++)sum=sum+buf[i];
	
 	return (40+((Dev.rand+sum)%40));

}

/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 取得STM单片机唯一串号
*	形    参：
*			 
*	返 回 值: 无
*********************************************************************************************************
*/

void Get_SerialNum(uint8_t *pt)
{
	  uint32_t IDbuf[3];
	  uint8_t i;
	  
    IDbuf[0] = *(uint32_t*)(0x1FFFF7E8);
    IDbuf[1] = *(uint32_t*)(0x1FFFF7EC);
    IDbuf[2] = *(uint32_t*)(0x1FFFF7F0);
	
	 for(i=0;i<3;i++)
	 {
				 *(pt+i*4+0)=(uint8_t)((IDbuf[i]>>24)&0x000000ff);
				 *(pt+i*4+1)=(uint8_t)((IDbuf[i]>>16)&0x000000ff);;
				 *(pt+i*4+2)=(uint8_t)((IDbuf[i]>>8)&0x000000ff);;
				 *(pt+i*4+3)=(uint8_t)((IDbuf[i])&0x000000ff);
	 }

}

/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 
*	形    参：
*			 
*	返 回 值: 无
*********************************************************************************************************
*/
//CRC校验
uint8_t  Crc8CheckSum(uint8_t *ptr,uint8_t length)
{
  unsigned char i;
  unsigned char Crc8=0;
  while(length--)
  {
    //g(x)=x8+x5+x4+1
    for(i=0x80; i; i>>=1)
    {
      if((Crc8&0x80)!=0)
      {
        Crc8<<=1;
        Crc8^=0x31;
      }
      else
      {
        Crc8<<=1;
      }
      if(*ptr&i)
      {
        Crc8^=0x31;
      }
    }
    ptr++;
  }
  return Crc8;
}


//=====================================================================================================
//初始化转发表
void Init_Ex_Table(uint8_t st)
{
			if(st==ALL)
			{
						uint8_t i;
						for(i=0;i<EX_TABLE_NUM;i++)
						{
								Exchangebuf.type=0xff;
								Exchangebuf.addr=0xff;
								Exchangebuf.stu=0xff;
								Exchangebuf.wl_addr[0]=0xff;
								Exchangebuf.wl_addr[1]=0xff;
								Exchangebuf.wl_addr[2]=0xff;
								Exchangebuf.channel=0xff;
							  Exchangebuf.sync=0;
								Exchangebuf.secret=0xff;
								Write_Ex_Table(i);	
						}
			}
			else
			{
						if(st>=EX_TABLE_NUM)return;
						Exchangebuf.type=0xff;
						Exchangebuf.addr=0xff;
						Exchangebuf.stu=0xff;
						Exchangebuf.wl_addr[0]=0xff;
						Exchangebuf.wl_addr[1]=0xff;
						Exchangebuf.wl_addr[2]=0xff;
						Exchangebuf.channel=0xff;
						Exchangebuf.sync=0;
						Exchangebuf.secret=0xff;
						Write_Ex_Table(st);	
			}
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 
*	形    参：
*			 
*	返 回 值: 无
*********************************************************************************************************
*/

uint8_t Write_Ex_Table(uint8_t index)
{
	uint8_t i,sum,*pt,buf[EX_ITEM_NUM],err,cnt;
	
	if(index>=EX_TABLE_NUM)return RE_ERR;
	
	//=====================================================
	//上锁
	
	Ex_Table.Exchange[index].type=Exchangebuf.type;
	Ex_Table.Exchange[index].addr=Exchangebuf.addr;
	Ex_Table.Exchange[index].stu=Exchangebuf.stu;
	Ex_Table.Exchange[index].wl_addr[0]=Exchangebuf.wl_addr[0];
	Ex_Table.Exchange[index].wl_addr[1]=Exchangebuf.wl_addr[1];
	Ex_Table.Exchange[index].wl_addr[2]=Exchangebuf.wl_addr[2];
	Ex_Table.Exchange[index].channel=Exchangebuf.channel;
	Ex_Table.Exchange[index].sync=Exchangebuf.sync;
	Ex_Table.Exchange[index].secret=Exchangebuf.secret;

	pt=(uint8_t *)(Ex_Table.Exchange+index);
    sum=0;	
	for(i=0;i<EX_ITEM_NUM-1;i++)
	{
		buf[i]=*(pt+i);
		sum=sum+buf[i];
	}
	buf[EX_ITEM_NUM-1]=sum;
	
	cnt=2;
	while(cnt>0)
	{
		I2C_WP_SET(0);
		err=WriteDataToFlash(EX_TABLE_ADDR+index*(EX_ITEM_NUM),&buf[0],EX_ITEM_NUM);
		err=WriteDataToFlash(EX_TABLE_ADDR_BCK+index*(EX_ITEM_NUM),&buf[0],EX_ITEM_NUM);
		I2C_WP_SET(1);
		cnt--;
		if(err==0)break;
	}
	
    //====================================================
	//解锁
	if(err==0)return RE_OK;
 	else      return RE_ERR;
}

 /*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 
*	形    参：
*			 
*	返 回 值: 无
*********************************************************************************************************
*/
//读取转发表数据
uint8_t Read_Ex_Table(void)
{
	uint8_t i,j,sum,buf[EX_ITEM_NUM],err,cnt=0;
	uint8_t *pt;
		
  err=0;
	for(i=0;i<EX_TABLE_NUM;i++)
	{	
		  cnt=2;
		  while(cnt>0)
			{	
				err=ReadDataFromFlash(EX_TABLE_ADDR+i*(EX_ITEM_NUM),&buf[0],EX_ITEM_NUM);
				cnt--;
				if(err==0)break;
			}
		  sum=0;
		  for(j=0;j<EX_ITEM_NUM-1;j++)sum=sum+buf[j];
		  if(sum==buf[EX_ITEM_NUM-1])											//校验通过，装入转发表
			{
				  pt=(uint8_t *)(Ex_Table.Exchange+i);
				  for(j=0;j<EX_ITEM_NUM;j++)*(pt+j)=buf[j];
			}
			else																						  //校验不通道，读备份区
			{
				  cnt=2;
				  while(cnt>0)
					{
						err=ReadDataFromFlash(EX_TABLE_ADDR_BCK+i*(EX_ITEM_NUM),&buf[0],EX_ITEM_NUM);
						cnt--;
						if(err==0)break;
					}
					sum=0;
					for(j=0;j<EX_ITEM_NUM-1;j++)sum=sum+buf[j];
					
					if(sum==buf[EX_ITEM_NUM-1])											//校验通过，装入转发表
					{				
							 pt=(uint8_t *)(Ex_Table.Exchange+i);
							 for(j=0;j<EX_ITEM_NUM;j++)*(pt+j)=buf[j];
							 Write_Ex_Table(i);
            					
				  }
					else																							//还是不等于，没辙了，清0
					{
							Exchangebuf.type=0xff;
							Exchangebuf.addr=0xff;
							Exchangebuf.stu=0xff;
							Exchangebuf.wl_addr[0]=0xff;
							Exchangebuf.wl_addr[1]=0xff;
							Exchangebuf.wl_addr[2]=0xff;
							Exchangebuf.channel=0xff;
						  Exchangebuf.sync=0;
							Exchangebuf.secret=0xff;
							Write_Ex_Table(i);						  
					}
			}
	}
	if(err!=0)return RE_ERR;
	else      return RE_OK;
}
//======================================================================================================
//初始化面板配置表
void Init_Pannel_Table(void)
{
 	    uint8_t i,j;
 	    for(i=0;i<KEY_NUM;i++)
 	    {
					
				  if((Dev.err & ERR_RESET)==0x00)IWDG_Feed();
				
 				 	for(j=0;j<8;j++)Pannel.item[i].irflagbuf[j]=0;				//标记，表示是否使用
				  for(j=0;j<8;j++)Pannel.item[i].onoffbuf[j]=0;				//标记，表示是否使用
 					//Pannel.item[i].type=0;															//类型 设备 场景 序列 反向
 					//Pannel.item[i].tg_id=0;															//目标地址
 					//Pannel.item[i].datalen=0;														//数据字节长度
 					Write_Pannel_Table(i);	
 			}
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 
*	形    参：
*			 
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t Write_Pannel_Table(uint8_t index)
{

 	uint8_t i,sum,*pt,buf[KEY_ITEM_NUM],err,cnt;
 	
 	if(index>=KEY_NUM)return RE_ERR;
 	
 	//=====================================================
 	//上锁
 	
 	pt=(uint8_t *)(Pannel.item+index);
  sum=0;	
 	for(i=0;i<KEY_ITEM_NUM-1;i++)
 	{
 		buf[i]=*(pt+i);
 		sum=sum+buf[i];
 	}
 	buf[KEY_ITEM_NUM-1]=sum;
 	
 	cnt=2;
 	while(cnt>0)
 	{
 		I2C_WP_SET(0);
 		err=WriteDataToFlash(EE_PANNEL_ADDR+index*(KEY_ITEM_NUM),&buf[0],KEY_ITEM_NUM);
 		err=WriteDataToFlash(EE_PANNEL_ADDR_BCK+index*(KEY_ITEM_NUM),&buf[0],KEY_ITEM_NUM);
 		I2C_WP_SET(1);
 		cnt--;
 		if(err==0)break;
 	}
 	
   //====================================================
 	 //解锁
 	if(err==0)return RE_OK;
  else      return RE_ERR;

}	


 /*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 
*	形    参：
*			 
*	返 回 值: 无
*********************************************************************************************************
*/
//读取面板配置数据

uint8_t Read_Pannel_Table(void)
{
 	uint8_t i,j,sum,buf[KEY_ITEM_NUM],err,cnt=0;
 	uint8_t *pt;
 		
   err=0;
 	for(i=0;i<KEY_NUM;i++)
 	{	
 		  cnt=2;
 		  while(cnt>0)
 			{	
 				err=ReadDataFromFlash(EE_PANNEL_ADDR+i*(KEY_ITEM_NUM),&buf[0],KEY_ITEM_NUM);
 				cnt--;
 				if(err==0)break;
 			}
 		  sum=0;
 		  for(j=0;j<KEY_ITEM_NUM-1;j++)sum=sum+buf[j];
 		  if(sum==buf[KEY_ITEM_NUM-1])											//校验通过，装入转发表
 			{
 				  pt=(uint8_t *)(Pannel.item+i);
 				  for(j=0;j<KEY_ITEM_NUM;j++)*(pt+j)=buf[j];
 			}
 			else																						  //校验不通道，读备份区
 			{
 				  cnt=2;
 				  while(cnt>0)
 					{
 						err=ReadDataFromFlash(EE_PANNEL_ADDR_BCK+i*(KEY_ITEM_NUM),&buf[0],KEY_ITEM_NUM);
 						cnt--;
 						if(err==0)break;
 					}
 					sum=0;
 					for(j=0;j<KEY_ITEM_NUM-1;j++)sum=sum+buf[j];
 					
 					if(sum==buf[KEY_ITEM_NUM-1])											//校验通过，装入转发表
 					{				
 						 pt=(uint8_t *)(Pannel.item+i);
 						 for(j=0;j<KEY_ITEM_NUM;j++)*(pt+j)=buf[j];
						 Write_Pannel_Table(i);													//2017.0405修改
             					
 				  }
 					else																							//还是不等于，没辙了，清0
 					{
 							for(j=0;j<8;j++)Pannel.item[i].irflagbuf[i]=0x00;
						  for(j=0;j<7;j++)Pannel.item[i].onoffbuf[i]=0x00;
 							//Pannel.item[i].type=0x00;
 							//Pannel.item[i].tg_id=0x00;
 							//Pannel.item[i].datalen=0x00;
 							//Write_Pannel_Table(i);
						  Write_Pannel_Table(i);											 //2017.4.05
 					}
 			}
 	}
 	if(err!=0)return RE_ERR;
 	else      return RE_OK;
}


	


//=====================================================================================================
//初始化转发表
void Init_Scene_Table(void)
{
	    uint8_t i,j;
	    for(i=0;i<EX_TABLE_NUM;i++)
	    {
				   for(j=0;j<50;j++)
					 {
							 Scene[i].item[j].flag=0;
							 Scene[i].item[j].id=0;
							 Scene[i].item[j].chl=0;
							 Scene[i].item[j].dat[0]=0;
							 Scene[i].item[j].dat[1]=0;
							 Scene[i].item[j].dat[2]=0;
							 Scene[i].item[j].dlytime=0;
							 Scene[i].item[j].sum=0;
							 Write_Scene(i,j);
					 }
			}
}

 /*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 
*	形    参：
*			 
*	返 回 值: 无
*********************************************************************************************************
*/
//保存场景
void Write_Scene(uint8_t dev_index,uint8_t sc_index)
{
	            uint8_t i,cnt;
				uint8_t err,sum,buf[SCENE_ITEM_NUM];
				uint8_t *pt;
				uint16_t start_addr;
                											//取得设备对应的地址
				start_addr=EE_DEV_SCENE_ADDR+dev_index*DEV_SCENE_SIZE+SCENE_ITEM_NUM*sc_index;															//取得指定场景号的地址
				pt=(uint8_t *)(Scene[dev_index].item+sc_index);
	   
				sum=0;	
				for(i=0;i<SCENE_ITEM_NUM-1;i++)
				{
							buf[i]=*(pt+i);
							sum=sum+buf[i];
				}
				buf[SCENE_ITEM_NUM-1]=sum;
				
				cnt=2;
				while(cnt>0)
				{
							I2C_WP_SET(0);
							err=WriteDataToFlash(start_addr,&buf[0],SCENE_ITEM_NUM);
							err=WriteDataToFlash(start_addr+SCENE_BCK_OFFSET,&buf[0],SCENE_ITEM_NUM);
							I2C_WP_SET(1);
							cnt--;
							if(err==0)break;
				}
}

 /*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 
*	形    参：
*			 
*	返 回 值: 无
*********************************************************************************************************
*/
//读场景数据
uint8_t Read_Scene(uint8_t dev_index)
{
	uint8_t i,j,sum,buf[SCENE_ITEM_NUM],err,cnt=0;
	uint8_t *pt;
	
	uint16_t start_addr;

  //dev_index=0;
	start_addr=EE_DEV_SCENE_ADDR+DEV_SCENE_SIZE*dev_index;	
		
    err=0;
	for(i=0;i<DEV_SCENE_NUM;i++)																											//一共50个场景
	{	
		  cnt=2;
		  while(cnt>0)
			{	
				err=ReadDataFromFlash(start_addr+i*SCENE_ITEM_NUM,&buf[0],SCENE_ITEM_NUM);
				cnt--;
				if(err==0)break;
			}
		  sum=0;
		  for(j=0;j<SCENE_ITEM_NUM-1;j++)sum=sum+buf[j];
		  if(sum==buf[SCENE_ITEM_NUM-1])											//校验通过，装入转发表
			{
				  pt=(uint8_t *)(Scene[dev_index].item+i);
				  for(j=0;j<SCENE_ITEM_NUM;j++)*(pt+j)=buf[j];
			}
			else																						  			//校验不通道，读备份区
			{
				  cnt=2;
				  while(cnt>0)
					{
						err=ReadDataFromFlash(start_addr+i*SCENE_ITEM_NUM+SCENE_BCK_OFFSET,&buf[0],SCENE_ITEM_NUM);
						cnt--;
						if(err==0)break;
					}
					sum=0;
					for(j=0;j<SCENE_ITEM_NUM-1;j++)sum=sum+buf[j];
					
					if(sum==buf[SCENE_ITEM_NUM-1])											//校验通过，装入转发表
					{				
							 pt=(uint8_t *)(Scene[dev_index].item+i);
							 for(j=0;j<SCENE_ITEM_NUM;j++)*(pt+j)=buf[j];
							 Write_Scene(dev_index,i);
            					
				  }
					else																								//还是不等于，没辙了，清0
					{
							 Scene[dev_index].item[i].flag=0;
						   Scene[dev_index].item[i].id=0;
						   Scene[dev_index].item[i].chl=0;
						   Scene[dev_index].item[i].dat[0]=0;
						   Scene[dev_index].item[i].dat[1]=0;
						   Scene[dev_index].item[i].dat[2]=0;
						   Scene[dev_index].item[i].dlytime=0;
						   Scene[dev_index].item[i].sum=0;
							 Write_Scene(dev_index,i);					  
					}
			}
	}
	if(err!=0)return RE_ERR;
	else      return RE_OK;
}



 /*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 
*	形    参：
*			 
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t Write_Dev(uint8_t dat,uint8_t type)
{
	uint8_t i,err,cnt,buf[4],addr;
	
	if(type==TYPE_EE_ADDR_USED)        addr=EE_DEV_USED;
	else if(type==TYPE_EE_ADDR_ADDR0)  addr=EE_DEV_ADDR0;
	else if(type==TYPE_EE_ADDR_ADDR1)  addr=EE_DEV_ADDR1;
	else if(type==TYPE_EE_ADDR_MODE)   addr=EE_DEV_MODE;
	else if(type==TYPE_EE_ADDR_PROFILE)addr=EE_DEV_PROFILE;
	else if(type==TYPE_EE_ADDR_TYPES)  addr=EE_DEV_TYPES;
	else if(type==TYPE_EE_ADDR_TYPESH)  addr=EE_DEV_TYPESH;
	else                               return RE_ERR;
	
	cnt=2;
	for(i=0;i<3;i++)buf[i]=dat;
	
	while(cnt>0)
	{
		I2C_WP_SET(0);
		err=WriteDataToFlash(addr,&buf[0],3);
		I2C_WP_SET(1);
		cnt--;
		if(err==0)return RE_OK;
	}
	return RE_ERR;
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 
*	形    参：
*			 
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t Read_Dev(uint8_t type)
{
	 uint8_t err,cnt,buf[3],addr,dat;
	  
	if(type==TYPE_EE_ADDR_USED)        addr=EE_DEV_USED;
	else if(type==TYPE_EE_ADDR_ADDR0)  addr=EE_DEV_ADDR0;
	else if(type==TYPE_EE_ADDR_ADDR1)  addr=EE_DEV_ADDR1;
	else if(type==TYPE_EE_ADDR_MODE)   addr=EE_DEV_MODE;
	else if(type==TYPE_EE_ADDR_PROFILE)addr=EE_DEV_PROFILE;
	else if(type==TYPE_EE_ADDR_TYPES)  addr=EE_DEV_TYPES;
	else if(type==TYPE_EE_ADDR_TYPESH)  addr=EE_DEV_TYPESH;
	else                               return RE_ERR;
	
	  cnt=2;
        while(cnt>0)
	{
		   err=ReadDataFromFlash(addr,&buf[0],3);
		  cnt--;
		  if(err==0)break;
	}
	
	if((buf[0]==buf[1])&&(buf[1]==buf[2])&&(buf[0]==buf[2]))
	{
		  return buf[0];
	}
	else 
	{
		  if(buf[0]==buf[1])
			{
				     Write_Dev(buf[0],type);
				     dat=buf[0];
			}
			else if(buf[0]==buf[2])
			{
				     Write_Dev(buf[0],type);
				     dat=buf[0];
			}
			else if(buf[1]==buf[2])
			{
				     Write_Dev(buf[1],type);
				     dat=buf[1];
			}
			else
			{
			             Write_Dev(0xff,type);
				     dat=RE_ERR;
			}
	}	  
	return dat;	
}

//======================================================================================================
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 
*	形    参：
*			 
*	返 回 值: 无
*********************************************************************************************************
*/
void Msg_hanndle(void)
{
	
#if 0
			static uint8_t tick=0;
			uint8_t cmd,index,type;
			uint16_t type_cmd=0;
			MSG_T mymsg;
			uint8_t rt;
	    tick++;
	    if(tick<5)return;
			tick=0;
			rt=bsp_GetMsg(&mymsg);
			if(rt==1)
			{
					type_cmd=mymsg.MsgCode;
					type=(uint8_t)(type_cmd>>8);
					cmd=(uint8_t)(type_cmd&0x00ff);
					index=(uint8_t)mymsg.MsgParam;
					if(type==0x01)
					{
							IT1_Turn_DataLoad(index,cmd,0);
					}
					else
					{
							IT1_Key_DataLoad(index,cmd,0);
					}
			}
#else
			static uint8_t tickbuf[8]={0,0,0,0,0,0,0,0};
			uint8_t cmd,index,type,i;
			uint16_t type_cmd=0;
			MSG_T mymsg;
			uint8_t rt;
			rt=bsp_GetMsg(&mymsg);
			if(rt==1)
			{
					type_cmd=mymsg.MsgCode;
					type=(uint8_t)(type_cmd>>8);
					cmd=(uint8_t)(type_cmd&0x00ff);
					index=(uint8_t)mymsg.MsgParam;
				
				  //处理旋转遥控器
					if(type==0x01)
					{
							tickbuf[index]=7; 
							//IT1_Turn_DataLoad(index,cmd,0);
					}
					else
					{
							IT1_Key_DataLoad(index,cmd,0);
					}
			}
			
			//=======================================================================
			for(i=0;i<8;i++)
			{
					if(tickbuf[i]>0)tickbuf[i]--;
				  if(tickbuf[i]==1)
					{
						  IT1_Turn_DataLoad(i,7,0);
					}
				
			}
			
#endif
}
//======================================================================================================
//用于IR数据发下测试
//数据分包和数据组包
void Ir_SetupLoad_Test(void)
{
						uint8_t i;
						uint8_t chksum;
						Ir.cmd_key=0xfe;																	//键值
						//复制遥控器类型
	          Ir.typelen=99;																		//遥控器类型长度
	          chksum=0;
						for(i=0;i<Ir.typelen;i++)
						{
									Ir.typebuf[i]=i;
							    chksum=chksum+Ir.typebuf[i];
						}
						Ir.typebuf[Ir.typelen]=chksum;										//累加和
						Ir.typelen=Ir.typelen+1;
						//复制按键数据
						Ir.keylen=99;
						chksum=0;
						if(Ir.keylen>IR_DAT_MAX)Ir.keylen=IR_DAT_MAX;
						for(i=0;i<Ir.keylen;i++)
						{
									Ir.keybuf[i]=i+0x80;
									chksum=chksum+Ir.keybuf[i];
						}
            Ir.keybuf[Ir.keylen]=chksum;										//累加和
						Ir.keylen=Ir.keylen+1;
						
						Ir.flag=1;																	//表示数据装入完毕
						Ir.wait=0;
						Ir.curps=0;
						Ir.serial=0;
						Ir.curtype=0;																		//先发送数据
						Ir.sendtime=SEND_TIME_MAX;									//最大发送时间	
}

//======================================================================================================
void Pannel_Table_Refresh(uint8_t index,uint8_t serial,uint8_t *pdat)
{
				  uint8_t i,j/*,k*/;
//	        uint8_t flag;
//					if(index==0xe0)i=0;	
//					else					 i=32;
	
//					for(j=0;j<4;j++)
//					{
//							flag=*(pdat+j);
//							for(k=0;k<8;k++)
//							{
//									if((flag&0x01)==0x01)
//									{
//												Pannel.item[i].flag=0x01;				//使用标记
//												//Pannel.item[i].type=0x01;				//默认只单击																 
//									}
//									else
//									{
//												Pannel.item[i].flag=0x00;					//使用标记
//												//Pannel.item[i].type=0x00;				//默认只单击	
//									}
//									Write_Pannel_Table(i);									//键值
//									i++;
//									flag=flag>>1;
//							}
//					}


						if(serial==0xe0)i=0;	
						else					  i=4;
					
						for(j=0;j<4;j++)
						{
								Pannel.item[index].irflagbuf[i+j]=*(pdat+j);
						}
						
						Write_Pannel_Table(index);									//键值
						
}

//=====================================================================================================
//====================================================================================================
//注册间隔常量清零
void RegTimeClr(void)
{
	Reg.cnt=3;
	Reg.time=0;
	Reg.wait=0;
	Reg.fullflag=0;
	//Reg.flashflag=0;
}

//=====================================================================================================
void Remote_Key_En(uint8_t index,uint8_t st)
{
			  uint8_t tmp=0,lin,row,i;
				
				if(Ex_Table.Exchange[index].type==EX_KEY_54KEY)			tmp=58;
				else if(Ex_Table.Exchange[index].type==EX_KEY_42KEY)tmp=46;
				else if(Ex_Table.Exchange[index].type==EX_KEY_TURN)	tmp=7;
	      else if(Ex_Table.Exchange[index].type==EX_DEV_SUN)	tmp=1;
	      else if(Ex_Table.Exchange[index].type==EX_DEV_SUN1)	tmp=1;
				//else                                              	tmp=0;
				else                                                tmp=st;
	
	      if((st==0)||((st>tmp)&&(st<255)))return;   
	      if(st!=0xff)
				{
						st=st-1;
						lin=st/8;
						row=st%8;
						Pannel.item[index].onoffbuf[lin] |=0x01<<row;
				}
				else
				{
						for(i=0;i<7;i++)Pannel.item[index].onoffbuf[i] =0xff;
				}
				Write_Pannel_Table(index);				
}

//=======================================================================================================
//========================================================================================
void Remote_Key_Dis(uint8_t index,uint8_t st)
{
	
		    uint8_t tmp=0,lin,row,i;
				
				if(Ex_Table.Exchange[index].type==EX_KEY_54KEY)			tmp=58;
				else if(Ex_Table.Exchange[index].type==EX_KEY_42KEY)tmp=46;
				else if(Ex_Table.Exchange[index].type==EX_KEY_TURN)	tmp=7;
	      else if(Ex_Table.Exchange[index].type==EX_DEV_SUN)	tmp=1;
				else if(Ex_Table.Exchange[index].type==EX_DEV_SUN1)	tmp=1;
				//else                                              tmp=0;
				else                                                tmp=st;
	
	      if((st==0)||((st>tmp)&&(st<255)))return;
	
	      if(st!=0xff)
				{
						st=st-1;
						lin=st/8;
						row=st%8;
						Pannel.item[index].onoffbuf[lin] &=~(0x01<<row);
				}
				else
				{
						for(i=0;i<7;i++)Pannel.item[index].onoffbuf[lin] =0x00;
				}
				Write_Pannel_Table(index);	
}

//=====================================================================================================
//index表示转发器的通道
void Ir_Config_Add(uint8_t index,uint8_t key,uint8_t mode)
{
		 uint8_t t1,t2;
	   t1=Ir.cmd_key/8;
	   t2=Ir.cmd_key%8;
	   Pannel.item[index].irflagbuf[t1] |=(1<<t2);
		 Write_Pannel_Table(index);
}

//=====================================================================================================
//index表示转发器的通道
void Ir_Config_Del(uint8_t index,uint8_t key,uint8_t mode)
{
		 uint8_t t1,t2;
	   t1=Ir.cmd_key/8;
	   t2=Ir.cmd_key%8;
	   Pannel.item[index].irflagbuf[t1] &=~(1<<t2);
		 Write_Pannel_Table(index);
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 
*	形    参：
*			 
*	返 回 值: 无
**********************************************************************************************************/
void DlyEvent_handle(uint8_t channel,uint8_t type,uint8_t dat,uint16_t time)
{

	  
		DlyTable.item[channel].flag=1;
		DlyTable.item[channel].index=type;
		DlyTable.item[channel].dat=dat;
	
	  if(time>10000)time=10000;
	  if(time==0)time=1;
		bsp_StartTimer(channel,(uint32_t)time);
}



//=========================================================================================================
//====================================================================================
//====================================================================================
//按表格中的秘钥解密数据包
void ComposePacket(unsigned char *pdat,unsigned char len,unsigned char index)
{
	unsigned char key;
	key=Ex_Table.Exchange[index].secret;
	InfoDecode(pdat,len,key);												
}
//===================================================================================
//====================================================================================
//====================================================================================
//新数据包，把数据放入DS28E02，然后计算返回数据
//st==0:解密
//st==1:加密
void ComposePacketFromDS28E02(unsigned char *pid,unsigned char *pdat,unsigned char len,unsigned char st)
{
	unsigned char buf[4],i;
	unsigned char TKEY;
	for(i=0;i<4;i++)buf[i]=*(pid+i);
	buf[3]&=0xf0;							
	TKEY=GetSecret(&buf[0],4);
	if(st==0)
	{
			InfoDecode(pdat,len,TKEY);	
	}
	else
	{
			InfoEncode(pdat,len,TKEY);
	}
}

//===================================================================================
//信息加密
void InfoEncode(unsigned char *pdat,unsigned char len,unsigned char key)
{
	unsigned char i,tmp;

    tmp=*(pdat+1);
    *(pdat+0)=*(pdat+0)^tmp;
		for(i=2;i<len;i++) 
		{
				*(pdat+i)=*(pdat+i)^tmp;
		}
		*(pdat+1)=*(pdat+1)^key;
}
//===================================================================================
//信息解密
void InfoDecode(unsigned char *pdat,unsigned char len,unsigned char key)
{
		unsigned char i,tmp;
    *(pdat+1)=*(pdat+1)^key;
    tmp=*(pdat+1);

    *(pdat+0)=*(pdat+0)^tmp;
		for(i=2;i<len;i++)
		{
				*(pdat+i)=*(pdat+i)^tmp;
		}
}
//====================================================================================
//比较同步字
#define  SYNC_MAX  0xcfff
#define  SYNC_DIFF  100
unsigned char CompareSync(unsigned char *pdat,unsigned char index)
{
		unsigned int t_sync1,t_sync2;

		t_sync1=Ex_Table.Exchange[index].sync;
		t_sync2=pdat[0]*256 +pdat[1];

		if(t_sync2>SYNC_MAX)return OPT_ERR;

		t_sync1&=0x3fff;
    t_sync2&=0x3fff;
    if(t_sync2>t_sync1)
		{		
				Ex_Table.Exchange[index].sync=t_sync2;
				Ex_Table.SyncErr[index]=0;
				return OPT_OK;
		}
		else 
		{
				if((t_sync1>(SYNC_MAX-SYNC_DIFF))&&(t_sync2<SYNC_DIFF))			//???????,????
				{
							Ex_Table.Exchange[index].sync=t_sync2;
							Ex_Table.SyncErr[index]=0;
							return OPT_OK;
				}
				else
				{
							//如果连续3次同步字加1，认为再次同步
							if((t_sync2-Dev.sync)==1)
							{
									Dev.sync=t_sync2;
									if(Ex_Table.SyncErr[index]>0)Ex_Table.SyncErr[index]--;
									if(Ex_Table.SyncErr[index]==0)
									{
												Ex_Table.Exchange[index].sync=t_sync2;
												Ex_Table.SyncErr[index]=0;
												return OPT_OK;
									}
									else
									{
												return OPT_ERR;
									}
							}
							else
							{
									Dev.sync=t_sync2;
									if(Ex_Table.SyncErr[index]<3)Ex_Table.SyncErr[index]++;									//???????
									return OPT_ERR;
							}
				}
		}
}

////====================================================================================
//得到秘钥
unsigned char GetSecret(unsigned char *pdat,unsigned char len)
{
    unsigned char t_secret;
    Secret_Verify(pdat,len);
    t_secret=*(pdat+0)+*(pdat+1);
    return t_secret;
}

//====================================================================================
void Secret_Verify(unsigned char *pdat,unsigned char len)
{

//================================================================	
#ifdef USE_DS28E02
	  unsigned char rbuf[32],datbuf[10],i,cnt,ret;

		if(len>=5)len=5;
		for(i=0;i<len;i++)datbuf[i]=*(pdat+i);
    for(i=len;i<5;i++)datbuf[i]=0x00;

		for(i=0;i<32;i++)rbuf[i]=0x00;

		cnt=3;
		while(cnt>0)
		{
				__set_PRIMASK(1);	
				ret=ANON_READ_AuthPage(&rbuf[0],&datbuf[0],0);
				__set_PRIMASK(0);	;
				if(ret==1)break;
				cnt--;
		}   

	//*(pdat+0)=Crc8Check(rbuf,20);
	*(pdat+0)=Crc8CheckSum(rbuf,20);	
	*(pdat+1)=SumCheck(rbuf,20);
//================================================================
#else
	static unsigned char buf[50],i,cnt,ret;

	for(i=0;i<len;i++)buf[i]=*(pdat+i);
	for(i=len;i<32;i++)buf[i]=0x00;
	//================================================================================

	cnt=3;
  while(cnt>0)
	{
			__set_PRIMASK(1);	
			ret=ReadWriteScratchpad((bool)0,buf);
			__set_PRIMASK(0);
			if(ret==0xaa)break;				//????,??
			cnt--;
    }
	//================================================================================= 

  cnt=3;
  while(cnt>0)
	{
			__set_PRIMASK(1);	
			ret=ComputeAndReadPageMAC((unsigned char)1,0x00,buf);									//???MAC?buf?,?32???
			__set_PRIMASK(0);
			if((ret!=0xff)&&(ret!=0x00))break;
			cnt--;
  }   

	*(pdat+0)=Crc8CheckSum(buf,32);
	*(pdat+1)=SumCheck(buf,32);
		
#endif
		
		
}
//======================================================================================================

//======================================================================================================
//对应锁的数据保存和读取
//=====================================================================================================
//初始化转发表
void Init_Lock_Table(uint8_t st)
{
			uint8_t i,j;
			if(st==ALL)
			{
							for(j=0;j<LOCK_ITEMNUM;j++)LockItem.Serail[j]=0;
						  for(j=0;j<LOCK_ITEMNUM;j++)LockItem.IdType[j]=0;
						  for(j=0;j<LOCK_ITEMNUM;j++)LockItem.UserType[j]=0;
							for(j=0;j<4;j++)LockItem.addr[j]=0xff;
							//初始密码为123456
						  for(j=0;j<6;j++)LockItem.key[j]=j+1;LockItem.key[6]=0xff;LockItem.key[7]=0xff;
				
							LockItem.secret=0;																			
							LockItem.cardnum=0; 
							LockItem.encodenum=0;
							LockItem.fingernum=0;
							LockItem.sync=0;			
							LockItem.stu=0xff;
							Write_Lock_Table(0);

							for(j=0;j<LOCK_ITEMNUM;j++)Write_Lock_Table(j+1);
              lock.SyncErr=0;
			}
			else
			{
							LockItem.Serail[st]=0;
						  LockItem.IdType[st]=0;
						  LockItem.UserType[st]=0;
							Write_Lock_Table(st+1);
			}
}
/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 
*	形    参：
*			 
*	返 回 值: 无
*********************************************************************************************************
*/

uint8_t Write_Lock_Table(uint8_t index)
{
	uint8_t j,i,sum,*pt,buf[LOCK_ALLNUM],err,cnt;
	
	if(index>=LOCK_ITEMNUM)return RE_ERR;
	
	//=====================================================
	//上锁
	
	if(index==0)
	{			
				for(j=0;j<4;j++)lock.item.addr[j]=LockItem.addr[j];
				for(j=0;j<8;j++)lock.item.key[j]=LockItem.key[j];
				lock.item.stu=LockItem.stu;
				
				lock.item.safe=LockItem.safe;	
				lock.item.secret=LockItem.secret;																			
				lock.item.cardnum=LockItem.cardnum; 
				lock.item.encodenum=LockItem.encodenum;
				lock.item.fingernum=LockItem.fingernum;
				lock.item.sync=LockItem.sync;		
	}
	else
	{
				index=index-1;
				lock.item.Serail[index]=LockItem.Serail[index];
				lock.item.IdType[index]=LockItem.IdType[index];
				lock.item.UserType[index]=LockItem.UserType[index];
				
	}
  pt=(uint8_t *)(&lock.item);	
  sum=0;	
	for(i=0;i<LOCK_ALLNUM-1;i++)
	{
			buf[i]=*(pt+i);
			sum=sum+buf[i];
	}
	buf[LOCK_ALLNUM-1]=sum;
	
	cnt=2;
	while(cnt>0)
	{
		I2C_WP_SET(0);
		err=WriteDataToFlash(LOCK_TABLE_ADDR+index*(LOCK_ALLNUM),&buf[0],LOCK_ALLNUM);
		err=WriteDataToFlash(LOCK_TABLE_ADDR_BCK+index*(LOCK_ALLNUM),&buf[0],LOCK_ALLNUM);
		I2C_WP_SET(1);
		cnt--;
		if(err==0)break;
	}
	
    //====================================================
	//解锁
	if(err==0)return RE_OK;
 	else      return RE_ERR;
}

 /*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 
*	形    参：
*			 
*	返 回 值: 无
*********************************************************************************************************
*/
//读取转发表数据
uint8_t Read_Lock_Table(void)
{
	uint8_t i,j,sum,buf[LOCK_ALLNUM],err,cnt=0;
	uint8_t *pt;
		
  err=0;
	i=0;
	//for(i=0;i<LOCK_ITEMNUM;i++)
	{	
		  cnt=2;
		  while(cnt>0)
			{	
					err=ReadDataFromFlash(LOCK_TABLE_ADDR+i*(LOCK_ALLNUM),&buf[0],LOCK_ALLNUM);
					cnt--;
					if(err==0)break;
			}
		  sum=0;
		  for(j=0;j<LOCK_ALLNUM-1;j++)sum=sum+buf[j];
		  if(sum==buf[LOCK_ALLNUM-1])											//校验通过，装入转发表
			{
				  pt=(uint8_t *)(&lock.item);
				  for(j=0;j<LOCK_ALLNUM;j++)*(pt+j)=buf[j];
			}
			else																						  //校验不通道，读备份区
			{
				  cnt=2;
				  while(cnt>0)
					{
						err=ReadDataFromFlash(LOCK_TABLE_ADDR_BCK+0*(LOCK_ALLNUM),&buf[0],LOCK_ALLNUM);
						cnt--;
						if(err==0)break;
					}
					sum=0;
					for(j=0;j<LOCK_ALLNUM-1;j++)sum=sum+buf[j];
					
					if(sum==buf[LOCK_ALLNUM-1])											//校验通过，装入转发表
					{				
							 pt=(uint8_t *)(&lock.item);
							 for(j=0;j<LOCK_ALLNUM;j++)*(pt+j)=buf[j];
							 Write_Lock_Table(i);
            					
				  }
					else																							//还是不等于，没辙了，清0
					{
							Init_Lock_Table(ALL);
					}
			}
	}
	if(err!=0)return RE_ERR;
	else      return RE_OK;
}
