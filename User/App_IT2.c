
//================================================================================

#define MAIN_VARIABLE

#include <includes.h>
//==================================================================================


//==================================================================================
//模块类型     
	#define MODULE_TYPE  	0x0060
	
// 	const uint8_t IT2_INFO[8] __attribute__((at(0x0800fc00))) = {
// 	0x00,					//产品系列区分
// 	0x16,					//年
// 	0x15,					//周
// 	0x60,         //型号
// 	0xff,					//预留
// 	0xff,         //预留
// 	0x00,					//序列号-高位
// 	0x02,					//序列号-低位
// 	};

 

////====================================================================================
////传感器通道属性配置
//#define SENSOR_NUM   4
//typedef struct __SENSOR_ITEM
//{
//	uint8_t flag;
//	uint16_t type;
//	uint8_t Isnew;		//是否最新更新
//	uint16_t time;		//用于上一个按键到这次按键的时间间隔，每10mS累计一次
//	uint8_t time_en;	//是否启用
//        	
//}_SENSOR_ITEM;   
//typedef struct __SENSOR_TABLE
//{
//	uint8_t cnt;		//已配通道总数
//	_SENSOR_ITEM item[SENSOR_NUM];
//}_SENSOR_TABLE;
//_SENSOR_TABLE Sensor;		//面板按键数据库 
//
////=====================================================================================
////传感器-模拟量
//typedef struct __SENSOR_AN
//{
//	uint8_t flag;
//	uint16_t type;
//	uint8_t Isnew;		//是否最新更新
//	uint8_t dat[2];		//用于模拟量输入的数据
//	uint8_t cmd1[3];	//包含一个命令，2个自己的数据，数据有网关下发
//	uint8_t cmd2[3];	//包含一个命令，2个自己的数据，数据有网关下发
//}_SENSOR_AN;     
//_SENSOR_AN Sensor_AN;
////=====================================================================================


uint8_t  Crc8CheckSum(uint8_t *ptr,uint8_t length);
void IT2_Get_PannelType(uint8_t key,uint8_t *pt);

//=================================================================================================
//接收帧完整性验证
void IT2_Frame_availability(uint8_t *p)
{
	//uint8_t i;
	uint8_t len/*,tmp1,tmp2*/,tmp3;

	*p=OPT_OK;
	 
	//帧头判断 -帧控制域效应0x05
	if(RxDat.buf[0]!=0xa5)
	{
		*p=OPT_CANCEL; 						//大于0x05不处理
		return;
	}
	
	//网关下发广播报文，命令报文，应答报文和数据报文，第六个字节表示为数据长度
	len=RxDat.buf[5];						//
	len=len+6+1;							//从头到数据长度字节，1个CRC
	if(RxDat.cnt!=len)
	{
		*p=OPT_CANCEL; 
		return;
	}
	
	//地址判断
	if((RxDat.buf[2]!=0xff)&&(RxDat.buf[2]!=Dev.addr[0]))
	{
		*p=OPT_CANCEL; 
		return;
	}
	
	if(RxDat.buf[1]==F_ACK_DEV)
	{
		 //if(RxDat.buf[4]!=Dev.addr[2])
		 {
				 *p=OPT_CANCEL; 
				 return;
		 }
	}
	
	//CRC校验
	tmp3=Crc8CheckSum(RxDat.buf,(RxDat.cnt-1));		//计算Crc8值
	if(tmp3!=RxDat.buf[RxDat.cnt-1])			//判断Crc校验是否正确
	{
		//错误则退出
		*p=OPT_CANCEL; 
		return;
	}
}

//==================================================================================================
//RS485解析-IT第二代协议
void IT2_ParseFrame(void)
{
		uint8_t i;
		uint8_t m_cmd=0;
		//uint8_t s_cmd=0;
		uint8_t err;

		//===================================================================================
		//帧完整性检测
		IT2_Frame_availability(&err);
		
		if((err==OPT_ERR)||(err==OPT_CANCEL))                     //clf mod 2014.6.21
		{
			//错误返回
			for(i=0;i<RX_MAX;i++)RxDat.buf[i]=0;
			RxDat.cnt=0;
			return;
		}
		
		Led_set(0,30,50,2,1);
					 
		//===================================================================================
		//帧分析
		m_cmd=RxDat.buf[1];	                        //帧命令			
		//s_cmd=RxDat.buf[6];					//操作类型
		err=OPT_CANCEL;
        
        switch(m_cmd)
        {
        	//=========================================================================
        	//广播类报文处理
        	case  B_ONLINE:  		
					case  B_DIS_BUS:   		
					case  B_EN_BUS:   			
        		IT2_Function_Broadcast(&err);
        	break;
        	//==========================================================================
        	//时间类报文处理
					case  F_ASK_TIME:
					case  F_SET_TIME:			
        		IT2_Function_Time(&err);      	
        	break;
        	//===========================================================================
        	//设备版本
        	case F_ASK_VERSION:				 
		     	IT2_Function_Version(&err);				
        	break;
        	//===========================================================================
        	//设备更新
        	case F_ASK_UPDATE:
        	case F_ASK_UPDATE_START:					
        		IT2_Function_Update(&err);         	
        	break;
        	//==========================================================================
        	//模块配置
        	//case F_ASK_CONFIG_ADDR:                                                       //不会有设备向面板申请地址
        	case F_ASK_CONFIG_MODULE:                                     //修改请求
        	case F_DEL_MODULE_CONFIG:                                     //删除请求
					//case F_ACK_REQUEST_ADDR:
        		IT2_Function_Config(&err); 
        	break;
        	//========================================================================== 
        	//设备参数设置 执行
        	case F_ASK_MODIFY:
        	case F_ASK_INFO:
        	case F_SETUP_DEV_PARA:
        	case F_READ_DEV_PARA:
								IT2_Dev_Setup(&err);     
          break;
					//============================================================================
					//设置执行
					case F_ACT_DEVICE:
						    IT2_Dev_Action(&err);   
					break;
        	//==========================================================================
        	//随身贴配置和同步
        	case F_ASK_CONFIG_KEY:
        	case F_ASK_SYNC_KEY:
								IT2_Key_Setup(&err);     
          break;
                //==========================================================================
                //转发器
        	case F_ASK_EXCHANGER_DELCODE:
					case F_ASK_EXCHANGER_ADDCODE:
								IT2_Exchange_Action(&err);  
        	break;
        	//==========================================================================
        	//场景相关
					case  B_EXEC_SCENE:		
					case  B_DEL_SCENE:	
        	case F_ACT_SCENE:
        	case F_ASK_SCENE_DATA:
        	case F_ASK_SCENE_ADD:
        	case F_ASK_SCENE_DEL:
        		IT2_Scene_Action(&err);  
        	break;
        	//==========================================================================
        	//定时器相关
        	case F_ASK_TIMER_ADD:
        	case F_ASK_TIMER_EDIT:
        	case F_ASK_TIMER_DEL:
        	case F_ASK_TIMER_ONOFF:
        		IT2_Timer_Action(&err);  
        	break;
        	//==========================================================================
        	//序列相关
        	case F_ASK_ARRAY_ADD:
        	case F_ASK_ARRAY_DEL:
        		IT2_Array_Action(&err);  
        	break;
					
        	
        	//==========================================================================
        	//出错
        	default:
        		err=OPT_CANCEL;
        	break;
        }
        if(err!=OPT_CANCEL)
        {
        	IT2_Load_RS485_Ack(err);
        }
		
	for(i=0;i<RxDat.cnt;i++)RxDat.buf[i]=0;
	RxDat.cnt=0;
}

