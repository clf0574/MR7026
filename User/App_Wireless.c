		
//==================================================================================================

//Ӳ��֧�� CC1101
//֧��BSP bsp_cc1101
#include <includes.h>
//==================================================================================================
//�����������ܼҾ�-IDϵͳ-��һ��Э�� ����868Mת����
//==================================================================================================  
//==================================================================================================
#define WIRELESS_SEND_DLY_TIME 2


	
	
	

//uint8_t  SumCheck(uint8_t  *p,uint8_t length);
//uint8_t  Crc8CheckSum(uint8_t *ptr,uint8_t length);
uint8_t Wireless_Send_Check(uint8_t cmd,uint8_t index);
void Check_Wireless_Addr(uint8_t *pbuf,uint8_t *rt);
void Check_WlLock_Addr(uint8_t *pbuf,uint8_t *rt);
void Wireless_Recv_Disable(uint8_t *pdat,uint8_t time);
uint8_t Wireless_Recv_IsRun(uint8_t *pdat);
void Wireless_Learn_Finish(void);
void WlLock_Learn_Finish(void);
//���ڹ����ظ�֡
static uint8_t wl_frame[16],dis_tim=0;
uint8_t wbuf[80];														
	
//==================================================================================================
//���߷��ʹ���
//ÿ��100mS����һ��
void Wireless_Send(void)
{
				_Frame *pws;
				uint8_t i,index;
				
				//pws=GetQueue_rear(&WirelessSend_Queue);
	      pws=GetQueue_OutQ(&WirelessSend_Queue,&index);
				//�ж��Ƿ���ջ�����Ϊ��
				if(pws==NULL)
				{
					return;
				}
				
				//============================================================================================
				if(pws->flag==1)				//������Ҫ����
				{
					if(pws->ackwait>0)			//���ڵȴ�Ӧ�𣬲�����
					{
						pws->ackwait--;
						if(pws->ackwait==0)
						{
							if(pws->resend>0)       //�ط�������ȥ-    
							{
										pws->resend--;
										//pws->rand=Dev.rand%3;//���ʱ��Ϊ0~150mS֮��
								    pws->rand=1;//���ʱ��Ϊ0~150mS֮��
							}
							else
							{
										//��֡�˳������������ڷ���
										for(i=0;i<pws->cnt;i++)pws->buf[i]=0x00;
										pws->cnt=0;
										pws->flag=0;
										pws->ackwait=0;
										//DeQueueBuf(&WirelessSend_Queue);
										DeQueueBuf(&WirelessSend_Queue,index);
								
										if(Ir.wait==1)Ir.wait=0;																//2017.1.12
							}
						}
					}
					else
					{
						if(pws->rand==0)			//��ʱʱ�䵽�����Է�������
						{
									#ifdef USE_A7139
							    RF_TX(pws->buf,pws->cnt);
									bsp_InitRF(0);
							    #else
									RfSendPacket(pws->buf,pws->cnt);      //��������
									//bsp_InitCC1101();
							    #endif
									
									if(pws->resend==1)     i=3;
									else if(pws->resend==2)i=2;
									else                   i=1;
									
									pws->ackwait=i;		//�ȴ�Ӧ��ʱ��Ϊ150mS
							
						}
						else
						{
									pws->rand--;
						}
					}
				}
				//���ݷ������
				else
				{
									for(i=0;i<pws->cnt;i++)pws->buf[i]=0x00;
									pws->cnt=0;
									pws->flag=0;
									pws->ackwait=0;
									//DeQueueBuf(&WirelessSend_Queue);
									DeQueueBuf(&WirelessSend_Queue,index);
									
									if(Ir.wait==1)Ir.wait=0;																//2017.1.12
				}
}
//���߽��մ���======================================================================================
//�߸����ȼ�
void Wireless_Recv(void)
{
	_Frame *pws;
	uint8_t i,tmp_sum,tmp_crc/*,tmp*/;
	uint8_t index,cmd=0,dat,rt,recv_ok;
	uint8_t len,buf[5];
	uint8_t datlen;
	uint8_t type;
	uint16_t type_cmd,SyncNum;
	static uint16_t CC1101_ResetTime=0;
	static uint8_t step;
  uint8_t lin,row,tt;
	
	//����֡����ʱ��
	if(dis_tim>0)dis_tim--;
	//���ڼ��CC1101�Ƿ���
	CC1101_ResetTime++;
	#ifdef USE_A7139
	recv_ok=RF_RX(&wbuf[0],&len);
	if((1==recv_ok)&&(len>=16))
	#else
	recv_ok=RfReceivePacket(&wbuf[0],&len); 
	if((1==recv_ok)&&(len>=16))
	#endif
	//�н�������
	{
					CC1101_ResetTime=0;
					//=============================================================
					//����У��
					tmp_sum=SumCheck(&wbuf[0],(len-2));
					tmp_crc=Crc8CheckSum(&wbuf[0],(len-2));	
					if((tmp_crc!=wbuf[len-2])||(tmp_sum!=wbuf[len-1]))return;
		      //=============================================================
		      //����ֱ�����빦��==ֻ���ڵ��ԣ�ʵ��ʱӦ�����Σ���ֹ����USE_SERIAL_DEBUG
		      #ifdef USE_SERIAL_DEBUG
					//ComposePacket(&wbuf[10],4,index);
		      ComposePacketFromDS28E02(&wbuf[6],&wbuf[10],4,0);
		      RS485_Send(wbuf,len);
		      Led_set(1,25,25,1,2);
		      Dev.stu=DEV_NORMAL;
		      return;
					#endif
		      //==============================================================
					//Ŀ���ַ�ж� �ǹ㲥��ַ���߱����ַ �����
					rt=0;
					for(i=0;i<4;i++){if(wbuf[2+i]!=0xff)rt++;}
					if(rt!=0)
					{
							rt=0;
							for(i=0;i<4;i++)if(wbuf[2+i]!=Ex_Table.addr[i])rt++;		
					}
					if(rt!=0)return;
					//=============================================================
					//Դ��ַ�жϣ��Ƿ񱣴�	
					if((Dev.type&0xf000)==0xb000)
					{
							Check_Wireless_Addr(&wbuf[6],&index);	
					}
					else if((Dev.type&0xf000)==0x3000)
					{
							Check_WlLock_Addr(&wbuf[6],&index);	
					}
					//=============================================================
					//����ͬ�������߽���֡?
					rt=Wireless_Recv_IsRun(&wbuf[0]);
					if(rt!=OPT_OK)return;		
					//=============================================================
					if(index!=RE_ERR){Led_set(1,25,25,1,2);}
					//=============================================================
					//��������״̬
					if(Dev.stu==DEV_NORMAL)
					{		
								if(index==RE_ERR)return;
								//=======================================================
								if((wbuf[1]&0x80)==0x80)				//����Ҫ����
								{
												Wireless_Recv_Disable(&wbuf[0],150);			//���Ժ��100*5mSʱ���ڣ���ֹ����ͬ����֡
												//==============================================
												//ң�����򴫸����ϱ�
												if(wbuf[1]==0x11)
												{															
																if(Dev.profile==IT1)
																{
																	//54����42���ϱ�
																	if((Ex_Table.Exchange[index].type==EX_KEY_54KEY)||(Ex_Table.Exchange[index].type==EX_KEY_42KEY))
																	{
																			cmd=wbuf[12];				//��ֵ������
																			dat=wbuf[13];				//ģʽ������
																			//IT1_RS485_DataLoad(index,cmd,dat);
																		  IT1_Key_DataLoad(index,cmd,dat);
																	}
																	else if(Ex_Table.Exchange[index].type==EX_DEV_SUN)
																	{
																			buf[0]=2;
																			buf[1]=wbuf[12];				//��ֵ������
																			buf[2]=wbuf[13];				//ģʽ������
																		  IT1_Sensor_DataLoad(index,&buf[0]);
																	}
																	else if(Ex_Table.Exchange[index].type==EX_DEV_SUN1)
																	{
																			buf[0]=2;
																			buf[1]=wbuf[10];				//
																			buf[2]=wbuf[11];				//�
																		  IT1_Sensor_DataLoad(index,&buf[0]);
																	}
																	else if((Ex_Table.Exchange[index].type==EX_KEY_NORMAL1)||(Ex_Table.Exchange[index].type==EX_KEY_FULL1))
																	{
																			cmd=wbuf[12]+1;				//��ֵ������
																			dat=wbuf[13];				//ģʽ������
																		  IT1_Key_DataLoad(index,cmd,dat);
																	}
																	else
																	{
																			len=wbuf[12];
																	    for(i=0;i<len;i++)buf[i]=wbuf[12+i];
																			IT1_RS485_StuLoad(index,len,&buf[0]);
																	}
																}
												}
												//===============================================================================
												//��ת������ң���ϱ�
												else if(wbuf[1]==0x12)
												{
																SyncNum=(wbuf[10]<<8)+wbuf[11];
																if(Ex_Table.SyncNum[index]==SyncNum)return;
																Ex_Table.SyncNum[index]=SyncNum;
																cmd=wbuf[12];				//��ֵ������
																dat=wbuf[13];				//ģʽ������
																IT1_Turn_DataLoad(index,cmd,0);
																type_cmd=0;
																type_cmd=0x01<<8;
																type_cmd=type_cmd+cmd;
																if(dat>0){dat--;while(dat>0){bsp_PutMsg(type_cmd,index);dat--;}}
												}
												//===============================================================================
												//54��ң������������Ӧ�� ��16���ֽ�
												else if((wbuf[1]==0x00)&&(Ir.wait==1)&&((wbuf[10]&0x0f)==Ir.serial))
												{
																pws=GetQueue_OutQ(&WirelessSend_Queue,&index);
																//�ж��Ƿ���ջ�����Ϊ��
																if(pws==NULL){return;}
																
																//��֡�˳������������ڷ���
																for(i=0;i<pws->cnt;i++)pws->buf[i]=0x00;
																pws->cnt=0;
																pws->flag=0;
																pws->ackwait=0;
													
																DeQueueBuf(&WirelessSend_Queue,index);
																//�����ٴ�װ���������
																Ir.wait=0;																//2017.1.12
												}
												//=================================================================================
												//����������ɺ�DQ8028�����ϱ�һ�� DTM_IR_SETUP_FINISH
												else if((wbuf[1]==DTM_IR_SETUP_FINISH)&&(Ir.flag==0))
												{
																//2017.4.5�޸�
																Wireless_Recv_Disable(&wbuf[0],100);			//���Ժ��100*5mSʱ���ڣ���ֹ����ͬ����֡
													
																 //�Ƚ�DQ8010�������ۼӺͺͻ��������ۼӺͱȽ�
																 if((Ir.typechm==wbuf[10])&&(Ir.keychm==wbuf[11]))
																 {
																				IT1_IrSetup_Report(OPT_OK);
																				Ir_Config_Add(index,Ir.cmd_key,Ir.key_mode);						//��ֵ
																 }
																 else
																 {
																				IT1_IrSetup_Report(OPT_ERR);
																 }
												}
												//=====================================================================================
												//��������ɾ�����ϱ�
												else if((wbuf[1]==0x00)&&(wbuf[10]==Ir.serial)&&(wbuf[12]==MTD_IR_DEL))
												{
																Wireless_Recv_Disable(&wbuf[0],230);
																IT1_IrDel_Report(index);
																Ir_Config_Del(index,Ir.cmd_key,Ir.key_mode);					//��ֵ
												}
												//=====================================================================================
												//ִ�к󷵻ص�״̬
												else if((wbuf[1]==0xe0)||(wbuf[1]==0xe1))
												{
																Pannel_Table_Refresh(index,wbuf[1],&wbuf[10]);
												}
													
									}
									//==========================================================================================
									//����Ҫ�õ����ܳ��� 20171206�¼�
									else
									{
												Wireless_Recv_Disable(&wbuf[0],250);			//���Ժ��100*5mSʱ���ڣ���ֹ����ͬ����֡
												//���ݴ���
												if((Dev.type&0xf000)==0xb000)
												{
															//�Ƚ��ܣ����ܵ���Կ�ڱ����
															if(wbuf[1]!=0x00)
															{
																	datlen=len-12;
																	ComposePacket(&wbuf[10],datlen,index);
																	//�Ƚ�ͬ����
																	if(OPT_OK!=CompareSync(&wbuf[10],index))return;
															}
															//====================================================================================
															//���λ��ʾ�Ƿ�Ҫ����
															//wbuf[1]=wbuf[1]&0x7f;
															//wbuf[1] Ϊ0x01 02 03 04 05 06 07 08 09ʱ������
															//��ת������ң���ϱ�
															if(wbuf[1]==0x12)
															{
																		SyncNum=(wbuf[10]<<8)+wbuf[11];
																		if(Ex_Table.SyncNum[index]==SyncNum)return;
																		Ex_Table.SyncNum[index]=SyncNum;
																		cmd=wbuf[12];				//��ֵ������
																		dat=wbuf[13];				//ģʽ������
																		IT1_Turn_DataLoad(index,cmd,0);
																
																    //20180307 ��תʱ����ʼ����ͽ�������
																    #if 0
																		type_cmd=0;
																		type_cmd=0x01<<8;
																		type_cmd=type_cmd+cmd;
																		if(dat>0){dat--;while(dat>0){bsp_PutMsg(type_cmd,index);dat--;}}
																    #else
																		type_cmd=0;
																		type_cmd=0x01<<8;
																		type_cmd=type_cmd+0x06;
																    if((cmd==5)||(cmd==6))
																		{
																				bsp_PutMsg(type_cmd,index);
																		}
																    #endif
															}
															else if(wbuf[1]==0x11)
															{
																		cmd=wbuf[12]+1;				//��ֵ������
																		dat=wbuf[13];				//ģʽ������
																		IT1_Key_DataLoad(index,cmd,0x01);
															}
												}
												//=============================================================================================
												//���´�����ת����
												else if((Dev.type&0xf000)==0x3000)
												{
															if(wbuf[1]!=0x00)
															{
																    #ifdef USE_LOCK_BY_DS28E11   //��ʹ�ü���
																		ComposePacketFromDS28E02(&wbuf[6],&wbuf[10],4,0);
																		//�Ƚ�ͬ����
																		if(OPT_OK!=CompareSync(&wbuf[10],0))return;
																    #endif
															}
															
															//��״̬��ָ������ϱ�
															//if((wbuf[1]==0x31)&&(wbuf[12]>0))
															if(wbuf[1]==0x31)
															{
																		//20180328���޸�ָ��
																		if((wbuf[13]==0x03)&&(wbuf[14]==0x02))
																		{
																				type_cmd=wbuf[22]*256+wbuf[21];
																			  for(i=0;i<LOCK_ITEMNUM;i++)
																			  {
																						if(lock.item.Serail[i]==type_cmd)break;
																				}
																				//���������ָ�Ƶ�ID
																				if(i<LOCK_ITEMNUM)
																				{
																						if(lock.waittime>0)
																						{
																									lock.waittime=0;
																							    //����Ѿ����棬���ϱ���һ��
																							    IT1_Lock_DataLoad(i+1,0x02,0xbb);
																						}
																						else
																						{
																							    if((wbuf[23]==0x01)||(wbuf[23]==0x02)||(wbuf[23]==0x03)||((wbuf[23]==0x05)))
																									{
																													//���ϱ�״̬
																													lock.item.stu|=0x01;
																										      IT1_Lock_DataLoad(0,0x01,lock.item.stu); 
																										      //�����ϱ�
																													lin=i/8;
																													row=i%8;
																													if((Pannel.item[0].onoffbuf[lin]&(0x01<<row))!=0x00)
																													{
																																//IT1_Lock_DataLoad(i+1,0x0,0x01);
																														    tt=i+1;
																														    DlyEvent_handle(tt,0,0x01,1000);
																													}
																													//���������
																													//if((Dev.mode&0x02)!=0x00)
																													if((lock.item.safe!=0)||((lock.item.stu&0x02)!=0x00))
																													{
																														    DlyEvent_handle(0,0x03,0x05,1000);
																													}
																									}
																									else if(wbuf[23]==0x00)
																									{
																										      //>>>>>=====================20180324 add=========
																										      //���ϱ�״̬
																										      lock.item.stu&=0xfe;
																										      IT1_Lock_DataLoad(0,0x01,lock.item.stu);  
																										      //�����ϱ�
																													lin=i/8;
																													row=i%8;
																													if((Pannel.item[0].onoffbuf[lin]&(0x01<<row))!=0x00)
																													{
																														    tt=i+1;
																														    DlyEvent_handle(tt,0,0x00,1000);
																													}
																													//���������
																													//if((Dev.mode&0x02)!=0x00)
																													if((lock.item.safe!=0)||((lock.item.stu&0x02)!=0x00))
																													{
																														    DlyEvent_handle(0,0x03,0x06,1000);
																													}
																													//<<<<<<<==========================================
																									}
																									else
																									{
																										
																									}
								
																						}
																				}
																				//û�б������ָ��ID
																				else
																				{
																					 	//�������ָ�����״̬
																						if(lock.waittime>0)
																						{
																									lock.waittime=0;
																									index=lock.index;
																																														
																							    if((wbuf[23]==0x01)||(wbuf[23]==0x02)||(wbuf[23]==0x03)||((wbuf[23]==0x05)))dat=0xbb;
																							    else dat=0xff;
																									
																									
																									IT1_Lock_DataLoad(index,0x02,dat);
																									LockItem.Serail[lock.index-1]=(uint16_t)(wbuf[22]*256+wbuf[21]);    //�û����+ID����+�û�����;
																									LockItem.IdType[lock.index-1]=0x00;
																									LockItem.UserType[lock.index-1]=0x00;																		  
																									Write_Lock_Table(lock.index);
																						}
																						else
																						{
																									//������
																							    if((wbuf[23]==0x01)||(wbuf[23]==0x02)||(wbuf[23]==0x03)||((wbuf[23]==0x05)))
																									{
																													lock.item.stu|=0x01;
																										      IT1_Lock_DataLoad(0,0x01,lock.item.stu); 
																										      //���������																													//���������
																													//if((Dev.mode&0x02)!=0x00)
																										      if((lock.item.safe!=0)||((lock.item.stu&0x02)!=0x00))
																													{
																														   DlyEvent_handle(0,0x03,0x05,1000);
																													}
																									}
																									//�����ر�
																									else if(wbuf[23]==0x00)
																									{
																													lock.item.stu&=0xfe;
																										      IT1_Lock_DataLoad(0,0x01,lock.item.stu); 
																										      //���������																													//���������
																													//if((Dev.mode&0x02)!=0x00)
																										      if((lock.item.safe!=0)||((lock.item.stu&0x02)!=0x00))
																													{
																														   tt=0;tt=(3<<5);
																														   DlyEvent_handle(0,0x03,0x06,1000);
																													}
																									}
																									//����������
																									else
																									{
																										
																									}
																						}
																				}
																		}
															}
															//Ӧ����
															else if(wbuf[1]==0x00)
															{
																		//�鿴��ַ�Ƿ��Ѿ�����
																		pws=GetQueue_OutQ(&WirelessSend_Queue,&index);
																		if(pws==NULL)return;
																		if((pws->ackwait!=0)&&(pws->buf[6]==wbuf[6])&&(pws->buf[7]==wbuf[7]) \
																			&&(pws->buf[8]==wbuf[8])&&(pws->buf[9]==wbuf[9]))
																		{
																					for(i=0;i<pws->cnt;i++)pws->buf[i]=0x00;
																					pws->cnt=0;
																					pws->flag=0;
																					pws->ackwait=0;
																					DeQueueBuf(&WirelessSend_Queue,index);						
																		}
															}
														}
													}
				}
	      //���ϴ�������ģʽ�£��յ�����֡
	      //====================================================================================================
	      //====================================================================================================
	      //ת������������״̬
	      else if(Dev.stu==DEV_SETUP)
	      {
	      	  //ѧϰң����
	      		if(Dev.learn_time>0)
	      		{
								if((Dev.type&0xf000)==0xb000)
								{
												if((wbuf[1]&0x80)==0x80)
												//if((wbuf[1]<0x0a)&&(wbuf[1]!=0x00))	//С��0x0a�Ҳ�Ϊ0ʱΪ����
												{
														if(((wbuf[1]==0x11)||(wbuf[1]==0x12))&&(wbuf[2]==0xff)&&(wbuf[3]==0xff)&&(wbuf[4]==0xff)&&(wbuf[5]==0xff))
														{
																	//==========================================================================
																	#ifdef USE_3MATCH
																	if(step==0)
																	{
																				index=Exchangebuf.addr;
																				Exchangebuf.wl_addr[0]=wbuf[6];
																				Exchangebuf.wl_addr[1]=wbuf[7];
																				Exchangebuf.wl_addr[2]=wbuf[8];
																				Exchangebuf.channel   =wbuf[9];	
																				step++;
																				Wireless_Recv_Disable(&wbuf[0],100);
																	}
																	else
																	{
																				if((Exchangebuf.wl_addr[0]==wbuf[6])&&(Exchangebuf.wl_addr[1]==wbuf[7]) \
																				&&(Exchangebuf.wl_addr[2]==wbuf[8])&&(Exchangebuf.channel  ==wbuf[9]))
																				{
																						step++;
																						Wireless_Recv_Disable(&wbuf[0],100);			//
																						if(step>=3)
																						{
																									Wireless_Learn_Finish();
																									step=0;
																						}
																				}
																	}									
																	//===========================================================================
																	#else   
																	Wireless_Learn_Finish();
																	#endif
																	
															}
												}
												//============================================================================================
												//������Ҫ����
												else
												{
															//�Ƚ�������
															for(i=0;i<4;i++)buf[i]=wbuf[10+i];
															ComposePacketFromDS28E02(&wbuf[6],&buf[0],4,0);//Դ��ַΪǰ3���ֽ�
															for(i=0;i<4;i++)wbuf[10+i]=buf[i];
													
															//wbuf[WL_FT]=wbuf[WL_FT]&0x7f;
															if(wbuf[WL_FT]==0x0b)															//����֡
															{
																	//û�д�����������Ϊͨ��ң����,�����ü�
																	//if((wbuf[WL_CMD]==0xff)&&(wbuf[WL_DAT]==0xff))
																	if(wbuf[WL_CMD]==0xff)
																	{
																				Wireless_Learn_Finish();
																	}
																	//������������һ��Ϊ������
																	else
																	{
																				type=IT1_RS4852Wireless(wbuf[WL_CMD],wbuf[WL_DAT]);
																				if(type==Ex_Table.Exchange[index].type)				//���Ͷ�													 
																				{
																						Wireless_Learn_Finish();
																				}
																		}
															}
															//��ָ�����ͣ��豸û�����ü�
															else if((wbuf[1]==0x10)&&(wbuf[2]==0xff)&&(wbuf[3]==0xff)&&(wbuf[4]==0xff)&&(wbuf[5]==0xff))
															{
																	if(step==0)
																	{
																				index=Exchangebuf.addr;
																				Exchangebuf.wl_addr[0]=wbuf[6];
																				Exchangebuf.wl_addr[1]=wbuf[7];
																				Exchangebuf.wl_addr[2]=wbuf[8];
																				Exchangebuf.channel   =wbuf[9];	
																				step++;
																				Wireless_Recv_Disable(&wbuf[0],200);
																	}
																	else
																	{
																				if((Exchangebuf.wl_addr[0]==wbuf[6])&&(Exchangebuf.wl_addr[1]==wbuf[7]) \
																				&&(Exchangebuf.wl_addr[2]==wbuf[8])&&(Exchangebuf.channel  ==wbuf[9]))
																				{
																						step++;
																						Wireless_Recv_Disable(&wbuf[0],200);			//
																						if(step>=3)
																						{
																								Wireless_Learn_Finish();
																								step=0;
																						}
																				}
																	}									
															}
												}
									}
									//==========================================================================================
									//���ﴦ����ת����
									else if((Dev.type&0xf000)==0x3000)
									{
												//�Ƚ�������
										    #ifdef USE_LOCK_BY_DS28E11   //��ʹ�ü���
												for(i=0;i<4;i++)buf[i]=wbuf[10+i];
												ComposePacketFromDS28E02(&wbuf[6],&buf[0],4,0);//Դ��ַΪǰ3���ֽ�
												for(i=0;i<4;i++)wbuf[10+i]=buf[i];
										    #endif
										
												if(wbuf[WL_FT]==0x0b)														
												{
														if(wbuf[WL_CMD]==0xf0)
														{
															    lock.index=0;
																	WlLock_Learn_Finish();
														}
												}
								
										
									}
					   }
							//==============================================================================================
							//�˳�����״̬
							else
							{
										Dev.stu=DEV_NORMAL;
										//#ifdef USE_3MATCH
										step=0;	
										//#endif
										Exchangebuf.addr=0xff;
										Exchangebuf.type=0xff;
										Exchangebuf.wl_addr[0]=0xff;
										Exchangebuf.wl_addr[1]=0xff;
										Exchangebuf.wl_addr[2]=0xff;
										Exchangebuf.channel   =0xff;
								
										Led_set(1,25,25,1,2);	
							}
	      }
				//����ת�������ڶ���״̬
				else
				{
							//�豸����δע��״̬��������
				}
	  //=======================================================================================
	}
	if(CC1101_ResetTime>300)
	{
			CC1101_ResetTime=0;
		  #ifdef USE_A7139
		  bsp_InitRF(1);	
		  #else
			bsp_InitCC1101();
		  #endif
	}
  //=============================================================================================	
}
//=======================================================================================================
//ѧϰ�豸���-ң����
void Wireless_Learn_Finish(void)
{
			uint8_t index,i,buf[4];
			index=Exchangebuf.addr;
			Exchangebuf.wl_addr[0]=wbuf[6];
			Exchangebuf.wl_addr[1]=wbuf[7];
			Exchangebuf.wl_addr[2]=wbuf[8];
			Exchangebuf.channel   =wbuf[9];	
	    Exchangebuf.wltype=wbuf[WL_CMD];				//ң��������
	
			if((Exchangebuf.key!=0x00)&&(Exchangebuf.key!=0xff)&&(wbuf[WL_DAT]!=0x00)&&(wbuf[WL_DAT]!=0xff))Exchangebuf.key=wbuf[WL_DAT];						//ң������������
			else Exchangebuf.key=0x04;
				
	    for(i=0;i<4;i++)buf[i]=wbuf[6+i];
	    buf[3]&=0xf0;
			Exchangebuf.secret=GetSecret(&buf[0],4);
			Exchangebuf.sync=0;
			//д�뵽ת����
			Write_Ex_Table(index);
	
			Remote_Key_En(index,0xff);
			IT1_Dev_Declare(index,0xbb);				//clf add 20171121
			//============================================================================
			//LEDָʾ
			Led_set(1,25,25,8,2);	
			
			Dev.learn_time=0;
			Dev.encode_time=0;
			Dev.stu=DEV_NORMAL;
			Ex_Table.SyncErr[index]=0;
}


