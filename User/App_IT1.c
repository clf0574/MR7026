		
//=========================================================================================================

//#define MAIN_VARIABLE

#include <includes.h>

//=========================================================================================================
//�����������ܼҾ�-ITϵͳ-��һ��Э��

//��ת�����������ӣ�Ϊһ��һ��ϵ������16��ͨ����ÿ��ͨ��������ǿ����ķ�ʽ������
#define LOCK_YELU    		0X3100
#define LOCK_TONGCHUANG 0x3200
#define LOCK_HAOLISHI   0x3300

#define VERFLAG  0x12
//#define VERFLAG  0x21


#define DEV_INFO_NUM   21
unsigned char DEV_INFO[DEV_INFO_NUM]={'M','R','3','2','0','0','-','1','8','-','0','3','-','2','7','-','S','1','.','0','1',};
unsigned char DEV_INFO_1[DEV_INFO_NUM]={'M','R','7','0','2','6','-','1','8','-','0','3','-','2','8','-','S','1','.','0','1',};
unsigned char DEV_INFO_2[DEV_INFO_NUM]={'M','R','3','x','0','0','-','1','8','-','0','3','-','2','8','-','S','1','.','0','1',};

//=========================================================================================================  
//֡������
#define MSG_broadcast 0x00
#define MSG_command   0x01
#define MSG_request   0x02
#define MSG_ask       0x03
#define MSG_data      0x04
#define MSG_setup     0x05
#define MSG_modify		0x08	



#define Dev_State_CMD   		0x07
#define Dev_OnLine_CMD			0x08
#define ACK_CMD 						0x20



//485����֡ �������
#define DOOYA_LIGHT_OFF	0x01
#define DOOYA_LIGHT_ON	0x02
#define DOOYA_LIGHT_SET	0x03
#define DOOYA_MOTOR_ALARM	0x0e
#define DOOYA_MOTOR_VENETIAN	0x0f
#define DOOYA_MOTOR_OFF		0x10
#define DOOYA_MOTOR_ON      	0x11
#define DOOYA_MOTOR_STOP	0x12
#define DOOYA_MOTOR_SETTING	0x13
#define DOOYA_MOTOR_POINT	0x14
#define DOOYA_MOTOR_LIGHT	0x15
#define DOOYA_MOTOR_CANCEL	0x16
#define DOOYA_MOTOR_CONFIG	0x17
#define DOOYA_MOTOR_LITTLE_UP	0x18
#define DOOYA_MOTOR_LITTLE_DOWN	0x19
#define DOOYA_MOTOR_SET_MIN	0x1A
#define DOOYA_MOTOR_SET_MAX	0x1B
#define DOOYA_MOTOR_AUTODEL	0x1C
#define DOOYA_MOTOR_REVERSE	0x1D
#define DOOYA_MOTOR_AUTOSET	0x1E
#define DOOYA_MOTOR_AUTORUN	0x1F

//===========================================================================================

void IT1_AddressSetting(void);
uint8_t IT1_Encode_Handle(uint8_t *pt,uint8_t num);
void IT1_Ex_Table_Save(void);
void IT1_Function_BroadCast(void);
void IT1_Function_CMD(void);
void IT1_Function_Date(void);
void IT1_Function_Setup(void);
void IT1_Load_RS485_Ack(unsigned char st);


void IT1_PROGRAM_Uadata(void);
void IT1_Modify_Addr_Old(void);
void IT1_Load_RS485_InfoReport(void);//��Ʒ��Ϣ�ϱ�
uint8_t IT1_Modify_Type(void);
void IT1_Load_RS485_InfoReport(void);//��Ʒ��Ϣ�ϱ�
uint8_t IT1_Wireless2RS485(uint8_t type,uint8_t *pbuf);

void IT1_LoadInfo_Report(void);
void IT1_Function_Modfiy(void);
void IT1_IR_Setup(void);
void IT1_IR_DEL(void);
void IT1_DevPara_Report(unsigned char chl,unsigned char addr,unsigned char len);
void IT1_ExTable_Stu_Ack(uint8_t index);

#ifdef USE_NEWREGISTER
void IT1_Register_Refresh(void);
#endif



extern uint8_t wbuf[16];	
//=========================================================================================================
//��������
void IT1_Key_Handle(void)
{
//	#if 0
//      uint8_t ucKeyCode;
//	    uint8_t index,key_value;//,i;
//	
//	    if(Dev.power_time<5)return; 
//	
//		ucKeyCode = bsp_GetKey();	/* ��ȡ��ֵ, �޼�����ʱ���� KEY_NONE = 0 */

//    if (ucKeyCode != KEY_NONE)
//		{
//			//================================================================================
//			index=(ucKeyCode-1)/3;							//����ͨ��0~7					
//			key_value=(ucKeyCode-1)%3;					//����ֵ0-ѹ�£�1-����2-����	
//			//������״̬
//			if(Dev.stu<=DEV_NORMAL)
//			{			
//						//==========================================================================
//				    //��尴��
//						if(index<KEY_NUM)
//						{
//								//�ǳ���������,��Ҫ�жϵ�����˫��
//								if((key_value==1)&&(Pannel[index].key_old!=2))
//								{
//									  Pannel[index].key_old=key_value;
//									  //Pannel[index].key_times=50;
//									  //Pannel[index].key_cnt++;
//                    IT1_RS485_DataLoad(index,S_CLICK,0);
//								}
//								//������������ж��Ƿ�������Ҫ����
//								else if(key_value==2)
//								{
//									  //Pannel[index].key_cnt=0;
//									  //Pannel[index].key_times=0;
//									  Pannel[index].key_old=key_value;
//									  IT1_RS485_DataLoad(index,L_KEYDOWN,0);
//								}
//								//���������𣬲���ж��Ƿ������ݷ���
//								else if((key_value==1)&&(Pannel[index].key_old==2))
//								{
//									  //Pannel[index].key_cnt=0;
//									  //Pannel[index].key_times=0;
//									  Pannel[index].key_old=key_value;
//									  IT1_RS485_DataLoad(index,L_KEYUP,0);
//								}
//									
//								//�̰������º��������
//								else 
//								{
//									  Pannel[index].key_old=key_value;
//									
//									  Led_Disp(10,5);
//								}	
//						}
//						//����Ϊ���ð�������
//		        else
//						{
//								if((ucKeyCode==KEY_SETUP_LONG)&&(Dev.stu==DEV_NORMAL))
//								{
//											Dev.stu=DEV_SETUP;				//�������ü�
//									    Dev.encode_time=8;
//									    encode_index=0;
//									    Led_Disp(50,1);
//								}
//						}
//			 }
//			 //==============================================================================
//			 //����״̬
//			 else
//			 {
//						LedMode_SetForKey(index,key_value);
//		   }
//		 }
//	#endif
}


//����֡����
void IT1_ParseFrame(void)
{
  unsigned char tmp1,tmp_addr;
	//unsigned int temp_address;

	unsigned char CheckSum/*,i*/;
	unsigned int temp_address;
	uint8_t buf[4];
	
	if(RxDat.cnt==0)return;
		
	if(RxDat.buf[0]==0x02)return;	

	temp_address=RxDat.buf[1];
	temp_address=(temp_address<<8)+RxDat.buf[2];

	tmp_addr=RxDat.buf[2];
	
	if(temp_address!=0xffff && tmp_addr!=Dev.addr[0])
  {
			RxDat.cnt=0;						                        		//��������ݸ���
			return;								                        			//����
	}
		
	CheckSum=Crc8CheckSum(RxDat.buf,(RxDat.cnt-1));		    	//����Crc8ֵ
	if(CheckSum==RxDat.buf[RxDat.cnt-1])				          	//�ж�CrcУ���Ƿ���ȷ
	{
	  	
	  if(Reg.fullflag==0)Led_set(0,12,12,1,1);
		tmp1=RxDat.buf[0];
		switch(tmp1)		
		{
			//============================================================================
			case MSG_broadcast:					                        //�㲥��ַ����ҪӦ��				
				    IT1_Function_BroadCast();		
			break;
			//==============================================================================
      case MSG_command:					        	            		//�����
            IT1_Function_CMD();			
			break;
			//===============================================================================
			case MSG_request:										    						//������

			break;
			//================================================================================
			case MSG_ask:					    					    						//Ӧ����
						//�˴�û���õ�
			break;
			//=================================================================================
			case MSG_data:					    				        				//���ݱ��� 
			     	IT1_Function_Date();					            		//Ҫת��������				
			break;
			//==================================================================================
			case MSG_setup:											    						//���ñ���	
						IT1_Function_Setup(); 
			break;
			//==================================================================================
			case MSG_modify:							                			//RS485��ַ����		
						IT1_Function_Modfiy();
			break;
			//==================================================================================
			//20161226�޸�Ϊ֧�ֺ�����������
			case 0x06:
			     IT1_IR_Setup();
			break;	
			//===================================================================================
			//������������ɾ��
			case 0x26:
					IT1_IR_DEL();
			break;
			//==================================================================================
			case 0x07:                      		//��������
				    IT1_PROGRAM_Uadata();
			break;
						//==================================================================================
			//�ϱ���Ʒ��Ϣ�Ͱ汾
			case 0x20:
						if((RxDat.buf[S_DATLEN]==0x01)&&(RxDat.buf[S_DAT]==0x00))
						{
								IT1_LoadInfo_Report();
								IT1_Load_RS485_Ack(OPT_OK); 
						}
						else if(RxDat.buf[6]>0x01)
						{
								IT1_DevPara_Report(RxDat.buf[5],RxDat.buf[7],RxDat.buf[8]);
								IT1_Load_RS485_Ack(OPT_OK); 
							
							  //=====================================
							  //2017.05.12������ң���������������ָ��
							  buf[0]=0x03;
							  buf[1]=Dev.rand;
							  buf[2]=0x01;
							  buf[3]=0x00;
							  Wireless_Control_IR(0,3,&buf[0]);
							  //======================================
						}
			break;
			//===================================================================================
			//��λ����������
			case 0x22:
						Dev.err|=ERR_RESET;
						IT1_Load_RS485_Ack(OPT_OK);
			break;
			//===================================================================================
			//��������
			//23 00 xx 00 00 10 01 xx
			case 0x23:			
					  IT1_Load_RS485_Ack(OPT_ERR);
			break;
			
			//===================================================================================
			//????
			//e0 00 b1 00 00 10 08 11 22 33 44 55 66 77 88 8f
			//e0 00 b1 00 00 10 08 ff ff ff ff ff ff ff ff 61
			case 0xe0:

			break;
			//===================================================================================
			default:
			     IT1_Load_RS485_Ack(OPT_ERR);  
			break;
			//====================================================================================
		}

	}
	RxDat.cnt=0;	

}

//===========================================================================================
//���ͺ������ڲ��������߾�����ʽ
void IT1_SendData(void)
{
	static uint8_t send_step=0,index=0;														 //���ͽ׶�
	static uint8_t t_buf[200],t_cnt,t_resend,t_offline;
	
	_Frame *ps;
	
	uint8_t i,ret;
	
  //=========================================================================================	
	if(send_step==0)																								//��δ���뷢��״̬
	{
		if(Bus.distime>0)				//���߾���				
		{
					return;												
		}
		else
		{
					ps=GetQueue_OutQ(&RS485_Queue,&index);											//���ȷ���
					if(ps!=0)
					{     

									if(ps->flag==1)
									{
													t_cnt=ps->cnt;
													t_resend=ps->resend;
										      t_offline=ps->offline;
													for(i=0;i<t_cnt;i++)
													{
																t_buf[i]=ps->buf[i];
																ps->buf[i]=0;
													}					
													send_step=1;															//������Ҫ����		

													Bus.randtime=GetRandTime();								//���ʱ��Ϊ40mmS+rand%40	
													Bus.WaitForSend=1;
													Bus.SelfSend=1;
													Bus.timeout=0;
													Bus.time=0;
																				
									
									}
									else
									{
										      t_cnt=0;
										      send_step=0;
									}
									ps->resend=0;
									ps->cnt=0;
									ps->flag=0;
									DeQueueBuf(&RS485_Queue,index);//�����
					}
					else     
					{
									send_step=0;															//������Ҫ����		
									Bus.WaitForSend=0;
									Bus.SelfSend=0;
									return;
					}
		}		    
	}
	//================================================================================================
	else if(send_step==1)					//������Ҫ����
	{				
				//�ж������Ƿ����
				if((Bus.WaitForSend==1)&&(Bus.time>=Bus.randtime))
				{
									Bus.SelfSend=1;
									Bus.WaitForSend=0;
									RS485_Send(t_buf,t_cnt);									
									send_step=2;
									Bus.timeout=0;				//��ʱ�ۼ�
					        Bus.rxok=0;
					        Bus.interval=0;
				}
			  else
				{
// 									if(timeout>3000)
// 									{
// 												Bus.SelfSend=1;
// 												Bus.WaitForSend=0;
// 												RS485_Send(t_buf,t_cnt);
// 												send_step=2;
// 												timeout=0;				//��ʱ�ۼ�
// 									}
				}
	}
	//================================================================================================
	else if(send_step==2)																	//�ȴ��յ��Լ�����������
	{	
				//==================================
				if(Bus.rxok==1)
				{				
								Bus.rxok=0;
								ret=Check_SelfSend(&t_buf[0],t_cnt);
					
								if(ret==0)															//�Է����� У�Դ�
								{
											if(t_resend>0)
											{
												    t_resend--;
														send_step=1;
														Bus.randtime=GetRandTime();	//���ʱ��Ϊ40mmS+rand%40	
														Bus.WaitForSend=1;
												    Bus.time=0;
														Bus.timeout=0;
											}
											else
											{
														send_step=0;
											}
								}
								else if(ret==1)		//�Է�����ͨ��
								{
											send_step=3;
											Bus.timeout=0;
								}
								//��������������Է����պͶԷ�Ӧ��ճ����һ��
								else if(ret==2)
								{
											send_step=3;
											Bus.timeout=0;
								}
				}
				//==================================
				//��ʱ����
				if(Bus.timeout>=100)				//����100mS��û�յ��Լ�����������
				{
								    if(t_resend>0)
								{
											t_resend--;
											send_step=1;
											Bus.randtime=GetRandTime();	//���ʱ��Ϊ40mmS+rand%40	
											Bus.WaitForSend=1;
									    Bus.time=0;
											Bus.timeout=0;
								}
								else
								{
											send_step=0;
								}			
				}
	 }
	 //================================================================================================
	 else if(send_step==3)					//�ȴ��յ�Ӧ������
	 {
				//===================================================================
				if(Bus.rxok==1)
				{
							 Bus.rxok=0;

							 ret=IT1_PraseASK();
		
							 if(ret==0)				//�д���
							 {				
											if(t_resend>0)	
											{
														t_resend--;
														send_step=1;
														Bus.randtime=GetRandTime();	//���ʱ��Ϊ40mmS+rand%40	
														Bus.WaitForSend=1;
														Bus.timeout=0;
												    Bus.time=0;
											}
											else
											{
														send_step=0;
											}
								}
								else 					//�ɹ��յ�Ӧ���ź�
								{
											send_step=4;
											Bus.timeout=0;
								}
				}
				//===================================================================
				//Ӧ��ʱ
				if(Bus.timeout>=150)				//����100mS��û�յ��Լ�����������
				{
								if(t_resend>0)
								{
										    t_resend--;
									  	   send_step=1;
									      Bus.randtime=GetRandTime();	//���ʱ��Ϊ40mmS+rand%40	
												Bus.WaitForSend=1;
												Bus.timeout=0;
									      Bus.time=0;
								}	
								else
								{
												send_step=0;			//�˳�
									      //���ߴ���
									      if(t_offline==1)
												{
													      t_offline=0; 
													      //IT2_Offline_DataLoad(index);
												}
								}
				}
	 }	
	 //====================================================================================================
	 else if(send_step==4)					//�յ�Ӧ������
	 {
				//==================================================
				//���е������ʾӦ����ȷ
				//����ǿ���485Э�飬Dev.stu״��ϵ�ֱ��Ϊ����NORMAL�������IT1Э�飬Dev.stu״̬�ϵ��ֱ��ΪREGISTER
				//����ǵ�ַ����
				//if(Dev.stu==DEV_REQUESTADDR)
// 		    if(m_cmd==F_ASK_CONFIG_ADDR)&&(RxDat.buf[1]==F_ACK_REQUEST_ADDR)								//�����ַ
// 				{
// 					   
// 						 Dev.stu=DEV_REGISTER;
// 						 Dev.addr[0]=RxDat.buf[14];
// 				}				
// 				//�����ע���ϱ�
// 				else if(Dev.stu==DEV_REGISTER)
// 				else if(m_cmd==F_REPORT_ONLINE)							//�����ϱ�
// 				{
// 						 Dev.stu=DEV_NORMAL;
// 				}
// 				//����״̬
// 				else if(Dev.stu>DEV_NORMAL)
// 				{

// 				}
// 				//���������ϱ�,������
// 				else
// 				{
// 					
// 				}
				//====================================================
				send_step=0;
				for(i=0;i<t_cnt;i++)
				{
						t_buf[i]=0;
				}
				t_cnt=0;
				t_resend=0;
				for(i=0;i<RxDat.cnt;i++)
				{
						RxDat.buf[i]=0;
				}
				RxDat.cnt=0;
				
				Bus.SelfSend=0;
	 }
	
}