//=======================================================================================================
//用于简析广播命令报文
void IT2_Function_Broadcast(uint8_t *pt)					//广播无应答
{
	uint8_t m_cmd,i;
	*pt=OPT_CANCEL;
	
	m_cmd=RxDat.buf[1];
	
  	//===================================================================================================
	if(m_cmd==B_DIS_BUS)							//广播占用总线
	{
		Bus.distime=RxDat.buf[6]*10000;					//总线禁用时间,单位ms
	}
	else if(RxDat.buf[4]==B_EN_BUS)						//取消总线禁用时间
	{
		Bus.distime=0;							
	}
	else if(m_cmd==B_ONLINE)									//广播查询在线
	{	    	
	    	Dev.stu=DEV_REQUESTADDR;
		    for(i=0;i<9;i++)Ex_Table.reg[i]=0;
	}
// 	else if(m_cmd==B_EXEC_SCENE)							//广播场景码
// 	{
// 				//面板不响应场景码
// 	}
// 	else if(m_cmd==B_DEL_SCENE)
// 	{
// 				//面板不保存场景
// 	}
}

//=======================================================================================================
//用于简析时间报文
void IT2_Function_Time(uint8_t *pt)						//主机发送命令
{
	uint8_t m_cmd;
	
	*pt=OPT_ERR;
	
	m_cmd=RxDat.buf[1];
	
	if(m_cmd==F_ASK_TIME)							//查询本地时间
	{		       
		IT2_Load_RS485_Ack(F_ACK_TIME);
		*pt=F_ACK_TIME;
	}
	else if(m_cmd==F_SET_TIME)						//设置时间
	{
		Time.year=RxDat.buf[8];
		Time.month=RxDat.buf[9];
		Time.date=RxDat.buf[10];
		Time.hour=RxDat.buf[11];
		Time.min=RxDat.buf[12];
		Time.sec=RxDat.buf[13];
		//==========================================
		//加入RTC时钟命令
		
		*pt=OPT_OK;
	}
}
//=======================================================================================================
//用于简析版本报文
void IT2_Function_Version(uint8_t *pt)						//主机发送命令
{
	uint8_t m_cmd;
	
	*pt=OPT_ERR;
	
	m_cmd=RxDat.buf[1];
	
	if(m_cmd==F_ASK_VERSION)							//查询本地时间
	{		       
		IT2_Load_RS485_Ack(F_ACK_VERSION);
		*pt=F_ACK_VERSION;
	}
}
//========================================================================================================
//用于简析程序升级部分
void IT2_Function_Update(uint8_t *pt)
{
	uint8_t m_cmd;
	
	*pt=OPT_ERR;
	
	m_cmd=RxDat.buf[1];
	
	if(m_cmd==F_ASK_UPDATE)							//查询本地时间
	{		       
		IT2_Load_RS485_Ack(F_ACK_UPDATE);
		*pt=F_ACK_UPDATE;
	}
}
//=======================================================================================================
//模块配置
void IT2_Function_Config(uint8_t *pt)
{
//     uint8_t m_cmd,i;
//     
// 	  *pt=OPT_ERR;
//     m_cmd=RxDat.buf[1];
//     if(m_cmd==F_DEL_MODULE_CONFIG)                  //删除配置信息
//         {
//               Pannel.cnt=0;
//               for(i=0;i<KEY_NUM;i++)
//                 {
//                     Pannel.item[i].flag=0;
//                     Pannel.item[i].type=0;
//                     Pannel.item[i].tg_id=0;
//                     Pannel.item[i].datalen=0;
//                     Write_Pannel_Table(i);
//                 }
//               IT2_Dev_Report(F_REPORT_MODULE_DEL,0);//模块配置删除上报
//         }
//				else if(m_cmd==F_ACK_REQUEST_ADDR)
//				{
//							if(RxDat.buf[6]==0x20)
//							{
//									rt=1;
//									for(i=0;i<RxDat.buf[7];i++)
//									{
//										if(IT2_INFO[i]!=RxDat.buf[8+i])
//										{
//													rt=0;
//													break;
//										}
//									}
//									if(rt==1)
//									{
//											Dev.addr[0]=RxDat.buf[18];
//											Write_Dev(Dev.addr[0],TYPE_EE_ADDR_ADDR0);
//											Dev.stu=DEV_REGISTER;
//										  *pt=OPT_OK;
//									}
//									else
//									{
//										   *pt=OPT_CANCEL;
//									}
//							}
//				}
}
//======================================================================================================
//设备配置
void IT2_Dev_Setup(uint8_t *pt)
{
//	uint8_t dat;
	uint8_t m_cmd;
	
	m_cmd=RxDat.buf[1];
	if(m_cmd==F_ACT_DEVICE)
	{
		
	}
	//===============================================================================================
	//修改设备类型
	else if(m_cmd==F_ASK_MODIFY)
	{
		//转发器不修改类型，通道对应类型，通过转发器添码和删除更改 2016.3.30
		if((RxDat.buf[6]==0x29)&&(RxDat.buf[9]==0x22))
		{
// 			Dev.type=(RxDat.buf[11]<<8)+RxDat.buf[12];
// 			*pt=OPT_OK;					//应答
// 			//写入EPROM
// 			dat=RxDat.buf[12]&0x0f;
// 			Write_Dev(dat,TYPE_EE_ADDR_TYPES);
// 					
// 			//修改上报
// 			IT2_Dev_Report(F_REPORT_DEV_MODIFY,0);
		}
	}
	//================================================================================================
	//设备信息请求
	else if(m_cmd==F_ASK_INFO)
	{
			*pt=OPT_OK;					//应答
		  //上报设备类型
		  IT2_Dev_Report(F_REPORT_DEV_MODIFY,0);
	}
} 

//=========================================================================================================================
//面板或传感器配置
void IT2_Key_Setup(uint8_t *pt)
{
// 	  uint8_t i,j;
// 		uint8_t key,type,len,len1,index,offset,operation; 
// 		*pt=OPT_ERR;
// 	  
// 		//配置按键
// 		if(RxDat.buf[1]==F_ASK_SYNC_KEY)																		//随身贴同步请求
// 		{			
// 					//=============================================================================
// 					//先取得要配置的按键个数
// 			    len=RxDat.cnt-2;																							//数据长度
// 			    offset=6;
// 					for(i=0;i<8;i++)																							//最多8个按键
// 					{				
// 									//======================================================================
//                   operation=RxDat.buf[offset+0];												//操作数	
//                   len1=	RxDat.buf[offset+1];														//操作数长度						
// 									key=RxDat.buf[offset+2];															//键值
// 									type=RxDat.buf[offset+3];															//设备类型为0x01~0x04	
// 									//======================================================================
// 									//检测类型是否不匹配
// 									if((type>0x04)||((type==1)&&(Dev.type==0x0403))||((type==2)&&(Dev.type<0x0403))||((type==3)&&(Dev.type!=0x0404))||((type==4)&&(Dev.type!=0x0404)))				
// 									{
// 												*pt=OPT_ERR;
// 												return;	
// 									}
// 									if(operation!=0x2d)
// 									{
// 												*pt=OPT_ERR;
// 												return;	
// 									}
// 									if(key>=8)
// 									{
// 												*pt=OPT_ERR;
// 												return;	
// 									}
// 									//=======================================================================
// 									//分析配置下来的数据
// 									index=key;																						//这里按键值就是通道0~7
//                                     if(Pannel.item[index].flag!=1)
//                                         {
//                                             Pannel.item[index].flag=1;
//                                             Pannel.cnt++;
//                                         }
// 									Pannel.item[index].type=type;													//类型
// 									if((type==1)||(type==4))Pannel.item[index].tg_id=RxDat.buf[offset+4];
// 									else       Pannel.item[index].tg_id=0xff;   
// 										
// 									//=======================================================================
// 									//类型判断，分别处理,直接把数据放入缓冲区
// 									Pannel.item[index].datalen=len1-2;											//此处长度为除去 通道 类型 设备ID后的长度
// 									for(j=0;j<Pannel.item[index].datalen;j++)
// 									{
// 												Pannel.item[index].buf[j]=RxDat.buf[offset+4+j];
// 									}	
// 									offset=offset+len1+2;
// 									//=======================================================================
// 									//放入EPROM 全部保存
// 									Write_Pannel_Table(index);
// 									//========================================================================
// 									//判断是否结束
// 									if(offset>=len)break;
// 					}	
// 					*pt=OPT_OK;
// 		}
        
}

