		
//==================================================================================================

//Ó²¼þÖ§³Ö CC1101
//Ö§³ÖBSP bsp_cc1101
#include <includes.h>
//==================================================================================================
//¶ÅÑÇÓÐÏßÖÇÄÜ¼Ò¾Ó-IDÏµÍ³-µÚÒ»´úÐ­Òé ÎÞÏß868M×ªÓÐÏß
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
//ÓÃÓÚ¹ýÂËÖØ¸´Ö¡
static uint8_t wl_frame[16],dis_tim=0;
uint8_t wbuf[80];														
	
//==================================================================================================
//ÎÞÏß·¢ËÍ´¦Àí
//Ã¿¸ô100mSµ÷ÓÃÒ»´Î
void Wireless_Send(void)
{
				_Frame *pws;
				uint8_t i,index;
				
				//pws=GetQueue_rear(&WirelessSend_Queue);
	      pws=GetQueue_OutQ(&WirelessSend_Queue,&index);
				//ÅÐ¶ÏÊÇ·ñ½ÓÊÕ»º³åÇøÎª¿Õ
				if(pws==NULL)
				{
					return;
				}
				
				//============================================================================================
				if(pws->flag==1)				//ÓÐÊý¾ÝÒª·¢ËÍ
				{
					if(pws->ackwait>0)			//ÕýÔÚµÈ´ýÓ¦´ð£¬²»·¢ËÍ
					{
						pws->ackwait--;
						if(pws->ackwait==0)
						{
							if(pws->resend>0)       //ÖØ·¢´ÎÊý¼õÈ¥-    
							{
										pws->resend--;
										//pws->rand=Dev.rand%3;//Ëæ»úÊ±¼äÎª0~150mSÖ®¼ä
								    pws->rand=1;//Ëæ»úÊ±¼äÎª0~150mSÖ®¼ä
							}
							else
							{
										//ÕâÖ¡ÍË³ö»º³åÇø£¬²»ÔÚ·¢ËÍ
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
						if(pws->rand==0)			//ÑÓÊ±Ê±¼äµ½£¬¿ÉÒÔ·¢ËÍÊý¾Ý
						{
									#ifdef USE_A7139
							    RF_TX(pws->buf,pws->cnt);
									bsp_InitRF(0);
							    #else
									RfSendPacket(pws->buf,pws->cnt);      //·¢ËÍÊý¾Ý
									//bsp_InitCC1101();
							    #endif
									
									if(pws->resend==1)     i=3;
									else if(pws->resend==2)i=2;
									else                   i=1;
									
									pws->ackwait=i;		//µÈ´ýÓ¦´ðÊ±¼äÎª150mS
							
						}
						else
						{
									pws->rand--;
						}
					}
				}
				//Êý¾Ý·¢ËÍÍê³É
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
//ÎÞÏß½ÓÊÕ´¦Àí======================================================================================
//×ß¸ßÓÅÏÈ¼¶
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
	
	//ÎÞÏßÖ¡¹ýÂËÊ±¼ä
	if(dis_tim>0)dis_tim--;
	//ÓÃÓÚ¼à¿ØCC1101ÊÇ·ñ¹¤×÷
	CC1101_ResetTime++;
	#ifdef USE_A7139
	recv_ok=RF_RX(&wbuf[0],&len);
	if((1==recv_ok)&&(len>=16))
	#else
	recv_ok=RfReceivePacket(&wbuf[0],&len); 
	if((1==recv_ok)&&(len>=16))
	#endif
	//ÓÐ½ÓÊÕÊý¾Ý
	{
					CC1101_ResetTime=0;
					//=============================================================
					//Êý¾ÝÐ£Ñé
					tmp_sum=SumCheck(&wbuf[0],(len-2));
					tmp_crc=Crc8CheckSum(&wbuf[0],(len-2));	
					if((tmp_crc!=wbuf[len-2])||(tmp_sum!=wbuf[len-1]))return;
		      //=============================================================
		      //´®¿ÚÖ±½ÓÊäÈë¹¦ÄÜ==Ö»ÓÃÓÚµ÷ÊÔ£¬Êµ¼ÊÊ±Ó¦¸ÃÆÁ±Î£¬½ûÖ¹¶¨ÒåUSE_SERIAL_DEBUG
		      #ifdef USE_SERIAL_DEBUG
					//ComposePacket(&wbuf[10],4,index);
		      ComposePacketFromDS28E02(&wbuf[6],&wbuf[10],4,0);
		      RS485_Send(wbuf,len);
		      Led_set(1,25,25,1,2);
		      Dev.stu=DEV_NORMAL;
		      return;
					#endif
		      //==============================================================
					//Ä¿±êµØÖ·ÅÐ¶Ï ÊÇ¹ã²¥µØÖ·»òÕß±¾ÉíµØÖ· Ôò½ÓÊÕ
					rt=0;
					for(i=0;i<4;i++){if(wbuf[2+i]!=0xff)rt++;}
					if(rt!=0)
					{
							rt=0;
							for(i=0;i<4;i++)if(wbuf[2+i]!=Ex_Table.addr[i])rt++;		
					}
					if(rt!=0)return;
					//=============================================================
					//Ô´µØÖ·ÅÐ¶Ï£¬ÊÇ·ñ±£´æ	
					if((Dev.type&0xf000)==0xb000)
					{
							Check_Wireless_Addr(&wbuf[6],&index);	
					}
					else if((Dev.type&0xf000)==0x3000)
					{
							Check_WlLock_Addr(&wbuf[6],&index);	
					}
					//=============================================================
					//¹ýÂËÍ¬ÑùµÄÎÞÏß½ÓÊÕÖ¡?
					rt=Wireless_Recv_IsRun(&wbuf[0]);
					if(rt!=OPT_OK)return;		
					//=============================================================
					if(index!=RE_ERR){Led_set(1,25,25,1,2);}
					//=============================================================
					//Õý³£·¢ËÍ×´Ì¬
					if(Dev.stu==DEV_NORMAL)
					{		
								if(index==RE_ERR)return;
								//=======================================================
								if((wbuf[1]&0x80)==0x80)				//²»ÐèÒª½âÃÜ
								{
												Wireless_Recv_Disable(&wbuf[0],150);			//ÔÚÒÔºóµÄ100*5mSÊ±¼äÄÚ£¬½ûÖ¹´¦ÀíÍ¬ÑùµÄÖ¡
												//==============================================
												//Ò£¿ØÆ÷»ò´«¸ÐÆ÷ÉÏ±¨
												if(wbuf[1]==0x11)
												{															
																if(Dev.profile==IT1)
																{
																	//54¼üºÍ42¼üÉÏ±¨
																	if((Ex_Table.Exchange[index].type==EX_KEY_54KEY)||(Ex_Table.Exchange[index].type==EX_KEY_42KEY))
																	{
																			cmd=wbuf[12];				//¼üÖµ»òÃüÁî
																			dat=wbuf[13];				//Ä£Ê½»òÊý¾Ý
																			//IT1_RS485_DataLoad(index,cmd,dat);
																		  IT1_Key_DataLoad(index,cmd,dat);
																	}
																	else if(Ex_Table.Exchange[index].type==EX_DEV_SUN)
																	{
																			buf[0]=2;
																			buf[1]=wbuf[12];				//¼üÖµ»òÃüÁî
																			buf[2]=wbuf[13];				//Ä£Ê½»òÊý¾Ý
																		  IT1_Sensor_DataLoad(index,&buf[0]);
																	}
																	else if(Ex_Table.Exchange[index].type==EX_DEV_SUN1)
																	{
																			buf[0]=2;
																			buf[1]=wbuf[10];				//
																			buf[2]=wbuf[11];				//Ý
																		  IT1_Sensor_DataLoad(index,&buf[0]);
																	}
																	else if((Ex_Table.Exchange[index].type==EX_KEY_NORMAL1)||(Ex_Table.Exchange[index].type==EX_KEY_FULL1))
																	{
																			cmd=wbuf[12]+1;				//¼üÖµ»òÃüÁî
																			dat=wbuf[13];				//Ä£Ê½»òÊý¾Ý
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
												//Ðý×ª±àÂëÆ÷Ò£¿ØÉÏ±¨
												else if(wbuf[1]==0x12)
												{
																SyncNum=(wbuf[10]<<8)+wbuf[11];
																if(Ex_Table.SyncNum[index]==SyncNum)return;
																Ex_Table.SyncNum[index]=SyncNum;
																cmd=wbuf[12];				//¼üÖµ»òÃüÁî
																dat=wbuf[13];				//Ä£Ê½»òÊý¾Ý
																IT1_Turn_DataLoad(index,cmd,0);
																type_cmd=0;
																type_cmd=0x01<<8;
																type_cmd=type_cmd+cmd;
																if(dat>0){dat--;while(dat>0){bsp_PutMsg(type_cmd,index);dat--;}}
												}
												//===============================================================================
												//54¼üÒ£¿ØÆ÷ºìÍâÅäÖÃÓ¦´ð ¹²16¸ö×Ö½Ú
												else if((wbuf[1]==0x00)&&(Ir.wait==1)&&((wbuf[10]&0x0f)==Ir.serial))
												{
																pws=GetQueue_OutQ(&WirelessSend_Queue,&index);
																//ÅÐ¶ÏÊÇ·ñ½ÓÊÕ»º³åÇøÎª¿Õ
																if(pws==NULL){return;}
																
																//ÕâÖ¡ÍË³ö»º³åÇø£¬²»ÔÚ·¢ËÍ
																for(i=0;i<pws->cnt;i++)pws->buf[i]=0x00;
																pws->cnt=0;
																pws->flag=0;
																pws->ackwait=0;
													
																DeQueueBuf(&WirelessSend_Queue,index);
																//ÔËÐÐÔÙ´Î×°ÈëºìÍâÊý¾Ý
																Ir.wait=0;																//2017.1.12
												}
												//=================================================================================
												//ºìÍâÅäÖÃÍê³Éºó£¬DQ8028Ö÷¶¯ÉÏ±¨Ò»´Î DTM_IR_SETUP_FINISH
												else if((wbuf[1]==DTM_IR_SETUP_FINISH)&&(Ir.flag==0))
												{
																//2017.4.5ÐÞ¸Ä
																Wireless_Recv_Disable(&wbuf[0],100);			//ÔÚÒÔºóµÄ100*5mSÊ±¼äÄÚ£¬½ûÖ¹´¦ÀíÍ¬ÑùµÄÖ¡
													
																 //±È½ÏDQ8010·¢À´µÄÀÛ¼ÓºÍºÍ»º³åÇøµÄÀÛ¼ÓºÍ±È½Ï
																 if((Ir.typechm==wbuf[10])&&(Ir.keychm==wbuf[11]))
																 {
																				IT1_IrSetup_Report(OPT_OK);
																				Ir_Config_Add(index,Ir.cmd_key,Ir.key_mode);						//¼üÖµ
																 }
																 else
																 {
																				IT1_IrSetup_Report(OPT_ERR);
																 }
												}
												//=====================================================================================
												//ºìÍâÅäÖÃÉ¾³ýºó£¬ÉÏ±¨
												else if((wbuf[1]==0x00)&&(wbuf[10]==Ir.serial)&&(wbuf[12]==MTD_IR_DEL))
												{
																Wireless_Recv_Disable(&wbuf[0],230);
																IT1_IrDel_Report(index);
																Ir_Config_Del(index,Ir.cmd_key,Ir.key_mode);					//¼üÖµ
												}
												//=====================================================================================
												//Ö´ÐÐºó·µ»ØµÄ×´Ì¬
												else if((wbuf[1]==0xe0)||(wbuf[1]==0xe1))
												{
																Pannel_Table_Refresh(index,wbuf[1],&wbuf[10]);
												}
													
									}
									//==========================================================================================
									//ÒÔÏÂÒªÓÃµ½½âÃÜ³ÌÐò 20171206ÐÂ¼Ó
									else
									{
												Wireless_Recv_Disable(&wbuf[0],250);			//ÔÚÒÔºóµÄ100*5mSÊ±¼äÄÚ£¬½ûÖ¹´¦ÀíÍ¬ÑùµÄÖ¡
												//Êý¾Ý´¦Àí
												if((Dev.type&0xf000)==0xb000)
												{
															//ÏÈ½âÃÜ£¬½âÃÜµÄÃØÔ¿ÔÚ±í¸ñÖÐ
															if(wbuf[1]!=0x00)
															{
																	datlen=len-12;
																	ComposePacket(&wbuf[10],datlen,index);
																	//±È½ÏÍ¬²½Êý
																	if(OPT_OK!=CompareSync(&wbuf[10],index))return;
															}
															//====================================================================================
															//×î¸ßÎ»±íÊ¾ÊÇ·ñÒª½âÃÜ
															//wbuf[1]=wbuf[1]&0x7f;
															//wbuf[1] Îª0x01 02 03 04 05 06 07 08 09Ê±£¬Ã÷Âë
															//Ðý×ª±àÂëÆ÷Ò£¿ØÉÏ±¨
															if(wbuf[1]==0x12)
															{
																		SyncNum=(wbuf[10]<<8)+wbuf[11];
																		if(Ex_Table.SyncNum[index]==SyncNum)return;
																		Ex_Table.SyncNum[index]=SyncNum;
																		cmd=wbuf[12];				//¼üÖµ»òÃüÁî
																		dat=wbuf[13];				//Ä£Ê½»òÊý¾Ý
																		IT1_Turn_DataLoad(index,cmd,0);
																
																    //20180307 Ðý×ªÊ±±¨¿ªÊ¼µ÷¹âºÍ½áÊøµ÷¹â
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
																		cmd=wbuf[12]+1;				//¼üÖµ»òÃüÁî
																		dat=wbuf[13];				//Ä£Ê½»òÊý¾Ý
																		IT1_Key_DataLoad(index,cmd,0x01);
															}
												}
												//=============================================================================================
												//ÒÔÏÂ´¦ÀíËø×ª·¢Æ÷
												else if((Dev.type&0xf000)==0x3000)
												{
															if(wbuf[1]!=0x00)
															{
																    #ifdef USE_LOCK_BY_DS28E11   //ËøÊ¹ÓÃ¼ÓÃÜ
																		ComposePacketFromDS28E02(&wbuf[6],&wbuf[10],4,0);
																		//±È½ÏÍ¬²½Êý
																		if(OPT_OK!=CompareSync(&wbuf[10],0))return;
																    #endif
															}
															
															//Ëø×´Ì¬»òÖ¸ÎÆÌí¼ÓÉÏ±¨
															//if((wbuf[1]==0x31)&&(wbuf[12]>0))
															if(wbuf[1]==0x31)
															{
																		//20180328ÐÂÐÞ¸ÄÖ¸Áî
																		if((wbuf[13]==0x03)&&(wbuf[14]==0x02))
																		{
																				type_cmd=wbuf[22]*256+wbuf[21];
																			  for(i=0;i<LOCK_ITEMNUM;i++)
																			  {
																						if(lock.item.Serail[i]==type_cmd)break;
																				}
																				//±£´æÓÐÕâ¸öÖ¸ÎÆµÄID
																				if(i<LOCK_ITEMNUM)
																				{
																						if(lock.waittime>0)
																						{
																									lock.waittime=0;
																							    //Èç¹ûÒÑ¾­±£´æ£¬ÔòÉÏ±¨ÉÏÒ»¸ö
																							    IT1_Lock_DataLoad(i+1,0x02,0xbb);
																						}
																						else
																						{
																							    if((wbuf[23]==0x01)||(wbuf[23]==0x02)||(wbuf[23]==0x03)||((wbuf[23]==0x05)))
																									{
																													//ÏÈÉÏ±¨×´Ì¬
																													lock.item.stu|=0x01;
																										      IT1_Lock_DataLoad(0,0x01,lock.item.stu); 
																										      //´¥·¢ÉÏ±¨
																													lin=i/8;
																													row=i%8;
																													if((Pannel.item[0].onoffbuf[lin]&(0x01<<row))!=0x00)
																													{
																																//IT1_Lock_DataLoad(i+1,0x0,0x01);
																														    tt=i+1;
																														    DlyEvent_handle(tt,0,0x01,1000);
																													}
																													//Èç¹ûÓÐÍÆËÍ
																													//if((Dev.mode&0x02)!=0x00)
																													if((lock.item.safe!=0)||((lock.item.stu&0x02)!=0x00))
																													{
																														    DlyEvent_handle(0,0x03,0x05,1000);
																													}
																									}
																									else if(wbuf[23]==0x00)
																									{
																										      //>>>>>=====================20180324 add=========
																										      //ÏÈÉÏ±¨×´Ì¬
																										      lock.item.stu&=0xfe;
																										      IT1_Lock_DataLoad(0,0x01,lock.item.stu);  
																										      //´¥·¢ÉÏ±¨
																													lin=i/8;
																													row=i%8;
																													if((Pannel.item[0].onoffbuf[lin]&(0x01<<row))!=0x00)
																													{
																														    tt=i+1;
																														    DlyEvent_handle(tt,0,0x00,1000);
																													}
																													//Èç¹ûÓÐÍÆËÍ
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
																				//Ã»ÓÐ±£´æÕâ¸öÖ¸ÎÆID
																				else
																				{
																					 	//Èç¹û´¦ÓÚÖ¸ÎÆÌí¼Ó×´Ì¬
																						if(lock.waittime>0)
																						{
																									lock.waittime=0;
																									index=lock.index;
																																														
																							    if((wbuf[23]==0x01)||(wbuf[23]==0x02)||(wbuf[23]==0x03)||((wbuf[23]==0x05)))dat=0xbb;
																							    else dat=0xff;
																									
																									
																									IT1_Lock_DataLoad(index,0x02,dat);
																									LockItem.Serail[lock.index-1]=(uint16_t)(wbuf[22]*256+wbuf[21]);    //ÓÃ»§±àºÅ+IDÀàÐÍ+ÓÃ»§ÀàÐÍ;
																									LockItem.IdType[lock.index-1]=0x00;
																									LockItem.UserType[lock.index-1]=0x00;																		  
																									Write_Lock_Table(lock.index);
																						}
																						else
																						{
																									//±¨Ëø´ò¿ª
																							    if((wbuf[23]==0x01)||(wbuf[23]==0x02)||(wbuf[23]==0x03)||((wbuf[23]==0x05)))
																									{
																													lock.item.stu|=0x01;
																										      IT1_Lock_DataLoad(0,0x01,lock.item.stu); 
																										      //Èç¹ûÓÐÍÆËÍ																													//Èç¹ûÓÐÍÆËÍ
																													//if((Dev.mode&0x02)!=0x00)
																										      if((lock.item.safe!=0)||((lock.item.stu&0x02)!=0x00))
																													{
																														   DlyEvent_handle(0,0x03,0x05,1000);
																													}
																									}
																									//±¨Ëø¹Ø±Õ
																									else if(wbuf[23]==0x00)
																									{
																													lock.item.stu&=0xfe;
																										      IT1_Lock_DataLoad(0,0x01,lock.item.stu); 
																										      //Èç¹ûÓÐÍÆËÍ																													//Èç¹ûÓÐÍÆËÍ
																													//if((Dev.mode&0x02)!=0x00)
																										      if((lock.item.safe!=0)||((lock.item.stu&0x02)!=0x00))
																													{
																														   tt=0;tt=(3<<5);
																														   DlyEvent_handle(0,0x03,0x06,1000);
																													}
																									}
																									//ÆäËû²»´¦Àí
																									else
																									{
																										
																									}
																						}
																				}
																		}
															}
															//Ó¦´ð´¦Àí
															else if(wbuf[1]==0x00)
															{
																		//²é¿´µØÖ·ÊÇ·ñÒÑ¾­±£´æ
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
	      //ÒÔÉÏ´¦ÀíÕý³£Ä£Ê½ÏÂ£¬ÊÕµ½ÎÞÏßÖ¡
	      //====================================================================================================
	      //====================================================================================================
	      //×ª·¢Æ÷´¦ÓÚÉèÖÃ×´Ì¬
	      else if(Dev.stu==DEV_SETUP)
	      {
	      	  //Ñ§Ï°Ò£¿ØÆ÷
	      		if(Dev.learn_time>0)
	      		{
								if((Dev.type&0xf000)==0xb000)
								{
												if((wbuf[1]&0x80)==0x80)
												//if((wbuf[1]<0x0a)&&(wbuf[1]!=0x00))	//Ð¡ÓÚ0x0aÇÒ²»Îª0Ê±ÎªÃ÷Âë
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
												//ÒÔÏÂÐèÒª½âÃÜ
												else
												{
															//ÏÈ½âÃÜÊý¾Ý
															for(i=0;i<4;i++)buf[i]=wbuf[10+i];
															ComposePacketFromDS28E02(&wbuf[6],&buf[0],4,0);//Ô´µØÖ·ÎªÇ°3¸ö×Ö½Ú
															for(i=0;i<4;i++)wbuf[10+i]=buf[i];
													
															//wbuf[WL_FT]=wbuf[WL_FT]&0x7f;
															if(wbuf[WL_FT]==0x0b)															//¶ÔÂëÖ¡
															{
																	//Ã»ÓÐ´øÀàÐÍÉÏÀ´£¬ÎªÍ¨ÓÃÒ£¿ØÆ÷,ÓÐÉèÖÃ¼ü
																	//if((wbuf[WL_CMD]==0xff)&&(wbuf[WL_DAT]==0xff))
																	if(wbuf[WL_CMD]==0xff)
																	{
																				Wireless_Learn_Finish();
																	}
																	//´øÀàÐÍÉÏÀ´£¬Ò»°ãÎª´«¸ÐÆ÷
																	else
																	{
																				type=IT1_RS4852Wireless(wbuf[WL_CMD],wbuf[WL_DAT]);
																				if(type==Ex_Table.Exchange[index].type)				//ÀàÐÍ¶Ô													 
																				{
																						Wireless_Learn_Finish();
																				}
																		}
															}
															//²»Ö¸¶¨ÀàÐÍ£¬Éè±¸Ã»ÓÐÉèÖÃ¼ü
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
									//ÕâÀï´¦ÀíËø×ª·¢Æ÷
									else if((Dev.type&0xf000)==0x3000)
									{
												//ÏÈ½âÃÜÊý¾Ý
										    #ifdef USE_LOCK_BY_DS28E11   //ËøÊ¹ÓÃ¼ÓÃÜ
												for(i=0;i<4;i++)buf[i]=wbuf[10+i];
												ComposePacketFromDS28E02(&wbuf[6],&buf[0],4,0);//Ô´µØÖ·ÎªÇ°3¸ö×Ö½Ú
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
							//ÍË³öÉèÖÃ×´Ì¬
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
				//ÒÔÉÏ×ª·¢Æ÷´¦ÓÚ¶ÔÂë×´Ì¬
				else
				{
							//Éè±¸´¦ÓÚÎ´×¢²á×´Ì¬£¬²»ÔËÐÐ
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
//Ñ§Ï°Éè±¸Íê³É-Ò£¿ØÆ÷
void Wireless_Learn_Finish(void)
{
			uint8_t index,i,buf[4];
			index=Exchangebuf.addr;
			Exchangebuf.wl_addr[0]=wbuf[6];
			Exchangebuf.wl_addr[1]=wbuf[7];
			Exchangebuf.wl_addr[2]=wbuf[8];
			Exchangebuf.channel   =wbuf[9];	
	    Exchangebuf.wltype=wbuf[WL_CMD];				//Ò£¿ØÆ÷ÀàÐÍ
	
			if((Exchangebuf.key!=0x00)&&(Exchangebuf.key!=0xff)&&(wbuf[WL_DAT]!=0x00)&&(wbuf[WL_DAT]!=0xff))Exchangebuf.key=wbuf[WL_DAT];						//Ò£¿ØÆ÷°´¼ü¸öÊý
			else Exchangebuf.key=0x04;
				
	    for(i=0;i<4;i++)buf[i]=wbuf[6+i];
	    buf[3]&=0xf0;
			Exchangebuf.secret=GetSecret(&buf[0],4);
			Exchangebuf.sync=0;
			//Ð´Èëµ½×ª»»±í
			Write_Ex_Table(index);
	
			Remote_Key_En(index,0xff);
			IT1_Dev_Declare(index,0xbb);				//clf add 20171121
			//============================================================================
			//LEDÖ¸Ê¾
			Led_set(1,25,25,8,2);	
			
			Dev.learn_time=0;
			Dev.encode_time=0;
			Dev.stu=DEV_NORMAL;
			Ex_Table.SyncErr[index]=0;
}


//=======================================================================================================
//Ñ§Ï°Éè±¸Íê³É-Ò£¿ØÆ÷
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
			//Ð´Èëµ½Ëø×ª·¢±í
			Write_Lock_Table(index);

			//============================================================================
			//LEDÖ¸Ê¾
			Led_set(1,25,25,8,2);	
			
			Dev.learn_time=0;
			Dev.encode_time=0;
			Dev.stu=DEV_NORMAL;
			lock.SyncErr=0;
}

//=======================================================================================================
//¿ØÖÆÎÞÏßÉè±¸¶¯×÷
uint8_t Wireless_Dev_Action(uint8_t index,uint8_t cmd,uint8_t route,uint8_t scene_num)
{
				_Frame *pws;
				//uint8_t addr_buf[4];
					

				//============================================================================
				//¼ì²âÉè±¸ÀàÐÍ
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
				
				if((cmd==0x01)||(cmd==0x02))												//¶ÔÂë×´Ì¬
				{
						pws->buf[7]=Exchangebuf.type;
						pws->buf[8]=Exchangebuf.wl_addr[1];
						pws->buf[9]=Exchangebuf.wl_addr[2];
				}
				else if(cmd==0x06)																//³¡¾°Ö´ÐÐ
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
				
				pws->cnt=16;				//·¢ËÍ×ÜµÄ×Ö½ÚÊý
				pws->flag=1;
				pws->resend=3;				//ÖØ¸´ÉÏ±¨Èý´Î	
				pws->rand=1;				//µÚÒ»´Î¿ÕÏÐ¼È·¢ËÍ
				pws->ackwait=2;	
				//EnQueueBuf(&WirelessSend_Queue);
				EnQueueBuf(&WirelessSend_Queue,index);//Çå¶ÓÁÐ
				return RE_OK;
}
//======================================================================================================
//=======================================================================================================
//¿ØÖÆËø
uint8_t Wireless_Lock_Action(uint8_t index,uint8_t cmd)
{
				_Frame *pws;
				uint8_t i/*,len*/;

	      pws=GetQueue_InQ(&WirelessSend_Queue,index);	
				if(pws==NULL){return RE_ERR;}
				
				pws->buf[0]=0x20;								//Ö¡³¤¶È
				pws->buf[1]=0x30;								//Ö¡ÀàÐÍ
				for(i=0;i<4;i++)pws->buf[2+i]=lock.item.addr[i];			//Ä¿±êµØÖ·
				for(i=0;i<4;i++)pws->buf[6+i]=Ex_Table.addr[i];							//Ô´µØÖ·
				//pws->buf[6+0]=0x76;
				//pws->buf[6+1]=0xa1;
				//pws->buf[6+2]=0xe9;
				//pws->buf[6+3]=0x01;
				
				pws->buf[10]=(uint8_t)(Dev.sync>>8);
				pws->buf[11]=(uint8_t)(Dev.sync&0x00ff);
				Dev.sync++;
				
				if(cmd==0x01)										//¿ªËø
				{
							pws->buf[12]=3;
						  pws->buf[13]=0x02;
					    pws->buf[14]=0x06;
							pws->buf[15]=0x00;
							for(i=0;i<13;i++)pws->buf[16+i]=0xff;
				}
				else if(cmd==0x02)							//¹ØËø
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
				else if(cmd==0xbb)							//Ìí¼ÓÖ¸ÎÆ
				{
							pws->buf[12]=4;
						  pws->buf[13]=0x02;
					    pws->buf[14]=0x07;
							pws->buf[15]=index;
					    pws->buf[16]=0x18;
							for(i=0;i<12;i++)pws->buf[17+i]=0xff;
				}
				else if(cmd==0xcc)							//É¾³ýÖ¸ÎÆ
				{
					    if((index==0)||(index>16))return 0;
							pws->buf[12]=4;
						  pws->buf[13]=0x02;
					    pws->buf[14]=0x08;
					    pws->buf[15]=(uint8_t)(lock.item.Serail[index-1]&0x00ff);
					    pws->buf[16]=(uint8_t)(lock.item.Serail[index-1]>>8);
							for(i=0;i<12;i++)pws->buf[17+i]=0xff;
				}
				//²âÊÔÖ¸Áî
				else if(cmd==0xcf)
				{
							pws->buf[0]=0x10;								//Ö¡³¤¶È
							pws->buf[1]=0x30;								//Ö¡ÀàÐÍ
							for(i=0;i<4;i++)pws->buf[2+i]=0xff;			//Ä¿±êµØÖ·
							for(i=0;i<4;i++)pws->buf[6+i]=0xcc;							//Ô´µØÖ·
							
							pws->buf[10]=0xcf;
							pws->buf[11]=0xcf;
					    pws->buf[12]=0xcf;
					    pws->buf[13]=0xcf;
							//======================================================================
							//ÔÚÕâÀï¼ÓÃÜ
					    #ifdef USE_DS28E02
							ComposePacketFromDS28E02(&(pws->buf[6]),&(pws->buf[10]),4,1);
					    #endif
							//======================================================================
					
					    pws->buf[14]=Crc8CheckSum(pws->buf,14);
							pws->buf[15]=SumCheck(pws->buf,14);
							
							pws->cnt=16;				//·¢ËÍ×ÜµÄ×Ö½ÚÊý
							pws->flag=1;
							pws->resend=3;				//ÖØ¸´ÉÏ±¨Èý´Î	
							pws->rand=1;				//µÚÒ»´Î¿ÕÏÐ¼È·¢ËÍ
							pws->ackwait=2;	
							EnQueueBuf(&WirelessSend_Queue,index);//Çå¶ÓÁÐ
							return RE_OK;
					
				}
				else
				{
						return 0;
				}
				//======================================================================
				//ÔÚÕâÀï¼ÓÃÜ
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
				
				pws->cnt=32;				//·¢ËÍ×ÜµÄ×Ö½ÚÊý
				pws->flag=1;
				pws->resend=5;				//ÖØ¸´ÉÏ±¨Èý´Î	
				pws->rand=1;				//µÚÒ»´Î¿ÕÏÐ¼È·¢ËÍ
				pws->ackwait=2;	
				EnQueueBuf(&WirelessSend_Queue,index);//Çå¶ÓÁÐ
				return RE_OK;
}


//=======================================================================================================
//Í¨¹ýÎÞÏß¿ØÖÆºìÍâ¶àÃ½ÌåÉè±¸
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
				
				//2017.11.16ÐÞ¸Ä0Îªindex
				for(i=0;i<3;i++)pws->buf[2+i]=Ex_Table.Exchange[index].wl_addr[i]; //Ä¿±êµØÖ·
				pws->buf[5]=Ex_Table.Exchange[index].channel;
				
				for(i=0;i<4;i++)pws->buf[6+i]=Ir.TgAddr[i];											//54¼üÒ£¿ØÆ÷µØÖ·	Ô´µØÖ·
				
				if(len>3)len=3;
				pws->buf[10]=len;
				for(i=0;i<len;i++)pws->buf[11+i]=*(pdat+i);
				
				pws->buf[14]=Crc8CheckSum(pws->buf,14);
				pws->buf[15]=SumCheck(pws->buf,14);
				
				pws->cnt=16;				//·¢ËÍ×ÜµÄ×Ö½ÚÊý
				pws->flag=1;
				pws->resend=1;				//ÖØ¸´ÉÏ±¨Èý´Î	
				pws->rand=1;				//µÚÒ»´Î¿ÕÏÐ¼È·¢ËÍ
				pws->ackwait=2;	
				//EnQueueBuf(&WirelessSend_Queue);
				EnQueueBuf(&WirelessSend_Queue,index);//Çå¶ÓÁÐ
				return RE_OK;
}
//======================================================================================
//ÎÞÏßÉè±¸ºÍÃüÁîÊÇ·ñÆ¥Åä
uint8_t Wireless_Send_Check(uint8_t cmd,uint8_t index)
{
//	uint8_t rt;
	
	if((cmd==0x01)||(cmd==0x14))return 1;
	
	//Èç¹ûÒª×ª·¢Í¨µÀµÄÀàÐÍ Ò£¿ØÆ÷£¬ÔòÍË³ö
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
//ÀÛ¼ÓºÍÐ£Ñé
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
//¼ì²éÊÕµ½µÄÎÞÏßÖ¡£¬ÊÇ·ñÔÚ×ª·¢±íÖÐ
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
//¼ì²éÊÕµ½µÄÎÞÏßÖ¡£¬ÊÇ·ñÔÚ×ª·¢±íÖÐ
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
//ÔÚÖ¸¶¨Ê±¼äÄÚ£¬½ûÖ¹½ÓÊÕÍ¬ÑùÖ¡
void Wireless_Recv_Disable(uint8_t *pdat,uint8_t time)
{
	uint8_t i;
	for(i=0;i<16;i++)wl_frame[i]=*(pdat+i);
	dis_tim=time;
	
}
//=============================================================================================
//ÅÐ¶ÏÊÇ·ñ´¦Àí½ÓÊÕµ½µÄÖ¡
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
		  //Èç¹ûÍ¬²½ÊýÏàÍ¬£¬ÔòÍË³ö
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
			RfSendPacket(buf,len);      //·¢ËÍÊý¾Ý
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
		
				pws->cnt=16;				   //·¢ËÍ×ÜµÄ×Ö½ÚÊý
				pws->flag=1;
				pws->resend=5;				 //ÖØ¸´ÉÏ±¨Èý´Î	
				pws->rand=0;				   //µÚÒ»´Î¿ÕÏÐ¼È·¢ËÍ
				pws->ackwait=0;	
				EnQueueBuf(&WirelessSend_Queue,0);	
}
//============================================================================
//=====================================================================================
//²âÊÔÓÃ
//¿ØÖÆÎÞÏßÉè±¸¶¯×÷
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
				pws->buf[2]=0x01;				//Ä¿±êµØÖ·
				pws->buf[3]=0x02;
				pws->buf[4]=0x03;
				pws->buf[5]=0x01;
				
				pws->buf[6]=0x11;				//Ô´µØÖ·
				pws->buf[7]=0x12;
				pws->buf[8]=0x13;
				pws->buf[9]=0x11;
				
				pws->buf[10]=0x00;			 //Í¬²½Êý
				pws->buf[11]=0x01;
				
				pws->buf[12]=0xcf;			 //ÃüÁî
				
				for(i=0;i<33;i++)pws->buf[13+i]=i;
				
				pws->buf[46]=Crc8CheckSum(pws->buf,46);
				pws->buf[47]=SumCheck(pws->buf,46);
				
				pws->cnt=48;				//·¢ËÍ×ÜµÄ×Ö½ÚÊý
				pws->flag=1;
				pws->resend=3;				//ÖØ¸´ÉÏ±¨Èý´Î	
				pws->rand=0;				//µÚÒ»´Î¿ÕÏÐ¼È·¢ËÍ
				pws->ackwait=0;	
				EnQueueBuf(&WirelessSend_Queue,0);//Çå¶ÓÁÐ
				return RE_OK;
}
/***************************end  of  code ***************************/
