
//================================================================================

#define MAIN_VARIABLE

#include <includes.h>
//==================================================================================


//==================================================================================
//ģ������     
	#define MODULE_TYPE  	0x0060
	
// 	const uint8_t IT2_INFO[8] __attribute__((at(0x0800fc00))) = {
// 	0x00,					//��Ʒϵ������
// 	0x16,					//��
// 	0x15,					//��
// 	0x60,         //�ͺ�
// 	0xff,					//Ԥ��
// 	0xff,         //Ԥ��
// 	0x00,					//���к�-��λ
// 	0x02,					//���к�-��λ
// 	};

 

////====================================================================================
////������ͨ����������
//#define SENSOR_NUM   4
//typedef struct __SENSOR_ITEM
//{
//	uint8_t flag;
//	uint16_t type;
//	uint8_t Isnew;		//�Ƿ����¸���
//	uint16_t time;		//������һ����������ΰ�����ʱ������ÿ10mS�ۼ�һ��
//	uint8_t time_en;	//�Ƿ�����
//        	
//}_SENSOR_ITEM;   
//typedef struct __SENSOR_TABLE
//{
//	uint8_t cnt;		//����ͨ������
//	_SENSOR_ITEM item[SENSOR_NUM];
//}_SENSOR_TABLE;
//_SENSOR_TABLE Sensor;		//��尴�����ݿ� 
//
////=====================================================================================
////������-ģ����
//typedef struct __SENSOR_AN
//{
//	uint8_t flag;
//	uint16_t type;
//	uint8_t Isnew;		//�Ƿ����¸���
//	uint8_t dat[2];		//����ģ�������������
//	uint8_t cmd1[3];	//����һ�����2���Լ������ݣ������������·�
//	uint8_t cmd2[3];	//����һ�����2���Լ������ݣ������������·�
//}_SENSOR_AN;     
//_SENSOR_AN Sensor_AN;
////=====================================================================================


uint8_t  Crc8CheckSum(uint8_t *ptr,uint8_t length);
void IT2_Get_PannelType(uint8_t key,uint8_t *pt);

//=================================================================================================
//����֡��������֤
void IT2_Frame_availability(uint8_t *p)
{
	//uint8_t i;
	uint8_t len/*,tmp1,tmp2*/,tmp3;

	*p=OPT_OK;
	 
	//֡ͷ�ж� -֡������ЧӦ0x05
	if(RxDat.buf[0]!=0xa5)
	{
		*p=OPT_CANCEL; 						//����0x05������
		return;
	}
	
	//�����·��㲥���ģ�����ģ�Ӧ���ĺ����ݱ��ģ��������ֽڱ�ʾΪ���ݳ���
	len=RxDat.buf[5];						//
	len=len+6+1;							//��ͷ�����ݳ����ֽڣ�1��CRC
	if(RxDat.cnt!=len)
	{
		*p=OPT_CANCEL; 
		return;
	}
	
	//��ַ�ж�
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
	
	//CRCУ��
	tmp3=Crc8CheckSum(RxDat.buf,(RxDat.cnt-1));		//����Crc8ֵ
	if(tmp3!=RxDat.buf[RxDat.cnt-1])			//�ж�CrcУ���Ƿ���ȷ
	{
		//�������˳�
		*p=OPT_CANCEL; 
		return;
	}
}