//================================================================================================================
//场景配置或场景执行
void IT2_Scene_Action(uint8_t *pt)
{
		uint8_t m_cmd,mode,num,chl,cmd,cmd_dat[3];
	  uint16_t id,dly;
	
	  uint8_t i,j,k,offset=0,len,rt=0;
	
	 *pt=OPT_ERR;
	 m_cmd=RxDat.buf[1];
	 id=0;
	 mode=0;
	 num=0;
	 chl=0;
	 cmd=0;
	 dly=0;
	 //=============================================================================================================
	 //场景执行
	 if((m_cmd==F_ACT_SCENE)||(m_cmd==B_EXEC_SCENE))
	 {
		 id=(uint16_t)((RxDat.buf[8]<< 8)+(RxDat.buf[9]));
         mode=RxDat.buf[12];
		     rt=OPT_ERR;
                                 //查找场景号
        for(i=0;i<EX_TABLE_NUM;i++)
		{
					if(Ex_Table.Exchange[i].type<EX_DEV_KEY)									//如果是对应通道是遥控器或未使用，则不查找这个通道
					{
								for(j=0;j<DEV_SCENE_NUM;j++)
						       {
											if(Scene[i].item[j].id==id)
											{
												  rt=OPT_OK;
											    //执行场景
											    if((mode==0x01)||(Scene[i].item[j].dlytime==0))
                                                {          
                                                          IT2_Dev_To_Wireless(i,Scene[i].item[j].cmd,Scene[i].item[j].dat[0],0);																								
																													//Wireless_Dev_Action(i,Scene[i].item[j].cmd,Scene[i].item[j].dat[0],0);
                                                }
                                                else
                                                    {
													        Dly_Table.item[i].flag=1;;															//使用标记
														 	Dly_Table.item[i].index=i;															//场景号
													        Dly_Table.item[i].cmd=Scene[i].item[j].cmd;
															Dly_Table.item[i].dat[0]=Scene[i].item[j].dat[0];          //通道号    
															bsp_StartTimer(i,(uint32_t)(Scene[i].item[j].dlytime*100));
                                                    }
                                                 //break;
											}
								}
					}
		}
		
		if(m_cmd==B_EXEC_SCENE)
		{
				*pt=OPT_CANCEL;
		}
		else
		{
			  *pt=rt;
		}
    return;		
           
	 }
	 //==============================================================================================================
	 //以下场景设置 
	 
	 //场景数据请求
	 else if(m_cmd==F_ASK_SCENE_DATA)
	 {
				//把场景表格的数据，发送出去
				id=(uint16_t)((RxDat.buf[8]<< 8)+RxDat.buf[9]);
		    //查找场景号
		    for(i=0;i<EX_TABLE_NUM;i++)
				{
							if(Ex_Table.Exchange[i].type<EX_DEV_KEY)									//如果是对应通道是遥控器或未使用，则不查找这个通道
							{
										for(j=0;j<DEV_SCENE_NUM;j++)
								       {
													if(Scene[i].item[j].id==id)
													{
                                                         //装入数据
                                                         IT2_Dev_Report(F_ACK_SCENE_DATA,i);
                                                         Scene[i].num=j;
                                                         break;
													}
										}
							}
				}
				*pt=OPT_OK;
				return;
			
	 }
	 //场景添加
	 else if(m_cmd==F_ASK_SCENE_ADD)
	 {
		    if(RxDat.buf[6]!=0x40)return;
		 
				id=(uint16_t)((RxDat.buf[8]<<8 )+RxDat.buf[9]);	
		    num=RxDat.buf[12];
		    if(num==0)return;
		    offset=13;
		    //=========================================================================================================
		    //循环分析
		    for(i=0;i<num;i++)
				{
					  chl=RxDat.buf[offset+2];
					  cmd=RxDat.buf[offset+5];
					  len=RxDat.buf[offset+7];				//命令数据长度
					  if(len>5)len=5;
					  for(j=0;j<len;j++)cmd_dat[j]=RxDat.buf[offset+8+j];
					  dly=(uint16_t)((RxDat.buf[offset+10+len]<< 8)+RxDat.buf[offset+10+len+1]);//场景执行延时时间
					
					  offset=offset+10+len+2;
					
					  //如果转发器对应通道未配置或对应通道是遥控器，则错误退出
					  if(Ex_Table.Exchange[chl].type>=EX_DEV_KEY)return;
					  
					  //通道只能是1-8
					  if((chl==0)||(chl>8))return;
					  chl=chl-1;
					
					  
						//检测此场景号，是否已经保存，如果保存，则覆盖
		          rt=0xff;
					    for(j=0;j<DEV_SCENE_NUM;j++)
				      {
									if(Scene[chl].item[j].id==id)
									{
											rt=j;
										  break;
									}
						  }
							if(rt>=DEV_SCENE_NUM)																//场景ID是新的，则查找未使用的表项
							{
										//查找对应通道设备，未使用的表格项，如果已满,退出返回错误
										for(j=0;j<DEV_SCENE_NUM;j++)
										{
												if(Scene[chl].item[j].flag==0)break;
										}
										if(j==DEV_SCENE_NUM)return;
										rt=j;																					//未使用表项
							}
						
							//写入场景表
							Scene[chl].item[rt].flag=1;
							Scene[chl].item[rt].id=id;
							Scene[chl].item[rt].chl=chl;
                            Scene[chl].item[rt].cmd=cmd;
                            Scene[chl].item[rt].len=len;
							for(j=0;j<len;j++)Scene[chl].item[rt].dat[j]=cmd_dat[j];
							Scene[chl].item[rt].dlytime=dly;
							Write_Scene(chl,rt);
							
							if(offset>=(RxDat.cnt-1))break;
					
				}
				*pt=OPT_OK;
				return;
				
	 }
	 //场景删除
	 else if((m_cmd==F_ASK_SCENE_DEL)||(m_cmd==B_DEL_SCENE))
	 {
				id=(uint16_t)((RxDat.buf[8]<< 8)+RxDat.buf[9]);
		    //查找场景号
		    for(i=0;i<EX_TABLE_NUM;i++)
				{
							if(Ex_Table.Exchange[i].type<EX_DEV_KEY)									//如果是对应通道是遥控器或未使用，则不查找这个通道
							{
										for(j=0;j<DEV_SCENE_NUM;j++)
								    {
													if(Scene[i].item[j].id==id)
													{
														  //删除场景
														  Scene[i].item[j].flag=0;
															Scene[i].item[j].id=0;
															Scene[i].item[j].chl=0;
                                                            Scene[i].item[j].cmd=0;
															for(k=0;k<Scene[i].item[j].len;k++)Scene[i].item[j].dat[k]=0;
                                                            Scene[i].item[j].len=0;
                                                            Scene[i].item[j].dlytime=0;
															Write_Scene(i,j);
													}
										}
							}
				}	
        if(m_cmd==B_DEL_SCENE)*pt=OPT_CANCEL;				
				else                  *pt=OPT_OK;
        return;
	 }
	 
   *pt=OPT_CANCEL;
} 
//================================================================================================================
//定时器编辑或执行
void IT2_Timer_Action(uint8_t *pt)
{
	*pt=OPT_ERR;
}

//===============================================================================================================
//序列添加
void IT2_Array_Action(uint8_t *pt)
{
	*pt=OPT_ERR;
}