//===========================================================================================================
//Ӧ��֡����
unsigned char IT1_PraseASK(void)
{
  	unsigned short int tempdata;
  	//unsigned char tmp;
	if(RxDat.cnt>=4)
	{
		//=========================================================================================		
		if(RxDat.buf[0]==MSG_ask)	        //�����Ӧ���
		{
		  	//tempdata=RxDat.buf[1];
				//tempdata=(tempdata<<8)+RxDat.buf[2];
				tempdata=RxDat.buf[2];
			//========================================================================================
			//�ж��Ƿ����豸�����ַ�����������豸��ַ
		  if(tempdata==Dev.addr[0])
			{
				if(Dev.stu<DEV_NORMAL)
				{
						if(RxDat.buf[S_DATLEN]==0X01)
						{
									if(RxDat.buf[S_DAT]==0xf2)
									{
												Reg.fullflag=1;
										    Led_set(0,25,25,0xff,1);
												return 0;
									}
									else if(RxDat.buf[S_DAT]==0x00)
									{
												Dev.stu=DEV_NORMAL;
												InitQueueBUF(&RS485_Queue);
												return 1;
									}
						}
						else if(RxDat.buf[S_DATLEN]==0X00)
						{
									Dev.stu=DEV_NORMAL;
									InitQueueBUF(&RS485_Queue);
									return 1;
						}
				}
				else
				{
						return 1;
				}
			}
		}
	}
	return 0;
}


//===================================================================================
//�����豸ע��
void IT1_Dev_Register(void)
{
	
	 _Frame *ps;
	
	 #ifdef USE_NEWREGISTER	
	 uint8_t index,len=0,i;
	 #endif
	  
	
	if(Dev.stu<DEV_NORMAL)
	{
		
			ps=GetQueue_InQ(&RS485_Queue,8);
			//�ж��Ƿ���ջ��������
			if(ps==NULL){return;}
				
		
			//=======================================================================
			//���ϵķ�ʽע��
			#ifndef USE_NEWREGISTER		
			//�ϵ緢����������
			ps->buf[0] =0x02;									//֡����,���豸��������
			ps->buf[1] =0x00;
			ps->buf[2] =0x00;
			ps->buf[3] =Dev.serial++;					//֡���к�
			ps->buf[4] =Dev.addr[0];					//����ID
			ps->buf[5] =0x21;									//�����������
			
			ps->buf[6] =(uint8_t)(Dev.type>>8);						//�豸����
			ps->buf[7] =(uint8_t)(Dev.type&0x00ff);

			if((Dev.type&0xf000)==0xb000)ps->buf[8] =0x80;									//�豸�����Լ��豸�ӵ�ַ
			else                         ps->buf[8] =0x10;
			
			ps->buf[9] =0x00;									//���ݸ���
			ps->buf[10]=0;										//У���
			

			ps->buf[10]=Crc8CheckSum((uint8_t*)ps->buf,10);	//CRC8У��
  			
			ps->flag=1;
			ps->cnt=11;
			ps->resend=1;
			ps->ackwait=0;
			EnQueueBuf(&RS485_Queue,8);
			//============================================================================
			//���µķ�ʽע��
			#else
			//�ϵ緢����������
			//���ݲ��� 0x03 0x02 0x01 0x08 0x77 0x10 0x02 0x01 0x02 0x00 0x04 0x01 0x8b 0x00
			
			//��������
			IT1_Register_Refresh();
					
			ps->buf[0] =0x02;															//֡����,���豸��������
			ps->buf[1] =VERFLAG;//ps->buf[1] =0x00;
			ps->buf[2] =0x00;
			ps->buf[3] =Dev.serial++;											//֡���к�
			ps->buf[4] =Dev.addr[0];											//����ID
			ps->buf[5] =0x21;															//�����������
			
			ps->buf[6] =(uint8_t)(Dev.type>>8);						//�豸����
			ps->buf[7] =(uint8_t)(Dev.type&0x00ff);

			ps->buf[8] =0x80;															//�豸�����Լ��豸�ӵ�ַ
			ps->buf[9] =0;																//���ݸ���
			
			if(NewRegType.num==0)
			{
						len=0;
			}
			else
			{
						ps->buf[10]=NewRegType.num;							//ͨ����
						ps->buf[11]=0x01;												//����
						index=0;len=0;
						for(i=0;i<8;i++)
						{
								if(NewRegType.item[i].bitpos!=0)
								{
										ps->buf[12+index*3+0]=NewRegType.item[i].bitpos;
										ps->buf[12+index*3+1]=(uint8_t)((NewRegType.item[i].type>>8)& 0x00ff);
										ps->buf[12+index*3+2]=(uint8_t)((NewRegType.item[i].type)& 0x00ff);
										index++;
										len=len+3;
								}
						}
						len=len+2;
						ps->buf[9]=len;															//���ݳ���
		  }
			

			ps->buf[len+10]=Crc8CheckSum((uint8_t*)ps->buf,len+10);	//CRC8У��
  			
			ps->flag=1;
			ps->cnt=len+11;
			ps->resend=1;
			ps->ackwait=0;
			EnQueueBuf(&RS485_Queue,8);
			
			#endif
		
			
	}
}

//=========================================================================================================
//�������ر����ú���������豸û���ϱ�����ģ���ϱ�һ�Σ�ʹ�����豸����
void IT1_Dev_Declare(uint8_t index,uint8_t cmd)
{
 	    _Frame *ps;
 	    uint8_t buf[2],rt;

 			ps=GetQueue_InQ(&RS485_Queue,index);
 			//�ж��Ƿ���ջ��������
 			if(ps==NULL){return;}			
 			//�ϵ緢����������
 			ps->buf[0] =0x05;									//֡����,���豸��������
 			ps->buf[1] =(uint8_t)(Master_Address>>8);
 			ps->buf[2] =(uint8_t)(Master_Address&0xff);
 			ps->buf[3] =0x00;					        //
 			ps->buf[4] =Dev.addr[0];					//���ص�ַ
 			ps->buf[5] =index+1;					//ͨ����
			
			rt=IT1_Wireless2RS485(Exchangebuf.type,&buf[0]);
      //if(rt==RE_ERR)return;
			
			if((Exchangebuf.key!=0x00)&&(Exchangebuf.key!=0xff))
			{
					ps->buf[6] =0x04;						      //�����ֽ�
					ps->buf[7] =cmd;
							
					ps->buf[8] =buf[0];								
					ps->buf[9] =buf[1];
					ps->buf[10] =Exchangebuf.key;	
					
					ps->buf[11]=Crc8CheckSum((uint8_t*)ps->buf,11);	//CRC8У��
					ps->cnt=12;
			}
			else
		  {
					ps->buf[6] =0x03;						      //�����ֽ�
					ps->buf[7] =cmd;						
					ps->buf[8] =buf[0];								
					ps->buf[9] =buf[1];						
					ps->buf[10]=Crc8CheckSum((uint8_t*)ps->buf,10);	//CRC8У��
					ps->cnt=11;
			}
   			
 			ps->flag=1;
 			ps->resend=5;
 			ps->ackwait=0;
 			EnQueueBuf(&RS485_Queue,index);
}
//==========================================================================================================
//�㲥֡����
void IT1_Function_BroadCast(void)                                                   //�㲥��Ӧ��
{
	      //�����þ�̬����
        _Frame *ps;
	      //uint8_t i;
	      ps=GetQueue_InQ(&RS485_Queue,8);
	      if(ps==0)return;

        //===================================================================================================
        if(RxDat.buf[5]==0x01)                                                             //�㲥��ѯ״̬
        {
                //�豸�����ַ
								if((Dev.type&0xf000)==0xb000)
								{
										ps->buf[0] =0x04;                                                         //֡����,���豸��������
										ps->buf[1] =(Master_Address>>8)&0x00ff;                       						//Ŀ���ַ
										ps->buf[2] =Master_Address;                                       				//Ŀ���ַ
										ps->buf[3] =0x00;                                                         //֡���к�
										ps->buf[4] =Dev.addr[0];                                             			//����ID
										ps->buf[5] =0x10;                     
										ps->buf[6] =0x03;                                                         //���ݳ���
												
										ps->buf[7]=0x00;
										ps->buf[8]=0x00;																													//�����¶�
										ps->buf[9]=0x00;																													//ʵ���¶�	
											
										ps->buf[10]=Crc8CheckSum(ps->buf,10);                         						//CRC8У��
										ps->cnt=11;                                                               //�����ܵ��ֽ���
										ps->flag=1;
										ps->resend=0;
										ps->ackwait=0;
										EnQueueBuf(&RS485_Queue,8);	
								}
								//������������ϱ���ǰ��״̬���������
								else if((Dev.type&0xf000)==0x3000)
								{
										IT1_Load_RS485_Ack(OPT_OK);
										IT1_Lock_DataLoad(0,0x01,0x00);
								}
         }
        //=====================================================================================================
        else if(RxDat.buf[5]==0x02)                                                        //�㲥��ѯ����
        {
                Dev.stu=DEV_REGISTER;
                RegTimeClr();					
                InitQueueBUF(&RS485_Queue);					
        }
}

//=========================================================================================================
//����֡����
void IT1_Function_CMD(void)                                                           //������������
{
        // _Frame *ps;
				uint8_t chl;
	      //ps=GetQueue_InQ(&RS485_Queue,8);
	      //if(ps==0)return;
	
        switch (RxDat.buf[5])
        {
                case Dev_State_CMD:                                                             //�豸״̬�ϱ�											
                      if((Dev.type&0xf000)==0xb000)
											{
													chl=RxDat.buf[7]&0x0f;
													if((chl==0)||(chl>8))
													{
															IT1_Load_RS485_Ack(OPT_ERR);
													}
													else
													{
															chl=chl-1;
															if(Ex_Table.Exchange[chl].type==0xff)
															{
																		IT1_Load_RS485_Ack(OPT_ERR);
															}
															else
															{
																		DlyEvent_handle(chl,chl,chl,(1000+200*chl));
																		IT1_Load_RS485_Ack(OPT_OK);
															}
													}
											}
											//��ת��������
											else if((Dev.type&0xf000)==0x3000)
											{
													IT1_Load_RS485_Ack(OPT_OK);
													IT1_Lock_DataLoad(0,0x01,0x00);
											}
                break;
                     
                //=======================================================================================================
                //0x03 + 0x00+Addr + 0xXX + 0x00 + 0x20 + 0x00 + CRC
                case Dev_OnLine_CMD:                                            //��ѯ�豸�Ƿ�����
                      //ʵ�ʵ�ַ 
                        if(RxDat.buf[7]>=4)
												{
													      IT1_Load_RS485_Ack(OPT_ERR);
												}	
                        else
                        {													
																IT1_Load_RS485_Ack(OPT_OK);
												}
                break;
                case ACK_CMD:                                                   //Ӧ������
                
                break;
								//===========================================================================================
								//20171104������ָ���Ѿ�û��
								//���Ʒ���к�
								//����:0x01 + 0x00+Addr + 0xXX + 0x00 + 0x09 + 0x01+0xXX+ CRC
								//�ӻ�:0x06 + 0x0000 + 0xXX + Addr + 0xXX���豸������+ ���ݳ���+ ���� + CRC
								case 0x09:
											  IT1_Load_RS485_InfoReport();
								break;
								
                default:
									
                break;
        }
}
//============================================================================================================
//����֡����
//˵������ң������˵������֡������������Կ����豸��˵������֡���ڿ���
//�������������ԣ� clf -֡����=0x04
//֡����(1)+Ŀ���ַ��2��+֡���кţ�1��+����ID��1��+�豸������1��+���ݳ��ȣ�1��+�����루1��+CRC
//�����룺0x01��ʾ�����óɵƹ�������,0x02��ʾ�����óɵ��������,0x03�����óɳ���������

void IT1_Function_Date(void)                                                                //������������֡����
{
		
		uint8_t Date_Num,rt/*,i*/;
		uint8_t index,type/*,cmd,route,cmd_dat=0*/;
	
		//������������
	  uint8_t i,cmd,userkey[8],len,flagok,tmp;

		//================================================================================================
		Date_Num=RxDat.buf[6];                                                         			//�����ֽڸ���
		
		if(Date_Num==0)
		{
				IT1_Load_RS485_Ack(OPT_ERR); 
				return;
		}
		
		//ң�����ʹ�������ת����
		if((Dev.type&0xf000)==0xb000)
		{
        
				index=RxDat.buf[5]&0x0f; 
				if((index==0)||(index>8))
				{
					  IT1_Load_RS485_Ack(OPT_ERR); 
						return;
				}
				
				//==============================================================================================
				rt=RE_OK;
				index=index-1;
				type=Ex_Table.Exchange[index].type;
				//cmd=RxDat.buf[7];
				//if(Date_Num>=2)cmd_dat=RxDat.buf[8];
				//===============================================================================================
				//54����42�� ��ת����ң����
				if((type>=0xf0)&&(type<=0xff))
				{
							//����ʹ��
							if((RxDat.buf[6]==2)&&(RxDat.buf[7]==4)){Remote_Key_En(index,RxDat.buf[8]);}
							//������ֹ
							else if((RxDat.buf[6]==2)&&(RxDat.buf[7]==5)){Remote_Key_Dis(index,RxDat.buf[8]);}			
							//����ȫ����ֹ
							else if((RxDat.buf[6]==1)&&(RxDat.buf[7]==7)){Remote_Key_Dis(index,0xff);}
							//����ȫ��ʹ��
							else if((RxDat.buf[6]==1)&&(RxDat.buf[7]==8)){Remote_Key_En(index,0xff);}
							
							IT1_Dev_StuLoad(index);
							rt=OPT_OK;
				}
				//������
				else if((type>=0xe0)&&(type<=0xef))
				{
							//������ʹ��
							if((RxDat.buf[6]==1)&&(RxDat.buf[7]==2)){Remote_Key_En(index,1);}
							//��������ֹ
							else if((RxDat.buf[6]==1)&&(RxDat.buf[7]==1)){Remote_Key_Dis(index,1);}	

							IT1_Dev_StuLoad(index);
							rt=OPT_OK;
				}
				//==============================================================================================
				//��ý��
				else if(type==EX_DEV_MEDIA)
				{
//							  //�Ȼ��ѣ��ڷ�������
//						    wl_wakeup_frame(0);
//						    bsp_StartTimer(0,1000);
//								
//								//��ʱ����
//						   	Dly_Table.item[0].flag=1;
//							  Dly_Table.item[0].index=0;
//						    Dly_Table.item[0].cmd=0;
//						    Dly_Table.item[0].len=Date_Num;
//						    for(i=0;i<Date_Num;i++)Dly_Table.item[0].dat[i]=RxDat.buf[7+i];	
				}
				//�����
//			else if((type==EX_DEV_MOTOR_R)||(type==EX_DEV_MOTOR_E)||(type==EX_DEV_BLIND_R)||(type==EX_DEV_BLIND_E))
//				{
//						if(cmd==0x10)			//�����
//						{
//							  if(Ex_Table.Exchange[index].stu==0)cmd=0x07;
//							  else                                    cmd=0x09;
//								Wireless_Dev_Action(index,cmd,0,0);
//							  Ex_Table.stu[index]=0x10;
//						}
//						else if(cmd==0x12)//�����
//						{
//							  Wireless_Dev_Action(index,0x08,0,0);
//							  Ex_Table.stu[index]=0x12;
//						}
//						else if(cmd==0x11)//�����
//						{
//							  if(Ex_Table.Exchange[index].stu==0)cmd=0x09;
//							  else                                    cmd=0x07;
//							  Wireless_Dev_Action(index,cmd,0,0);
//							  Ex_Table.stu[index]=0x11;
//						}
//						else if(cmd==0x1d)//�����ת
//						{
//							  Exchangebuf.type=Ex_Table.Exchange[index].type;
//							  Exchangebuf.addr=Ex_Table.Exchange[index].addr;
//							  Exchangebuf.wl_addr[0]=Ex_Table.Exchange[index].wl_addr[0];
//							  Exchangebuf.wl_addr[1]=Ex_Table.Exchange[index].wl_addr[1];
//							  Exchangebuf.wl_addr[2]=Ex_Table.Exchange[index].wl_addr[2];
//							  Exchangebuf.channel=Ex_Table.Exchange[index].channel;
//								if(Ex_Table.Exchange[index].stu==0)
//								{
//									  Exchangebuf.stu= 0x01;
//								}
//								else
//								{
//									  Exchangebuf.stu= 0x00;
//								}
//								
//								Write_Ex_Table(index);
//						}
//						else if(cmd==0x14)//���е�ָ����
//						{
//							   route=RxDat.buf[8];
//	               if(route<=0x64)
//								 {
//									   route=100-route;
//										 if(Ex_Table.Exchange[index].stu==0x01)route=100-route;
//							   }
//								 else
//								 {
//									   route=0;
//								 }
//								 
//								 cmd=0x19;
//								 Wireless_Dev_Action(index,cmd,route,0);
//						}
//						else
//						{
//							   rt=RE_ERR;
//						}
//				}
//				//==========================================================================================
//				else if(type==EX_DEV_SOCKET)   //���ܲ���
//				{
//						if(cmd==0xe6)     //�ص�
//						{
//							  Wireless_Dev_Action(index,0x08,0,0);
//							  Ex_Table.stu[index]=0x03;
//						}
//						else if(cmd==0xe5)//����
//						{
//								Wireless_Dev_Action(index,0x07,0,0);
//							  Ex_Table.stu[index]=0x02;
//						}
//						else
//						{
//							   rt=RE_ERR;
//						}
//				}
//        //=========================================================================================				
//				else if(type==EX_DEV_LIGHT)    //�ƹ�			
//				{
//					  if(cmd==0x01)     //�ص�
//						{
//							   Wireless_Dev_Action(index,0x08,0,0);
//							   Ex_Table.stu[index]=0x00;
//						}
//						else if(cmd==0x02)//����
//						{
//								Wireless_Dev_Action(index,0x07,0,0);
//							  Ex_Table.stu[index]=0x01;
//						}
//						else
//						{
//							   rt=RE_ERR;
//						}
//				}
//				//==========================================================================================
//				//����� add 2016.2.23
//				else if(type==EX_DEV_LIGHT_ADJ)    //�ƹ�			
//				{
//					  if(cmd==0x01)     //�ص�
//						{
//							   Wireless_Dev_Action(index,0x08,0,0);      //�Թ�����Ʋ�֧��
//							   //Wireless_Dev_Action(index,0x19,0,0);    
//							   Ex_Table.stu[index]=0x00;
//						}
//						else if(cmd==0x02)//����
//						{
//								 Wireless_Dev_Action(index,0x07,0,0);
//							   //Wireless_Dev_Action(index,0x19,0x64,0); 
//							   Ex_Table.stu[index]=0x64;
//						}
//						else if(cmd==0x03)
//						{
//							  //Wireless_Dev_Action(index,0x1d,cmd_dat,0);  //�Թ�������0x19
//							  Wireless_Dev_Action(index,0x19,cmd_dat,0);  //�Թ�������0x19
//							  if(cmd_dat>100)cmd_dat=100;
//							  Ex_Table.stu[index]=100-cmd_dat;
//						}
//						else
//						{
//							   rt=RE_ERR;
//						}
//				}
				//==========================================================================================
        else
				{  
						rt=RE_ERR;
        }	
				
        if(rt==RE_OK)IT1_RS485_Report(index);
        IT1_Load_RS485_Ack(rt); 
		}
				//������������������
		else if((Dev.type&0xf000)==0x3000)
		{
			  index=RxDat.buf[5]&0x0f; 
				//�����������
			  if(index==0)																
				{	
							if(Date_Num==1)
							{
										cmd=RxDat.buf[7]; 
										//����
										if(cmd==0x05)
										{
													LockItem=lock.item;
													LockItem.safe =0x01;
											    //LockItem.stu|=0x02;			//����ʱͬʱ�������ͣ�ͬʱ������ָ��
											    LockItem.stu|=0x06;
													Write_Lock_Table(0);
	
													IT1_Load_RS485_Ack(OPT_OK);

											    LockItem.stu&=0xfe;				//����
													IT1_Lock_DataLoad(0,0x01,LockItem.stu);		//20180328
											    //IT1_Lock_DataLoad(0,0x01,0x06);											    
													DlyEvent_handle(0,0x03,0x06,1000);
											
											    
										}
										//����
										else if(cmd==0x06)
										{
													LockItem=lock.item;
													LockItem.safe =0x00;
											
											    LockItem.stu&=0xf9;				//20180328 ����ͬʱȥ������
											
													Write_Lock_Table(0);
													IT1_Load_RS485_Ack(OPT_OK);
											    IT1_Lock_DataLoad(0,0x01,0x00);
										}
										//20180320
										else if(cmd==0x02)
										{
											    IT1_Load_RS485_Ack(OPT_OK);
											    IT1_Lock_DataLoad(0,0x01,0x00);
										}
										//0x08ָ���ʼ������
										else if(cmd==0x08)
										{
											    LockItem=lock.item;
													for(i=0;i<6;i++)LockItem.key[i]=i+1+0x30;LockItem.key[6]=0xff;LockItem.key[7]=0xff;								
													Write_Lock_Table(0);
													IT1_Load_RS485_Ack(OPT_OK);
											
											    //tmp=lock.item.stu;
											    //tmp=tmp|0x18;
													//if(cmd==0x04)tmp=tmp|0x18;
													//else if(cmd==0x03)tmp=tmp|0x28;
											    IT1_Lock_DataLoad(0,0x01,0x38);
													
										}
										return;
								    
							}
							//����ָ���
							else if(Date_Num==2)
							{
								    //if((RxDat.buf[7]==7)&&(RxDat.buf[8]==1))Dev.mode|=0x02;
										//else if((RxDat.buf[7]==7)&&(RxDat.buf[8]==2))Dev.mode&=0xfd;
								    //Write_Dev(Dev.mode,TYPE_EE_ADDR_MODE);
								
								    //20180326 add================================================
								   	LockItem=lock.item;
								    if((RxDat.buf[7]==7)&&(RxDat.buf[8]==1))LockItem.stu|=0x02;
										else if((RxDat.buf[7]==7)&&(RxDat.buf[8]==2))LockItem.stu&=0xfd;
										Write_Lock_Table(0);
								    //=============================================================
								
										IT1_Load_RS485_Ack(OPT_OK);
										//IT1_Lock_DataLoad(0,0x01,Dev.mode);
								    IT1_Lock_DataLoad(0,0x01,lock.item.stu);
								    return;
							}
							else if(Date_Num==9)
							{
										cmd=RxDat.buf[7]; 
										for(i=0;i<8;i++)userkey[i]=RxDat.buf[8+i]^lock.randkey[i];		
										i=0;
										len=8;
										if(lock.item.key[7]==0xff)len--;
										if(lock.item.key[6]==0xff)len--;
										
										while(i<len)
										{
													if(userkey[i]!=lock.item.key[i])break;
													i++;
										}
										//�������
										if(cmd==0x03)					flagok=1;
										else if(i==len)					flagok=1;
										else                    flagok=0;
							}
							else
							{
										flagok=0;
							}
							
							//�д��󲻴���
							if(flagok==0)
							{
									IT1_Load_RS485_Ack(OPT_OK);
									//IT1_Lock_DataLoad(0,0x01,0x00);
								  IT1_Lock_DataLoad(0,0x01,0x20);
									return;
							}
							
							index=RxDat.buf[5]&0x0f; 
							
							if((cmd==0x01)||(cmd==0x02))		//���� �͹���
							{
										//ι��
										IWDG_Feed();	
										//Ӧ��
										IT1_Load_RS485_Ack(OPT_OK);
										RS485_Send(TxDat.buf,TxDat.cnt);
										TxDat.f_tx=0;
										lock.index=index;

										//���߻��Ѱ�����Ҫ0.5��
										wl_wakeup_frame(0);
										//��������
										Wireless_Lock_Action(0,cmd);	
										//Wireless_Lock_Action(0,0xcf);	
							}
							//У������
							else if(cmd==0x04)
							{
										lock.setkeytime=100;	
										IT1_Load_RS485_Ack(OPT_OK);
							}	
							//��������������
							else if(cmd==0x03)
							{
										if(lock.setkeytime>0)
										{
													LockItem=lock.item;
													for(i=0;i<8;i++)LockItem.key [i]=userkey[i];									
													Write_Lock_Table(0);
													IT1_Load_RS485_Ack(OPT_OK);
										}
										else
										{
													IT1_Load_RS485_Ack(OPT_ERR);
										}
							}
//							//����
//							else if(cmd==0x05)
//							{
//										LockItem=lock.item;
//										LockItem.safe =0x01;
//										Write_Lock_Table(0);
//										IT1_Load_RS485_Ack(OPT_OK);
//							}
//							//����
//							else if(cmd==0x06)
//							{
//										LockItem=lock.item;
//										LockItem.safe =0x00;
//										Write_Lock_Table(0);
//										IT1_Load_RS485_Ack(OPT_OK);
//							}

							tmp=lock.item.stu;
//ʵ��ԭ��ΪAPP����ָ��ʱ��ģ�ⷢ��һ����ָ��Ա�APP�ܻظ��������������ʵ�ʺ�ת������һ��
							if (cmd==0x02)tmp|=0x01;
							else 					tmp&=~0x01;
//							if(cmd==0x04)tmp=tmp|0x18;
//							else if(cmd==0x03)tmp=tmp|0x28;
							if(cmd==0x03)tmp=tmp|0x28;
							if(cmd==0x04)tmp=tmp|0x18;
							else         tmp=tmp|0x08;
							
							IT1_Lock_DataLoad(0,0x01,tmp);		
					}
					//��ָ�ƽ�ֹ��ʹ�ܴ���
					else
					{
						  //ָ����������Ч
						  tmp=index;
							if((RxDat.buf[6]==1)&&(RxDat.buf[7]==2)){Remote_Key_En(0,tmp);}
							//ָ����������ֹ
							else if((RxDat.buf[6]==1)&&(RxDat.buf[7]==1)){Remote_Key_Dis(0,tmp);}	
           
							IT1_Load_RS485_Ack(OPT_OK);
							IT1_Dev_StuLoad(index);
						
					}
	}
}

