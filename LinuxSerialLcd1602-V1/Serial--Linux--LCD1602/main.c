#include<reg51.h>
#include"lcd.h"

#define uint unsigned int 
#define uchar unsigned char

unsigned char rFlag=0;
unsigned char dataFlag=0;
unsigned char showFlag=0;


unsigned char rxLength=0;
unsigned char rxBuf[20];//={1,0,12,0x30,0x31,0x34,0x34,0x33,0x43,0x54,0x53,0x65,0x32,0x45,0x56};



void UsartConfiguration()
{
	SCON=0X50;			//����Ϊ������ʽ1
	TMOD=0X20;			//���ü�����������ʽ2
	PCON=0X00;			//�����ʼӱ�
	TH1=0XF3;				//��������ʼֵ���ã�ע�Ⲩ������4800��
	TL1=0XF3;
	ES=1;						//�򿪽����ж�
	EA=1;						//�����ж�
	TR1=1;					//�򿪼�����
}


void main()
{
	unsigned char i=0;
	UsartConfiguration();
	LCD1602_Init();
	showFlag=0;
	
	while(1)
	{
		if(showFlag==1)
		{
			if(rxBuf[0]==1)
			{
					LCD1602_WriteCom(0x80+rxBuf[1]);
			}
			else
			{
				LCD1602_WriteCom(0x80+0x40+rxBuf[1]);
			}
			
			if(rxBuf[2]>16)
			{
				rxBuf[2]=16;
			}
			for(i=3;i<3+rxBuf[2];i++)
			{
				LCD1602_WriteData(rxBuf[i]);
			}			
			rxLength=0;
			showFlag=0;		
		}
		
	}
}



void Usart() interrupt 4
{
	unsigned char receiveData=0;
	receiveData=SBUF;//��ȥ���յ�������
	RI = 0;//��������жϱ�־λ
	

	if(showFlag==0)
	{
		if(receiveData==0xfa)
		{
			dataFlag=0;
			showFlag=1;
		}
		
		if(dataFlag==1)
		{
			rxBuf[rxLength]=receiveData;
			SBUF=rxBuf[rxLength];
			while(!TI);		  //�ȴ������������
			TI=0;			  //���������ɱ�־λ	
			
			rxLength++;
		}
		
		if(receiveData==0xf5)
		{
			rxLength=0;
			dataFlag=1;
		}
	}
}