//=========================================================================================================================
//485应答
void IT2_Load_RS485_Ack(uint8_t st)
{	
	uint8_t len=0,i;
	
	TxDat.buf[0]=0xa5;						//帧头
  TxDat.buf[2]=0x00;						//目标地址	
	TxDat.buf[3]=RxDat.buf[3];					//序列号
	TxDat.buf[4]=Dev.addr[0];					//本地地址
	//=================================================================================			
	if(st>0xf0)							//通用返回
	{
				if(st==OPT_OK)
				{
								TxDat.buf[1]=F_ACK_DEV;						//帧命令
								TxDat.buf[5]=0x00;		                        //数据域长度		
								len=6;							//发送总的字节数
				}
				else if(st==OPT_ERR)
				{	
								TxDat.buf[1]=F_ACK_DEV;						//帧命令
								TxDat.buf[5]=0x03;		                        //数据域长度
								TxDat.buf[6]=0x06;		                        //操作类型
								TxDat.buf[7]=0x01;		                        //长度
								TxDat.buf[8]=0xf1;								//错误应答							
								len=9;														//发送总的字节数												
				}
	}
	//=================================================================================
	else if(st==F_ACK_TIME)						//返回时间
	{
				TxDat.buf[1]=F_ACK_TIME;	
				TxDat.buf[5]=0x08;		                        //数据域长度
				TxDat.buf[6]=0x0c;
				TxDat.buf[7]=0x06;
				TxDat.buf[8]=Time.year;
				TxDat.buf[9]=Time.month;
				TxDat.buf[10]=Time.date;
				TxDat.buf[11]=Time.hour;
				TxDat.buf[12]=Time.min;
				TxDat.buf[13]=Time.sec;
				len=14;	
	}
	//=================================================================================
	//返回版本
	else if(st==F_ACK_VERSION)
	{
				TxDat.buf[1]=F_ACK_VERSION;									//协议不通
				TxDat.buf[5]=0x09;		                      //数据域长度
				
				//模块mac
				TxDat.buf[6]=0x20;
				TxDat.buf[7]=0x08;
				for(i=0;i<8;i++)TxDat.buf[8+i]=IT2_INFO[i];
				//版本号
				TxDat.buf[16]=0x0d;
				TxDat.buf[17]=0x02;
				TxDat.buf[18]=0x00;
				TxDat.buf[19]=DEV_IT2_VER;	
				len=20;		
	}
	
	TxDat.buf[len]=Crc8CheckSum(TxDat.buf,len);		//CRC8校验
	TxDat.cnt=len+1;	
	TxDat.f_tx=1;						//置位485发送标志,直接发送
	
}
//==========================================================================================
//主动上报信息
void IT2_Dev_Report(uint8_t st,uint8_t index)
{
		_Frame *p;			//发送帧缓存队列
		uint8_t tmp,i;
		//==================================================================================
		//设备类型修改上报
		if(st==F_REPORT_DEV_MODIFY)				
		{
					p=GetQueue_InQ(&RS485_Queue,index);		       //通道8为共用通道
					if(p==NULL)return;
				
					p->buf[0]=0xa5;
					p->buf[1]=F_REPORT_DEV_MODIFY;
					p->buf[2]=0x00;
					p->buf[3]=Dev.serial++;
					p->buf[4]=Dev.addr[0];
					p->buf[5]=7;					//数据长度(总个数-7)
					
					//设备通道
					p->buf[6]=0x29;
					p->buf[7]=0x01;
					p->buf[8]=index;				//时间是设备通道
					
					//设备类型
					p->buf[9]=0x22;
					p->buf[10]=0x02;

                    if(index==0)                    //转发器本身通道
                        {
					p->buf[11]=(uint8_t)(Dev.type>>8);		
					p->buf[12]=(uint8_t)(Dev.type&0x00ff);
                        }
                    else
                        {
                            p->buf[11]=(uint8_t)(Ex_Table.Exchange[index-1].reg_type >> 8);		
					        p->buf[12]=(uint8_t)(Ex_Table.Exchange[index-1].reg_type&0x00ff);
                        }
							
					p->buf[13]=Crc8CheckSum(p->buf,13);
					
					p->cnt=14;					//发送总的字节数			
					p->flag=1;
					p->resend=5;
					p->offline=0;
						
					EnQueueBuf(&RS485_Queue,index);	
		}
    //===============================================
    //设备配置删除上报
    else if(st==F_REPORT_MODULE_DEL)
		{
					p=GetQueue_InQ(&RS485_Queue,index);		       //通道8为共用通道
					if(p==NULL)return;
				
					p->buf[0]=0xa5;
					p->buf[1]=F_REPORT_DEV_MODIFY;
					p->buf[2]=0x00;
					p->buf[3]=Dev.serial++;
					p->buf[4]=Dev.addr[0];
					p->buf[5]=0;					//数据长度(总个数-7)
								
					p->buf[6]=Crc8CheckSum(p->buf,6);
					
					p->cnt=7;					//发送总的字节数			
					p->flag=1;
					p->resend=5;
					p->offline=0;
						
					EnQueueBuf(&RS485_Queue,index);	
		}
	//===================================================================================
   else if(st==F_ASK_CONFIG_KEY)
    {
// 					p=GetQueue_InQ(&RS485_Queue,8);		       //通道8为共用通道
// 					if(p==NULL)return;
// 				
// 					p->buf[0]=0xa5;
// 					p->buf[1]=F_ASK_CONFIG_KEY;
// 					p->buf[2]=0x00;
// 					p->buf[3]=Dev.serial++;
// 					p->buf[4]=Dev.addr[0];
// 					p->buf[5]=3;					//数据长度(总个数-7)

// 					//=================================================
// 					//键值
// 					p->buf[6]=0x29;
// 					p->buf[7]=0x01;
// 					p->buf[8]=index;
// 								
// 					p->buf[9]=Crc8CheckSum(p->buf,9);
// 					
// 					p->cnt=10;					//发送总的字节数			
// 					p->flag=1;
// 					p->resend=5;
// 					p->offline=0;
// 						
// 					EnQueueBuf(&RS485_Queue,8);	
    }
    //设备删除上报
    else if(st==F_REPORT_DEV_DEL)
        {
                    p=GetQueue_InQ(&RS485_Queue,index);		       //通道8为共用通道
					if(p==NULL)return;
				
					p->buf[0]=0xa5;
					p->buf[1]=F_REPORT_DEV_DEL;
					p->buf[2]=0x00;
					p->buf[3]=Dev.serial++;
					p->buf[4]=Dev.addr[0];
					p->buf[5]=3;					//数据长度(总个数-7)
					p->buf[6]=0x29;
                    p->buf[7]=0x01;
                    p->buf[8]=index;
								
					p->buf[9]=Crc8CheckSum(p->buf,9);
					
					p->cnt=10;					//发送总的字节数			
					p->flag=1;
					p->resend=5;
					p->offline=0;
						
					EnQueueBuf(&RS485_Queue,index);	
        }
        //场景数据上报
        else if(st==F_ACK_SCENE_DATA)
            {
                    p=GetQueue_InQ(&RS485_Queue,index);		       //通道8为共用通道
					if(p==NULL)return;
                    
				
					p->buf[0]=0xa5;
					p->buf[1]=F_ACK_SCENE_DATA;
					p->buf[2]=0x00;
					p->buf[3]=Dev.serial++;
					p->buf[4]=Dev.addr[0];
					p->buf[5]=3;					                //数据长度(总个数-7)

                    //场景ID
					p->buf[6]=0x40;
                    p->buf[7]=0x02;
                    tmp=Scene[index].num;
                    p->buf[8]=(uint8_t)(Scene[index].item[tmp].id>>8);
                    p->buf[9]=(uint8_t)(Scene[index].item[tmp].id&0x00ff);
                    //场景数据个数
                    p->buf[10]=0x43;
                    p->buf[11]=0x01;
                    p->buf[12]=8+Scene[index].item[tmp].len;
                    //设备通道号
                    p->buf[13]=0x29;
                    p->buf[14]=0x01;
                    p->buf[15]=index;
                    //命令
                    p->buf[16]=0x23;
                    p->buf[17]=0x01;                    
                    p->buf[18]=Scene[index].item[tmp].cmd;
                    //命令数据
                    p->buf[19]=0x24;
                    p->buf[20]=Scene[index].item[tmp].len;  
                    for(i=0;Scene[index].item[tmp].len;i++)p->buf[21+i]=Scene[index].item[tmp].dat[i];
								
					p->buf[21+Scene[index].item[tmp].len]=Crc8CheckSum(p->buf,21+Scene[index].item[tmp].len);
					
					p->cnt=21+Scene[index].item[tmp].len+1;					                    //发送总的字节数			
					p->flag=1;
					p->resend=5;
					p->offline=0;
						
					EnQueueBuf(&RS485_Queue,index);	
            }
}
//==========================================================================================
//地址请求，用8个字节的ID，请求1个字节的短地址
void IT2_Dev_RequestAddr(void)
{
	
	_Frame *p;			//发送帧缓存队列
	uint8_t i;
	p=GetQueue_InQ(&RS485_Queue,8);		       //通道8为共用通道
	if(p==NULL)return;
	
	p->buf[0]=0xa5;
	p->buf[1]=F_ASK_CONFIG_ADDR;
	p->buf[2]=0x00;
	p->buf[3]=Dev.serial++;
	p->buf[4]=Dev.addr[0];
  //p->buf[4]=0xFF;
	p->buf[5]=20;					//数据长度
	
	//模块MAC
	p->buf[6]=0x20;
	p->buf[7]=0x08;
	for(i=0;i<8;i++)p->buf[8+i]=IT2_INFO[i];
	
	//模块类型
	p->buf[16]=0x2e;
	p->buf[17]=0x02;
	p->buf[18]=(uint8_t)(MODULE_TYPE>>8);		
	p->buf[19]=(uint8_t)(MODULE_TYPE&0x00ff);
	
	//已结配置通道
	p->buf[20]=0x2a;
	p->buf[21]=0x01;
	p->buf[22]=0x08;
	
	//全部通道
	p->buf[23]=0x2b;
	p->buf[24]=0x01;
	p->buf[25]=0x08;
	
	p->buf[26]=Crc8CheckSum(p->buf,26);
	
	p->cnt=27;					//发送总的字节数
	p->flag=1;
	p->resend=1;
	p->offline=0;
		
	EnQueueBuf(&RS485_Queue,8);
}
//===========================================================================================
//设备注册，只有在取得短地址后，才操作这一步
void IT2_Dev_Register(void)
{
	_Frame *p;				//发送帧缓存队列
	uint8_t i,rt;
	
	if(Dev.stu>=DEV_NORMAL)return;
	
	//先上报注册转发器本身
	if(Ex_Table.reg[0]==0)
	{
			p=GetQueue_InQ(&RS485_Queue,0);
			if(p==NULL)return;

			p->buf[0]=0xa5;
			p->buf[1]=F_REPORT_ONLINE;
			p->buf[2]=0x00;
			p->buf[3]=Dev.serial++;
			p->buf[4]=Dev.addr[0];
			p->buf[5]=13;				//数据长度
			
			//设备个数
			p->buf[6]=0x10;
			p->buf[7]=0x01;
			p->buf[8]=0x01;				//随身贴设备个数为1
			
			//设备通道号
			p->buf[9]=0x29;				
			p->buf[10]=0x01;
			p->buf[11]=0x00;			//通道号
		
		  //设备通道数
			p->buf[12]=0x2f;				
			p->buf[13]=0x01;
			p->buf[14]=0x08;			//通道号
			
			//设备类型
			p->buf[15]=0x22;
			p->buf[16]=0x02;
			p->buf[17]=(uint8_t)(Dev.type>>8);
			p->buf[18]=(uint8_t)(Dev.type&0x00ff);
			
			//设备状态
			p->buf[19]=0x25;
			p->buf[20]=0x01;
			p->buf[21]=0x00;			//设备状态
			
			p->buf[22]=Crc8CheckSum(p->buf,22);
			
			p->cnt=23;				//发送总的字节数
			p->flag=1;
			p->resend=1;
			p->offline=0;
			Ex_Table.index=0;
				
			EnQueueBuf(&RS485_Queue,0);
  }
	//各通道逐一上报注册
	else
	{
			for(i=0;i<EX_TABLE_NUM;i++)
		  {
						if((Ex_Table.Exchange[i].type!=0xff)&&(Ex_Table.Exchange[i].type!=0x00)&&(Ex_Table.reg[i+1]==0))
						{
									p=GetQueue_InQ(&RS485_Queue,i+1);
									if(p==NULL)return;

									p->buf[0]=0xa5;
									p->buf[1]=F_REPORT_ONLINE;
									p->buf[2]=0x00;
									p->buf[3]=Dev.serial++;
									p->buf[4]=Dev.addr[0];
									p->buf[5]=13;				//数据长度
									
									//设备个数
									p->buf[6]=0x10;
									p->buf[7]=0x01;
									p->buf[8]=0x01;				//随身贴设备个数为1
									
									//设备通道号
									p->buf[9]=0x29;				
									p->buf[10]=0x01;
									p->buf[11]=i+1;			//通道号
								
									//设备通道数
									p->buf[12]=0x2f;				
									p->buf[13]=0x01;
									p->buf[14]=0x01;			//通道号
									
									//设备类型
									p->buf[15]=0x22;
									p->buf[16]=0x02;
									p->buf[17]=(uint8_t)(Ex_Table.Exchange[i].reg_type>>8);
									p->buf[18]=(uint8_t)(Ex_Table.Exchange[i].reg_type&0x00ff);
									
									//设备状态
									p->buf[19]=0x25;
									p->buf[20]=0x01;
									p->buf[21]=0x00;			//设备状态
									
									p->buf[22]=Crc8CheckSum(p->buf,22);
									
									p->cnt=23;				//发送总的字节数
									p->flag=1;
									p->resend=1;
									p->offline=0;
									
									Ex_Table.index=i+1;
										
									EnQueueBuf(&RS485_Queue,i+1);
									
									break;
						}
						else
						{
							    Ex_Table.reg[i+1]=1;													//如果这个通道未空，则直接注册完毕
						}
			}
			if(Dev.stu==DEV_REGISTER)
			{
				  rt=0;
				  for(i=0;i<EX_TABLE_NUM+1;i++)
				  {
							if(Ex_Table.reg[i]==1)rt++;
					}
					if(rt>=(EX_TABLE_NUM+1))Dev.stu=DEV_NORMAL;
			}
	}
	
}