//=======================================================================================================
//ѧϰ�豸���-ң����
void WlLock_Learn_Finish(void)
{
			uint8_t index,i,buf[4];
			index=lock.index;
			LockItem.addr[0]=wbuf[6];
			LockItem.addr[1]=wbuf[7];
			LockItem.addr[2]=wbuf[8];
			LockItem.addr[3]=wbuf[9];	
			LockItem.stu=0;
			LockItem.safe=0;
			for(i=0;i<8;i++)LockItem.key[i]=0x31+i;
		  LockItem.key[6]=0xff;
	    LockItem.key[7]=0xff;
	    for(i=0;i<4;i++)buf[i]=wbuf[6+i];
	    buf[3]&=0xf0;
			LockItem.secret=GetSecret(&buf[0],4);
			LockItem.sync=0;
			//д�뵽��ת����
			Write_Lock_Table(index);

			//============================================================================
			//LEDָʾ
			Led_set(1,25,25,8,2);	
			
			Dev.learn_time=0;
			Dev.encode_time=0;
			Dev.stu=DEV_NORMAL;
			lock.SyncErr=0;
}

//=======================================================================================================
//���������豸����
uint8_t Wireless_Dev_Action(uint8_t index,uint8_t cmd,uint8_t route,uint8_t scene_num)
{
				_Frame *pws;
				//uint8_t addr_buf[4];
					

				//============================================================================
				//����豸����
				if(0==Wireless_Send_Check(cmd,index))return RE_ERR;
				
				//pws=GetQueue_front(&WirelessSend_Queue);
	      pws=GetQueue_InQ(&WirelessSend_Queue,index);	
				if(pws==NULL)
				{
					return RE_ERR;
				}
				
				pws->buf[0]=0x10;
				pws->buf[1]=0x0b;
				pws->buf[2]=cmd;
				pws->buf[3]=scene_num;
				
				//pws->buf[4]=Dev.addr[0];
				//pws->buf[5]=Dev.addr[1];
				pws->buf[4]=Ex_Table.addr[0];
				pws->buf[5]=Ex_Table.addr[1];
				
				pws->buf[6]=0x00;
				
				if((cmd==0x01)||(cmd==0x02))												//����״̬
				{
						pws->buf[7]=Exchangebuf.type;
						pws->buf[8]=Exchangebuf.wl_addr[1];
						pws->buf[9]=Exchangebuf.wl_addr[2];
				}
				else if(cmd==0x06)																//����ִ��
				{
						pws->buf[7]=0xff;
						pws->buf[8]=0xff;
						pws->buf[9]=0xff;
				}		
				else
				{
						pws->buf[7]=Ex_Table.Exchange[index].type;
						pws->buf[8]=Ex_Table.Exchange[index].wl_addr[1];
						pws->buf[9]=Ex_Table.Exchange[index].wl_addr[2];
				}
				pws->buf[10]=0x01;
				pws->buf[11]=route;
				pws->buf[12]=0x00;
				pws->buf[13]=0x00;
				
				pws->buf[14]=Crc8CheckSum(pws->buf,14);
				pws->buf[15]=SumCheck(pws->buf,14);
				
				pws->cnt=16;				//�����ܵ��ֽ���
				pws->flag=1;
				pws->resend=3;				//�ظ��ϱ�����	
				pws->rand=1;				//��һ�ο��мȷ���
				pws->ackwait=2;	
				//EnQueueBuf(&WirelessSend_Queue);
				EnQueueBuf(&WirelessSend_Queue,index);//�����
				return RE_OK;
}
//======================================================================================================
//=======================================================================================================
//������
uint8_t Wireless_Lock_Action(uint8_t index,uint8_t cmd)
{
				_Frame *pws;
				uint8_t i/*,len*/;

	      pws=GetQueue_InQ(&WirelessSend_Queue,index);	
				if(pws==NULL){return RE_ERR;}
				
				pws->buf[0]=0x20;								//֡����
				pws->buf[1]=0x30;								//֡����
				for(i=0;i<4;i++)pws->buf[2+i]=lock.item.addr[i];			//Ŀ���ַ
				for(i=0;i<4;i++)pws->buf[6+i]=Ex_Table.addr[i];							//Դ��ַ
				//pws->buf[6+0]=0x76;
				//pws->buf[6+1]=0xa1;
				//pws->buf[6+2]=0xe9;
				//pws->buf[6+3]=0x01;
				
				pws->buf[10]=(uint8_t)(Dev.sync>>8);
				pws->buf[11]=(uint8_t)(Dev.sync&0x00ff);
				Dev.sync++;
				
				if(cmd==0x01)										//����
				{
							pws->buf[12]=3;
						  pws->buf[13]=0x02;
					    pws->buf[14]=0x06;
							pws->buf[15]=0x00;
							for(i=0;i<13;i++)pws->buf[16+i]=0xff;
				}
				else if(cmd==0x02)							//����
				{
//							pws->buf[12]=4;
//						  pws->buf[13]=0x01;
//					    pws->buf[14]=0x0a;
//							pws->buf[15]=0x00;
//					    pws->buf[16]=0x03;
//							for(i=0;i<12;i++)pws->buf[17+i]=0xff;
					    pws->buf[12]=3;
						  pws->buf[13]=0x02;
					    pws->buf[14]=0x06;
							pws->buf[15]=0x01;
							for(i=0;i<13;i++)pws->buf[16+i]=0xff;
				}
				else if(cmd==0xbb)							//���ָ��
				{
							pws->buf[12]=4;
						  pws->buf[13]=0x02;
					    pws->buf[14]=0x07;
							pws->buf[15]=index;
					    pws->buf[16]=0x18;
							for(i=0;i<12;i++)pws->buf[17+i]=0xff;
				}
				else if(cmd==0xcc)							//ɾ��ָ��
				{
					    if((index==0)||(index>16))return 0;
							pws->buf[12]=4;
						  pws->buf[13]=0x02;
					    pws->buf[14]=0x08;
					    pws->buf[15]=(uint8_t)(lock.item.Serail[index-1]&0x00ff);
					    pws->buf[16]=(uint8_t)(lock.item.Serail[index-1]>>8);
							for(i=0;i<12;i++)pws->buf[17+i]=0xff;
				}
				//����ָ��
				else if(cmd==0xcf)
				{
							pws->buf[0]=0x10;								//֡����
							pws->buf[1]=0x30;								//֡����
							for(i=0;i<4;i++)pws->buf[2+i]=0xff;			//Ŀ���ַ
							for(i=0;i<4;i++)pws->buf[6+i]=0xcc;							//Դ��ַ
							
							pws->buf[10]=0xcf;
							pws->buf[11]=0xcf;
					    pws->buf[12]=0xcf;
					    pws->buf[13]=0xcf;
							//======================================================================
							//���������
					    #ifdef USE_DS28E02
							ComposePacketFromDS28E02(&(pws->buf[6]),&(pws->buf[10]),4,1);
					    #endif
							//======================================================================
					
					    pws->buf[14]=Crc8CheckSum(pws->buf,14);
							pws->buf[15]=SumCheck(pws->buf,14);
							
							pws->cnt=16;				//�����ܵ��ֽ���
							pws->flag=1;
							pws->resend=3;				//�ظ��ϱ�����	
							pws->rand=1;				//��һ�ο��мȷ���
							pws->ackwait=2;	
							EnQueueBuf(&WirelessSend_Queue,index);//�����
							return RE_OK;
					
				}
				else
				{
						return 0;
				}
				//======================================================================
				//���������
				#ifdef USE_LOCK_BY_DS28E11
				#ifdef USE_DS28E02
				ComposePacketFromDS28E02(&(pws->buf[6]),&(pws->buf[10]),4,1);
				#else
				ComposePacketFromDS28E02(&(pws->buf[6]),&(pws->buf[10]),4,1);
				#endif
				
				#endif
				//======================================================================
				
				pws->buf[30]=Crc8CheckSum(pws->buf,30);
				pws->buf[31]=SumCheck(pws->buf,30);
				
				pws->cnt=32;				//�����ܵ��ֽ���
				pws->flag=1;
				pws->resend=5;				//�ظ��ϱ�����	
				pws->rand=1;				//��һ�ο��мȷ���
				pws->ackwait=2;	
				EnQueueBuf(&WirelessSend_Queue,index);//�����
				return RE_OK;
}


