#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
#include<iostream>
#include "serial.h"
#include<QMessageBox>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->connectStatus=0;//disconnect
    this->modeStatus=0;
    this->threadStatus=0;
    ui->setupUi(this);
    this->setMaximumSize(400,240);
    this->setMinimumSize(400,240);
    QObject::connect(ui->connectButton,SIGNAL(clicked()),this,SLOT(connectButtonSlot()));
    QObject::connect(ui->quitButton,SIGNAL(clicked()),this,SLOT(quitButtonSlot()));
    QObject::connect(ui->disConnectButton,SIGNAL(clicked()),this,SLOT(disConnectButtonSlot()));
    QObject::connect(ui->sendButton,SIGNAL(clicked()),this,SLOT(sendButtonSlot()));
    QObject::connect(ui->showTimeButton,SIGNAL(clicked()),this,SLOT(showTimeButtonSlot()));
    QObject::connect(ui->clearButton,SIGNAL(clicked()),this,SLOT(clearButtonSlot()));
    QObject::connect(ui->settingButton,SIGNAL(clicked()),this,SLOT(setButtonSlot()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::quitButtonSlot()
{
    UART0_Close(this->serial_fd);
    close();
}

void MainWindow::disConnectButtonSlot()
{
    UART0_Close(this->serial_fd);
    this->connectStatus=0;
    ui->connectButton->setEnabled(1);
}


void MainWindow::connectButtonSlot()
{
    int err;
    int cnt=0;
    char path[]={'/','d','e','v','/','t','t','y','U','S','B','0','\0'};
    this->serial_fd = UART0_Open(this->serial_fd,path);
    do
    {
        err = UART0_Init( this->serial_fd,4800,0,8,1,'N');
        printf("Set Port Exactly!\n");
        sleep(1);
        cnt++;
        if(cnt>=3)
        {
            this->connectStatus=0;
            QMessageBox::information(this,"ERROR","CAN NOT CONNECT DEVICE!");
            return;
        }
    }while(FALSE == err || FALSE ==  this->serial_fd);
    this->connectStatus=1;
    ui->connectButton->setEnabled(0);
}

void MainWindow::sendButtonSlot()
{
    char row=0,col=0,len=0;
    char *s;

    QString line;

    if(this->connectStatus==0)
    {
        QMessageBox::information(this,"ERROR","PLEASE CONNECT DEVICE");
        return;
    }
    row=char(ui->comboBox_2->currentIndex()+1);
    col=char(ui->comboBox->currentIndex());


    line=ui->lineEdit->text();
    QByteArray ba=line.toLatin1();
    s=ba.data();//qstring-->char *

    len=char(line.length());
    qDebug()<<line[1]<<endl;
    cout<<"row "<<int(row)<<"  col "<<int(col)<<"  len "<<int(len)<<endl;

    showLcd1602(this->serial_fd,row,col,len,s);
}


void* timeThread(void *arg)
{
    time_t timep;
    char *str;
    char time_buff[25];
    MainWindow *tempW=new MainWindow;
    tempW=(MainWindow *)arg;
    usleep(100000);
    while(tempW->threadStatus==1)
    {
        time(&timep);
        sprintf(time_buff,"%s",ctime(&timep));
         str=time_buff;
        showLcd1602(tempW->serial_fd,1,0,10,str);
        usleep(100000);
        str=time_buff+20;
        showLcd1602(tempW->serial_fd,1,11,4,str);
        usleep(100000);
        str=time_buff+11;
        showLcd1602(tempW->serial_fd,2,4,8,time_buff+11);
        usleep(100000);
    }
}


void MainWindow::showTimeButtonSlot()
{
    if(this->connectStatus==0)
    {
        QMessageBox::information(this,"ERROR","PLEASE CONNECT DEVICE");
        return;
    }
    //this->clearButtonSlot();
    this->modeStatus=1;
    //creat sub thread to show time

    int res=0;
    res=pthread_create(&(this->userThread),NULL,timeThread,(void *)(this));
    if(res!=0)
    {
        this->threadStatus=0;
        QMessageBox::information(this,"ERROR","CREATE THREAD ERROR");
        return;
    }
    this->threadStatus=1;
}

void MainWindow::clearButtonSlot()
{
     char str[17]="                ";
     char str1[17]="                ";
     if(this->connectStatus==0)
     {
         QMessageBox::information(this,"ERROR","PLEASE CONNECT DEVICE");
         return;
     }
     this->modeStatus=0;
     //kill all sub thread
     this->threadStatus=0;
     usleep(300000);
     showLcd1602(this->serial_fd,1,0,16,str);
     usleep(100000);
     showLcd1602(this->serial_fd,2,0,16,str1);
}

void MainWindow::setButtonSlot()
{
    setDialog *setD=new setDialog;
    setD->show();
}
