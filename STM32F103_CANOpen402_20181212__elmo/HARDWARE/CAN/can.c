#include "can.h"
#include "led.h"
#include "delay.h"
#include "usart.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//CAN���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/11
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
//CAN��ʼ��
//tsjw:����ͬ����Ծʱ�䵥Ԫ.��Χ:1~3; CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
//tbs2:ʱ���2��ʱ�䵥Ԫ.��Χ:1~8;
//tbs1:ʱ���1��ʱ�䵥Ԫ.��Χ:1~16;	  CAN_BS1_1tq ~CAN_BS1_16tq
//brp :�����ʷ�Ƶ��.��Χ:1~1024;(ʵ��Ҫ��1,Ҳ����1~1024) tq=(brp)*tpclk1
//ע�����ϲ����κ�һ����������Ϊ0,�������.
//������=Fpclk1/((tsjw+tbs1+tbs2)*brp);
//mode:0,��ͨģʽ;1,�ػ�ģʽ;
//Fpclk1��ʱ���ڳ�ʼ����ʱ������Ϊ36M,�������CAN_Normal_Init(1,8,7,5,1);
//������Ϊ:36M/((1+8+7)*5)=450Kbps
//����ֵ:0,��ʼ��OK;
//    ����,��ʼ��ʧ��;


u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{

	GPIO_InitTypeDef GPIO_InitStructure; 
	CAN_InitTypeDef        CAN_InitStructure;
 	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
#if CAN_RX0_INT_ENABLE 
   	NVIC_InitTypeDef  NVIC_InitStructure;
#endif

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��PORTAʱ��	                   											 

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//ʹ��CAN1ʱ��	

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);		//��ʼ��IO
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��IO
	  
 	//CAN��Ԫ����
 	CAN_InitStructure.CAN_TTCM=DISABLE;						 //��ʱ�䴥��ͨ��ģʽ  //
 	CAN_InitStructure.CAN_ABOM=DISABLE;						 //����Զ����߹���	 //
  	CAN_InitStructure.CAN_AWUM=DISABLE;						 //˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)//
  	CAN_InitStructure.CAN_NART=ENABLE;						 	//��ֹ�����Զ����� //
  	CAN_InitStructure.CAN_RFLM=DISABLE;						 //���Ĳ�����,�µĸ��Ǿɵ� // 
  	CAN_InitStructure.CAN_TXFP=DISABLE;						 //���ȼ��ɱ��ı�ʶ������ //
  	CAN_InitStructure.CAN_Mode= mode;	         //ģʽ���ã� mode:0,��ͨģʽ;1,�ػ�ģʽ; //
  	//���ò�����
  	CAN_InitStructure.CAN_SJW=tsjw;				//����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ  CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=tbs1; //Tbs1=tbs1+1��ʱ�䵥λCAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=tbs2;//Tbs2=tbs2+1��ʱ�䵥λCAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=brp;            //��Ƶϵ��(Fdiv)Ϊbrp+1	//
  	CAN_Init(CAN1, &CAN_InitStructure);            // ��ʼ��CAN1 

 	CAN_FilterInitStructure.CAN_FilterNumber=0;	  //������0
 	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32λ 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32λID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32λMASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//������0������FIFO0
 	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //���������0

  	CAN_FilterInit(&CAN_FilterInitStructure);//�˲�����ʼ��
#if CAN_RX0_INT_ENABLE
	
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0��Ϣ�Һ��ж�����.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // �����ȼ�Ϊ1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // �����ȼ�Ϊ0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
#endif
	return 0;
}   