//=======================================================================================================
//ͨ�����߿��ƺ����ý���豸
uint8_t Wireless_Control_IR(uint8_t index,uint8_t len,uint8_t *pdat)
{
				uint8_t i;
				_Frame *pws;
	      pws=GetQueue_InQ(&WirelessSend_Queue,index);	
				if(pws==NULL)
				{
					return RE_ERR;
				}
				
				pws->buf[0]=0x10;
				pws->buf[1]=0x45;
				
				//2017.11.16�޸�0Ϊindex
				for(i=0;i<3;i++)pws->buf[2+i]=Ex_Table.Exchange[index].wl_addr[i]; //Ŀ���ַ
				pws->buf[5]=Ex_Table.Exchange[index].channel;
				
				for(i=0;i<4;i++)pws->buf[6+i]=Ir.TgAddr[i];											//54��ң������ַ	Դ��ַ
				
				if(len>3)len=3;
				pws->buf[10]=len;
				for(i=0;i<len;i++)pws->buf[11+i]=*(pdat+i);
				
				pws->buf[14]=Crc8CheckSum(pws->buf,14);
				pws->buf[15]=SumCheck(pws->buf,14);
				
				pws->cnt=16;				//�����ܵ��ֽ���
				pws->flag=1;
				pws->resend=1;				//�ظ��ϱ�����	
				pws->rand=1;				//��һ�ο��мȷ���
				pws->ackwait=2;	
				//EnQueueBuf(&WirelessSend_Queue);
				EnQueueBuf(&WirelessSend_Queue,index);//�����
				return RE_OK;
}
//======================================================================================
//�����豸�������Ƿ�ƥ��
uint8_t Wireless_Send_Check(uint8_t cmd,uint8_t index)
{
//	uint8_t rt;
	
	if((cmd==0x01)||(cmd==0x14))return 1;
	
	//���Ҫת��ͨ�������� ң���������˳�
	if(Ex_Table.Exchange[index].type>=EX_DEV_KEY)
	{
		return 0;	
	}	
	else
	{
//		if((Ex_Table.Exchange[index].type==EX_DEV_LIGHT)||(Ex_Table.Exchange[index].type==EX_DEV_SOCKET))
//		{
//			if((cmd!=0x07)&&(cmd!=0x08))
//			{
//				return 0;	
//			}
//		}
//		else
//		{
//			if((cmd!=0x07)&&(cmd!=0x08)&&(cmd!=0x09)&&(cmd!=0x19))
//			{
//				return 0;	
//			}	
//		}	
	}
	return 1;
}