//===============================================================================================================
//应答帧简析
uint8_t IT2_PraseASK(uint8_t cmd)
{
  	uint8_t crc,chl;
  	uint8_t i;
  	
  	if(RxDat.buf[0]!=0xa5)return 0;					//帧头不对
  	
  	//if((RxDat.buf[2]!=Dev.addr[0])&&(RxDat.buf[1]!=F_ACK_REQUEST_ADDR))return 0;	//地址不对
  	if(RxDat.buf[2]!=Dev.addr[0])return 0;
	
	  //应答帧的帧命令 为 0x3e
	  if(RxDat.buf[1]!=F_ACK_DEV)return 0;
	
  	crc=Crc8CheckSum(RxDat.buf,RxDat.cnt-1);
  	if(crc!=RxDat.buf[RxDat.cnt-1])return 0;			//crc不对
	
		Led_set(0,10,50,1,1);
		  	
  	//=============================================================================
  	//取得主机分配的地址
  	if(cmd==F_ASK_CONFIG_ADDR)					//主机分配短地址
  	{
				if(RxDat.buf[6]==0x20)
				{
						for(i=0;i<8;i++)
						{
							if(IT2_INFO[i]!=RxDat.buf[8+i])return 0;
						}
						Dev.addr[0]=RxDat.buf[18];
						Write_Dev(Dev.addr[0],TYPE_EE_ADDR_ADDR0);
						Dev.stu=DEV_REGISTER;
						return 1;
				}
				return 0;
  	}
		else
		{
			  chl=Ex_Table.index;
				//其他应答信号
				if((Dev.stu==DEV_REGISTER)&&(cmd==F_REPORT_ONLINE))
				{
						//Dev.stu=DEV_NORMAL;
					  Ex_Table.reg[chl]=1;
				}
				return 1;
  	}
}		


