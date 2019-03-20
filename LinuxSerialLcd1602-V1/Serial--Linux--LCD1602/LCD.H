#ifndef __LCD_H_
#define __LCD_H_
/**********************************
当使用的是4位数据传输的时候定义
**********************************/
//#define LCD1602_4PINS

/**********************************
包含头文件
**********************************/
#include<reg51.h>

#define uint unsigned int 
#define uchar unsigned char
/**********************************
PIN口定义
**********************************/
#define LCD1602_DATAPINS P0
sbit LCD1602_E=P3^4;
sbit LCD1602_RW=P3^6;
sbit LCD1602_RS=P3^5;
/**********************************
函数声明
**********************************/
/*在51单片机12MHZ时钟下的延时函数*/
void Delay1us(uint a);
/*LCD1602写入8位命令子函数*/
void LCD1602_WriteCom(uchar com);
/*LCD1602写入8位数据子函数*/	
void LCD1602_WriteData(uchar dat)	;
/*LCD1602初始化子程序*/		
void LCD1602_Init();						  

#endif