//===========================================================================================
//uint8_t  Crc8CheckSum(uint8_t *ptr,uint8_t length)
//{
//  uint8_t i;
//  uint8_t Crc8=0;
//  while(length--)
//  {
//    //g(x)=x8+x5+x4+1
//    for(i=0x80; i; i>>=1)
//    {
//      if((Crc8&0x80)!=0)
//      {
//        Crc8<<=1;
//        Crc8^=0x31;
//      }
//      else
//      {
//        Crc8<<=1;
//      }
//      if(*ptr&i)
//      {
//        Crc8^=0x31;
//      }
//    }
//    ptr++;
//  }
//  return Crc8;
//}
//=============================================================================================
//�ۼӺ�У��
uint8_t  SumCheck(uint8_t *p,uint8_t length)
{
		uint8_t  i;
		uint8_t sum=0;
		for(i=0;i<length;i++)
		{
			 sum+=p[i];
		}
		return sum;
}
//=============================================================================================
//����յ�������֡���Ƿ���ת������
void Check_Wireless_Addr(uint8_t *pbuf,uint8_t *rt)
{
	uint8_t i,buf[4],rtdat;
	
	buf[0]=*(pbuf+0);
	buf[1]=*(pbuf+1);	
	buf[2]=*(pbuf+2);
	buf[3]=*(pbuf+3);
	rtdat=RE_ERR;
	for(i=0;i<EX_TABLE_NUM;i++)
	{
		if((buf[0]==Ex_Table.Exchange[i].wl_addr[0])&&(buf[1]==Ex_Table.Exchange[i].wl_addr[1])&&(buf[2]==Ex_Table.Exchange[i].wl_addr[2])&&(buf[3]==Ex_Table.Exchange[i].channel))
		//if((buf[0]==Ex_Table.Exchange[i].wl_addr[0])&&(buf[1]==Ex_Table.Exchange[i].wl_addr[1])&&(buf[2]==Ex_Table.Exchange[i].wl_addr[2]))
		{
			rtdat=i;
			break;									
		}
	}
	*rt=rtdat;
}