//==========================================================================
//离线命令发送
void IT2_Offline_DataLoad(uint8_t key)
{	
// 			_Frame *p;						//发送帧缓存队列
// 	    uint8_t len,i;
// 	
// 			//=============================================================================
// 			//如果按键未配置离线发送，则退出
// 			if(Pannel.item[key].flag==0)return;
// 	
// 	    p=GetQueue_InQ(&RS485_Queue,key);
// 			if(p==NULL)return;
// 	
// 			if(Pannel.item[key].type==1)				//设备
// 			{
// 				p->buf[0]=0xa5;
// 				p->buf[1]=F_ACT_DEVICE;				//上报帧
// 				p->buf[2]=Pannel.item[key].tg_id;		
// 				p->buf[3]=Dev.serial++;
// 				p->buf[4]=Dev.addr[0];
// 				//p->buf[5]=;					//数据长度
// 				//==========================================================
// 				//设备通道号
// 				p->buf[6]=0x29;
// 				p->buf[7]=0x01;
// 				p->buf[8]=Pannel.item[key].buf[1];				
// 				//==========================================================
// 				//设备命令
// 				p->buf[9]=0x23;
// 				p->buf[10]=0x01;
// 				p->buf[11]=Pannel.item[key].buf[2];
// 				//==========================================================
// 				//设备命令数据
// 				p->buf[12]=0x24;
// 				p->buf[13]=Pannel.item[key].buf[3];
// 				for(i=0;i<p->buf[13];i++)
// 				{
// 					p->buf[14+i]=Pannel.item[key].buf[4+i];
// 				}
// 				
// 				//==========================================================
// 				p->buf[14+p->buf[13]]=Crc8CheckSum(p->buf,14+p->buf[13]);
// 						
// 				p->cnt=15+p->buf[13];			//发送总的字节数
// 				
// 				p->buf[5]=p->cnt-7;
// 				
// 			  p->flag=1;
// 			  p->resend=5;
// 				p->offline=0;
// 				EnQueueBuf(&RS485_Queue,key);	
// 			}
// 			//===================================================================
// 			else if(Pannel.item[key].type==2)		//场景
// 			{
// 				p->buf[0]=0xa5;
// 				p->buf[1]=B_EXEC_SCENE;				//上报帧
// 				p->buf[2]=Pannel.item[key].tg_id;		
// 				p->buf[3]=Dev.serial++;
// 				//p->buf[4]=Dev.addr[0];
// 				p->buf[4]=0x00;
// 				//p->buf[5]=;					//数据长度
// 				//==========================================================
// 				//场景ID
// 				p->buf[6]=0x40;
// 				p->buf[7]=0x02;
// 				p->buf[8]=Pannel.item[key].buf[0];
// 				p->buf[9]=Pannel.item[key].buf[1];					
// 				//==========================================================
// 				//场景执行模式
// 				//p->buf[10]=0x41;
// 				//p->buf[11]=0x01;
// 				//p->buf[12]=Pannel.item[key].buf[2];
// 				//==========================================================
// 				p->buf[10]=Crc8CheckSum(p->buf,10);
// 						
// 				p->cnt=11;			//发送总的字节数
// 				
// 				p->buf[5]=4;
// 				p->flag=1;
// 			  p->resend=5;
// 				p->offline=0;
// 				EnQueueBuf(&RS485_Queue,key);
// 			}
// 			//==================================================================
// 			else if(Pannel.item[key].type==3)		//序列
// 			{
// //				p->buf[0]=0xa5;
// //				p->buf[1]=B_EXEC_SCENE;				//上报帧
// //				p->buf[2]=Pannel.item[key].tg_id;		
// //				p->buf[3]=Dev.serial++;
// //				p->buf[4]=Dev.addr[0];
// //				//p->buf[5]=;					//数据长度
// //				//==========================================================
// //				//场景ID
// //				p->buf[6]=0x40;
// //				p->buf[7]=0x02;
// //				p->buf[8]=Pannel.item[key].buf[2];
// //				p->buf[9]=Pannel.item[key].buf[3];					
// //				//==========================================================
// //				//场景执行模式
// //				p->buf[10]=0x41;
// //				p->buf[11]=0x01;
// //				p->buf[12]=Pannel.item[key].buf[4];
// //				//==========================================================
// //				p->buf[13]=Crc8CheckSum(p->buf,13);
// //						
// //				p->cnt=14;			//发送总的字节数
// //				
// //				p->buf[5]=7;
// //				
// //				EnQueueBuf(&RS485_Queue,key);
// 				//==========================================================
// 				//取出序列，放入array_buf中
// 				len=(Pannel.item[key].datalen-2)/5;	//去掉序列ID
// 				for(i=0;i<len;i++)
// 				{
// 					Array.flag=1;
// 					Array.key=key;
// 					Array.index=0;
// 					Array.cnt=len;
// 					Array.buf[i].id=(Pannel.item[key].buf[2+i*5+0]<<8)+Pannel.item[key].buf[2+i*5+1];
// 					Array.buf[i].mode=Pannel.item[key].buf[2+i*5+2];
// 					Array.buf[i].dly=(Pannel.item[key].buf[2+i*5+3]<<8)+Pannel.item[key].buf[2+i*5+4];
// 				}	
// 			}
// 			//===================================================================
// 			else if(Pannel.item[key].type==4)			//反向
// 			{
// 				p->buf[0]=0xa5;
// 				p->buf[1]=F_ACT_DEVICE;				//设备执行
// 				p->buf[2]=Pannel.item[key].tg_id;		
// 				p->buf[3]=Dev.serial++;
// 				p->buf[4]=Dev.addr[0];
// 				//p->buf[5]=;					//数据长度
// 				//==========================================================
// 				//设备通道号
// 				p->buf[6]=0x29;
// 				p->buf[7]=0x01;
// 				p->buf[8]=Pannel.item[key].buf[1];				
// 				//==========================================================
// 				//设备命令
// 				p->buf[9]=0x23;
// 				p->buf[10]=0x01;
// 				p->buf[11]=0x04;
// 				//==========================================================
// 				p->buf[12]=Crc8CheckSum(p->buf,12);
// 						
// 				p->cnt=13;			//发送总的字节数
// 				
// 				p->buf[5]=6;
// 				p->flag=1;
// 			  p->resend=5;
// 				p->offline=0;
// 				EnQueueBuf(&RS485_Queue,key);	
// 					
// 			}
}

