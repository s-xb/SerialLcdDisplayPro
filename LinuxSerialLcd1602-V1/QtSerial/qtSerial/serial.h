#ifndef SERIAL_H
#define SERIAL_H
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<termios.h>
#include<errno.h>
#include<string.h>
#include<time.h>
#define FALSE  -1
#define TRUE   0

int UART0_Open(int fd,char* port);

void UART0_Close(int fd);


int UART0_Set(int fd,int speed,int flow_ctrl,int databits,int stopbits,int parity);

int UART0_Init(int fd, int speed,int flow_ctrl,int databits,int stopbits,int parity);

int UART0_Recv(int fd, char *rcv_buf,int data_len);


int UART0_Send(int fd, char *send_buf,int data_len);
int showLcd1602(int fd,char row,char col,char len,char *s);
/*
 *
 *     fd = UART0_Open(fd,"/dev/ttyUSB0"); //Žò¿ªŽ®¿Ú£¬·µ»ØÎÄŒþÃèÊö·û
    do
    {
    err = UART0_Init(fd,4800,0,8,1,'N');
    printf("Set Port Exactly!\n");
    sleep(1);
     }while(FALSE == err || FALSE == fd);
 * */

#endif // SERIAL_H