//==============================================================================================================
//����֡
void IT1_Function_Setup(void)
{
	 	uint8_t type,tt=0,index,tmp,Iscode=0;
		uint8_t keynum,len;

    //�����������״̬������������
    if(Dev.stu==DEV_SETUP)
    {
            IT1_Load_RS485_Ack(OPT_ERR);
            return;
    }
		
		if((Dev.type&0xf000)==0xb000)
		{
				len=RxDat.buf[6];	
				type=RxDat.buf[7];						       //����	
				keynum=RxDat.buf[10];								 //ͨ���򰴼�����
				index=RxDat.buf[5]&0x0f;
				if((index==0)||(index>8))                                
				{
							IT1_Load_RS485_Ack(OPT_ERR); 
							 return;
				}
								
				switch(type)
				{

					//����
					case 0xbb:									
					
					tmp=IT1_RS4852Wireless(RxDat.buf[8],RxDat.buf[9]);												//����Ҫ����Ե��豸����
				
					//if((tmp==EX_KEY_54KEY)||(tmp==EX_KEY_42KEY)||(tmp==EX_DEV_MEDIA)||(tmp==EX_DEV_SUN)||(tmp==EX_DEV_SUN1))
					if(tmp!=RE_ERR)
					{
									Dev.stu=DEV_SETUP;
									Dev.learn_time=LEARN_TIME_MAX;
									Dev.encode_time=0;
									Iscode=1;
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
									Exchangebuf.type= tmp;                                    //�豸����
									Exchangebuf.addr= index-1;;
									Exchangebuf.channel=RxDat.buf[11];
									Exchangebuf.wl_addr[0]=	0;				                  			//�ȴ�����ң�����ĵ�ַ
									Exchangebuf.wl_addr[1]= 0;  		                      		//
									Exchangebuf.wl_addr[2]= 0;  
						
									if(len==4)Exchangebuf.key=keynum;													//��������
									else      Exchangebuf.key=4;	
									Led_set(1,0xff,1,0xff,2);																	//����
					}
		//			//Ҫ�����ӵ������豸�ǿ����豸
		//			else if(Iscode==2)
		//			{          
		//							Exchangebuf.type= tmp;                                    //�豸����
		//							Exchangebuf.addr= index-1;;            
		//							Exchangebuf.channel=1;
		//							Exchangebuf.wl_addr[0]=	~Dev.rand;              
		//							Exchangebuf.wl_addr[1]= (Dev.rand);  	                  	//�����-ң������ַ1 
		//							//Exchangebuf.wl_addr[2]= (~Dev.rand|RxDat.buf[8]);       //�����-ң������ַ2    
		//							Exchangebuf.wl_addr[2]=Dev.serial++;			
		//							Wireless_Dev_Action(Exchangebuf.addr,0x01,0,0);         	//װ���߷�������
		//			}
		//			//�豸δʶ��
					else
					{
									tt=1;
					}					
					break;	
					//===================================================================================
					//ɾ��ָ���豸
					case 0xcc:
						index=index-1;
					
						Exchangebuf.type=Ex_Table.Exchange[index].type;
						Exchangebuf.wl_addr[1]=Ex_Table.Exchange[index].wl_addr[1];
						Exchangebuf.wl_addr[2]=Ex_Table.Exchange[index].wl_addr[2];
						//Wireless_Dev_Action(index,0x02,0,0);         //װ���߷�������
					
						tmp=Exchangebuf.type;
						Exchangebuf.type=0xff;
						Exchangebuf.addr=0xff;	
						Exchangebuf.addr=0xff;
						Exchangebuf.channel=0xff;
						Exchangebuf.wl_addr[0]=0xff;
						Exchangebuf.wl_addr[1]=0xff;
						Exchangebuf.wl_addr[2]=0xff;
						Exchangebuf.key=0;
						//д����
						Write_Ex_Table(index);
						Exchangebuf.type=tmp;		
						IT1_Dev_Declare(index,0xCC);
					break;
					//=============================================================================
					default:
						tt=1;
					break;
					
				}
					if(tt==1)IT1_Load_RS485_Ack(OPT_ERR); 
					else     IT1_Load_RS485_Ack(OPT_OK); 
	  }
		//==============================================================================
		//���������ָ�ƺ�ɾ��ָ�ƴ���
		else
		{
						len=RxDat.buf[6];	
						type=RxDat.buf[7];						       //����	
						keynum=RxDat.buf[10];								 //ͨ���򰴼�����
						index=RxDat.buf[5]&0x0f;
										
						if((type==0xbb)||(type==0xcc))
						{
//====================================================================================>>>>>>>>>
#if 0
									//ι��
									IWDG_Feed();	
									//Ӧ��
									IT1_Load_RS485_Ack(OPT_OK);
									RS485_Send(TxDat.buf,TxDat.cnt);
									TxDat.f_tx=0;
							    lock.waittime=100;
							    lock.index=index;
									//���߻��Ѱ�����Ҫ0.5��
									//wl_wakeup_frame(0);
									//��������					     
									Wireless_Lock_Action(index,type);
									#ifdef USE_LOCK_TEST
									IT1_Lock_DataLoad(index,0x02,type);
									#endif
#else

									IT1_Load_RS485_Ack(OPT_OK);
							
							    if(type==0xbb)
									{
												lock.waittime=100;
												lock.index=index;
												#ifdef USE_LOCK_TEST
												IT1_Lock_DataLoad(index,0x02,0xbb);
												#endif
									}
									else
									{
												//ɾ�����
												lock.index=index;			     
										    IT1_Lock_DataLoad(lock.index,0x02,0xcc);
												LockItem.Serail[lock.index-1]=0x00;    //�û����+ID����+�û�����;
												LockItem.IdType[lock.index-1]=0x00;
												LockItem.UserType[lock.index-1]=0x00;																		  
												Write_Lock_Table(lock.index);
									}
							
#endif
							
//<<<<<<<===========================================================================
						}				
		}
		
}
//==================================================================================

//==============================================================================================================
// �޸�����
// 08 00 FF 00 00 88 07 BB AA BB FF 72 20 08 F6 
uint8_t IT1_Modify_Type(void)
{
	uint8_t t1,t2;
	uint16_t tt;
	//��Ϊ�޸�ָ��0x08�ڹ㲥���ַʱ��������
	if(RxDat.buf[2]!=Dev.addr[0])return 0;

	t1=RxDat.buf[11];
	t2=RxDat.buf[12];
	tt=(uint16_t)(t1<<8)+(uint16_t)t2;
	//if((tt==0xb100)||(tt==0xb140))
	if((tt==0xb100)||(tt==0xb140)||((tt&0xf000)==0x3000))		//������ת����֧��
	{
			Write_Dev(t1,TYPE_EE_ADDR_TYPESH);
			Write_Dev(t2,TYPE_EE_ADDR_TYPES);
		  Dev.type=tt;
		  Dev.stu=DEV_REGISTER;
		  RegTimeClr(); 
			return 0;
	}
	else
	{
			return 1;
	}

}

//========================================================================================================
//RS485����Ӧ��
void IT1_Load_RS485_Ack(unsigned char st)
{

	TxDat.buf[0]=0x03;														//֡����-Ӧ����
	TxDat.buf[1]=(Master_Address>>8)&0x00ff;			//Ŀ���ַH
	TxDat.buf[2]=Master_Address;									//Ŀ���ַL

  TxDat.buf[3]=0X00;														//֡���к�

	TxDat.buf[4]=RxDat.buf[2];										//����ID
	
			if((st==OPT_OK)||(st==RE_OK))							//����Ӧ��
			{
					TxDat.buf[5]=0x20;		
					TxDat.buf[6]=0x00;														//���ݸ���
					TxDat.buf[7]=Crc8CheckSum(TxDat.buf,7);				//CRC8У��
					TxDat.cnt=8;																	//�����ܵ��ֽ���
					TxDat.f_tx=1;		
			}
			else								//����Ӧ��
			{
				TxDat.buf[5]=0x20;						//�����������
				TxDat.buf[6]=0x01;	
				if(st==OPT_ERR_SETUP)TxDat.buf[7]=0xf3;	
				else                 TxDat.buf[7]=0xf1;	
				TxDat.buf[8]=Crc8CheckSum(TxDat.buf,8);				//CRC8У��
				TxDat.cnt=9;																	//�����ܵ��ֽ���
				TxDat.f_tx=1;					
			}
}

//===========================================================================================================
//װ���Ʒ��Ϣ����Ӧ��
//����:0x01 + 0x00+Addr + 0xXX + 0x00 + 0x09 + 0x01+0xXX+ CRC
//�ӻ�:0x06 + 0x0000 + 0xXX + Addr + 0xXX���豸������+ ���ݳ���+ ���� + CRC
void IT1_Load_RS485_InfoReport(void)
{
		_Frame *ps;
	  uint8_t i;	
	  uint8_t *p;
	
		ps=GetQueue_InQ(&RS485_Queue,8);
		//���������
		if(ps==NULL)
		{
			return;
		}
		
		ps->buf[0]=0x06;			//֡����
		ps->buf[1]=(Master_Address>>8)&0x00ff;  //Ŀ���ַH
		ps->buf[2]=Master_Address;		//Ŀ���ַL
		ps->buf[3]=0x00;	
		ps->buf[4]=Dev.addr[0];
		ps->buf[5]=0x88;		//ͨ����
		ps->buf[6]=0x08;						//���ݳ���
		
		//2016.5.12�޸�
		//for(i=0;i<8;i++)ps->buf[7+i]=(uint8_t)(IT_INFO[i]);								//��Ʒ��Ϣ
		p=(uint8_t*)0x0800fc00;
		for(i=0;i<8;i++)
		{
				ps->buf[7+i]=p[i];
		}
		
		ps->buf[15]=Crc8CheckSum(ps->buf,15);		//CRC8У��
		ps->cnt=16;					                 //�����ܵ��ֽ���
		
		ps->flag=1;
		ps->resend=4;
		//ps->rand=100;
		ps->ackwait=30;					//�ȴ�Ӧ��ʱ�����300mS
		EnQueueBuf(&RS485_Queue,8);
}

//============================================================================================================
//===========================================================================================================
//����˵��:
//����1������ֵ
//����2: ����
void IT1_Key_DataLoad(uint8_t index,uint8_t cmd,uint8_t dat)
{
				uint8_t lin,row,tt;
	
				_Frame *ps;
	
	
				//�����54����42�� ��ת��������������
				if((Ex_Table.Exchange[index].type==EX_KEY_54KEY)||(Ex_Table.Exchange[index].type==EX_KEY_42KEY) \
					||(Ex_Table.Exchange[index].type==EX_KEY_TURN)||(Ex_Table.Exchange[index].type==EX_KEY_FULL1) \
				  ||(Ex_Table.Exchange[index].type==EX_KEY_SCENE1)||(Ex_Table.Exchange[index].type==EX_KEY_NORMAL1))
				  
				{
						if(cmd>0)cmd=cmd-1;
						lin=cmd/8;
						row=cmd%8;
						if((Pannel.item[index].onoffbuf[lin]&(0x01<<row))==0x00)return;
				}
				//�����ͨ��ң����
				if((Ex_Table.Exchange[index].type>=EX_KEY_SCENE1)&&(Ex_Table.Exchange[index].type<=EX_KEY_FULL1))
				{
							if(cmd>0)cmd=cmd-1;
							lin=cmd/8;
							row=cmd%8;
							tt=(Pannel.item[index].onoffbuf[lin]&(0x01<<row));

//							//���ߵƹ�
//							if(Ex_Table.Exchange[index].type==EX_KEY_LIGHT1)
//							{
//										if((tt&0x01)==0x00){if((dat==1)||(dat==2))return;}
//										if((tt&0x02)==0x00){if((dat==3)||(dat==4))return;}
//							}
//							//���ߵ��
//							else if(Ex_Table.Exchange[index].type==EX_KEY_MOTOR1)
//							{
//										if((tt&0x01)==0x00)return;
//								
//							}
//							//�����������
//							else
//							{
//									if(((tt&0x01)==0x00)&&(dat==1))return;
//									if(((tt&0x02)==0x00)&&(dat==2))return;
//									if(((tt&0x04)==0x00)&&(dat==3))return;
//									if(((tt&0x08)==0x00)&&(dat==4))return;
//							}
							
							if((Ex_Table.Exchange[index].type==EX_KEY_LIGHT1)||(Ex_Table.Exchange[index].type==EX_KEY_MOTOR1))
							{
									if(Pannel.item[0].onoffbuf[0]==0)return;
							}
							//�����������
							else
							{
									if(tt==0)return;
							}
				}
		
				
				ps=GetQueue_InQ(&RS485_Queue,index);
				//���������
				if(ps==NULL)
				{
					return;
				}
				
				ps->buf[0]=0x04;			//֡����
				ps->buf[1]=(Master_Address>>8)&0x00ff;  //Ŀ���ַH
				ps->buf[2]=Master_Address;		//Ŀ���ַL
				ps->buf[3]=0x00;	
				ps->buf[4]=Dev.addr[0];
				ps->buf[5]=0x80+index+1;		//ͨ����
				

				ps->buf[6]=2;                        	  //���ݳ���
				ps->buf[7]=cmd+1;													//��ֵ
				ps->buf[8]=dat;												  //ģʽ

				ps->buf[9]=Crc8CheckSum(ps->buf,9);		//CRC8У��
				ps->cnt=10;					//�����ܵ��ֽ���


				ps->flag=1;
				ps->resend=4;
				ps->rand=0;
				ps->ackwait=30;					//�ȴ�Ӧ��ʱ�����300mS
				EnQueueBuf(&RS485_Queue,index);
}

//===========================================================================================================
//��ת����ң�����ϱ�
void IT1_Turn_DataLoad(uint8_t index,uint8_t cmd,uint8_t dat)
{
				//static uint8_t flag=0;
				uint8_t lin,row;
	
				_Frame *ps;
	
				//��ֹ�ϱ�����
				//===================================================================================================
				if((Ex_Table.Exchange[index].type==EX_KEY_LIGHT1)||(Ex_Table.Exchange[index].type==EX_KEY_MOTOR1))
				{
							if((Pannel.item[index].onoffbuf[0]&0x01)==0x00)return;
				}
				else
				{
							if(cmd>0)cmd=cmd-1;
							lin=cmd/8;
							row=cmd%8;
							if((Pannel.item[index].onoffbuf[lin]&(0x01<<row))==0x00)return;
				}
				//=====================================================================================================
				
				ps=GetQueue_InQ(&RS485_Queue,index);
				//���������
				if(ps==NULL)
				{
					return;
				}
				
				ps->buf[0]=0x04;			//֡����
				ps->buf[1]=(Master_Address>>8)&0x00ff;  //Ŀ���ַH
				ps->buf[2]=Master_Address;		//Ŀ���ַL
				ps->buf[3]=0x00;	
				ps->buf[4]=Dev.addr[0];
				ps->buf[5]=0x80+index+1;		//ͨ����
				
				//���ߵƹ�
//				if(Ex_Table.Exchange[index].type==EX_KEY_LIGHT1)
//				{
//							if(flag==0)flag=1;
//					    else       flag=0;
//					
//							ps->buf[6]=2;                       //
//							ps->buf[7]=cmd;  
//							if(cmd==3)ps->buf[8]=2;												//����
//							else if(cmd==4)ps->buf[8]=1;	
//							else if(cmd==1)
//							{
//											if(flag==0)								ps->buf[8]=0x06;                       //
//											else                			ps->buf[8]=0x08;       
//							}
//							else if(cmd==2)
//							{
//											if(flag==0)								ps->buf[8]=0x07;                       //
//											else                			ps->buf[8]=0x08;       
//							}
//							else if(cmd==5)										ps->buf[8]=0x05;
//							else if(cmd==6)										ps->buf[8]=0x06; 
//							else                              return;
//							
//							ps->buf[9]=Crc8CheckSum(ps->buf,9);		//CRC8У��
//							ps->cnt=10;					//�����ܵ��ֽ���
//							
//				}
//				//���ߵ��
//				else if(Ex_Table.Exchange[index].type==EX_KEY_MOTOR1)
//				{
//							ps->buf[6]=2;                       //
//					    ps->buf[7]=cmd;
//							if(cmd==3)												ps->buf[8]=0x12;												//����
//							else if(cmd==4)										ps->buf[8]=0x12;	
//							else if(cmd==1)										ps->buf[8]=0x11; 
//							else if(cmd==2)										ps->buf[8]=0x10; 
//							else if(cmd==5)										ps->buf[8]=0x05;
//							else if(cmd==6)										ps->buf[8]=0x06; 
//							else                              return;			
//							ps->buf[9]=Crc8CheckSum(ps->buf,9);		//CRC8У��
//							ps->cnt=10;					//�����ܵ��ֽ���
//				}
				//���� ͨ�� 
				if((Ex_Table.Exchange[index].type==EX_KEY_NORMAL1)||(Ex_Table.Exchange[index].type==EX_KEY_SCENE1))
				{
							ps->buf[6]=1;                        	  //���ݳ���
							ps->buf[7]=cmd+1;												//��ֵ
							ps->buf[8]=Crc8CheckSum(ps->buf,8);		//CRC8У��
							ps->cnt=9;					//�����ܵ��ֽ���
				}
				//����
				else if(Ex_Table.Exchange[index].type==EX_KEY_FULL1)
				{		
						  ps->buf[6]=2;                        	  //���ݳ���
							ps->buf[7]=cmd+1;												  //��ֵ
							ps->buf[8]=0x01;												//ģʽ������Ĭ��Ϊ����
							ps->buf[9]=Crc8CheckSum(ps->buf,9);		//CRC8У��
							ps->cnt=10;					//�����ܵ��ֽ���
				}
				else
				{
							return;
				}


				ps->flag=1;
				ps->resend=4;
				ps->rand=0;
				ps->ackwait=30;					//�ȴ�Ӧ��ʱ�����300mS
				EnQueueBuf(&RS485_Queue,index);
}

