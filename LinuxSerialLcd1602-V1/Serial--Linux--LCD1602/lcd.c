#include"lcd.h"

/*******************************************************************************
* 函数名         : Delay1us
* 函数功能		   : 延时函数，延时1us
* 输入           : a
* 输出         	 : 无
*******************************************************************************/

void Delay1us(uint a)
{
	uint b,c;
	for(c=a;c>0;c--)
		for(b=110;b>0;b--);
}
/*******************************************************************************
* 函数名         : LCD1602_WriteCom
* 函数功能		   : 向LCD写入一个字节的命令
* 输入           : com
* 输出         	 : 无
*******************************************************************************/
#ifndef 	LCD1602_4PINS	 //当没有定义这个LCD1602_4PINS时
void LCD1602_WriteCom(uchar com)	  //写入命令
{
	LCD1602_E=0;
	LCD1602_RS=0;
	LCD1602_RW=0;
	LCD1602_DATAPINS=com;
	Delay1us(10);
	LCD1602_E=1;
	Delay1us(10);
	LCD1602_E=0;
}
#else 
void LCD1602_WriteCom(uchar com)	  //写入命令
{
	LCD1602_E=0;
	LCD1602_RS=0;
	LCD1602_RW=0;
	LCD1602_DATAPINS=com;	//由于4位的接线是接到P0口的高四位，所以传送高四位不用改
	Delay1us(10);
	LCD1602_E=1;
	Delay1us(10);
	LCD1602_E=0;
	Delay1us(10);
	LCD1602_DATAPINS=com<<4;
	Delay1us(10);
	LCD1602_E=1;
	Delay1us(10);
	LCD1602_E=0;
}
#endif
/*******************************************************************************
* 函数名         : LCD1602_WriteData
* 函数功能		   : 向LCD写入一个字节的数据
* 输入           : dat
* 输出         	 : 无
*******************************************************************************/		   
#ifndef 	LCD1602_4PINS		   
void LCD1602_WriteData(uchar dat)			//写入数据
{
	LCD1602_E=0;
	LCD1602_RS=1;
	LCD1602_RW=0;
	LCD1602_DATAPINS=dat;
	Delay1us(10);
	LCD1602_E=1;
	Delay1us(10);
	LCD1602_E=0;
}
#else
void LCD1602_WriteData(uchar dat)			//写入数据
{
	LCD1602_E=0;
	LCD1602_RS=1;
	LCD1602_RW=0;
	LCD1602_DATAPINS=dat;	//由于4位的接线是接到P0口的高四位，所以传送高四位不用改
	Delay1us(10);
	LCD1602_E=1;
	Delay1us(10);
	LCD1602_E=0;
	LCD1602_DATAPINS=dat<<4;
	Delay1us(10);
	LCD1602_E=1;
	Delay1us(10);
	LCD1602_E=0;
}
#endif
/*******************************************************************************
* 函数名         : LCD1602_Init()
* 函数功能		   : 初始化LCD屏
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/		   
#ifndef		LCD1602_4PINS
void LCD1602_Init()						  //LCD初始化子程序
{
 	LCD1602_WriteCom(0x38);  //开显示
	LCD1602_WriteCom(0x0c);  //开显示不显示光标
	LCD1602_WriteCom(0x06);  //写一个指针加1
	LCD1602_WriteCom(0x01);  //清屏
	LCD1602_WriteCom(0x80);  //设置数据指针起点
}
#else
void LCD1602_Init()						  //LCD初始化子程序
{
	LCD1602_WriteCom(0x32);	 //将8位总线转为4位总线
	LCD1602_WriteCom(0x28);	 //在四位线下的初始化
	LCD1602_WriteCom(0x0c);  //开显示不显示光标
	LCD1602_WriteCom(0x06);  //写一个指针加1
	LCD1602_WriteCom(0x01);  //清屏
	LCD1602_WriteCom(0x80);  //设置数据指针起点
}
#endif
