#ifndef __LCD_H_
#define __LCD_H_
/**********************************
��ʹ�õ���4λ���ݴ����ʱ����
**********************************/
//#define LCD1602_4PINS

/**********************************
����ͷ�ļ�
**********************************/
#include<reg51.h>

#define uint unsigned int 
#define uchar unsigned char
/**********************************
PIN�ڶ���
**********************************/
#define LCD1602_DATAPINS P0
sbit LCD1602_E=P3^4;
sbit LCD1602_RW=P3^6;
sbit LCD1602_RS=P3^5;
/**********************************
��������
**********************************/
/*��51��Ƭ��12MHZʱ���µ���ʱ����*/
void Delay1us(uint a);
/*LCD1602д��8λ�����Ӻ���*/
void LCD1602_WriteCom(uchar com);
/*LCD1602д��8λ�����Ӻ���*/	
void LCD1602_WriteData(uchar dat)	;
/*LCD1602��ʼ���ӳ���*/		
void LCD1602_Init();						  

#endif