//===========================================================================================================
//�������ϱ�
//��Ϊ״̬�ϱ��ͱ��������ϱ�
void IT1_Lock_DataLoad(uint8_t index,uint8_t type,uint8_t dat)
{

				_Frame *ps;
				uint8_t i;
				//=====================================================================================================
				
				ps=GetQueue_InQ(&RS485_Queue,index);
				//���������
				if(ps==NULL){return;}
				
				//����������
				if(type==0x00)
				{
//						ps->buf[0]=0x24;			//֡����
//						ps->buf[1]=(Master_Address>>8)&0x00ff;  //Ŀ���ַH
//						ps->buf[2]=Master_Address;		//Ŀ���ַL
//						ps->buf[3]=0x00;	
//						ps->buf[4]=Dev.addr[0];
//						ps->buf[5]=index;		//ͨ����	
//						ps->buf[6]=9;                        	  //���ݳ���
//						ps->buf[7]=dat;												  //��ֵ
//						for(i=0;i<8;i++)ps->buf[8+i]=lock.randkey[i];												//ģʽ������Ĭ��Ϊ����
//						ps->buf[16]=Crc8CheckSum(ps->buf,16);		//CRC8У��
//						ps->cnt=17;					//�����ܵ��ֽ���
//						lock.report=1;
					
						ps->buf[0]=0x24;			//֡����
						ps->buf[1]=(Master_Address>>8)&0x00ff;  //Ŀ���ַH
						ps->buf[2]=Master_Address;		//Ŀ���ַL
						ps->buf[3]=0x00;	
						ps->buf[4]=Dev.addr[0];
						ps->buf[5]=0x10+index;		//ͨ����	
						ps->buf[6]=0x03;                        	  //���ݳ���
					  ps->buf[7]=0x01; 
						ps->buf[8]=0x00; 
						ps->buf[9]=dat;												  //��ֵ
						ps->buf[10]=Crc8CheckSum(ps->buf,10);		//CRC8У��
						ps->cnt=11;					//�����ܵ��ֽ���
						lock.report=1;
				}
				//��״̬�ϱ�
				else if(type==0x01)
				{			
						ps->buf[0]=0x04;			//֡����
						ps->buf[1]=(Master_Address>>8)&0x00ff;  //Ŀ���ַH
						ps->buf[2]=Master_Address;		//Ŀ���ַL
						ps->buf[3]=0x00;	
						ps->buf[4]=Dev.addr[0];
						ps->buf[5]=index;		//ͨ����	
						ps->buf[6]=9;                        	  //���ݳ���
						ps->buf[7]=dat;												  //��ֵ
						for(i=0;i<8;i++)ps->buf[8+i]=lock.randkey[i];												//ģʽ������Ĭ��Ϊ����
						ps->buf[16]=Crc8CheckSum(ps->buf,16);		//CRC8У��
						ps->cnt=17;					//�����ܵ��ֽ���
					  lock.report=1;
					
					  if((dat&0x01)==0x00){/*Dev.mode&=0xfe;*/LockItem.stu&=0xfe;}						//20180326 add tail
					  else                {/*Dev.mode|=0x01;;*/LockItem.stu|=0x01;}
				}
				//���ָ�ƻ�ɾ��ָ��
				else if(type==0x02)
				{
						ps->buf[0]=0x05;			//֡����
						ps->buf[1]=(Master_Address>>8)&0x00ff;  //Ŀ���ַH
						ps->buf[2]=Master_Address;		//Ŀ���ַL
						ps->buf[3]=0x00;	
						ps->buf[4]=Dev.addr[0];
						ps->buf[5]=index;		//ͨ����
						if(dat==0xbb)
						{
									ps->buf[6]=3;                        	  //���ݳ���
									ps->buf[7]=dat;												  //��ֵ
									ps->buf[8]=0x8e;
							    ps->buf[9]=0x00;
									ps->buf[10]=Crc8CheckSum(ps->buf,10);		//CRC8У��
									ps->cnt=11;					//�����ܵ��ֽ���
									lock.report=1;
						}
						else
						{
									ps->buf[6]=1;                        	  //���ݳ���
									ps->buf[7]=dat;												  //��ֵ
									ps->buf[8]=Crc8CheckSum(ps->buf,8);		//CRC8У��
									ps->cnt=9;					//�����ܵ��ֽ���
									lock.report=1;
						}
				}
				//������ָ��
				if(type==0x03)
				{

#if 0				//����֤���뷢��				
						ps->buf[0]=0x25;			//֡����
						ps->buf[1]=(Master_Address>>8)&0x00ff;  //Ŀ���ַH
						ps->buf[2]=Master_Address;		//Ŀ���ַL
						ps->buf[3]=0x00;	
						ps->buf[4]=Dev.addr[0];
						ps->buf[5]=0x10;//index;		//ͨ����	
						ps->buf[6]=0x01;                        	  //���ݳ���
						ps->buf[7]=dat;												  //��ֵ
						ps->buf[8]=Crc8CheckSum(ps->buf,8);		//CRC8У��
						ps->cnt=9;					//�����ܵ��ֽ���
						lock.report=1;
#else       //��֤���뷢��
						ps->buf[0]=0x25;			//֡����
						ps->buf[1]=(Master_Address>>8)&0x00ff;  //Ŀ���ַH
						ps->buf[2]=Master_Address;		//Ŀ���ַL
						ps->buf[3]=0x00;	
						ps->buf[4]=Dev.addr[0];
						ps->buf[5]=0x10;		//ͨ����	
						ps->buf[6]=9;                        	  //���ݳ���
						ps->buf[7]=dat;												  //��ֵ
						for(i=0;i<8;i++)ps->buf[8+i]=lock.randkey[i];												//ģʽ������Ĭ��Ϊ����
						ps->buf[16]=Crc8CheckSum(ps->buf,16);		//CRC8У��
						ps->cnt=17;					//�����ܵ��ֽ���
						lock.report=1;

#endif
				}
				else if(type==0x04)
				{
					
						ps->buf[0] =0x04;											//֡����,���豸��������
						ps->buf[1] =0x00;
						ps->buf[2] =0x00;
						ps->buf[3] =Dev.serial++;							//֡���к�
						ps->buf[4] =Dev.addr[0];							//����ID
						ps->buf[5] =0x10+index;											//�豸��ͨ���͵�ǰͨ��
						ps->buf[6] =01;						      			//���ݳ���
				    ps->buf[7]=dat;	
						ps->buf[8]=Crc8CheckSum(ps->buf,8);		//CRC8У��
						ps->cnt=9;					                 	//�����ܵ��ֽ���  
				}
				ps->flag=1;
				ps->resend=4;
				ps->rand=0;
				ps->ackwait=30;					//�ȴ�Ӧ��ʱ�����300mS
				EnQueueBuf(&RS485_Queue,index);
}
//============================================================================================================
//===========================================================================================================
//����˵��:
//����1������ֵ
//����2: ����
void IT1_Sensor_DataLoad(uint8_t index,uint8_t *pdat)
{
				uint8_t lin,row; 
				_Frame *ps;
	
	
				//������
				//if((Ex_Table.Exchange[index].type==EX_DEV_SUN)||(Ex_Table.Exchange[index].type==EX_DEV_SUN1))			  
				{
						lin=0;
						row=0;
						if((Pannel.item[index].onoffbuf[lin]&(0x01<<row))==0x00)return;
				}
		
				
				ps=GetQueue_InQ(&RS485_Queue,index);
				//���������
				if(ps==NULL)
				{
					return;
				}
				
				ps->buf[0]=0x24;			//֡����
				ps->buf[1]=(Master_Address>>8)&0x00ff;  //Ŀ���ַH
				ps->buf[2]=Master_Address;		//Ŀ���ַL
				ps->buf[3]=0x00;	
				ps->buf[4]=Dev.addr[0];
				
				if(Ex_Table.Exchange[index].type==EX_DEV_SUN)
				{
							ps->buf[5]=index+1;			//ͨ����
							ps->buf[6]=3;           //���ݳ���
					    ps->buf[7]=1;           //���ݳ���
							ps->buf[8]=0x00;				//��ֵ
							ps->buf[9]=*(pdat+1);					//ģʽ
							ps->buf[10]=Crc8CheckSum(ps->buf,10);		//CRC8У��
							ps->cnt=11;					//�����ܵ��ֽ���
				}
				else  if(Ex_Table.Exchange[index].type==EX_DEV_SUN1)
				{
							ps->buf[5]=index+1;			//ͨ����
							ps->buf[6]=3;           //���ݳ���
					    ps->buf[7]=2;           //							ps->buf[8]=0x00;				//
							ps->buf[8]=*(pdat+1);		//ģʽ
							ps->buf[9]=*(pdat+2);		//ģʽ
							ps->buf[10]=Crc8CheckSum(ps->buf,10);		//CRC8У��
							ps->cnt=11;					//�����ܵ��ֽ���
				}
				else
				{
							return;
				}

				ps->flag=1;
				ps->resend=4;
				ps->rand=0;
				ps->ackwait=30;					//�ȴ�Ӧ��ʱ�����300mS
				EnQueueBuf(&RS485_Queue,index);
}
//===========================================================================================================
//����˵��:
//����1������ֵ
//����2: ����
void IT1_RS485_DataLoad(uint8_t index,uint8_t cmd,uint8_t dat)
{
	
////	uint8_t wlrbuf[5];
////	uint16_t wlrdat;
////	uint8_t flagload;
////	
////	_Frame *ps;
////	
////	ps=GetQueue_InQ(&RS485_Queue,index);
////	//���������
////	if(ps==NULL)
////	{
////		return;
////	}
////	
////	ps->buf[0]=0x04;			//֡����
////	ps->buf[1]=(Master_Address>>8)&0x00ff;  //Ŀ���ַH
////	ps->buf[2]=Master_Address;		//Ŀ���ַL
////	ps->buf[3]=0x00;	
////	ps->buf[4]=Dev.addr[0];
////	ps->buf[5]=0x80+index+1;		//ͨ����
////	
////	flagload=1;
////	//װ��ң�����ϱ����ݻ�����
////	if((Ex_Table.Exchange[index].type>=EX_DEV_KEY)||(Ex_Table.Exchange[index].type==EX_DEV_WSR)||(Ex_Table.Exchange[index].type==EX_DEV_RAIN))
////	{
////				if(cmd==0x1e)cmd=0x08;
////							
////				if     (cmd==0x07)dat=1;
////				else if((cmd==0x08)||(cmd==0xe0))dat=2;
////				else if(cmd==0x09)dat=3;
////				else if((cmd==0x21)||(cmd==0x22))dat=4;
////		    else if(cmd==0x26)dat=5;
////		    else if(cmd==0x25)dat=6;
////		    else if(cmd==0x24)dat=7;
////		    else if(cmd==0x23)dat=8;
////			
////		
////		    //2016.5.10����ͨ����ʱ��
////				if((Ex_Table.Exchange[index].type==EX_KEY_SCENE)||(Ex_Table.Exchange[index].type==EX_KEY_GENERAL))	//����������+ͨ����ʱ��
////				{
////							ps->buf[6]=1;                        	//���ݳ���
////							ps->buf[7]=dat;
////							ps->buf[8]=Crc8CheckSum(ps->buf,8);		//CRC8У��
////							ps->cnt=9;					//�����ܵ��ֽ���
////				}                                       
////        else if(Ex_Table.Exchange[index].type==EX_KEY_LIGHT)  //�ƹ�������                              
////        {
////							ps->buf[6]=2;                         	//���ݳ���
////							ps->buf[7]=dat;
////							if((dat==1)||(dat==3))ps->buf[8]=0x02;
////							else                  ps->buf[8]=0x01;           
////							ps->buf[9]=Crc8CheckSum(ps->buf,9);		//CRC8У��
////    		      ps->cnt=10;					//�����ܵ��ֽ���
////        }
////				else if(Ex_Table.Exchange[index].type==EX_KEY_MOTOR)								//���������
////				{
////							ps->buf[6]=2;                         	//���ݳ���
////							ps->buf[7]=dat;
////							if(dat==1)ps->buf[8]=0x10;
////							else if(dat==2)ps->buf[8]=0x12;
////							else if(dat==3)ps->buf[8]=0x11;
////							else  return;           
////							ps->buf[9]=Crc8CheckSum(ps->buf,9);		//CRC8У��
////							ps->cnt=10;					//�����ܵ��ֽ���
////				}
////				//2016.9.7���ӷ���괫���� �ۺ�
////				else if(Ex_Table.Exchange[index].type==EX_DEV_WSR)								//���������
////				{
////							ps->buf[6]=3;                         	//���ݳ���
////					    //ȡ�÷��������
////					    wlrdat=(uint16_t)((wbuf[12]<<8)+wbuf[13]);
////					    Get_WLR_Data(wlrdat,wlrbuf);
////					    if(wlrbuf[0]<3)
////							{						
////									ps->buf[7]=0x01;
////									ps->buf[8]=0x00;
////									
////									ps->buf[9]=wlrbuf[1];
////									ps->buf[5]=0x30+wlrbuf[0];								//ͨ���� ͨ��0-�磬ͨ��1-�⣬ͨ��2-��
////							
////									ps->buf[10]=Crc8CheckSum(ps->buf,10);		//CRC8У��
////									ps->cnt=11;					//�����ܵ��ֽ���
////								
////							}
////							else
////							{
////									flagload=0;
////							}
////				}	
////				//����������
////				else if(Ex_Table.Exchange[index].type==EX_DEV_RAIN)
////				{
////							ps->buf[6]=3;                         	//���ݳ���
////					    //ȡ�÷��������
////					    wlrdat=(uint16_t)((wbuf[12]<<8)+wbuf[13]);
////					    Get_WLR_Data(wlrdat,wlrbuf);
////					    if(wlrbuf[0]==0X02)
////							{						
////									ps->buf[7]=0x01;
////									ps->buf[8]=0x00;
////									
////									ps->buf[9]=wlrbuf[1];
////									ps->buf[5]=0x30;								//
////							
////									ps->buf[10]=Crc8CheckSum(ps->buf,10);		//CRC8У��
////									ps->cnt=11;					//�����ܵ��ֽ���
////							}
////							else
////							{
////									flagload=0;
////							}
////							
////				}
////				//Ĭ��Ϊ����������
////				else
////				{
//////							ps->buf[6]=1;                        	//���ݳ���
//////							ps->buf[7]=dat;
//////							ps->buf[8]=Crc8CheckSum(ps->buf,8);		//CRC8У��
//////							ps->cnt=9;					//�����ܵ��ֽ���
////					
////								flagload=0;
////				}
////				if(flagload==1)
////				{
////								ps->flag=1;
////								ps->resend=4;
////								ps->rand=0;
////								ps->ackwait=30;					//�ȴ�Ӧ��ʱ�����300mS
////								EnQueueBuf(&RS485_Queue,index);
////				}
////	}
////	//װ���豸�����ϱ�����
////	#if 0    //clf mod 20161216ֻ��������
////	else 
////	{
////								dat=cmd;
////								//if(dat>100)dat=100;				//û�г�ʱ�ϱ�Ϊ0xff
////							  ps->buf[6]=3;
////	
////								if(Ex_Table.Exchange[index].type==EX_DEV_LIGHT)	            //�ƹ��ϱ�
////                {
////                	if(dat==0x00) ps->buf[7]=0x01;
////                	else          ps->buf[7]=0x00;
////                  ps->buf[8]=0x00;
////                  ps->buf[9]=0x00;
////                }
////								//==================================================================
////								//������ϱ� add 2016.2.23
////								else if(Ex_Table.Exchange[index].type==EX_DEV_LIGHT_ADJ)	  //���ƹ��ϱ�
////                {
////									if(dat>100)dat=100;
////									
////                	if(dat==0x00)
////									{										
////											ps->buf[7]=0x00;									  									
////											ps->buf[8]=0x00;
////											ps->buf[9]=0;
////									}
////                	else
////									{									
////											ps->buf[7]=0x01;
////										  ps->buf[8]=0x00;
////											ps->buf[9]=dat;
////									}	
////                }
////								
////                else if(Ex_Table.Exchange[index].type==EX_DEV_SOCKET)	//���ܲ���
////                {
////                        if(dat==0x00) ps->buf[7]=0x02;
////                	      else          ps->buf[7]=0x03;
////                        ps->buf[8]=0x00;
////                        ps->buf[9]=0x00;
////                }
////								//=========================================================================================================
////                else                                                           //��������ϱ�״̬
////                {
////                        if((Ex_Table.Exchange[index].type==EX_DEV_MOTOR_E)||(Ex_Table.Exchange[index].type==EX_DEV_BLIND_E))
////                        {
////																//ID����868 �� IT485 �г̱�ʾ�෴
////                                //if(((Ex_Table.Exchange[index].channel&0x80)==0x80)&&(dat<=0x64))
////																
////																if(dat>0x64)
////																{
////																				 ps->buf[7]=0x12;
////																				 ps->buf[8]=0x00;
////																				 ps->buf[9]=0x00;
////																}
////																else					//�����г�
////																{
////																	       if(Ex_Table.Exchange[index].stu==1)dat=0x64-dat; 
////																				 ps->buf[7]=0x12;
////																				 ps->buf[8]=0x00;
////																				 ps->buf[9]=0x64-dat;
////						
////																				 //=========================================
////																}
////												
////                        } 
////												else if((Ex_Table.Exchange[index].type==EX_DEV_MOTOR_R)||(Ex_Table.Exchange[index].type==EX_DEV_BLIND_R))
////												{
////																ps->buf[7]=Ex_Table.stu[index];
////																ps->buf[8]=0x00;
////																ps->buf[9]=0x00;
////												}
////			
////               } 
////							 ps->buf[10]=Crc8CheckSum(ps->buf,10);		//CRC8У��
////							 ps->cnt=11;					//�����ܵ��ֽ���
////							 ps->flag=1;
////							 ps->resend=4;
////							 ps->rand=0;
////							 ps->flag=1;
////							 ps->ackwait=30;					//�ȴ�Ӧ��ʱ�����300mS
////							 EnQueueBuf(&RS485_Queue,index);     
////         } 
////	//=========================================================================================================
////	#endif  //clf mod 20161216ֻ��������ת����
                                  
}