//=============================================================================================
//����յ�������֡���Ƿ���ת������
void Check_WlLock_Addr(uint8_t *pbuf,uint8_t *rt)
{
	uint8_t buf[4],rtdat;
	
	buf[0]=*(pbuf+0);
	buf[1]=*(pbuf+1);	
	buf[2]=*(pbuf+2);
	buf[3]=*(pbuf+3);
	rtdat=RE_ERR;

	if((buf[0]==lock.item.addr[0])&&(buf[1]==lock.item.addr[1])&&(buf[2]==lock.item.addr[2])&&(buf[3]==lock.item.addr[3]))
	{
		rtdat=0;								
	}
	
	*rt=rtdat;
}
//=============================================================================================
//��ָ��ʱ���ڣ���ֹ����ͬ��֡
void Wireless_Recv_Disable(uint8_t *pdat,uint8_t time)
{
	uint8_t i;
	for(i=0;i<16;i++)wl_frame[i]=*(pdat+i);
	dis_tim=time;
	
}
//=============================================================================================
//�ж��Ƿ�����յ���֡
uint8_t Wireless_Recv_IsRun(uint8_t *pdat)
{
#if 0
	uint8_t i,err;
	
	if(dis_tim==0)
	{
		  for(i=0;i<16;i++)wl_frame[i]=0x00;
			return OPT_OK;
	}
	else
	{
		
		  err=OPT_ERR;
		
		  for(i=0;i<16;i++)
		  {
					if(wl_frame[i]!=*(pdat+i))
					{
						  err=OPT_OK;
							break;
					}
			}
			return err;
	}
#else
	    uint8_t err;
	
			err=OPT_OK;
		  //���ͬ������ͬ�����˳�
			if((wl_frame[10]==*(pdat+10))&&(wl_frame[11]==*(pdat+11)))
			{
					err=OPT_ERR;
			}
			return err;
#endif
}