//===============================================================================================
//转发器收到遥控器的按键信号
//收到的键值应该是0x07,0x08,0x09,0x21或0x22
void IT2_Pannel_DataLoad(uint8_t index,uint8_t value)
{

 
			_Frame *p;						//发送帧缓存队列
			uint8_t dat=0,stu_dat=0;
	
	    //遥控器上报
			//=====================================================================
      if(Ex_Table.Exchange[index].type>=EX_KEY_SCENE)
			{
					if(value==0x0e)value=0x08;			//键值

					if(value==0x07)     stu_dat=0x00;
					else if(value==0x08)stu_dat=0x01;
					else if(value==0x09)stu_dat=0x02;
					else 								stu_dat=0x03;
				
				  dat=1;
				
			}
			//=================================================================================================================
			//设备状态上报
			else
			{
				        //========================================================================================================
								if(Ex_Table.Exchange[index].type==EX_DEV_LIGHT)	            //灯光上报
                {
												if(value==0x00) dat=0x01;
												else            dat=0x00;
												
												stu_dat=0;
                }
								//=========================================================================================================							
                else if(Ex_Table.Exchange[index].type==EX_DEV_SOCKET)				//智能插座
                {
                        if(value==0x00) dat=0x01;
                	      else            dat=0x00;
									     
									      stu_dat=0;
                }
								//=========================================================================================================
                else                                                           //电机主动上报状态
                {
                        if((Ex_Table.Exchange[index].type==EX_DEV_MOTOR_E)||(Ex_Table.Exchange[index].type==EX_DEV_BLIND_E))
                        {
																//ID无线868 和 IT485 行程表示相反
                                //if(((Ex_Table.Exchange[index].channel&0x80)==0x80)&&(dat<=0x64))
																
																if(value>0x64)
																{
																				 dat=Ex_Table.stu[index]+0x03;
																	       stu_dat=0;
																}
																else		//处理行程
																{
																	       if(Ex_Table.Exchange[index].stu==1)value=0x64-value; 
																				 //dat=0x12;
																	       dat=Ex_Table.stu[index];
																	       stu_dat=0x64-value;
																}
												
                        } 
												else if((Ex_Table.Exchange[index].type==EX_DEV_MOTOR_R)||(Ex_Table.Exchange[index].type==EX_DEV_BLIND_R))
												{
																dat=Ex_Table.stu[index];
																stu_dat=0x00;
												}
			
									}
			}
			
      //=============================================================================================================			

			p=GetQueue_InQ(&RS485_Queue,index);
			if(p==NULL)return;
						
			p->buf[0]=0xa5;
			p->buf[1]=F_REPORT_STATUS;			//上报帧
			p->buf[2]=0x00;		
			p->buf[3]=Dev.serial++;
			p->buf[4]=Dev.addr[0];	
				
			p->buf[5]=12;				//数据长度
			//=================================================================
			//状态个数为1
			p->buf[6]=0x10;			
			p->buf[7]=1;				
			p->buf[8]=1;			
			//=================================================================
			//设备通道号
			p->buf[9]=0x29;				
			p->buf[10]=0x01;		
			p->buf[11]=index;
			//=================================================================
			//设备状态
			p->buf[12]=0x25;				
			p->buf[13]=0x01;
			p->buf[14]=dat;		
			//==================================================================
			//状态值
			p->buf[15]=0x26;				
			p->buf[16]=0x01;		
			p->buf[17]=stu_dat;
			//==================================================================
			
			p->buf[18]=Crc8CheckSum(p->buf,18);			
			p->cnt=19;			//发送总的字节数
			p->flag=1;
			p->resend=5;
			p->offline=1;
			EnQueueBuf(&RS485_Queue,index);
}
//===============================================================================================
//序列启动发送
void IT2_Array_Handle(void)
{
	_Frame *p;
	
	if(Array.flag==1)
	{
		p=GetQueue_InQ(&RS485_Queue,Array.key);
		if(p==NULL)return;
		
		if(Array.buf[Array.index].dly>0)
		{
			Array.buf[Array.index].dly--;			//按最小单位递减
		}
		else
		{
			p->buf[0]=0xa5;
			p->buf[1]=B_EXEC_SCENE;				//上报帧
			p->buf[2]=0xff;		
			p->buf[3]=Dev.serial++;
			p->buf[4]=Dev.addr[0];
			//p->buf[5]=;					//数据长度
			//==========================================================
			//场景ID
			p->buf[6]=0x40;
			p->buf[7]=0x02;
			p->buf[8]=(uint8_t)(Array.buf[Array.index].id>>8);
			p->buf[9]=(uint8_t)(Array.buf[Array.index].id&0x00ff);					
			//==========================================================
			//场景执行模式
			p->buf[10]=0x41;
			p->buf[11]=0x01;
			p->buf[12]=Array.buf[Array.index].mode;
			//==========================================================
			p->buf[13]=Crc8CheckSum(p->buf,13);
					
			p->cnt=14;			//发送总的字节数
			
			p->buf[5]=7;			
			EnQueueBuf(&RS485_Queue,Array.key);
			
			Array.index++;
			if(Array.index>=Array.cnt)
			{
				Array.flag=0;
				Array.key=0x00;
				Array.cnt=0;
			}
		}	
	}	
}
//==========================================================================
void IT2_Get_PannelType(uint8_t key,uint8_t *pt)
{
//  	*pt++=Pannel.item[key].type;

}