//===========================================================================================================
//����˵��:
//����1������ֵ
//����2: ����
//״̬�ϱ�
void IT1_RS485_StuLoad(uint8_t index,uint8_t len,uint8_t *pdat)
{
	
				_Frame *ps;
	      uint8_t i;
				
				ps=GetQueue_InQ(&RS485_Queue,index);
				//���������
				if(ps==NULL)
				{
					return;
				}
				
				ps->buf[0]=0x04;			//֡����
				ps->buf[1]=(Master_Address>>8)&0x00ff;  //Ŀ���ַH
				ps->buf[2]=Master_Address;		//Ŀ���ַL
				ps->buf[3]=0x00;	
				ps->buf[4]=Dev.addr[0];
				ps->buf[5]=0x10;//0x80+index+1;		//ͨ����
				

				ps->buf[6]=len;                        	   //���ݳ���
				if(len>3)len=3;
				for(i=0;i<len;i++)ps->buf[7+i]=*(pdat+i);

				ps->buf[7+len]=Crc8CheckSum(ps->buf,7+len);		//CRC8У��
				ps->cnt=8+len;					//�����ܵ��ֽ���


				ps->flag=1;
				ps->resend=4;
				ps->rand=0;
				ps->ackwait=30;					//�ȴ�Ӧ��ʱ�����300mS
				EnQueueBuf(&RS485_Queue,index);
}
				
//============================================================================================================
//============================================================================================================
//������Ϻ�ģ���ϱ�һ��״̬
//����˵��:
//����1��ת�����ַ
//����2: ����
void IT1_RS485_Report(uint8_t index)
{	
 		_Frame *ps;
 		
 		ps=GetQueue_InQ(&RS485_Queue,index);
 		//���������
 		if(ps==NULL)
 		{
 			return;
 		}
 		
 		ps->buf[0]=0x04;			//֡����
 		ps->buf[1]=(Master_Address>>8)&0x00ff;  //Ŀ���ַH
 		ps->buf[2]=Master_Address;		//Ŀ���ַL
 		ps->buf[3]=0x00;	
 		ps->buf[4]=Dev.addr[0];
 		ps->buf[5]=0x80+index+1;		//ͨ����
 		
 		//װ��ң�����ϱ����ݻ�����
 		if((Ex_Table.Exchange[index].type==EX_DEV_LIGHT)||(Ex_Table.Exchange[index].type==EX_DEV_SOCKET))
 		{
 				ps->buf[6]=1;		
 				ps->buf[7]=Ex_Table.stu[index];
 				ps->buf[8]=Crc8CheckSum(ps->buf,8);		//CRC8У��
 				ps->cnt=9;					                 //�����ܵ��ֽ���
 		}
 		else if(Ex_Table.Exchange[index].type==EX_DEV_MOTOR_R)
 		{
 				ps->buf[6]=3;		
 				ps->buf[7]=Ex_Table.stu[index];
 				ps->buf[8]=0x00;										 //
 				ps->buf[9]=0x00;
 				ps->buf[10]=Crc8CheckSum(ps->buf,10);		//CRC8У��
 				ps->cnt=11;					                 //�����ܵ��ֽ���
 		}
 		else
 		{
 			  return;
 		}
 		ps->flag=1;
 		ps->resend=4;
 		//ps->rand=100;
 		ps->ackwait=30;					//�ȴ�Ӧ��ʱ�����300mS
 		EnQueueBuf(&RS485_Queue,index);
	
	}
	
//============================================================================================================
//485��ַ����
void IT1_AddressSetting(void)
{
		uint8_t i/*,tmp*/;
	 unsigned char IsACK=0,IsModify=0;

   if(RxDat.buf[2]==Dev.addr[0])
   {
        IsModify=1;
        IsACK=1;
   }
   else if((Dev.power_time<30)&&(RxDat.buf[2]==0xff)&&(RxDat.buf[1]==0xff))
   {        
        IsModify=1;
        IsACK=0;
        //Dev.power_time=0;
   }
   else
   {
        IsModify=0;
        IsACK=0;
   } 
   if(IsModify==1)
   {
        Dev.addr[0]=RxDat.buf[10];
		    
				//if((Dev.err&~ERR_POWER)==0)										//��ѹ��������д����ѹ��ֹ
				{
					Write_Dev(Dev.addr[0],TYPE_EE_ADDR_ADDR0);					  
		    }
				//========================================================================================
				//ɾ�����
				if(RxDat.buf[10]!=RxDat.buf[2])
				{
						for(i=0;i<8;i++)
						{
									Exchangebuf.type=Ex_Table.Exchange[i].type;
									Exchangebuf.wl_addr[1]=Ex_Table.Exchange[i].wl_addr[1];
									Exchangebuf.wl_addr[2]=Ex_Table.Exchange[i].wl_addr[2];
									Wireless_Dev_Action(i,0x02,0,0);         //װ���߷�������
								
									//tmp=Exchangebuf.type;
									Exchangebuf.addr=0xff;	
									Exchangebuf.addr=0xff;
									Exchangebuf.channel=0xff;
									Exchangebuf.wl_addr[0]=0xff;
									Exchangebuf.wl_addr[1]=0xff;
									Exchangebuf.wl_addr[2]=0xff;
									//д����
									Write_Ex_Table(i);
						}			
				}	
				//==============================================================================
		    Dev.stu=DEV_REGISTER;
				RegTimeClr(); 
				InitQueueBUF(&RS485_Queue);
				InitQueueBUF(&WirelessSend_Queue);
        IsModify=0;
        if(IsACK==1)
        {
            IsACK=0;
			      IT1_Load_RS485_Ack(OPT_OK);
        }
   }  
}
//============================================================================================================
//�޸ĵ�ַ���ϵķ�ʽ
void IT1_Modify_Addr_Old(void)
{
    if(RxDat.buf[3]==0xaa&&RxDat.buf[4]==0xaa&&RxDat.buf[5]==0xaa&&\
      RxDat.buf[6]==0xaa&&RxDat.buf[7]==0x00)
    { 		
					TxDat.buf[0] =0xCD;						//֡����,���豸��������
					TxDat.buf[1] =0x00;						//Ŀ���ַ
					TxDat.buf[2] =0x0A;						//Ŀ���ַ
					TxDat.buf[3] =0x03;						//֡���к�
					TxDat.buf[4] =0X00;						//�豸�����Լ��豸�ӵ�ַ
					TxDat.buf[5] =0X00;						//����ID��
					TxDat.buf[6] =0X00;						//���ݸ���  	
					TxDat.buf[7] =RxDat.buf[7];				//���ݸ��� 
					TxDat.buf[8] =RxDat.buf[8];				//���ݸ��� 
					TxDat.buf[9] =Crc8CheckSum(TxDat.buf,9);	
					RS485_Send(TxDat.buf,10);  
					
			
					if((Dev.err&ERR_POWER)==0)										//��ѹ��������д����ѹ��ֹ
					{
						    Dev.addr[0]=RxDat.buf[8];    
                Write_Dev(Dev.addr[0],TYPE_EE_ADDR_ADDR0);							
					}
					
					InitQueueBUF(&RS485_Queue);					//�����	
          InitQueueBUF(&WirelessSend_Queue);					//�����						
					Dev.stu=DEV_REGISTER;
					RegTimeClr(); 
					TxDat.f_tx=1;
	   }
}
//============================================================================================================
//������������,�ȸ�λ������BOOTLOADER����
void IT1_PROGRAM_Uadata(void)
{
	
		 if(RxDat.buf[3]==0xaa&&RxDat.buf[4]==0xaa&&RxDat.buf[5]==0xcf&&\
      RxDat.buf[6]==0xcf&&RxDat.buf[7]==0xfe&&RxDat.buf[8]==0x01)
		 {
			 
			  //*pt=OPT_OK;
				//==================================================================================================
				//ֹͣι��,�ȴ���λ
				Dev.err|=ERR_RESET;
		 }
}