//=============================================================================================
void wl_wakeup_frame(uint8_t index)
{		
			uint8_t buf[3],len;
	
			buf[0]=0x55;
			buf[1]=0x55;
			len=2;
			#ifdef USE_A7139
			RF_TX1(buf,len);
			bsp_InitRF(1);
			#else
			RfSendPacket(buf,len);      //��������
			bsp_InitCC1101();
			#endif
}
//==============================================================================================
//test
void wl_test_send(void)
{
				_Frame *pws;
				//10 0A 07 00 00 00 00 49 0C 49 01 00 00 00 FB C0
		
				pws=GetQueue_InQ(&WirelessSend_Queue,0);	
				if(pws==0)return;
							
				pws->buf[0]=0x10;
				pws->buf[1]=0x0a;
				pws->buf[2]=0x08;	
				pws->buf[3]=0x00;	
				pws->buf[4]=0x00;	
				pws->buf[5]=0x00;
				pws->buf[6]=0x00;
				pws->buf[7]=0x49;
				pws->buf[8]=0x0c;	
				pws->buf[9]=0x4a;	
				pws->buf[10]=0x01;	
				pws->buf[11]=0x00;
				pws->buf[12]=0x00;
				pws->buf[13]=0x00;
				pws->buf[14]=0xd0;
				pws->buf[15]=0xc2;
		
				pws->cnt=16;				   //�����ܵ��ֽ���
				pws->flag=1;
				pws->resend=5;				 //�ظ��ϱ�����	
				pws->rand=0;				   //��һ�ο��мȷ���
				pws->ackwait=0;	
				EnQueueBuf(&WirelessSend_Queue,0);	
}
//============================================================================
//=====================================================================================
//������
//���������豸����
uint8_t Wireless_Data_Send(uint8_t *pt,uint8_t num)
{
				_Frame *pws;
	      uint8_t i;

	      pws=GetQueue_InQ(&WirelessSend_Queue,0);	
				if(pws==NULL)
				{
					return RE_ERR;
				}
				
				pws->buf[0]=0x30;
				pws->buf[1]=0x40;
				pws->buf[2]=0x01;				//Ŀ���ַ
				pws->buf[3]=0x02;
				pws->buf[4]=0x03;
				pws->buf[5]=0x01;
				
				pws->buf[6]=0x11;				//Դ��ַ
				pws->buf[7]=0x12;
				pws->buf[8]=0x13;
				pws->buf[9]=0x11;
				
				pws->buf[10]=0x00;			 //ͬ����
				pws->buf[11]=0x01;
				
				pws->buf[12]=0xcf;			 //����
				
				for(i=0;i<33;i++)pws->buf[13+i]=i;
				
				pws->buf[46]=Crc8CheckSum(pws->buf,46);
				pws->buf[47]=SumCheck(pws->buf,46);
				
				pws->cnt=48;				//�����ܵ��ֽ���
				pws->flag=1;
				pws->resend=3;				//�ظ��ϱ�����	
				pws->rand=0;				//��һ�ο��мȷ���
				pws->ackwait=0;	
				EnQueueBuf(&WirelessSend_Queue,0);//�����
				return RE_OK;
}
/***************************end  of  code ***************************/