//==================================================================================================
//RS485����-IT�ڶ���Э��
void IT2_ParseFrame(void)
{
		uint8_t i;
		uint8_t m_cmd=0;
		//uint8_t s_cmd=0;
		uint8_t err;

		//===================================================================================
		//֡�����Լ��
		IT2_Frame_availability(&err);
		
		if((err==OPT_ERR)||(err==OPT_CANCEL))                     //clf mod 2014.6.21
		{
			//���󷵻�
			for(i=0;i<RX_MAX;i++)RxDat.buf[i]=0;
			RxDat.cnt=0;
			return;
		}
		
		Led_set(0,30,50,2,1);
					 
		//===================================================================================
		//֡����
		m_cmd=RxDat.buf[1];	                        //֡����			
		//s_cmd=RxDat.buf[6];					//��������
		err=OPT_CANCEL;
        
        switch(m_cmd)
        {
        	//=========================================================================
        	//�㲥�౨�Ĵ���
        	case  B_ONLINE:  		
					case  B_DIS_BUS:   		
					case  B_EN_BUS:   			
        		IT2_Function_Broadcast(&err);
        	break;
        	//==========================================================================
        	//ʱ���౨�Ĵ���
					case  F_ASK_TIME:
					case  F_SET_TIME:			
        		IT2_Function_Time(&err);      	
        	break;
        	//===========================================================================
        	//�豸�汾
        	case F_ASK_VERSION:				 
		     	IT2_Function_Version(&err);				
        	break;
        	//===========================================================================
        	//�豸����
        	case F_ASK_UPDATE:
        	case F_ASK_UPDATE_START:					
        		IT2_Function_Update(&err);         	
        	break;
        	//==========================================================================
        	//ģ������
        	//case F_ASK_CONFIG_ADDR:                                                       //�������豸����������ַ
        	case F_ASK_CONFIG_MODULE:                                     //�޸�����
        	case F_DEL_MODULE_CONFIG:                                     //ɾ������
					//case F_ACK_REQUEST_ADDR:
        		IT2_Function_Config(&err); 
        	break;
        	//========================================================================== 
        	//�豸�������� ִ��
        	case F_ASK_MODIFY:
        	case F_ASK_INFO:
        	case F_SETUP_DEV_PARA:
        	case F_READ_DEV_PARA:
								IT2_Dev_Setup(&err);     
          break;
					//============================================================================
					//����ִ��
					case F_ACT_DEVICE:
						    IT2_Dev_Action(&err);   
					break;
        	//==========================================================================
        	//���������ú�ͬ��
        	case F_ASK_CONFIG_KEY:
        	case F_ASK_SYNC_KEY:
								IT2_Key_Setup(&err);     
          break;
                //==========================================================================
                //ת����
        	case F_ASK_EXCHANGER_DELCODE:
					case F_ASK_EXCHANGER_ADDCODE:
								IT2_Exchange_Action(&err);  
        	break;
        	//==========================================================================
        	//�������
					case  B_EXEC_SCENE:		
					case  B_DEL_SCENE:	
        	case F_ACT_SCENE:
        	case F_ASK_SCENE_DATA:
        	case F_ASK_SCENE_ADD:
        	case F_ASK_SCENE_DEL:
        		IT2_Scene_Action(&err);  
        	break;
        	//==========================================================================
        	//��ʱ�����
        	case F_ASK_TIMER_ADD:
        	case F_ASK_TIMER_EDIT:
        	case F_ASK_TIMER_DEL:
        	case F_ASK_TIMER_ONOFF:
        		IT2_Timer_Action(&err);  
        	break;
        	//==========================================================================
        	//�������
        	case F_ASK_ARRAY_ADD:
        	case F_ASK_ARRAY_DEL:
        		IT2_Array_Action(&err);  
        	break;
					
        	
        	//==========================================================================
        	//����
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
//���ڼ����㲥�����
void IT2_Function_Broadcast(uint8_t *pt)					//�㲥��Ӧ��
{
	uint8_t m_cmd,i;
	*pt=OPT_CANCEL;
	
	m_cmd=RxDat.buf[1];
	
  	//===================================================================================================
	if(m_cmd==B_DIS_BUS)							//�㲥ռ������
	{
		Bus.distime=RxDat.buf[6]*10000;					//���߽���ʱ��,��λms
	}
	else if(RxDat.buf[4]==B_EN_BUS)						//ȡ�����߽���ʱ��
	{
		Bus.distime=0;							
	}
	else if(m_cmd==B_ONLINE)									//�㲥��ѯ����
	{	    	
	    	Dev.stu=DEV_REQUESTADDR;
		    for(i=0;i<9;i++)Ex_Table.reg[i]=0;
	}
// 	else if(m_cmd==B_EXEC_SCENE)							//�㲥������
// 	{
// 				//��岻��Ӧ������
// 	}
// 	else if(m_cmd==B_DEL_SCENE)
// 	{
// 				//��岻���泡��
// 	}
}

//=======================================================================================================
//���ڼ���ʱ�䱨��
void IT2_Function_Time(uint8_t *pt)						//������������
{
	uint8_t m_cmd;
	
	*pt=OPT_ERR;
	
	m_cmd=RxDat.buf[1];
	
	if(m_cmd==F_ASK_TIME)							//��ѯ����ʱ��
	{		       
		IT2_Load_RS485_Ack(F_ACK_TIME);
		*pt=F_ACK_TIME;
	}
	else if(m_cmd==F_SET_TIME)						//����ʱ��
	{
		Time.year=RxDat.buf[8];
		Time.month=RxDat.buf[9];
		Time.date=RxDat.buf[10];
		Time.hour=RxDat.buf[11];
		Time.min=RxDat.buf[12];
		Time.sec=RxDat.buf[13];
		//==========================================
		//����RTCʱ������
		
		*pt=OPT_OK;
	}
}
//=======================================================================================================
//���ڼ����汾����
void IT2_Function_Version(uint8_t *pt)						//������������
{
	uint8_t m_cmd;
	
	*pt=OPT_ERR;
	
	m_cmd=RxDat.buf[1];
	
	if(m_cmd==F_ASK_VERSION)							//��ѯ����ʱ��
	{		       
		IT2_Load_RS485_Ack(F_ACK_VERSION);
		*pt=F_ACK_VERSION;
	}
}
//========================================================================================================
//���ڼ���������������
void IT2_Function_Update(uint8_t *pt)
{
	uint8_t m_cmd;
	
	*pt=OPT_ERR;
	
	m_cmd=RxDat.buf[1];
	
	if(m_cmd==F_ASK_UPDATE)							//��ѯ����ʱ��
	{		       
		IT2_Load_RS485_Ack(F_ACK_UPDATE);
		*pt=F_ACK_UPDATE;
	}
}
//=======================================================================================================
//ģ������
void IT2_Function_Config(uint8_t *pt)
{
//     uint8_t m_cmd,i;
//     
// 	  *pt=OPT_ERR;
//     m_cmd=RxDat.buf[1];
//     if(m_cmd==F_DEL_MODULE_CONFIG)                  //ɾ��������Ϣ
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
//               IT2_Dev_Report(F_REPORT_MODULE_DEL,0);//ģ������ɾ���ϱ�
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
//�豸����
void IT2_Dev_Setup(uint8_t *pt)
{
//	uint8_t dat;
	uint8_t m_cmd;
	
	m_cmd=RxDat.buf[1];
	if(m_cmd==F_ACT_DEVICE)
	{
		
	}
	//===============================================================================================
	//�޸��豸����
	else if(m_cmd==F_ASK_MODIFY)
	{
		//ת�������޸����ͣ�ͨ����Ӧ���ͣ�ͨ��ת���������ɾ������ 2016.3.30
		if((RxDat.buf[6]==0x29)&&(RxDat.buf[9]==0x22))
		{
// 			Dev.type=(RxDat.buf[11]<<8)+RxDat.buf[12];
// 			*pt=OPT_OK;					//Ӧ��
// 			//д��EPROM
// 			dat=RxDat.buf[12]&0x0f;
// 			Write_Dev(dat,TYPE_EE_ADDR_TYPES);
// 					
// 			//�޸��ϱ�
// 			IT2_Dev_Report(F_REPORT_DEV_MODIFY,0);
		}
	}
	//================================================================================================
	//�豸��Ϣ����
	else if(m_cmd==F_ASK_INFO)
	{
			*pt=OPT_OK;					//Ӧ��
		  //�ϱ��豸����
		  IT2_Dev_Report(F_REPORT_DEV_MODIFY,0);
	}
} 

//=========================================================================================================================
//���򴫸�������
void IT2_Key_Setup(uint8_t *pt)
{
// 	  uint8_t i,j;
// 		uint8_t key,type,len,len1,index,offset,operation; 
// 		*pt=OPT_ERR;
// 	  
// 		//���ð���
// 		if(RxDat.buf[1]==F_ASK_SYNC_KEY)																		//������ͬ������
// 		{			
// 					//=============================================================================
// 					//��ȡ��Ҫ���õİ�������
// 			    len=RxDat.cnt-2;																							//���ݳ���
// 			    offset=6;
// 					for(i=0;i<8;i++)																							//���8������
// 					{				
// 									//======================================================================
//                   operation=RxDat.buf[offset+0];												//������	
//                   len1=	RxDat.buf[offset+1];														//����������						
// 									key=RxDat.buf[offset+2];															//��ֵ
// 									type=RxDat.buf[offset+3];															//�豸����Ϊ0x01~0x04	
// 									//======================================================================
// 									//��������Ƿ�ƥ��
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
// 									//������������������
// 									index=key;																						//���ﰴ��ֵ����ͨ��0~7
//                                     if(Pannel.item[index].flag!=1)
//                                         {
//                                             Pannel.item[index].flag=1;
//                                             Pannel.cnt++;
//                                         }
// 									Pannel.item[index].type=type;													//����
// 									if((type==1)||(type==4))Pannel.item[index].tg_id=RxDat.buf[offset+4];
// 									else       Pannel.item[index].tg_id=0xff;   
// 										
// 									//=======================================================================
// 									//�����жϣ��ֱ���,ֱ�Ӱ����ݷ��뻺����
// 									Pannel.item[index].datalen=len1-2;											//�˴�����Ϊ��ȥ ͨ�� ���� �豸ID��ĳ���
// 									for(j=0;j<Pannel.item[index].datalen;j++)
// 									{
// 												Pannel.item[index].buf[j]=RxDat.buf[offset+4+j];
// 									}	
// 									offset=offset+len1+2;
// 									//=======================================================================
// 									//����EPROM ȫ������
// 									Write_Pannel_Table(index);
// 									//========================================================================
// 									//�ж��Ƿ����
// 									if(offset>=len)break;
// 					}	
// 					*pt=OPT_OK;
// 		}
        
}

//================================================================================================================
//�������û򳡾�ִ��
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
	 //����ִ��
	 if((m_cmd==F_ACT_SCENE)||(m_cmd==B_EXEC_SCENE))
	 {
		 id=(uint16_t)((RxDat.buf[8]<< 8)+(RxDat.buf[9]));
         mode=RxDat.buf[12];
		     rt=OPT_ERR;
                                 //���ҳ�����
        for(i=0;i<EX_TABLE_NUM;i++)
		{
					if(Ex_Table.Exchange[i].type<EX_DEV_KEY)									//����Ƕ�Ӧͨ����ң������δʹ�ã��򲻲������ͨ��
					{
								for(j=0;j<DEV_SCENE_NUM;j++)
						       {
											if(Scene[i].item[j].id==id)
											{
												  rt=OPT_OK;
											    //ִ�г���
											    if((mode==0x01)||(Scene[i].item[j].dlytime==0))
                                                {          
                                                          IT2_Dev_To_Wireless(i,Scene[i].item[j].cmd,Scene[i].item[j].dat[0],0);																								
																													//Wireless_Dev_Action(i,Scene[i].item[j].cmd,Scene[i].item[j].dat[0],0);
                                                }
                                                else
                                                    {
													        Dly_Table.item[i].flag=1;;															//ʹ�ñ��
														 	Dly_Table.item[i].index=i;															//������
													        Dly_Table.item[i].cmd=Scene[i].item[j].cmd;
															Dly_Table.item[i].dat[0]=Scene[i].item[j].dat[0];          //ͨ����    
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
	 //���³������� 
	 
	 //������������
	 else if(m_cmd==F_ASK_SCENE_DATA)
	 {
				//�ѳ����������ݣ����ͳ�ȥ
				id=(uint16_t)((RxDat.buf[8]<< 8)+RxDat.buf[9]);
		    //���ҳ�����
		    for(i=0;i<EX_TABLE_NUM;i++)
				{
							if(Ex_Table.Exchange[i].type<EX_DEV_KEY)									//����Ƕ�Ӧͨ����ң������δʹ�ã��򲻲������ͨ��
							{
										for(j=0;j<DEV_SCENE_NUM;j++)
								       {
													if(Scene[i].item[j].id==id)
													{
                                                         //װ������
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
	 //�������
	 else if(m_cmd==F_ASK_SCENE_ADD)
	 {
		    if(RxDat.buf[6]!=0x40)return;
		 
				id=(uint16_t)((RxDat.buf[8]<<8 )+RxDat.buf[9]);	
		    num=RxDat.buf[12];
		    if(num==0)return;
		    offset=13;
		    //=========================================================================================================
		    //ѭ������
		    for(i=0;i<num;i++)
				{
					  chl=RxDat.buf[offset+2];
					  cmd=RxDat.buf[offset+5];
					  len=RxDat.buf[offset+7];				//�������ݳ���
					  if(len>5)len=5;
					  for(j=0;j<len;j++)cmd_dat[j]=RxDat.buf[offset+8+j];
					  dly=(uint16_t)((RxDat.buf[offset+10+len]<< 8)+RxDat.buf[offset+10+len+1]);//����ִ����ʱʱ��
					
					  offset=offset+10+len+2;
					
					  //���ת������Ӧͨ��δ���û��Ӧͨ����ң������������˳�
					  if(Ex_Table.Exchange[chl].type>=EX_DEV_KEY)return;
					  
					  //ͨ��ֻ����1-8
					  if((chl==0)||(chl>8))return;
					  chl=chl-1;
					
					  
						//���˳����ţ��Ƿ��Ѿ����棬������棬�򸲸�
		          rt=0xff;
					    for(j=0;j<DEV_SCENE_NUM;j++)
				      {
									if(Scene[chl].item[j].id==id)
									{
											rt=j;
										  break;
									}
						  }
							if(rt>=DEV_SCENE_NUM)																//����ID���µģ������δʹ�õı���
							{
										//���Ҷ�Ӧͨ���豸��δʹ�õı����������,�˳����ش���
										for(j=0;j<DEV_SCENE_NUM;j++)
										{
												if(Scene[chl].item[j].flag==0)break;
										}
										if(j==DEV_SCENE_NUM)return;
										rt=j;																					//δʹ�ñ���
							}
						
							//д�볡����
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
	 //����ɾ��
	 else if((m_cmd==F_ASK_SCENE_DEL)||(m_cmd==B_DEL_SCENE))
	 {
				id=(uint16_t)((RxDat.buf[8]<< 8)+RxDat.buf[9]);
		    //���ҳ�����
		    for(i=0;i<EX_TABLE_NUM;i++)
				{
							if(Ex_Table.Exchange[i].type<EX_DEV_KEY)									//����Ƕ�Ӧͨ����ң������δʹ�ã��򲻲������ͨ��
							{
										for(j=0;j<DEV_SCENE_NUM;j++)
								    {
													if(Scene[i].item[j].id==id)
													{
														  //ɾ������
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
//��ʱ���༭��ִ��
void IT2_Timer_Action(uint8_t *pt)
{
	*pt=OPT_ERR;
}

//===============================================================================================================
//�������
void IT2_Array_Action(uint8_t *pt)
{
	*pt=OPT_ERR;
}

//=========================================================================================================================
//485Ӧ��
void IT2_Load_RS485_Ack(uint8_t st)
{	
	uint8_t len=0,i;
	
	TxDat.buf[0]=0xa5;						//֡ͷ
  TxDat.buf[2]=0x00;						//Ŀ���ַ	
	TxDat.buf[3]=RxDat.buf[3];					//���к�
	TxDat.buf[4]=Dev.addr[0];					//���ص�ַ
	//=================================================================================			
	if(st>0xf0)							//ͨ�÷���
	{
				if(st==OPT_OK)
				{
								TxDat.buf[1]=F_ACK_DEV;						//֡����
								TxDat.buf[5]=0x00;		                        //�����򳤶�		
								len=6;							//�����ܵ��ֽ���
				}
				else if(st==OPT_ERR)
				{	
								TxDat.buf[1]=F_ACK_DEV;						//֡����
								TxDat.buf[5]=0x03;		                        //�����򳤶�
								TxDat.buf[6]=0x06;		                        //��������
								TxDat.buf[7]=0x01;		                        //����
								TxDat.buf[8]=0xf1;								//����Ӧ��							
								len=9;														//�����ܵ��ֽ���												
				}
	}
	//=================================================================================
	else if(st==F_ACK_TIME)						//����ʱ��
	{
				TxDat.buf[1]=F_ACK_TIME;	
				TxDat.buf[5]=0x08;		                        //�����򳤶�
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
	//���ذ汾
	else if(st==F_ACK_VERSION)
	{
				TxDat.buf[1]=F_ACK_VERSION;									//Э�鲻ͨ
				TxDat.buf[5]=0x09;		                      //�����򳤶�
				
				//ģ��mac
				TxDat.buf[6]=0x20;
				TxDat.buf[7]=0x08;
				for(i=0;i<8;i++)TxDat.buf[8+i]=IT2_INFO[i];
				//�汾��
				TxDat.buf[16]=0x0d;
				TxDat.buf[17]=0x02;
				TxDat.buf[18]=0x00;
				TxDat.buf[19]=DEV_IT2_VER;	
				len=20;		
	}
	
	TxDat.buf[len]=Crc8CheckSum(TxDat.buf,len);		//CRC8У��
	TxDat.cnt=len+1;	
	TxDat.f_tx=1;						//��λ485���ͱ�־,ֱ�ӷ���
	
}
//==========================================================================================
//�����ϱ���Ϣ
void IT2_Dev_Report(uint8_t st,uint8_t index)
{
		_Frame *p;			//����֡�������
		uint8_t tmp,i;
		//==================================================================================
		//�豸�����޸��ϱ�
		if(st==F_REPORT_DEV_MODIFY)				
		{
					p=GetQueue_InQ(&RS485_Queue,index);		       //ͨ��8Ϊ����ͨ��
					if(p==NULL)return;
				
					p->buf[0]=0xa5;
					p->buf[1]=F_REPORT_DEV_MODIFY;
					p->buf[2]=0x00;
					p->buf[3]=Dev.serial++;
					p->buf[4]=Dev.addr[0];
					p->buf[5]=7;					//���ݳ���(�ܸ���-7)
					
					//�豸ͨ��
					p->buf[6]=0x29;
					p->buf[7]=0x01;
					p->buf[8]=index;				//ʱ�����豸ͨ��
					
					//�豸����
					p->buf[9]=0x22;
					p->buf[10]=0x02;

                    if(index==0)                    //ת��������ͨ��
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
					
					p->cnt=14;					//�����ܵ��ֽ���			
					p->flag=1;
					p->resend=5;
					p->offline=0;
						
					EnQueueBuf(&RS485_Queue,index);	
		}
    //===============================================
    //�豸����ɾ���ϱ�
    else if(st==F_REPORT_MODULE_DEL)
		{
					p=GetQueue_InQ(&RS485_Queue,index);		       //ͨ��8Ϊ����ͨ��
					if(p==NULL)return;
				
					p->buf[0]=0xa5;
					p->buf[1]=F_REPORT_DEV_MODIFY;
					p->buf[2]=0x00;
					p->buf[3]=Dev.serial++;
					p->buf[4]=Dev.addr[0];
					p->buf[5]=0;					//���ݳ���(�ܸ���-7)
								
					p->buf[6]=Crc8CheckSum(p->buf,6);
					
					p->cnt=7;					//�����ܵ��ֽ���			
					p->flag=1;
					p->resend=5;
					p->offline=0;
						
					EnQueueBuf(&RS485_Queue,index);	
		}
	//===================================================================================
   else if(st==F_ASK_CONFIG_KEY)
    {
// 					p=GetQueue_InQ(&RS485_Queue,8);		       //ͨ��8Ϊ����ͨ��
// 					if(p==NULL)return;
// 				
// 					p->buf[0]=0xa5;
// 					p->buf[1]=F_ASK_CONFIG_KEY;
// 					p->buf[2]=0x00;
// 					p->buf[3]=Dev.serial++;
// 					p->buf[4]=Dev.addr[0];
// 					p->buf[5]=3;					//���ݳ���(�ܸ���-7)

// 					//=================================================
// 					//��ֵ
// 					p->buf[6]=0x29;
// 					p->buf[7]=0x01;
// 					p->buf[8]=index;
// 								
// 					p->buf[9]=Crc8CheckSum(p->buf,9);
// 					
// 					p->cnt=10;					//�����ܵ��ֽ���			
// 					p->flag=1;
// 					p->resend=5;
// 					p->offline=0;
// 						
// 					EnQueueBuf(&RS485_Queue,8);	
    }
    //�豸ɾ���ϱ�
    else if(st==F_REPORT_DEV_DEL)
        {
                    p=GetQueue_InQ(&RS485_Queue,index);		       //ͨ��8Ϊ����ͨ��
					if(p==NULL)return;
				
					p->buf[0]=0xa5;
					p->buf[1]=F_REPORT_DEV_DEL;
					p->buf[2]=0x00;
					p->buf[3]=Dev.serial++;
					p->buf[4]=Dev.addr[0];
					p->buf[5]=3;					//���ݳ���(�ܸ���-7)
					p->buf[6]=0x29;
                    p->buf[7]=0x01;
                    p->buf[8]=index;
								
					p->buf[9]=Crc8CheckSum(p->buf,9);
					
					p->cnt=10;					//�����ܵ��ֽ���			
					p->flag=1;
					p->resend=5;
					p->offline=0;
						
					EnQueueBuf(&RS485_Queue,index);	
        }
        //���������ϱ�
        else if(st==F_ACK_SCENE_DATA)
            {
                    p=GetQueue_InQ(&RS485_Queue,index);		       //ͨ��8Ϊ����ͨ��
					if(p==NULL)return;
                    
				
					p->buf[0]=0xa5;
					p->buf[1]=F_ACK_SCENE_DATA;
					p->buf[2]=0x00;
					p->buf[3]=Dev.serial++;
					p->buf[4]=Dev.addr[0];
					p->buf[5]=3;					                //���ݳ���(�ܸ���-7)

                    //����ID
					p->buf[6]=0x40;
                    p->buf[7]=0x02;
                    tmp=Scene[index].num;
                    p->buf[8]=(uint8_t)(Scene[index].item[tmp].id>>8);
                    p->buf[9]=(uint8_t)(Scene[index].item[tmp].id&0x00ff);
                    //�������ݸ���
                    p->buf[10]=0x43;
                    p->buf[11]=0x01;
                    p->buf[12]=8+Scene[index].item[tmp].len;
                    //�豸ͨ����
                    p->buf[13]=0x29;
                    p->buf[14]=0x01;
                    p->buf[15]=index;
                    //����
                    p->buf[16]=0x23;
                    p->buf[17]=0x01;                    
                    p->buf[18]=Scene[index].item[tmp].cmd;
                    //��������
                    p->buf[19]=0x24;
                    p->buf[20]=Scene[index].item[tmp].len;  
                    for(i=0;Scene[index].item[tmp].len;i++)p->buf[21+i]=Scene[index].item[tmp].dat[i];
								
					p->buf[21+Scene[index].item[tmp].len]=Crc8CheckSum(p->buf,21+Scene[index].item[tmp].len);
					
					p->cnt=21+Scene[index].item[tmp].len+1;					                    //�����ܵ��ֽ���			
					p->flag=1;
					p->resend=5;
					p->offline=0;
						
					EnQueueBuf(&RS485_Queue,index);	
            }
}
//==========================================================================================
//��ַ������8���ֽڵ�ID������1���ֽڵĶ̵�ַ
void IT2_Dev_RequestAddr(void)
{
	
	_Frame *p;			//����֡�������
	uint8_t i;
	p=GetQueue_InQ(&RS485_Queue,8);		       //ͨ��8Ϊ����ͨ��
	if(p==NULL)return;
	
	p->buf[0]=0xa5;
	p->buf[1]=F_ASK_CONFIG_ADDR;
	p->buf[2]=0x00;
	p->buf[3]=Dev.serial++;
	p->buf[4]=Dev.addr[0];
  //p->buf[4]=0xFF;
	p->buf[5]=20;					//���ݳ���
	
	//ģ��MAC
	p->buf[6]=0x20;
	p->buf[7]=0x08;
	for(i=0;i<8;i++)p->buf[8+i]=IT2_INFO[i];
	
	//ģ������
	p->buf[16]=0x2e;
	p->buf[17]=0x02;
	p->buf[18]=(uint8_t)(MODULE_TYPE>>8);		
	p->buf[19]=(uint8_t)(MODULE_TYPE&0x00ff);
	
	//�ѽ�����ͨ��
	p->buf[20]=0x2a;
	p->buf[21]=0x01;
	p->buf[22]=0x08;
	
	//ȫ��ͨ��
	p->buf[23]=0x2b;
	p->buf[24]=0x01;
	p->buf[25]=0x08;
	
	p->buf[26]=Crc8CheckSum(p->buf,26);
	
	p->cnt=27;					//�����ܵ��ֽ���
	p->flag=1;
	p->resend=1;
	p->offline=0;
		
	EnQueueBuf(&RS485_Queue,8);
}
//===========================================================================================
//�豸ע�ᣬֻ����ȡ�ö̵�ַ�󣬲Ų�����һ��
void IT2_Dev_Register(void)
{
	_Frame *p;				//����֡�������
	uint8_t i,rt;
	
	if(Dev.stu>=DEV_NORMAL)return;
	
	//���ϱ�ע��ת��������
	if(Ex_Table.reg[0]==0)
	{
			p=GetQueue_InQ(&RS485_Queue,0);
			if(p==NULL)return;

			p->buf[0]=0xa5;
			p->buf[1]=F_REPORT_ONLINE;
			p->buf[2]=0x00;
			p->buf[3]=Dev.serial++;
			p->buf[4]=Dev.addr[0];
			p->buf[5]=13;				//���ݳ���
			
			//�豸����
			p->buf[6]=0x10;
			p->buf[7]=0x01;
			p->buf[8]=0x01;				//�������豸����Ϊ1
			
			//�豸ͨ����
			p->buf[9]=0x29;				
			p->buf[10]=0x01;
			p->buf[11]=0x00;			//ͨ����
		
		  //�豸ͨ����
			p->buf[12]=0x2f;				
			p->buf[13]=0x01;
			p->buf[14]=0x08;			//ͨ����
			
			//�豸����
			p->buf[15]=0x22;
			p->buf[16]=0x02;
			p->buf[17]=(uint8_t)(Dev.type>>8);
			p->buf[18]=(uint8_t)(Dev.type&0x00ff);
			
			//�豸״̬
			p->buf[19]=0x25;
			p->buf[20]=0x01;
			p->buf[21]=0x00;			//�豸״̬
			
			p->buf[22]=Crc8CheckSum(p->buf,22);
			
			p->cnt=23;				//�����ܵ��ֽ���
			p->flag=1;
			p->resend=1;
			p->offline=0;
			Ex_Table.index=0;
				
			EnQueueBuf(&RS485_Queue,0);
  }
	//��ͨ����һ�ϱ�ע��
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
									p->buf[5]=13;				//���ݳ���
									
									//�豸����
									p->buf[6]=0x10;
									p->buf[7]=0x01;
									p->buf[8]=0x01;				//�������豸����Ϊ1
									
									//�豸ͨ����
									p->buf[9]=0x29;				
									p->buf[10]=0x01;
									p->buf[11]=i+1;			//ͨ����
								
									//�豸ͨ����
									p->buf[12]=0x2f;				
									p->buf[13]=0x01;
									p->buf[14]=0x01;			//ͨ����
									
									//�豸����
									p->buf[15]=0x22;
									p->buf[16]=0x02;
									p->buf[17]=(uint8_t)(Ex_Table.Exchange[i].reg_type>>8);
									p->buf[18]=(uint8_t)(Ex_Table.Exchange[i].reg_type&0x00ff);
									
									//�豸״̬
									p->buf[19]=0x25;
									p->buf[20]=0x01;
									p->buf[21]=0x00;			//�豸״̬
									
									p->buf[22]=Crc8CheckSum(p->buf,22);
									
									p->cnt=23;				//�����ܵ��ֽ���
									p->flag=1;
									p->resend=1;
									p->offline=0;
									
									Ex_Table.index=i+1;
										
									EnQueueBuf(&RS485_Queue,i+1);
									
									break;
						}
						else
						{
							    Ex_Table.reg[i+1]=1;													//������ͨ��δ�գ���ֱ��ע�����
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
//Ӧ��֡����
uint8_t IT2_PraseASK(uint8_t cmd)
{
  	uint8_t crc,chl;
  	uint8_t i;
  	
  	if(RxDat.buf[0]!=0xa5)return 0;					//֡ͷ����
  	
  	//if((RxDat.buf[2]!=Dev.addr[0])&&(RxDat.buf[1]!=F_ACK_REQUEST_ADDR))return 0;	//��ַ����
  	if(RxDat.buf[2]!=Dev.addr[0])return 0;
	
	  //Ӧ��֡��֡���� Ϊ 0x3e
	  if(RxDat.buf[1]!=F_ACK_DEV)return 0;
	
  	crc=Crc8CheckSum(RxDat.buf,RxDat.cnt-1);
  	if(crc!=RxDat.buf[RxDat.cnt-1])return 0;			//crc����
	
		Led_set(0,10,50,1,1);
		  	
  	//=============================================================================
  	//ȡ����������ĵ�ַ
  	if(cmd==F_ASK_CONFIG_ADDR)					//��������̵�ַ
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
				//����Ӧ���ź�
				if((Dev.stu==DEV_REGISTER)&&(cmd==F_REPORT_ONLINE))
				{
						//Dev.stu=DEV_NORMAL;
					  Ex_Table.reg[chl]=1;
				}
				return 1;
  	}
}		


//==========================================================================
//���������
void IT2_Offline_DataLoad(uint8_t key)
{	
// 			_Frame *p;						//����֡�������
// 	    uint8_t len,i;
// 	
// 			//=============================================================================
// 			//�������δ�������߷��ͣ����˳�
// 			if(Pannel.item[key].flag==0)return;
// 	
// 	    p=GetQueue_InQ(&RS485_Queue,key);
// 			if(p==NULL)return;
// 	
// 			if(Pannel.item[key].type==1)				//�豸
// 			{
// 				p->buf[0]=0xa5;
// 				p->buf[1]=F_ACT_DEVICE;				//�ϱ�֡
// 				p->buf[2]=Pannel.item[key].tg_id;		
// 				p->buf[3]=Dev.serial++;
// 				p->buf[4]=Dev.addr[0];
// 				//p->buf[5]=;					//���ݳ���
// 				//==========================================================
// 				//�豸ͨ����
// 				p->buf[6]=0x29;
// 				p->buf[7]=0x01;
// 				p->buf[8]=Pannel.item[key].buf[1];				
// 				//==========================================================
// 				//�豸����
// 				p->buf[9]=0x23;
// 				p->buf[10]=0x01;
// 				p->buf[11]=Pannel.item[key].buf[2];
// 				//==========================================================
// 				//�豸��������
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
// 				p->cnt=15+p->buf[13];			//�����ܵ��ֽ���
// 				
// 				p->buf[5]=p->cnt-7;
// 				
// 			  p->flag=1;
// 			  p->resend=5;
// 				p->offline=0;
// 				EnQueueBuf(&RS485_Queue,key);	
// 			}
// 			//===================================================================
// 			else if(Pannel.item[key].type==2)		//����
// 			{
// 				p->buf[0]=0xa5;
// 				p->buf[1]=B_EXEC_SCENE;				//�ϱ�֡
// 				p->buf[2]=Pannel.item[key].tg_id;		
// 				p->buf[3]=Dev.serial++;
// 				//p->buf[4]=Dev.addr[0];
// 				p->buf[4]=0x00;
// 				//p->buf[5]=;					//���ݳ���
// 				//==========================================================
// 				//����ID
// 				p->buf[6]=0x40;
// 				p->buf[7]=0x02;
// 				p->buf[8]=Pannel.item[key].buf[0];
// 				p->buf[9]=Pannel.item[key].buf[1];					
// 				//==========================================================
// 				//����ִ��ģʽ
// 				//p->buf[10]=0x41;
// 				//p->buf[11]=0x01;
// 				//p->buf[12]=Pannel.item[key].buf[2];
// 				//==========================================================
// 				p->buf[10]=Crc8CheckSum(p->buf,10);
// 						
// 				p->cnt=11;			//�����ܵ��ֽ���
// 				
// 				p->buf[5]=4;
// 				p->flag=1;
// 			  p->resend=5;
// 				p->offline=0;
// 				EnQueueBuf(&RS485_Queue,key);
// 			}
// 			//==================================================================
// 			else if(Pannel.item[key].type==3)		//����
// 			{
// //				p->buf[0]=0xa5;
// //				p->buf[1]=B_EXEC_SCENE;				//�ϱ�֡
// //				p->buf[2]=Pannel.item[key].tg_id;		
// //				p->buf[3]=Dev.serial++;
// //				p->buf[4]=Dev.addr[0];
// //				//p->buf[5]=;					//���ݳ���
// //				//==========================================================
// //				//����ID
// //				p->buf[6]=0x40;
// //				p->buf[7]=0x02;
// //				p->buf[8]=Pannel.item[key].buf[2];
// //				p->buf[9]=Pannel.item[key].buf[3];					
// //				//==========================================================
// //				//����ִ��ģʽ
// //				p->buf[10]=0x41;
// //				p->buf[11]=0x01;
// //				p->buf[12]=Pannel.item[key].buf[4];
// //				//==========================================================
// //				p->buf[13]=Crc8CheckSum(p->buf,13);
// //						
// //				p->cnt=14;			//�����ܵ��ֽ���
// //				
// //				p->buf[5]=7;
// //				
// //				EnQueueBuf(&RS485_Queue,key);
// 				//==========================================================
// 				//ȡ�����У�����array_buf��
// 				len=(Pannel.item[key].datalen-2)/5;	//ȥ������ID
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
// 			else if(Pannel.item[key].type==4)			//����
// 			{
// 				p->buf[0]=0xa5;
// 				p->buf[1]=F_ACT_DEVICE;				//�豸ִ��
// 				p->buf[2]=Pannel.item[key].tg_id;		
// 				p->buf[3]=Dev.serial++;
// 				p->buf[4]=Dev.addr[0];
// 				//p->buf[5]=;					//���ݳ���
// 				//==========================================================
// 				//�豸ͨ����
// 				p->buf[6]=0x29;
// 				p->buf[7]=0x01;
// 				p->buf[8]=Pannel.item[key].buf[1];				
// 				//==========================================================
// 				//�豸����
// 				p->buf[9]=0x23;
// 				p->buf[10]=0x01;
// 				p->buf[11]=0x04;
// 				//==========================================================
// 				p->buf[12]=Crc8CheckSum(p->buf,12);
// 						
// 				p->cnt=13;			//�����ܵ��ֽ���
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
//ת�����յ�ң�����İ����ź�
//�յ��ļ�ֵӦ����0x07,0x08,0x09,0x21��0x22
void IT2_Pannel_DataLoad(uint8_t index,uint8_t value)
{

 
			_Frame *p;						//����֡�������
			uint8_t dat=0,stu_dat=0;
	
	    //ң�����ϱ�
			//=====================================================================
      if(Ex_Table.Exchange[index].type>=EX_KEY_SCENE)
			{
					if(value==0x0e)value=0x08;			//��ֵ

					if(value==0x07)     stu_dat=0x00;
					else if(value==0x08)stu_dat=0x01;
					else if(value==0x09)stu_dat=0x02;
					else 								stu_dat=0x03;
				
				  dat=1;
				
			}
			//=================================================================================================================
			//�豸״̬�ϱ�
			else
			{
				        //========================================================================================================
								if(Ex_Table.Exchange[index].type==EX_DEV_LIGHT)	            //�ƹ��ϱ�
                {
												if(value==0x00) dat=0x01;
												else            dat=0x00;
												
												stu_dat=0;
                }
								//=========================================================================================================							
                else if(Ex_Table.Exchange[index].type==EX_DEV_SOCKET)				//���ܲ���
                {
                        if(value==0x00) dat=0x01;
                	      else            dat=0x00;
									     
									      stu_dat=0;
                }
								//=========================================================================================================
                else                                                           //��������ϱ�״̬
                {
                        if((Ex_Table.Exchange[index].type==EX_DEV_MOTOR_E)||(Ex_Table.Exchange[index].type==EX_DEV_BLIND_E))
                        {
																//ID����868 �� IT485 �г̱�ʾ�෴
                                //if(((Ex_Table.Exchange[index].channel&0x80)==0x80)&&(dat<=0x64))
																
																if(value>0x64)
																{
																				 dat=Ex_Table.stu[index]+0x03;
																	       stu_dat=0;
																}
																else		//�����г�
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
			p->buf[1]=F_REPORT_STATUS;			//�ϱ�֡
			p->buf[2]=0x00;		
			p->buf[3]=Dev.serial++;
			p->buf[4]=Dev.addr[0];	
				
			p->buf[5]=12;				//���ݳ���
			//=================================================================
			//״̬����Ϊ1
			p->buf[6]=0x10;			
			p->buf[7]=1;				
			p->buf[8]=1;			
			//=================================================================
			//�豸ͨ����
			p->buf[9]=0x29;				
			p->buf[10]=0x01;		
			p->buf[11]=index;
			//=================================================================
			//�豸״̬
			p->buf[12]=0x25;				
			p->buf[13]=0x01;
			p->buf[14]=dat;		
			//==================================================================
			//״ֵ̬
			p->buf[15]=0x26;				
			p->buf[16]=0x01;		
			p->buf[17]=stu_dat;
			//==================================================================
			
			p->buf[18]=Crc8CheckSum(p->buf,18);			
			p->cnt=19;			//�����ܵ��ֽ���
			p->flag=1;
			p->resend=5;
			p->offline=1;
			EnQueueBuf(&RS485_Queue,index);
}
//===============================================================================================
//������������
void IT2_Array_Handle(void)
{
	_Frame *p;
	
	if(Array.flag==1)
	{
		p=GetQueue_InQ(&RS485_Queue,Array.key);
		if(p==NULL)return;
		
		if(Array.buf[Array.index].dly>0)
		{
			Array.buf[Array.index].dly--;			//����С��λ�ݼ�
		}
		else
		{
			p->buf[0]=0xa5;
			p->buf[1]=B_EXEC_SCENE;				//�ϱ�֡
			p->buf[2]=0xff;		
			p->buf[3]=Dev.serial++;
			p->buf[4]=Dev.addr[0];
			//p->buf[5]=;					//���ݳ���
			//==========================================================
			//����ID
			p->buf[6]=0x40;
			p->buf[7]=0x02;
			p->buf[8]=(uint8_t)(Array.buf[Array.index].id>>8);
			p->buf[9]=(uint8_t)(Array.buf[Array.index].id&0x00ff);					
			//==========================================================
			//����ִ��ģʽ
			p->buf[10]=0x41;
			p->buf[11]=0x01;
			p->buf[12]=Array.buf[Array.index].mode;
			//==========================================================
			p->buf[13]=Crc8CheckSum(p->buf,13);
					
			p->cnt=14;			//�����ܵ��ֽ���
			
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
//��������ת����
//==============================================================================================
//�豸ִ��-ת�������պ��������豸ת��
//���ݸ�ʽ���豸ͨ����+�豸����+�豸��������
void IT2_Dev_Action(uint8_t *pt)
{
	      uint8_t index=0,cmd=0,cmd_dat=0;
   	     
	      *pt=OPT_OK;
				//======================================================================================
				//�豸ͨ����
				if(RxDat.buf[6]==0x29)											
				{
						index=RxDat.buf[8];
						if((index>8)||(index==0))*pt=OPT_ERR;
				}
				//=======================================================================================
				//�豸����
				if(RxDat.buf[9]==0x23)
				{
						 cmd=RxDat.buf[11];
				}
				else
				{
					   *pt=OPT_ERR;
				}
				//========================================================================================
				//�豸��������
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
				//����ת����������
				index=index-1;
				*pt=IT2_Dev_To_Wireless(index,cmd,cmd_dat,0);
		
}
//===================================================================================================
//RS485����ת�����߶���
uint8_t IT2_Dev_To_Wireless(uint8_t index,uint8_t cmd,uint8_t cmd_dat,uint8_t scene_num)
{
	      uint8_t type,route,rt;     
	
				type=Ex_Table.Exchange[index].type;
        rt=OPT_ERR;
				//===============================================================================================
				//�����
				if((type==EX_DEV_MOTOR_R)||(type==EX_DEV_MOTOR_E)||(type==EX_DEV_BLIND_E))
				{
						if(cmd==0x10)			//�����
						{
							  if(Ex_Table.Exchange[index].stu==0)cmd=0x07;
							  else                               cmd=0x09;
								Wireless_Dev_Action(index,cmd,0,0);
							  Ex_Table.stu[index]=0x10;
						}
						else if(cmd==0x12)//�����
						{
							  Wireless_Dev_Action(index,0x08,0,0);
							  Ex_Table.stu[index]=0x12;
						}
						else if(cmd==0x11)//�����
						{
							  if(Ex_Table.Exchange[index].stu==0)cmd=0x09;
							  else                               cmd=0x07;
							  Wireless_Dev_Action(index,cmd,0,0);
							  Ex_Table.stu[index]=0x11;
						}
						else if(cmd==0x1d)//�����ת
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
						else if(cmd==0x14)//���е�ָ����
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
				else if(type==EX_DEV_SOCKET)   //���ܲ���
				{
						if(cmd==0x02)     //�ص�
						{
							  Wireless_Dev_Action(index,0x08,0,0);
							  Ex_Table.stu[index]=0x03;
						}
						else if(cmd==0x01)//����
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
				else if(type==EX_DEV_LIGHT)    //�ƹ�			
				{
					  if(cmd==0x02)     //�ص�
						{
							   Wireless_Dev_Action(index,0x08,0,0);
							   Ex_Table.stu[index]=0x00;
						}
						else if(cmd==0x01)//����
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
				//����� add 2016.2.23
				else if(type==EX_DEV_LIGHT_ADJ)    //�ƹ�			
				{
					  if(cmd==0x01)     //�ص�
						{
							   Wireless_Dev_Action(index,0x08,0,0);
							   Ex_Table.stu[index]=0x00;
						}
						else if(cmd==0x02)//����
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
//ת��������豸��ɾ��
void IT2_Exchange_Action(uint8_t *pt)
{
	 	uint8_t cmd,type=0,index=0,Iscode=0;
	  	
	  *pt=OPT_OK;

    //�����������״̬������������
    if(Dev.stu==DEV_SETUP)
		{
			  *pt=OPT_CANCEL;
				return;
		}
 	 	
		cmd=RxDat.buf[1];
		
	  //=============================================================================
	  //�ж�ͨ��
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
					//�ж�����
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
		//����豸
    if(cmd==F_ASK_EXCHANGER_ADDCODE) 
 		{			
					//�����豸Ϊң����ʱ��ֻ����ѧϰʱ�䣬�������豸Ϊ�����豸ʱ��ֻ�ж���ʱ��
					if((type==EX_KEY_SCENE)||(type==EX_KEY_MOTOR)||(type==EX_KEY_LIGHT)||(type==EX_KEY_GENERAL))
					{
									Dev.stu=DEV_SETUP;
									Dev.learn_time=LEARN_TIME_MAX;
									Dev.encode_time=0;
									Iscode=1;
					}
					//else if(tmp<=EX_DEV_LIGHT)
					else if(type<EX_DEV_KEY)											//��������ƹ���
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
					//Ҫ�����ӵ������豸��ң����
					if(Iscode==1)
					{
									//��Ҫ�����߷��ͣ�ֻ�ȴ�ң��������
									Exchangebuf.type= type;                                    //�豸����
									Exchangebuf.addr= index;
									Exchangebuf.channel=1;
									Exchangebuf.wl_addr[0]=	0;				                  			//�ȴ�����ң�����ĵ�ַ
									Exchangebuf.wl_addr[1]= 0;  		                      		//
									Exchangebuf.wl_addr[2]= 0;  
                  Exchangebuf.reg_type=(uint16_t)((RxDat.buf[11]<< 8)+(RxDat.buf[12]));
					}
					//Ҫ�����ӵ������豸�ǿ����豸
					else if(Iscode==2)
					{          
									Exchangebuf.type= type;                                    //�豸����
									Exchangebuf.addr= index;;            
									Exchangebuf.channel=1;
									Exchangebuf.wl_addr[0]=	type;              
									//Exchangebuf.wl_addr[1]= Dev.addr[0];  	                  	//�����-ң������ַ1 
									Exchangebuf.wl_addr[1]=Crc8CheckSum(IT2_INFO,8);
									Exchangebuf.wl_addr[2]=Dev.serial++;
                  Exchangebuf.reg_type=(uint16_t)((RxDat.buf[11]<< 8)+(RxDat.buf[12]));
									Wireless_Dev_Action(Exchangebuf.addr,0x01,0,0);         	//װ���߷�������
					}
					//�豸δʶ��
					else
					{
									*pt=OPT_ERR;
					}					
		}
 		//===================================================================================
		else if(cmd==F_ASK_EXCHANGER_DELCODE)
		{
					//ɾ��ָ���豸
					Exchangebuf.type=Ex_Table.Exchange[index].type;
					Exchangebuf.wl_addr[1]=Ex_Table.Exchange[index].wl_addr[1];
					Exchangebuf.wl_addr[2]=Ex_Table.Exchange[index].wl_addr[2];
					Wireless_Dev_Action(index,0x02,0,0);         //װ���߷�������
				
					//type=Exchangebuf.type;
					Exchangebuf.type=0xff;	
					Exchangebuf.addr=0xff;
					Exchangebuf.channel=0xff;
					Exchangebuf.wl_addr[0]=0xff;
					Exchangebuf.wl_addr[1]=0xff;
					Exchangebuf.wl_addr[2]=0xff;
                    Exchangebuf.reg_type=0;
					//д����
					Write_Ex_Table(index);
					//Exchangebuf.type=type;		
					IT2_Dev_Report(F_REPORT_DEV_DEL,index+1);    //ת����ͨ���ͱ��ͨ����һ��
		}
		else
		{
					*pt=OPT_ERR;
		}
}

//=========================================================================================================
//����485�� �豸����ת������868
uint8_t IT2_Changer_GetType(uint8_t t1,uint8_t t2)
{
	  uint8_t type;
	
		if(t1==0x20)										//��е�г�-����
		{
				type=EX_DEV_MOTOR_R;
		}
		else if(((t1==0x21)&&(t2!=15))||(t1==0x22)||(t1==0x23)||(t1==0x25))			//�����г�-����
		{
			  type=EX_DEV_MOTOR_E;
		}
		else if((t1==0x21)&&(t2==15))			            //�����г�-������
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