//============================================================================================================
//����ת����RS485--->����
unsigned char IT1_RS4852Wireless(unsigned char pa1,unsigned char pa2)
{
 	unsigned char tmp;
 	
// 	if((pa1==0x71)&&(pa2==0x00))
// 	{
// 		tmp=EX_KEY_SCENE;
// 	}
// 	else if((pa1==0x72)&&(pa2==0x10))
// 	{
// 		tmp=EX_KEY_MOTOR;	
// 	}
// 	else if((pa1==0x72)&&(pa2==0x20))
// 	{
// 		tmp=EX_KEY_LIGHT;
// 	}
//	else if((pa1==0x75)&&(pa2==0x10))
//	{
//		tmp=EX_KEY_GENERAL;
//	}
	
//	if((pa1==0x72)&&(pa2==0x40))			//���ߵƹ����
// 	{
// 		tmp=EX_KEY_LIGHT1;
// 	}
//	else if((pa1==0x71)&&(pa2==0x40))//���߳������
// 	{
// 		tmp=EX_KEY_SCENE1;
// 	}
//	else if((pa1==0x72)&&(pa2==0x60))//���߳������
// 	{
// 		tmp=EX_KEY_MOTOR1;
// 	}
	if((pa1==0x75)&&(pa2==0x40))//����ͨ�����?
 	{
 		tmp=EX_KEY_NORMAL1;
 	}
	else if((pa1==0x77)&&(pa2==0x40))//�����������
 	{
 		tmp=EX_KEY_FULL1;
 	}
	
//	else if((pa1==0x91)&&(pa2==0x00))
// 	{
// 		tmp=EX_DEV_SOCKET;	
// 	}
// 	else if((pa1==0x01)&&(pa2==0x00))
// 	{
// 		tmp=EX_DEV_LIGHT;
// 	}
//	else if((pa1==0x02)&&(pa2==0x00))				//�����
// 	{
// 		tmp=EX_DEV_LIGHT_ADJ;
// 	}
// 	else if((pa1==0x11)&&(pa2==0x00))
// 	{
// 		tmp=EX_DEV_MOTOR_R;	
// 	}
  //else if((pa1==0x12)&&(pa2==0x00))
  //{
 	//	tmp=EX_DEV_BLIND_R;
  //}
//	else if((pa1==0x13)&&(pa2==0x30))       //����E���ֶ�
// 	{
// 		tmp=EX_DEV_MOTOR_E;	
// 	}
//	else if((pa1==0x14)&&(pa2==0x00))
//  {
// 		tmp=EX_DEV_BLIND_E;
//  }
	else if((pa1==0x76)&&(pa2==0x10))
  {
 		tmp=EX_KEY_54KEY;
  }
	else if((pa1==0x76)&&(pa2==0x20))
  {
 		tmp=EX_KEY_42KEY;
  }
	else if((pa1==0x8b)&&(pa2==0x00))
  {
 		tmp=EX_DEV_SUN;
  }
	else if((pa1==0x8b)&&(pa2==0x20))
  {
 		tmp=EX_DEV_SUN1;
  }
//	else if((pa1==0x79)&&(pa2==0x10))
//  {
// 		tmp=EX_KEY_TURN;
//  }
 	else
 	{
 		tmp=RE_ERR;				//���Ͳ���
 	}
 	
 	return tmp;
}
//============================================================================================================
//����ת��   ����->RS485
uint8_t IT1_Wireless2RS485(uint8_t type,uint8_t *pbuf)
{
 	uint8_t tmp;
	
	tmp=RE_OK;
	
// 	if(type==EX_KEY_SCENE)
//	{
//		*(pbuf+0)=0x71;
//		*(pbuf+1)=0x00;
//	}
//	else if(type==EX_KEY_MOTOR)
//	{
//		*(pbuf+0)=0x72;
//		*(pbuf+1)=0x10;
//	}
//	else if(type==EX_KEY_LIGHT)
//	{
//		*(pbuf+0)=0x72;
//		*(pbuf+1)=0x20;
//	}
//	else if(type==EX_KEY_GENERAL)
//	{
//		*(pbuf+0)=0x75;
//		*(pbuf+1)=0x10;
//	}
//	if(type==EX_KEY_LIGHT1)
//	{
//		*(pbuf+0)=0x72;
//		*(pbuf+1)=0x40;
//	}
//	else if(type==EX_KEY_MOTOR1)
//	{
//		*(pbuf+0)=0x72;
//		*(pbuf+1)=0x60;
//	}
//	else if(type==EX_KEY_SCENE1)
//	{
//		*(pbuf+0)=0x71;
//		*(pbuf+1)=0x40;
//	}
	if(type==EX_KEY_NORMAL1)
	{
		*(pbuf+0)=0x75;
		*(pbuf+1)=0x40;
	}
	else if(type==EX_KEY_FULL1)
	{
		*(pbuf+0)=0x77;
		*(pbuf+1)=0x40;
	}
	
//	else if(type==EX_DEV_SOCKET)
//	{
//		*(pbuf+0)=0x91;
//		*(pbuf+1)=0x00;
//	}
//	else if(type==EX_DEV_LIGHT)
//	{
//		*(pbuf+0)=0x01;
//		*(pbuf+1)=0x00;
//	}
//	else if(type==EX_DEV_LIGHT_ADJ)
//	{
//		*(pbuf+0)=0x02;
//		*(pbuf+1)=0x00;
//	}
//	
//	else if(type==EX_DEV_MOTOR_R)
//	{
//		*(pbuf+0)=0x11;
//		*(pbuf+1)=0x00;
//	}
// 	else if(type==EX_DEV_BLIND_R)
// 	{
// 		*(pbuf+0)=0x12;
// 		*(pbuf+1)=0x00;
// 	}
//	else if(type==EX_DEV_MOTOR_E)
//	{
//		*(pbuf+0)=0x13;
//		*(pbuf+1)=0x30;
//	}
//	else if(type==EX_DEV_BLIND_E)
//	{
//		*(pbuf+0)=0x14;
//		*(pbuf+1)=0x00;
//	}
	else if(type==EX_KEY_54KEY)
	{
		*(pbuf+0)=0x76;
		*(pbuf+1)=0x10;
	}
	else if(type==EX_KEY_42KEY)
	{
		*(pbuf+0)=0x76;
		*(pbuf+1)=0x20;
	}
	else if(type==EX_DEV_SUN)
	{
		*(pbuf+0)=0x8b;
		*(pbuf+1)=0x00;
	}
	else if(type==EX_DEV_SUN1)
	{
		*(pbuf+0)=0x8b;
		*(pbuf+1)=0x20;
	}
//	else if(type==EX_KEY_TURN)
//	{
//		*(pbuf+0)=0x79;
//		*(pbuf+1)=0x10;
//	}
	else
	{
		*(pbuf+0)=0x00;
		*(pbuf+1)=0x00;
		tmp=RE_ERR;
	}
	
 	return tmp;
}
//========================================================================================
//װ���ϱ���Ϣ
void IT1_LoadInfo_Report(void)
{
	uint8_t i;
	_Frame *ps;
	
	ps=GetQueue_InQ(&RS485_Queue,8);
	if(ps==NULL)
	{
		return;
	}
	
	ps->buf[0]=0x20;							//???
	ps->buf[1]=0x00;  						//????H
	ps->buf[2]=0x00;							//????L
	ps->buf[3]=0x00;	
	ps->buf[4]=Dev.addr[0];
	
	ps->buf[5]=0x10;							//???
	ps->buf[6]=DEV_INFO_NUM+1;
	ps->buf[7]=0x00;
	if((Dev.type&0xff00)==0x3200)
	{
				for(i=0;i<DEV_INFO_NUM;i++)ps->buf[8+i]=DEV_INFO[i];
	}
	else if((Dev.type&0xf000)==0xb000)
	{
				for(i=0;i<DEV_INFO_NUM;i++)ps->buf[8+i]=DEV_INFO_1[i];
	}
	else
	{
		    for(i=0;i<DEV_INFO_NUM;i++)ps->buf[8+i]=DEV_INFO_2[i];
	}
	ps->buf[8+DEV_INFO_NUM]=Crc8CheckSum(ps->buf,8+DEV_INFO_NUM);		//CRC8??
	ps->cnt=9+DEV_INFO_NUM;					                 //???????
	
	ps->flag=1;
	ps->resend=4;
	//ps->rand=100;
	ps->ackwait=30;					//
	EnQueueBuf(&RS485_Queue,8);
}
//============================================================================================================
//===========================================================================================================
//����˵��:
//����1������ֵ
//����2: ����
void IT1_IR_StuLoad(uint8_t index,uint8_t len,uint8_t *pdat)
{
	
				_Frame *ps;
	      uint8_t i;
				
				ps=GetQueue_InQ(&RS485_Queue,8);
				//���������
				if(ps==NULL)
				{
					return;
				}
				
				ps->buf[0]=0x04;			//֡����
				ps->buf[1]=(Master_Address>>8)&0x00ff;  //Ŀ���ַH
				ps->buf[2]=Master_Address;		//Ŀ���ַL
				ps->buf[3]=0x00;	
				ps->buf[4]=Dev.addr[0];
				ps->buf[5]=0x80+index+1;//0x10;
				

				ps->buf[6]=len;                        	   //���ݳ���
				if(len>3)len=3;
				for(i=0;i<len;i++)ps->buf[7+i]=*(pdat+i);

				ps->buf[7+len]=Crc8CheckSum(ps->buf,7+len);		//CRC8У��
				ps->cnt=8+len;					//�����ܵ��ֽ���


				ps->flag=1;
				ps->resend=4;
				ps->rand=0;
				ps->ackwait=30;					//�ȴ�Ӧ��ʱ�����300mS
				EnQueueBuf(&RS485_Queue,8);
}

//============================================================================================================
//2016.12.26�������ݴ���
//
void IT1_IR_Setup(void)
{
						uint8_t tmp;
						//===========================================================================================================
						//2017.1.6�޸ģ�������벿�ַ���ң������
						__IO uint8_t i;
						
						Ir.addr=(RxDat.buf[5]&0x0f)-1;
						Ir.cmd_key=RxDat.buf[7];
	          Ir.key_mode=RxDat.buf[8];
						//����ң��������
	          Ir.typelen=RxDat.buf[9];
	          if(Ir.typelen>IR_DAT_MAX)Ir.typelen=IR_DAT_MAX;
						for(i=0;i<Ir.typelen;i++)Ir.typebuf[i]=RxDat.buf[i+10];
				    Ir.typechm=0;
						for(i=0;i<Ir.typelen;i++)Ir.typechm=Ir.typechm+Ir.typebuf[i];
					
						//���ư�������
						Ir.keylen=RxDat.buf[Ir.typelen+10];
						if(Ir.keylen>IR_DAT_MAX)Ir.keylen=IR_DAT_MAX;
						
						for(i=0;i<Ir.keylen;i++)Ir.keybuf[i]=RxDat.buf[Ir.typelen+11+i];
	          Ir.keychm=0;
			      for(i=0;i<Ir.keylen;i++)
						{
							Ir.keychm=Ir.keychm+Ir.keybuf[i];
						}
				

						Ir.flag=1;																	//��ʾ����װ�����
						Ir.sendtime=SEND_TIME_MAX;									//�����ʱ��
						IT1_Load_RS485_Ack(OPT_OK); 
						
						//============ȡ���ܰ���=========
						Ir.FrameNum=Ir.typelen/32;
						tmp=Ir.typelen%32;
						if(tmp!=0)Ir.FrameNum++;
						tmp=Ir.keylen/32;
						Ir.FrameNum=Ir.FrameNum+tmp;
						tmp=Ir.keylen%32;
						if(tmp!=0)Ir.FrameNum++;
						if(Ir.FrameNum>15)Ir.FrameNum=15;
						Ir.serial=0;
						//================================
						
						IT1_IR_LoadToWireless(Ir.addr);		
}

//=============================================================================================================
//��������ɾ��
void IT1_IR_DEL(void)
{
						uint8_t flag=0;
						Ir.addr=(RxDat.buf[5]&0x0f)-1;
	          Ir.cmd_key=RxDat.buf[8];
	          Ir.key_mode=RxDat.buf[9];
						if((RxDat.buf[6]>0)&&(RxDat.buf[7]==1))
						{
									if((Ir.cmd_key==0)||(Ir.cmd_key>56))
									{
												flag=0;
							    }
									else
									{
												Ir.serial=Dev.rand;
												IT1_IR_UnLoadToWireless(Ir.addr,Ir.cmd_key,Ir.key_mode);
												flag=1;
									}
						}
						
						if(flag==0)IT1_Load_RS485_Ack(OPT_ERR); 
						else       IT1_Load_RS485_Ack(OPT_OK); 
	
}
//=============================================================================================================
//����������ɺ���PC�����������,������߲���ʧ�ܣ�������ȫ�����0xff
void IT1_IrSetup_Report(uint8_t st)
{
				uint8_t i;
				_Frame *ps;
				
				ps=GetQueue_InQ(&RS485_Queue,8);
				if(ps==NULL)
				{
					return;
				}
				
				ps->buf[0]=0x06;							//
				ps->buf[1]=0x00;  						//
				ps->buf[2]=0x00;							//
				ps->buf[3]=0x00;	
				ps->buf[4]=Dev.addr[0];
				ps->buf[5]=0x10;		

				if(st==OPT_ERR)
				{
							ps->buf[6]=0;						//���ݳ���
							ps->buf[7]=Crc8CheckSum(ps->buf,7);		//CRC8??
							ps->cnt=8;			
				}
				else
				{
							ps->buf[6]=2+Ir.typelen+Ir.keylen+2;						//�ܵ����ݳ���,�ں������¼���
							ps->buf[7]=Ir.cmd_key;
	            ps->buf[8]=Ir.key_mode;
					    ps->buf[9]=Ir.typelen;
					    for(i=0;i<Ir.typelen;i++)
							{
										ps->buf[10+i]=Ir.typebuf[i];
							}
							ps->buf[10+Ir.typelen]=Ir.keylen;
							for(i=0;i<Ir.keylen;i++)
							{
										ps->buf[10+Ir.typelen+1+i]=Ir.keybuf[i];
							}
							ps->buf[10+Ir.typelen+1+Ir.keylen]=Crc8CheckSum(ps->buf,10+Ir.typelen+1+Ir.keylen);		//CRC8??
							ps->cnt=10+Ir.typelen+Ir.keylen+1+1;	
				}
				
		
				
				ps->flag=1;
				ps->resend=4;
				//ps->rand=100;
				ps->ackwait=30;					//
				EnQueueBuf(&RS485_Queue,8);
}

//=============================================================================================================
//����������ɺ���PC�����������,������߲���ʧ�ܣ�������ȫ�����0xff
void IT1_IrDel_Report(uint8_t index)
{
				_Frame *ps;
				
				ps=GetQueue_InQ(&RS485_Queue,8);
				if(ps==NULL)
				{
					return;
				}
				
				ps->buf[0]=0x26;							//
				ps->buf[1]=0x00;  						//
				ps->buf[2]=0x00;							//
				ps->buf[3]=0x00;	
				ps->buf[4]=Dev.addr[0];
				ps->buf[5]=0x80+index+1;;		

				ps->buf[6]=0x03;						//�ܵ����ݳ���,�ں������¼���
				ps->buf[7]=0x01;						//ɾ��	
				ps->buf[8]=Ir.cmd_key;
				ps->buf[9]=0x00;
				ps->buf[10]=Crc8CheckSum(ps->buf,10);		//CRC8??
				ps->cnt=11;	

				ps->flag=1;
				ps->resend=2;
				//ps->rand=100;
				ps->ackwait=30;					//
				EnQueueBuf(&RS485_Queue,8);
}

//=============================================================================================================
//������������װ������
void IT1_IR_LoadToWireless(uint8_t chl)
{
				uint8_t i,j,len,len1;
				_Frame *pws;		 
				//=====================================================================================================
	      if((Ir.wait==0)&&(Ir.flag==1))
	      {
					if(chl>=8)return;
					pws=GetQueue_InQ(&WirelessSend_Queue,chl);	
					if(pws==0)return;
					
					Ir.wait=1;																	//
						
					pws->buf[0]=0x30;														//������
					pws->buf[1]=0x40;														//������
					
					//2017.11.16�޸�0Ϊchl
					for(i=0;i<3;i++)pws->buf[2+i]=Ex_Table.Exchange[chl].wl_addr[i]; //Ŀ���ַ
					pws->buf[5]=Ex_Table.Exchange[chl].channel;
					
					for(i=0;i<4;i++)pws->buf[6+i]=Ir.TgAddr[i];											//54��ң������ַ	Դ��ַ
					
					pws->buf[10]=(Ir.FrameNum<<4);
            				Ir.serial++;if(Ir.serial>15)Ir.serial=15;					
					pws->buf[10]=pws->buf[10]+Ir.serial;
					
					pws->buf[11]=Ir.key_mode;									 //
					pws->buf[12]=Ir.cmd_key;									//��ֵ������
					
					//�·���������ң��������
					if(Ir.curtype==0)
					{
						len=Ir.typelen;											
						if((len-Ir.curps)>32)
						{
								pws->buf[13]=32;
							  	for(i=0;i<32;i++)pws->buf[14+i]=Ir.typebuf[Ir.curps+i];
								Ir.curps=Ir.curps+32;
						}
						else
						{
								pws->buf[13]=len-Ir.curps;
							  len1=pws->buf[13];
							 	 for(i=0;i<pws->buf[13];i++)pws->buf[14+i]=Ir.typebuf[Ir.curps+i];
							  	j=0;
							  	for(i=len1;i<32;i++){pws->buf[14+len1+j]=0xff;j++;}
								Ir.curps=0;
							  	Ir.curtype=1;
						}
					}
					//�·���������ң��������
					else 
					{
								len=Ir.keylen;										
								if((len-Ir.curps)>32)
								{
									  pws->buf[13]=32;
									  for(i=0;i<32;i++)pws->buf[14+i]=Ir.keybuf[Ir.curps+i];
									  Ir.curps=Ir.curps+32;
								}
								else
								{
									 pws->buf[13]=len-Ir.curps;
									  len1=pws->buf[13];
									  for(i=0;i<pws->buf[13];i++)pws->buf[14+i]=Ir.keybuf[Ir.curps+i];
									  j=0;
									  for(i=len1;i<32;i++){pws->buf[14+len1+j]=0xff;j++;}
										Ir.curps=0;
									  Ir.curtype=0;
									  Ir.flag=0;
								}
								//bit7 bit6 ���ֵ�ǰ������ң�����ͻ�ң��������
								pws->buf[13]|=IR_TYPE_KEY;
					}
						
					pws->buf[46]=Crc8CheckSum(pws->buf,46);
					pws->buf[47]=SumCheck(pws->buf,46);
					
					pws->cnt=48;				   //�����ܵ��ֽ���
					pws->flag=1;
					pws->resend=2;				 //�ظ��ϱ�����	
					pws->rand=0;				   //��һ�ο��мȷ���
					pws->ackwait=0;	
					EnQueueBuf(&WirelessSend_Queue,chl);		
						
			}
			//=============================================================================================
}