//=============================================================================================
//以下用于转发器
//==============================================================================================
//设备执行-转发器接收后向无线设备转发
//数据格式：设备通道号+设备命令+设备命令数据
void IT2_Dev_Action(uint8_t *pt)
{
	      uint8_t index=0,cmd=0,cmd_dat=0;
   	     
	      *pt=OPT_OK;
				//======================================================================================
				//设备通道号
				if(RxDat.buf[6]==0x29)											
				{
						index=RxDat.buf[8];
						if((index>8)||(index==0))*pt=OPT_ERR;
				}
				//=======================================================================================
				//设备命令
				if(RxDat.buf[9]==0x23)
				{
						 cmd=RxDat.buf[11];
				}
				else
				{
					   *pt=OPT_ERR;
				}
				//========================================================================================
				//设备命令数据
				if(RxDat.buf[12]==0x24)
				{
						 cmd_dat=RxDat.buf[14];
				}
				else
				{
					   *pt=OPT_ERR;
				}
				//==============================================================================================
				
				if(*pt!=OPT_OK)return;
				
				//==============================================================================================
				//命令转换，并发送
				index=index-1;
				*pt=IT2_Dev_To_Wireless(index,cmd,cmd_dat,0);
		
}
//===================================================================================================
//RS485动作转到无线动作
uint8_t IT2_Dev_To_Wireless(uint8_t index,uint8_t cmd,uint8_t cmd_dat,uint8_t scene_num)
{
	      uint8_t type,route,rt;     
	
				type=Ex_Table.Exchange[index].type;
        rt=OPT_ERR;
				//===============================================================================================
				//电机类
				if((type==EX_DEV_MOTOR_R)||(type==EX_DEV_MOTOR_E)||(type==EX_DEV_BLIND_E))
				{
						if(cmd==0x10)			//电机开
						{
							  if(Ex_Table.Exchange[index].stu==0)cmd=0x07;
							  else                               cmd=0x09;
								Wireless_Dev_Action(index,cmd,0,0);
							  Ex_Table.stu[index]=0x10;
						}
						else if(cmd==0x12)//电机关
						{
							  Wireless_Dev_Action(index,0x08,0,0);
							  Ex_Table.stu[index]=0x12;
						}
						else if(cmd==0x11)//电机关
						{
							  if(Ex_Table.Exchange[index].stu==0)cmd=0x09;
							  else                               cmd=0x07;
							  Wireless_Dev_Action(index,cmd,0,0);
							  Ex_Table.stu[index]=0x11;
						}
						else if(cmd==0x1d)//电机翻转
						{
							  Exchangebuf.type=Ex_Table.Exchange[index].type;
							  Exchangebuf.addr=Ex_Table.Exchange[index].addr;
							  Exchangebuf.wl_addr[0]=Ex_Table.Exchange[index].wl_addr[0];
							  Exchangebuf.wl_addr[1]=Ex_Table.Exchange[index].wl_addr[1];
							  Exchangebuf.wl_addr[2]=Ex_Table.Exchange[index].wl_addr[2];
							  Exchangebuf.channel=Ex_Table.Exchange[index].channel;
								if(Ex_Table.Exchange[index].stu==0)
								{
									  Exchangebuf.stu= 0x01;
								}
								else
								{
									  Exchangebuf.stu= 0x00;
								}
								
								Write_Pannel_Table(index);
						}
						else if(cmd==0x14)//运行到指定点
						{
							   route=cmd_dat;
	               if(route<=0x64)
								 {
									   route=100-route;
										 if(Ex_Table.Exchange[index].stu==0x01)route=100-route;
							   }
								 else
								 {
									   route=0;
								 }
								 
								 cmd=0x19;
								 Wireless_Dev_Action(index,cmd,route,0);
						}
						else
						{
							   rt=OPT_ERR;
						}
				}
				//==========================================================================================
				else if(type==EX_DEV_SOCKET)   //智能插座
				{
						if(cmd==0x02)     //关灯
						{
							  Wireless_Dev_Action(index,0x08,0,0);
							  Ex_Table.stu[index]=0x03;
						}
						else if(cmd==0x01)//开灯
						{
								Wireless_Dev_Action(index,0x07,0,0);
							  Ex_Table.stu[index]=0x02;
						}
						else
						{
							   rt=OPT_ERR;
						}
				}
        //=========================================================================================				
				else if(type==EX_DEV_LIGHT)    //灯光			
				{
					  if(cmd==0x02)     //关灯
						{
							   Wireless_Dev_Action(index,0x08,0,0);
							   Ex_Table.stu[index]=0x00;
						}
						else if(cmd==0x01)//开灯
						{
								Wireless_Dev_Action(index,0x07,0,0);
							  Ex_Table.stu[index]=0x01;
						}
						else
						{
							   rt=OPT_ERR;
						}
				}
				//==========================================================================================
				//调光灯 add 2016.2.23
				else if(type==EX_DEV_LIGHT_ADJ)    //灯光			
				{
					  if(cmd==0x01)     //关灯
						{
							   Wireless_Dev_Action(index,0x08,0,0);
							   Ex_Table.stu[index]=0x00;
						}
						else if(cmd==0x02)//开灯
						{
								Wireless_Dev_Action(index,0x07,0,0);
							  Ex_Table.stu[index]=0x64;
						}
						else if(cmd==0x03)
						{
							  Wireless_Dev_Action(index,0x1d,cmd_dat,0);
							  if(cmd_dat>100)cmd_dat=100;
							  Ex_Table.stu[index]=100-cmd_dat;
						}
						else
						{
							   rt=OPT_ERR;
						}
				}
				//==========================================================================================
        else
				{  
						rt=OPT_ERR;
        }	

         return rt;				
}
//==============================================================================================
//转发器添加设备或删除
void IT2_Exchange_Action(uint8_t *pt)
{
	 	uint8_t cmd,type=0,index=0,Iscode=0;
	  	
	  *pt=OPT_OK;

    //如果处于设置状态，则不能再设置
    if(Dev.stu==DEV_SETUP)
		{
			  *pt=OPT_CANCEL;
				return;
		}
 	 	
		cmd=RxDat.buf[1];
		
	  //=============================================================================
	  //判断通道
	  if(RxDat.buf[6]==0x29)
		{
				index=RxDat.buf[8];
			  if((index>8)||(index==0))
				{
						*pt=OPT_ERR;
				}
				index=index-1;										//
		}
		else
		{
				*pt=OPT_ERR;
		}
		//===============================================================================
		if(cmd==F_ASK_EXCHANGER_ADDCODE)
		{
					//判断类型
					if(RxDat.buf[9]==0x22)
					{
							 type=IT2_Changer_GetType(RxDat.buf[11],RxDat.buf[12]);
							 if(type==OPT_ERR)*pt=OPT_ERR;
					}
					else
					{
							*pt=OPT_ERR;
					}
	  }
		//===============================================================================
		if(*pt!=OPT_OK)return;
		
		//================================================================================
		//添加设备
    if(cmd==F_ASK_EXCHANGER_ADDCODE) 
 		{			
					//虚拟设备为遥控器时，只能是学习时间，当虚拟设备为控制设备时，只有对码时间
					if((type==EX_KEY_SCENE)||(type==EX_KEY_MOTOR)||(type==EX_KEY_LIGHT)||(type==EX_KEY_GENERAL))
					{
									Dev.stu=DEV_SETUP;
									Dev.learn_time=LEARN_TIME_MAX;
									Dev.encode_time=0;
									Iscode=1;
					}
					//else if(tmp<=EX_DEV_LIGHT)
					else if(type<EX_DEV_KEY)											//新增调光灯功能
					{
									Dev.stu=DEV_SETUP;
									Dev.learn_time=0;
									Dev.encode_time=ENCODE_TIME_MAX;
									Iscode=2;	
					}
					else
					{
									Iscode=0;
					}
					//===============================================================================	
					//要求增加的虚拟设备是遥控器
					if(Iscode==1)
					{
									//不要求无线发送，只等待遥控器发送
									Exchangebuf.type= type;                                    //设备类型
									Exchangebuf.addr= index;
									Exchangebuf.channel=1;
									Exchangebuf.wl_addr[0]=	0;				                  			//等待接收遥控器的地址
									Exchangebuf.wl_addr[1]= 0;  		                      		//
									Exchangebuf.wl_addr[2]= 0;  
                  Exchangebuf.reg_type=(uint16_t)((RxDat.buf[11]<< 8)+(RxDat.buf[12]));
					}
					//要求增加的虚拟设备是控制设备
					else if(Iscode==2)
					{          
									Exchangebuf.type= type;                                    //设备类型
									Exchangebuf.addr= index;;            
									Exchangebuf.channel=1;
									Exchangebuf.wl_addr[0]=	type;              
									//Exchangebuf.wl_addr[1]= Dev.addr[0];  	                  	//随机数-遥控器地址1 
									Exchangebuf.wl_addr[1]=Crc8CheckSum(IT2_INFO,8);
									Exchangebuf.wl_addr[2]=Dev.serial++;
                  Exchangebuf.reg_type=(uint16_t)((RxDat.buf[11]<< 8)+(RxDat.buf[12]));
									Wireless_Dev_Action(Exchangebuf.addr,0x01,0,0);         	//装无线发送数据
					}
					//设备未识别
					else
					{
									*pt=OPT_ERR;
					}					
		}
 		//===================================================================================
		else if(cmd==F_ASK_EXCHANGER_DELCODE)
		{
					//删除指定设备
					Exchangebuf.type=Ex_Table.Exchange[index].type;
					Exchangebuf.wl_addr[1]=Ex_Table.Exchange[index].wl_addr[1];
					Exchangebuf.wl_addr[2]=Ex_Table.Exchange[index].wl_addr[2];
					Wireless_Dev_Action(index,0x02,0,0);         //装无线发送数据
				
					//type=Exchangebuf.type;
					Exchangebuf.type=0xff;	
					Exchangebuf.addr=0xff;
					Exchangebuf.channel=0xff;
					Exchangebuf.wl_addr[0]=0xff;
					Exchangebuf.wl_addr[1]=0xff;
					Exchangebuf.wl_addr[2]=0xff;
                    Exchangebuf.reg_type=0;
					//写入表格
					Write_Ex_Table(index);
					//Exchangebuf.type=type;		
					IT2_Dev_Report(F_REPORT_DEV_DEL,index+1);    //转发器通道和表格通道不一样
		}
		else
		{
					*pt=OPT_ERR;
		}
}

//=========================================================================================================
//有线485中 设备类型转到无线868
uint8_t IT2_Changer_GetType(uint8_t t1,uint8_t t2)
{
	  uint8_t type;
	
		if(t1==0x20)										//机械行程-卷帘
		{
				type=EX_DEV_MOTOR_R;
		}
		else if(((t1==0x21)&&(t2!=15))||(t1==0x22)||(t1==0x23)||(t1==0x25))			//电子行程-卷帘
		{
			  type=EX_DEV_MOTOR_E;
		}
		else if((t1==0x21)&&(t2==15))			            //电子行程-开合帘
		{
				type=EX_DEV_BLIND_E;
		}
		else if((t1==0x01)&&(t2==0x01))
		{
			  type=EX_DEV_LIGHT;
		}
		else if((t1==0x0a)&&(t2==0x01))
		{
			  type=EX_DEV_SOCKET;
		}
		else if((t1==0x04)&&(t2==0x01))
		{
			  type=EX_KEY_LIGHT;
		}
		else if((t1==0x04)&&(t2==0x02))
		{
			  type=EX_KEY_MOTOR;
		}
		else if((t1==0x04)&&(t2==0x02))
		{
			  type=EX_KEY_MOTOR;
		}
		else if((t1==0x04)&&(t2==0x02))
		{
			  type=EX_KEY_MOTOR;
		}
		else if((t1==0x04)&&(t2==0x03))
		{
			  type=EX_KEY_SCENE;
		}
		else if((t1==0x04)&&(t2==0x04))
		{
			  type=EX_KEY_GENERAL;
		}
		else
		{
				type=OPT_ERR;
		}
		
		return type;
		
}


/***************************end  of  code ***************************/
