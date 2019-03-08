#include "can.h"
#include "led.h"
#include "delay.h"
#include "usart.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//CAN驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/11
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
//CAN初始化
//tsjw:重新同步跳跃时间单元.范围:1~3; CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
//tbs2:时间段2的时间单元.范围:1~8;
//tbs1:时间段1的时间单元.范围:1~16;	  CAN_BS1_1tq ~CAN_BS1_16tq
//brp :波特率分频器.范围:1~1024;(实际要加1,也就是1~1024) tq=(brp)*tpclk1
//注意以上参数任何一个都不能设为0,否则会乱.
//波特率=Fpclk1/((tsjw+tbs1+tbs2)*brp);
//mode:0,普通模式;1,回环模式;
//Fpclk1的时钟在初始化的时候设置为36M,如果设置CAN_Normal_Init(1,8,7,5,1);
//则波特率为:36M/((1+8+7)*5)=450Kbps
//返回值:0,初始化OK;
//    其他,初始化失败;


u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{

	GPIO_InitTypeDef GPIO_InitStructure; 
	CAN_InitTypeDef        CAN_InitStructure;
 	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
#if CAN_RX0_INT_ENABLE 
   	NVIC_InitTypeDef  NVIC_InitStructure;
#endif

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能PORTA时钟	                   											 

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//使能CAN1时钟	

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽
    GPIO_Init(GPIOA, &GPIO_InitStructure);		//初始化IO
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化IO
	  
 	//CAN单元设置
 	CAN_InitStructure.CAN_TTCM=DISABLE;						 //非时间触发通信模式  //
 	CAN_InitStructure.CAN_ABOM=DISABLE;						 //软件自动离线管理	 //
  	CAN_InitStructure.CAN_AWUM=DISABLE;						 //睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)//
  	CAN_InitStructure.CAN_NART=ENABLE;						 	//禁止报文自动传送 //
  	CAN_InitStructure.CAN_RFLM=DISABLE;						 //报文不锁定,新的覆盖旧的 // 
  	CAN_InitStructure.CAN_TXFP=DISABLE;						 //优先级由报文标识符决定 //
  	CAN_InitStructure.CAN_Mode= mode;	         //模式设置： mode:0,普通模式;1,回环模式; //
  	//设置波特率
  	CAN_InitStructure.CAN_SJW=tsjw;				//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位  CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=tbs1; //Tbs1=tbs1+1个时间单位CAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=tbs2;//Tbs2=tbs2+1个时间单位CAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=brp;            //分频系数(Fdiv)为brp+1	//
  	CAN_Init(CAN1, &CAN_InitStructure);            // 初始化CAN1 

 	CAN_FilterInitStructure.CAN_FilterNumber=0;	  //过滤器0
 	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32位 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32位ID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32位MASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//过滤器0关联到FIFO0
 	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //激活过滤器0

  	CAN_FilterInit(&CAN_FilterInitStructure);//滤波器初始化
#if CAN_RX0_INT_ENABLE
	
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0消息挂号中断允许.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // 主优先级为1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // 次优先级为0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
#endif
	return 0;
}   

//can发送一组数据(标准帧,数据帧)	
//len:数据长度(最大为8)				     
//msg:数据指针,最大为8个字节.
//返回值:0,成功;
//		 其他,失败;
u8 Can_Send_Msg(struct cAN_FRAME frameBuf)
{	
  u8 mbox;
  u16 i=0;
  CanTxMsg TxMessage;
  TxMessage.StdId=frameBuf.frameId;					 // 标准标识符
  TxMessage.ExtId=0;				 							 // 设置扩展标示符为（29位）
  TxMessage.IDE=CAN_ID_STD;			           // 使用标准标识符
	
	if(frameBuf.frameType==0) 	TxMessage.RTR=CAN_RTR_DATA;		 // 消息类型为数据帧
	else	                      TxMessage.RTR=CAN_RTR_Remote;		 //消息类型为远程帧

  TxMessage.DLC=frameBuf.datLen;							 // 发送数据的长度
	
  for(i=0;i<frameBuf.datLen;i++)
	{
		TxMessage.Data[i]=frameBuf.dat[i];				 // 第一帧信息 
	}		
	mbox= CAN_Transmit(CAN1, &TxMessage); 
  
	i=0;
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF)) i++;	//等待发送结束
  if(i>=0XFFF)return 1;
	
  return 0;		

}
//can口接收数据查询
//buf:数据缓存区;	 
//返回值:0,无数据被收到;
//		 其他,接收的数据长度;
u8 Can_Receive_Msg(struct cAN_FRAME *frameBuf)
{	
	u8 i;
	CanRxMsg RxMessage;
	if( CAN_MessagePending(CAN1,CAN_FIFO0)==0) return 0;		//没有接收到数据,直接退出 

	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//读取数据
	
	frameBuf->frameId=RxMessage.StdId;
	frameBuf->datLen=RxMessage.DLC;
	for(i=0;i<RxMessage.DLC;i++)
		 	frameBuf->dat[i]=RxMessage.Data[i];  
	return 1;	
}


//u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
//返回0时表示初始初始化成功,返回1表示波特率设置不成功，返回2表示初始化设备不成功
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

#if CAN_RX0_INT_ENABLE	//使能RX0中断
//中断服务函数			    
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	struct cAN_FRAME canBuf;
	Can_Receive_Msg(&canBuf);
	can_DatReceive(0,canBuf);
}
#endif