//can����һ������(��׼֡,����֡)	
//len:���ݳ���(���Ϊ8)				     
//msg:����ָ��,���Ϊ8���ֽ�.
//����ֵ:0,�ɹ�;
//		 ����,ʧ��;
u8 Can_Send_Msg(struct cAN_FRAME frameBuf)
{	
  u8 mbox;
  u16 i=0;
  CanTxMsg TxMessage;
  TxMessage.StdId=frameBuf.frameId;					 // ��׼��ʶ��
  TxMessage.ExtId=0;				 							 // ������չ��ʾ��Ϊ��29λ��
  TxMessage.IDE=CAN_ID_STD;			           // ʹ�ñ�׼��ʶ��
	
	if(frameBuf.frameType==0) 	TxMessage.RTR=CAN_RTR_DATA;		 // ��Ϣ����Ϊ����֡
	else	                      TxMessage.RTR=CAN_RTR_Remote;		 //��Ϣ����ΪԶ��֡

  TxMessage.DLC=frameBuf.datLen;							 // �������ݵĳ���
	
  for(i=0;i<frameBuf.datLen;i++)
	{
		TxMessage.Data[i]=frameBuf.dat[i];				 // ��һ֡��Ϣ 
	}		
	mbox= CAN_Transmit(CAN1, &TxMessage); 
  
	i=0;
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF)) i++;	//�ȴ����ͽ���
  if(i>=0XFFF)return 1;
	
  return 0;		

}
//can�ڽ������ݲ�ѯ
//buf:���ݻ�����;	 
//����ֵ:0,�����ݱ��յ�;
//		 ����,���յ����ݳ���;
u8 Can_Receive_Msg(struct cAN_FRAME *frameBuf)
{	
	u8 i;
	CanRxMsg RxMessage;
	if( CAN_MessagePending(CAN1,CAN_FIFO0)==0) return 0;		//û�н��յ�����,ֱ���˳� 

	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//��ȡ����
	
	frameBuf->frameId=RxMessage.StdId;
	frameBuf->datLen=RxMessage.DLC;
	for(i=0;i<RxMessage.DLC;i++)
		 	frameBuf->dat[i]=RxMessage.Data[i];  
	return 1;	
}


//u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
//����0ʱ��ʾ��ʼ��ʼ���ɹ�,����1��ʾ���������ò��ɹ�������2��ʾ��ʼ���豸���ɹ�
u8 CAN_deviceInit(int rate,u8 mode)
{
	u8 tsjw;
	u8 tbs2;
	u8 tbs1;
	u16 brp;
	
	switch(rate)
	{
			case 1000:
				tsjw=CAN_SJW_1tq;//1-3
				tbs2=CAN_BS2_8tq;//1-8
				tbs1=CAN_BS1_9tq;//1-16
				brp=2;
				break;
			case 800:
				tsjw=CAN_SJW_1tq;//1-3
				tbs2=CAN_BS2_4tq;//1-8
				tbs1=CAN_BS1_4tq;//1-16
				brp=5;
				break;
			case 500:
				tsjw=CAN_SJW_1tq;//1-3
				tbs2=CAN_BS2_8tq;//1-8
				tbs1=CAN_BS1_9tq;//1-16
				brp=4;
				break;
			case 250:
				tsjw=CAN_SJW_1tq;//1-3
				tbs2=CAN_BS2_8tq;//1-8
				tbs1=CAN_BS1_9tq;//1-16
				brp=8;
				break;
			case 125:
				tsjw=CAN_SJW_1tq;//1-3
				tbs2=CAN_BS2_8tq;//1-8
				tbs1=CAN_BS1_9tq;//1-16
				brp=16;
				break;
			case 50:
				tsjw=CAN_SJW_1tq;//1-3
				tbs2=CAN_BS2_8tq;//1-8
				tbs1=CAN_BS1_9tq;//1-16
				brp=40;
				break;
			case 20:
				tsjw=CAN_SJW_1tq;//1-3
				tbs2=CAN_BS2_8tq;//1-8
				tbs1=CAN_BS1_9tq;//1-16
				brp=100;
				break;
			case 10:
				tsjw=CAN_SJW_1tq;//1-3
				tbs2=CAN_BS2_8tq;//1-8
				tbs1=CAN_BS1_9tq;//1-16
				brp=200;
				break;
			default:
				return 1;
		
	}
	if(CAN_Mode_Init(tsjw,tbs2,tbs1,brp,mode))
	{
		return 2;
	}

	return 0;
}

#if CAN_RX0_INT_ENABLE	//ʹ��RX0�ж�
//�жϷ�����			    
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	struct cAN_FRAME canBuf;
	Can_Receive_Msg(&canBuf);
	can_DatReceive(0,canBuf);
}
#endif