//=====================================================================================================
//=============================================================================================================
//������������װ������
void IT1_IR_UnLoadToWireless(uint8_t chl,uint8_t key,uint8_t mode)
{
						uint8_t  i;
						_Frame *pws;

	          if(chl>=8)return;
	
						pws=GetQueue_InQ(&WirelessSend_Queue,chl);	
					  if(pws==0)return;
					
					  Ir.wait=1;																	//
						
						pws->buf[0]=0x10;														//������
						pws->buf[1]=MTD_IR_DEL;														//������
					
					  //2017.11.16����ͨ�����޸�0Ϊchl
						for(i=0;i<3;i++)pws->buf[2+i]=Ex_Table.Exchange[chl].wl_addr[i]; //Ŀ���ַ
	          pws->buf[5]=Ex_Table.Exchange[chl].channel;
						for(i=0;i<4;i++)pws->buf[6+i]=Ir.TgAddr[i];											//54��ң������ַ	Դ��ַ
						
					  pws->buf[10]=Ir.serial;
					
					  pws->buf[11]=mode;									  //
					  pws->buf[12]=key;										//��ֵ������
					
						pws->buf[13]=0x00;							//��2 bit Ϊ����
						
						pws->buf[14]=Crc8CheckSum(pws->buf,14);
						pws->buf[15]=SumCheck(pws->buf,14);
						
						pws->cnt=16;				   //�����ܵ��ֽ���
						pws->flag=1;
						pws->resend=1;				 //�ظ��ϱ�����	
						pws->rand=0;				   //��һ�ο��мȷ���
						pws->ackwait=0;	
						EnQueueBuf(&WirelessSend_Queue,chl);	
					}
//=====================================================================================================
//========================================================================================================
//RS485��Ե��ѯ״̬��ֱ��Ӧ����0x04����
void IT1_ExTable_Stu_Ack(uint8_t index)
{
			 uint8_t chl;
	    chl=index&0x0f;
	    if((chl==0)||(chl>8))return;
	    chl=chl-1;
	
			TxDat.buf[0]=0x04;												//֡����-Ӧ����
			TxDat.buf[1]=0x00;//TxDat.buf[1]=(Master_Address>>8)&0x00ff;	//Ŀ���ַH
			TxDat.buf[2]=Master_Address;							//Ŀ���ַL

			TxDat.buf[3]=0X00;												//֡���к�

			TxDat.buf[4]=Dev.addr[0];								  //����ID
	    TxDat.buf[5]=0x80+chl+1;
	    TxDat.buf[6]=0x01;
	    //�Ǵ����� �ϱ� ʹ��״̬

			if(Ex_Table.Exchange[chl].type>=EX_DEV_KEY)
			{
					TxDat.buf[7]=0x01;
					TxDat.buf[8]=Crc8CheckSum(TxDat.buf,8);				//CRC8У��
					TxDat.cnt=9;																	//�����ܵ��ֽ���
					TxDat.f_tx=1;
			}
			else if(Ex_Table.Exchange[chl].type==EX_DEV_LIGHT_ADJ)//20170628 aaa
			{
					TxDat.buf[7]=0x01;														//�����һ������
				  TxDat.buf[8]=0x00;														//�����һ������
				  TxDat.buf[9]=Ex_Table.stu[chl];								//�����һ������
				
				  TxDat.buf[10]=Crc8CheckSum(TxDat.buf,10);			//CRC8У��
					TxDat.cnt=11;																	//�����ܵ��ֽ���
					TxDat.f_tx=1;
			}
			else
			{
					TxDat.buf[7]=Ex_Table.stu[chl];
				  TxDat.buf[8]=Crc8CheckSum(TxDat.buf,8);				//CRC8У��
					TxDat.cnt=9;																	//�����ܵ��ֽ���
					TxDat.f_tx=1;
			}
	//=====================================================================================	
}
//�����޸�
void IT1_Function_Modfiy(void)
{
				uint8_t ret=0,i,flag;
				//�޸ĵ�ַ
				if(RxDat.buf[7]==0xaa)	                           
				{
								if((RxDat.buf[10]==0x00)||(RxDat.buf[10]==0xff))
								{
											ret++;
											for(i=0;i<RxDat.cnt;i++)RxDat.buf[i]=0x00;
											RxDat.cnt=0;	
								}
								else
								{
											IT1_AddressSetting();
								}
				}
				//�޸�����
				if(RxDat.buf[8]==0xaa)
				{
									flag=IT1_Modify_Type();
									ret=ret+flag;
				} 
				//�޸�ͨ��
				if(RxDat.buf[9]==0xaa)
				{
										//�����õ�
					         //if(RxDat.buf[13]!=0x01)ret++;
				} 
				//����ǹ㲥�޸ģ���Ӧ���ַ
				if((RxDat.buf[2]==0xff)&&(RxDat.buf[1]==0xff))
				{
					
				}
				else
				{
							if(ret!=0x00)IT1_Load_RS485_Ack(OPT_ERR_SETUP);
							else           IT1_Load_RS485_Ack(OPT_OK);
				}
	
}
//======================================================================================================
//����ѧϰ54��ң����
void IT1_Learn54Key(void)
{
				Dev.stu=DEV_SETUP;
				Dev.learn_time=LEARN_TIME_MAX;
				Dev.encode_time=0;

				//��Ҫ�����߷��ͣ�ֻ�ȴ�ң��������
				Exchangebuf.type= EX_KEY_54KEY;                                    //�豸����
				Exchangebuf.addr= 0;;
				Exchangebuf.channel=0;
				Exchangebuf.wl_addr[0]=	0;				                  			//�ȴ�����ң�����ĵ�ַ
				Exchangebuf.wl_addr[1]= 0;  		                      		//
				Exchangebuf.wl_addr[2]= 0;  

				Led_set(1,0xff,1,0xff,2);	
}
//================================================================================================
//����ѧϰ��
void IT1_LearnLock(void)
{
				//uint8_t i;
				Dev.stu=DEV_SETUP;
				Dev.learn_time=LEARN_TIME_MAX;
				Dev.encode_time=0;

				//��Ҫ�����߷��ͣ�ֻ�ȴ�ң��������

				lock.index=0;
	
				Led_set(1,0xff,1,0xff,2);	
}


//===========================================================================================================
//�豸�����ϱ�
// ��Ҫ��LED״̬
void IT1_DevPara_Report(unsigned char chl,unsigned char addr,unsigned char len)
{

		_Frame *ps;	
	  uint8_t /*buf[8],*/i,index;
	  //uint8_t tt;
	
		ps=GetQueue_InQ(&RS485_Queue,8);
		//���������
		if(ps==NULL)
		{
			return;
		}
		chl=chl&0x0f;										//ת����ͨ��
		if((chl>8)||(chl==0))return;
		index=chl-1;
		
		ps->buf[0]=0x20;												//֡����
		ps->buf[1]=0x00;  											//ͬ����
		ps->buf[2]=Master_Address;							//Ŀ���ַ
		ps->buf[3]=0x00;												//ͬ����
		ps->buf[4]=Dev.addr[0];									//Դ��ַ
		ps->buf[5]=0x80+chl;												//�豸ͨ��������
			

		if((Ex_Table.Exchange[index].type==EX_KEY_54KEY)||(Ex_Table.Exchange[index].type==EX_KEY_42KEY))
		{
				ps->buf[6]=0x09;	
			  ps->buf[7]=0x01;
			  ps->buf[8]=0x07;
			  for(i=0;i<7;i++)ps->buf[9+i]=Pannel.item[index].irflagbuf[i];
				ps->buf[16]=Crc8CheckSum(ps->buf,16);		//CRC8У��
				ps->cnt=17;				
		}
		else
		{
				ps->buf[6]=0x09;										 //���ݳ���
				ps->buf[7]=addr;										 //������ʼ��ַ			
				ps->buf[8]=0x07;										 //��������
				
				ps->buf[9]=Ex_Table.Exchange[index].type;
				ps->buf[10]=Ex_Table.Exchange[index].addr;
				ps->buf[11]=Ex_Table.Exchange[index].stu;
				ps->buf[12]=Ex_Table.Exchange[index].wl_addr[0];
				ps->buf[13]=Ex_Table.Exchange[index].wl_addr[1];
				ps->buf[14]=Ex_Table.Exchange[index].wl_addr[2];
				ps->buf[15]=Ex_Table.Exchange[index].channel;
				
				
				ps->buf[16]=Crc8CheckSum(ps->buf,16);		//CRC8У��
				ps->cnt=17;		
			
		}
	  ps->flag=1;
		ps->resend=4;
		ps->rand=100;
		ps->ackwait=30;	
		EnQueueBuf(&RS485_Queue,8);
}

//======================================================================================================
//���·�ʽע��ʱ��ȡ��ÿ��ͨ����λ�ú����ͣ��������ͺϲ�
#ifdef USE_NEWREGISTER
void IT1_Register_Refresh(void)
{
				uint8_t index,i,buf[3],rt;
				index=0;
	      index=0;
				//����
				for(i=0;i<8;i++){NewRegType.item[i].bitpos=0;NewRegType.item[i].type=0;NewRegType.num=0;}
				//ȡ��ע����Ϣ
				while(index<8)
				{
						rt=IT1_Wireless2RS485(Ex_Table.Exchange[index].type,&buf[0]);
						if(rt==RE_OK)
						{
									NewRegType.item[index].type=(uint16_t)(buf[0]<<8)+(uint16_t)(buf[1]);
									NewRegType.item[index].bitpos |=(1<<index);
							    NewRegType.num++;
						}
						else	//���ͨ�����У���Ϊ0xb140 20170628
						{
									NewRegType.item[index].type=0xb140;
									NewRegType.item[index].bitpos |=(1<<index);
							    NewRegType.num++;
						}
						index++;
				}
				//�ϲ�ע����Ϣ
				index=0;
				while(index<7)
				{
						if(NewRegType.item[index].bitpos!=0)
						{
								for(i=index+1;i<8;i++)
								{
										if(NewRegType.item[index].type==NewRegType.item[i].type)
										{
												NewRegType.item[index].bitpos=NewRegType.item[index].bitpos | NewRegType.item[i].bitpos;
											  NewRegType.item[i].bitpos=0;
										}
								}
						}
						index++;
				}
}
#endif
//===================================================================================
//��0x27����
void IT1_Dev_StuLoad(uint8_t index)
{

				_Frame *ps;
				uint8_t i,num;
	      uint8_t lin,row;
				ps=GetQueue_InQ(&RS485_Queue,index);
				//���������
				if(ps==NULL){return;}
				
				if((Dev.type&0xf000)==0xb000)
				{
						if((Ex_Table.Exchange[index].type==EX_KEY_54KEY)||(Ex_Table.Exchange[index].type==EX_KEY_42KEY))
						{
							  if(Ex_Table.Exchange[index].type==EX_KEY_54KEY)num=7;
							  else                                           num=6;
								ps->buf[0] =0x27;											//֡����,���豸��������
								ps->buf[1] =0x00;
								ps->buf[2] =0x00;
								ps->buf[3] =Dev.serial++;							//֡���к�
								ps->buf[4] =Dev.addr[0];							//����ID
								ps->buf[5] =0x80+index+1;											//�豸��ͨ���͵�ǰͨ��
								ps->buf[6] =3+num;						      	//���ݳ���
								ps->buf[7]=0;													//LED״̬						
								ps->buf[8] =0;							  				//��ǰ����  												
							  for(i=0;i<num;i++)ps->buf[9+i]=Pannel.item[index].onoffbuf[i];
								ps->buf[9+num]=Crc8CheckSum(ps->buf,9+num);		//CRC8У��
								ps->cnt=10+num;					                 	//�����ܵ��ֽ���
						}
						else if((Ex_Table.Exchange[index].type>=EX_KEY_SCENE1)&&(Ex_Table.Exchange[index].type<=EX_KEY_FULL1))
						{
								ps->buf[0] =0x27;											//֡����,���豸��������
								ps->buf[1] =0x00;
								ps->buf[2] =0x00;
								ps->buf[3] =Dev.serial++;							//֡���к�
								ps->buf[4] =Dev.addr[0];							//����ID
								ps->buf[5] =0x80+index+1;							//�豸��ͨ���͵�ǰͨ��

								ps->buf[6] =3;						      			//���ݳ���
								ps->buf[7]=0;													//LED״̬
								
								ps->buf[8] =0;							  				//��ǰ����  
								
								ps->buf[9] =Pannel.item[index].onoffbuf[0];
							
								
								ps->buf[10]=Crc8CheckSum(ps->buf,10);		//CRC8У��
								ps->cnt=11;		
						}
						else if((Ex_Table.Exchange[index].type==EX_DEV_SUN)||(Ex_Table.Exchange[index].type==EX_DEV_SUN1))
						{
						    ps->buf[0] =0x04;											//֡����,���豸��������
								ps->buf[1] =0x00;
								ps->buf[2] =0x00;
								ps->buf[3] =Dev.serial++;							//֡���к�
								ps->buf[4] =Dev.addr[0];							//����ID
								ps->buf[5] =0x80+index+1;											//�豸��ͨ���͵�ǰͨ��
								ps->buf[6] =01;						      			//���ݳ���
		            ps->buf[7]=Pannel.item[index].onoffbuf[0]&0x01;
								ps->buf[8]=Crc8CheckSum(ps->buf,8);		//CRC8У��
								ps->cnt=9;					                 	//�����ܵ��ֽ���             
						}				
						ps->flag=1;
						ps->resend=4;
						ps->rand=100;
						ps->ackwait=30;					//�ȴ�Ӧ��ʱ�����300mS
						EnQueueBuf(&RS485_Queue,index);
			}
			//��ָ����
			else if((Dev.type&0xf000)==0x3000)
			{

						ps->buf[0] =0x04;											//֡����,���豸��������
						ps->buf[1] =0x00;
						ps->buf[2] =0x00;
						ps->buf[3] =Dev.serial++;							//֡���к�
						ps->buf[4] =Dev.addr[0];							//����ID
						ps->buf[5] =0x10+index;											//�豸��ͨ���͵�ǰͨ��
						ps->buf[6] =01;						      			//���ݳ���
				
						//ps->buf[7]=Pannel.item[index].onoffbuf[0]&0x01;
				
				    
				    lin=(index-1)/8;
						row=(index-1)%8;
				    if((Pannel.item[0].onoffbuf[lin]&(0x01<<row))!=0x00)ps->buf[7]=0x01;
				    else                                                ps->buf[7]=0x00;
				
						ps->buf[8]=Crc8CheckSum(ps->buf,8);		//CRC8У��
						ps->cnt=9;					                 	//�����ܵ��ֽ���             			
						ps->flag=1;
						ps->resend=4;
						ps->rand=100;
						ps->ackwait=30;					//�ȴ�Ӧ��ʱ�����300mS
						EnQueueBuf(&RS485_Queue,index);
			}
}


//==============================================================================================
//��ʱ���ͺ���.ÿ10mS����һ�� 
void IT1_Dlytime_Handle(void)
{
		 uint8_t i;
	 
	   for(i=0;i<CHL_NUM;i++)						//ͨ��0-������������ͨ��1-������ֹͣ
		 {
			    //================================================================================================
					//��ʱ����
					if(1==bsp_CheckTimer(i))
					{
							if(DlyTable.item[i].flag==1)
							{
										if((Dev.type&0xf000)==0xb000)
										{
												IT1_Dev_StuLoad(i);
										}
										else if((Dev.type&0xf000)==0x3000)
										{
											  
											  IT1_Lock_DataLoad(i,DlyTable.item[i].index,DlyTable.item[i].dat);
										}
							}
					}				
		 }
	
	
}

//=============================================================================================================
/***************************end  of  code ***************************/
