#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "canopen402.h"
#include "can.h"
#include "sys.h"
#include "delay.h"

#define sys_delay_s(s)  
#define sys_delay_ms(ms) delay_ms(ms) 
#define sys_delay_us(us) delay_us(us)


struct cAN_FRAME SdoRevBuf={0};//sdo frame buffer
struct cAN_HEARTBEAT_FRAME HeartBeatBuf[CAN_CLIENT_NUM]={0};//heartbeart frame buffer
int NodeId[CAN_CLIENT_NUM]={0};//client device node
char EmergencyBuff[CAN_CLIENT_NUM][8];//client device emergency buffer




//RPdoBuf,server receive
//TPdoBuf,Server transmit
struct pDO_DATA RPdoBuf[RPDO_NUM][CAN_CLIENT_NUM]={0};//redefinition of PDO address
struct cAN_FRAME TPdoBuf[TPDO_NUM][CAN_CLIENT_NUM]={0};

/*****************************************************************************************
* brief
*	  this function can send one frame 
*		user should this function to adapt the hardware
*param
*		busId:can channel id
*		canBuf:the info of frame
*				struct cAN_FRAME{
*					unsigned int frameId;
*					unsigned char frameType;
*					unsigned char dat[8];
*					unsigned char datLen;
*					unsigned char flag;//flag valid when receive only
*				};
*retval
*		1:write error
*		0:write success
*******************************************************************************************/
int can_DatSend(int busId, const struct  cAN_FRAME canDatBuf)
{
	/************user code start******************/
	Can_Send_Msg(canDatBuf);
	
	/************user code start******************/
	return 0;
}


/*****************************************************************************************
* brief
*	  this function can show the info of frame,it usually used in debug			
*param
*		str:this is a string
*		id:frame id
*		len:data length
*		*dat:frame data
*
*retval
*		none
*******************************************************************************************/
void show_FrameInfo(unsigned char *str,unsigned int id,unsigned char len,unsigned char *dat)
{
	unsigned char i=0;
	printf("\n%s: ID %x,length %x,dat ",str,id,len);
	for(i=0;i<len;i++)
	{
		printf("%x ",dat[i]);
	}
	printf("\n");
}



/*****************************************************************************************
* brief
*	  this function can init the client device,write the device id to server
*param
*		nodeId:all client device id
*		num:the number of client device
*
*retval
*		1:write error
*		0:write success
*******************************************************************************************/
int clientNodeInfo(unsigned char *nodeId,unsigned char num)
{
	int i=0;
	if(num>CAN_CLIENT_NUM)
	{
		return 1;
	}
	for(i=0;i<num;i++)
	{
		NodeId[i]=nodeId[i];
		printf("device %d,node %d\n",i,NodeId[i]);
	}
	return 0;
}



/*****************************************************************************************
* brief
*	  this function can receive and sort the frame,including SDO,PDO,heartbeat,emergency
*		the function run in interrupt or thread
*		user should add the code when use heartbeat check 
*param
*		busId:can channel id
*		canBuf:the info of frame
*				struct cAN_FRAME{
*					unsigned int frameId;
*					unsigned char frameType;
*					unsigned char dat[8];
*					unsigned char datLen;
*					unsigned char flag;//flag valid when receive only
*				};
*retval
*		1:write error
*		0:write success
*******************************************************************************************/
int can_DatReceive(int busId,struct cAN_FRAME canBuf)
{
#ifdef DEDUG_SHOW
	show_FrameInfo("source receive",canBuf.frameId,canBuf.datLen,canBuf.dat);
#endif
	if(0x580==(canBuf.frameId&0x780)) //sdo frame receive
	{
		SdoRevBuf.frameId=canBuf.frameId;
		SdoRevBuf.datLen=canBuf.datLen;
		memcpy(SdoRevBuf.dat,canBuf.dat,canBuf.datLen);
		SdoRevBuf.flag=1;
#ifdef DEDUG_SHOW
		{
			int k=0;
			printf("\nThis is can_DatReceive() function\nSDO Frame Info:ID %x,DataLen %x,Flag %x\ndat:",SdoRevBuf.frameId,SdoRevBuf.datLen,SdoRevBuf.flag);
			for(k=0;k<SdoRevBuf.datLen;k++)
				printf("%x ",SdoRevBuf.dat[k]);
			printf("\n");
		}
#endif
		return 0;
	}
	else if(0x700==(canBuf.frameId&0x780))//heartbeat frame receive
	{
		int i=0;
		for(i=0;i<CAN_CLIENT_NUM;i++)
		{
			if(NodeId[i]==(canBuf.frameId&0x07f))
			{
				/*
				user code
				HeartBeatBuf[i].useTime
				*/
				
				HeartBeatBuf[i].frameId=canBuf.frameId;
				HeartBeatBuf[i].state=canBuf.dat[0];
				HeartBeatBuf[i].flag=1;
#ifdef DEDUG_SHOW
				printf("\nThis is can_DatReceive() function\nHeartBeatBuf Frame Info:device %d,ID %x,state %x\ndat:",i,HeartBeatBuf[i].frameId,HeartBeatBuf[i].state);
				printf("\n");
#endif				
				return 0;
			}
		}
	}
	else if((canBuf.frameId>0x180)&&(canBuf.frameId<0x57f))//PDO frame receive
	{
		int i=0,j=0;
		for(i=0;i<CAN_CLIENT_NUM;i++)//calculate the client device node
		{
			if((canBuf.frameId&0x07f)==NodeId[i])
			{
				break;
			}
		}
		j=((canBuf.frameId&0x780)-0x180)/0x100;//calculate the client send pdo number
		if(j>RPDO_NUM||j<0)
		{
			return 1;
		}
		
		memcpy(RPdoBuf[j][i].dat,canBuf.dat,8);
		RPdoBuf[j][i].flag=1;
#ifdef DEDUG_SHOW
		{
			int k=0;
			printf("\nThis is can_DatReceive() function\n");
			printf("PDO Frame Info:NodeId %x PDONUM %d,DataLen %x,Flag %x\ndat:",NodeId[i],j,SdoRevBuf.datLen,RPdoBuf[j][i].flag);
			for(k=0;k<8;k++)
				printf("%x ",RPdoBuf[j][i].dat[k]);
			printf("\n");
		}
#endif
		return 0;
	}
	else if((canBuf.frameId>0x80)&&(canBuf.frameId<0xff))//emergency frame receive
	{
		int i=0;
		for(i=0;i<CAN_CLIENT_NUM;i++)
		{
			if(NodeId[i]==(canBuf.frameId&0x07f))
			{
				memcpy(EmergencyBuff[i],canBuf.dat,8);
#ifdef DEDUG_SHOW
			{
				int k=0;
				printf("\nThis is can_DatReceive() function\n");
				printf("EmergencyBuff Frame Info:NodeId %x\ndat:",NodeId[i]);
				for(k=0;k<8;k++)
					printf("%x ",EmergencyBuff[i][k]);
				printf("\n");
			}
#endif
				return 0;
			}
		}
	}
	else		//other frame receive ,this is undefined
	{
#ifdef DEDUG_SHOW
		printf("\nThis is can_DatReceive() function\nUndefined CAN frame\n");
#endif
		return 1;
	}
	return 0;
}





//-------------------------------------------------------------------NMT----------------------------------------------------------------------------------------------------------//
/*****************************************************************************************
* brief
*	  this function is NMT server
*		user can use it to set all client devices' status
*param
*		busId:can channel id
*		nodeId:the number of client node id
*					 when nodeId=0,set all client
*		nodeState:the status
*					0x01  NMT OPERATIONAL			
*					0x02  NMT STOP		    			
*					0x80  NMT PREOPERATIONAL		
*					0x81  NMT RESET APPLICATION					
*					0x82  NMT_RESET COMMUNICATION	
*retval
*		none
*******************************************************************************************/
void nmt_StateSet(int busId,int nodeId,int nodeState)
{
	struct  cAN_FRAME canDatBuf;

	canDatBuf.frameId=0;
	canDatBuf.frameType=0;
	canDatBuf.datLen=2;

	canDatBuf.dat[0]=nodeState;
	canDatBuf.dat[1]=nodeId;
	
	can_DatSend(busId,canDatBuf);
}

void nmt_StateResetApplication(int busId,int nodeId)
{
	nmt_StateSet(busId,nodeId,NMT_RESETAPP);
}
void nmt_StateResetCommunication(int busId,int nodeId)
{
	nmt_StateSet(busId,nodeId,NMT_RESETCOMM);
}
void nmt_StatePreOperational(int busId,int nodeId)
{
	nmt_StateSet(busId,nodeId,NMT_PREOPERATIONAL);
}
void nmt_StateOperational(int busId,int nodeId)
{
	nmt_StateSet(busId,nodeId,NMT_OPERATIONAL);
}
void nmt_StateStop(int busId,int nodeId)
{
	nmt_StateSet(busId,nodeId,NMT_STOP);
}



//-------------------------------------------------------------------SYNC----------------------------------------------------------------------------------------------------------//
/*****************************************************************************************
* brief
*	  this function can send sync frame

*param
*		busId:can channel id
*retval
*		none
*******************************************************************************************/
void sync_Send(int busId)
{
	struct cAN_FRAME syncBuf;
	syncBuf.frameId=0x80;
	syncBuf.frameType=0;
	syncBuf.datLen=1;
	syncBuf.dat[0]=0;
	can_DatSend(busId,syncBuf);
}

//------------------------------------------------------------------SDO----------------------------------------------------------------------------------------------------------//
/*****************************************************************************************
* brief
*	  this function can show the SDO frame imformation
*param
*		code: the error frame code
*retval
*		none
*******************************************************************************************/
static void sdo_ErrorCode(unsigned int code)
{
	switch(code)
	{
		case 0x05030000 :
			printf("Toggle bit not alternated\n");
			break;
		case 0x05040000 :
			printf("SDO protocol timed out\n");
			break;
		case 0x05040001 :
			printf("Client/Server command specifier not valid or unknown\n");
			break;
		case 0x05040002 :
			printf("Invalid block size(block mode only)\n");
			break;
		case 0x05040003 :
			printf("Invalid sequence number(balock mode only)\n");
			break;
		case 0x05040004 :
			printf("CRC Error(block mode only)\n");
			break;
		case 0x05040005 :
			printf("Out of Memory\n");
			break;
		case 0x06010000 :
			printf("Unsupported access to an object\n");
			break;
		case 0x06010001 :
			printf("Attempt to read a write only object\n");
			break;
		case 0x06010002 :
			printf("Attempt to write a read only object\n");
			break;
		case 0x06020000 :
			printf("Object does not exist in the object dictionary\n");
			break;
		case 0x06040041 :
			printf("Object cannot be mapped to the PDO\n");
			break;
		case 0x06040042 :
			printf("The number and length of the objects to be mapped would exceed PDO length\n");
			break;		
		case 0x06040043 :
			printf("General parameter incompatibility reason\n");
			break;	
		case 0x06040047 :
			printf("General internal incompatibility in the device\n");
			break;
		case 0x06060000 :
			printf("Access failed due to hardware error\n");
			break;
		case 0x06070010 :
			printf("Data length does not match,length of service parameter does not match\n");
			break;
		case 0x06070012 :
			printf("Data length does not match,length of service parameter too high\n");
			break;
		case 0x06070013 :
			printf("Data length does not match,length of service parameter too low\n");
			break;
		case 0x06090011 :
			printf("Sub-index does not exist\n");
			break;
		case 0x06090030 :
			printf("Value range of parameter exceeded(only for write access)\n");
			break;
		case 0x06090031 :
			printf("Value of parameter written too high\n");	
			break;
		case 0x06090032 :
			printf("Value of parameter written too low\n");	
			break;
		case 0x06090036 :
			printf("Maximum value is less than minmum value\n");	
			break;
		case 0x08000000 :
			printf("General error\n");	
			break;
		case 0x08000020 :
			printf("Data can not transferred or stored to the application\n");	
			break;
		case 0x08000021 :
			printf("Data can not transferred or stored to the application because of local control\n");	
			break;
		case 0x08000022 :
			printf("Data can not transferred or stored to the application because of the present device state\n");	
			break;
		case 0x08000023 :
			printf("Objiect dictionary dynamic generation fails or no object dictionary is present\n");	
			break;
		default:
			printf("May be I does not know this error ^_^\n");	
			break;

	}
}

/*****************************************************************************************
* brief
*	  this function can send one sdo frame(fast sdo)
*param
*		busId:can channel id
*		nodeId:the number of client node id
*		sdoBuf:send sdo data buf
*				struct sDO_OBJ
*				{
*					unsigned int index;		//object index
*					unsigned int subIndex;//object subindex
*					unsigned char dat[4]; //sdo data
*					unsigned char datLen; //sdo data length
*				};
*		time:sdo ack info wait time(ms)
*				user can correct "sys_delay_us(1000)" to change the timere solution
*example:
*			struct sDO_OBJ sdoBuf={1800,2,{1,2,3,4},4}
*    	sd0_SendData(0,1,sdoBuf,1)
*				can channel:			0
*				device node ID:		1
*      	object index:			1800h
*      	object subindex:	2
*      	sdo data:					1,2,3,4
*      	sdo data length:	4
*      	waiting time:			1ms
*				frame data: 601h, 23 00 18 02 1 2 3 4
*				client ack: 581h, 60 00 18 02 0 0 0 0(sdo send success)
*retval
*		1:write error
*		0:write success
*******************************************************************************************/
int sdo_SendDat(int busId, int nodeId,const struct sDO_OBJ sdoBuf,unsigned long  int time)
{
	struct cAN_FRAME canDatBuf;
	unsigned char i=0;
	unsigned int timeCnt=0;
	unsigned int err=0;
	//prepare the framedata
	canDatBuf.frameId=nodeId+0x600;
	canDatBuf.frameType=0;
	canDatBuf.datLen=8;
	canDatBuf.dat[0]=0x2f-(sdoBuf.datLen-1)*4;//calculate the data length 
	canDatBuf.dat[1]=sdoBuf.index&0x00ff;
	canDatBuf.dat[2]=sdoBuf.index>>8;
	canDatBuf.dat[3]=sdoBuf.subIndex;
	if(sdoBuf.datLen>4)
	{
#ifdef DEDUG_SHOW
		printf("\nThis is sdo_SendDat() function\nInput data length too length:%d\n",sdoBuf.datLen);
#endif
		return 1;
	}
	for(i=0;i<4;i++)
	{
		if(i<sdoBuf.datLen)
			canDatBuf.dat[i+4]=sdoBuf.dat[i];
		else
			canDatBuf.dat[i+4]=0;
	}
	can_DatSend(busId,canDatBuf);//send frame

	while(1)
	{
		if(1==SdoRevBuf.flag && SdoRevBuf.frameId==(nodeId+0x580))//check the sdo ack info
		{
			if(	SdoRevBuf.dat[1]==canDatBuf.dat[1]&&
				SdoRevBuf.dat[2]==canDatBuf.dat[2]&&
				SdoRevBuf.dat[3]==canDatBuf.dat[3])
			{	
					SdoRevBuf.flag=0;
					if(SdoRevBuf.dat[0]==0x60)
					{
#ifdef DEDUG_SHOW
						printf("\nThis is sdo_SendDat() function\nSDO Send data success\n");
#endif
						return 0;
					}
					else
					{
						memcpy(&err,&(SdoRevBuf.dat[4]),4);//receive error frame information
						printf("\nSDO SEND DATA ERROR\n");
						printf("INDEX 0x%x%x,ERROR CODE 0x%x\n",sdoBuf.index,sdoBuf.subIndex,err);
						sdo_ErrorCode(err);
						printf("\n");
						return 1;
					}	
			}
			else
			{

			}
		}
		sys_delay_us(1000);
		timeCnt++;
		if(timeCnt>=time)
		{
			break;//time out
		}
	}
#ifdef DEDUG_SHOW
		printf("\nThis is sdo_SendDat() function\nSDO ACK TIME OVER\n");			
#endif
	
	return 1;
}



/*****************************************************************************************
* brief
*	  this function is same as sdo_SendDat
*param
*		busId:can channel id
*		nodeId:the number of client node id
*		sdoBuf:send object info,to receive object data
*				struct sDO_OBJ
*				{
*					unsigned int index;		//object index
*					unsigned int subIndex;//object subindex
*					unsigned char dat[4]; //sdo data
*					unsigned char datLen; //sdo data length
*				};
*		time:sdo ack info wait time(ms)
*				user can correct "sys_delay_us(1000)" to change the timere solution
*example:
*			struct sDO_OBJ sdoBuf={1800,2,{0,0,0,0},0}
*    	sd0_SendData(0,1,sdoBuf,1)
*				can channel:			0
*				device node ID:		1
*      	object index:			1800h
*      	object subindex:	2
*      	sdo data:					0,0,0,0(not valid)
*      	sdo data length:	0(not valid)
*      	waiting time:			1ms
*				frame data: 601h, 40 00 18 02 1 2 3 4
*				client ack: 581h, 43 00 18 02 0 0 0 0(sdo send success)
*retval
*		1:write error
*		0:write success
*******************************************************************************************/
int sdo_RequestDat(int busId, int nodeId,struct sDO_OBJ *sdoBuf, unsigned long int time)
{
	struct cAN_FRAME canDatBuf;
	unsigned char i=0;
	unsigned int timeCnt=0;
	unsigned int err=0;

	canDatBuf.frameId=nodeId+0x600;
	canDatBuf.frameType=0;
	canDatBuf.datLen=8;
	canDatBuf.dat[0]=0x40;
	canDatBuf.dat[1]=sdoBuf->index&0x00ff;
	canDatBuf.dat[2]=sdoBuf->index>>8;
	canDatBuf.dat[3]=sdoBuf->subIndex;
	for(i=0;i<4;i++)
	{
		canDatBuf.dat[i+4]=0;
	}
	can_DatSend(busId,canDatBuf);//send frame

	while(1)
	{
		if(1==SdoRevBuf.flag &&SdoRevBuf.frameId==(nodeId+0x580))
		{
			if( SdoRevBuf.dat[1]==canDatBuf.dat[1]&&
				SdoRevBuf.dat[2]==canDatBuf.dat[2]&&
				SdoRevBuf.dat[3]==canDatBuf.dat[3])
			{
					SdoRevBuf.flag=0;
					if(SdoRevBuf.dat[0]!=0x80)
					{
						sdoBuf->datLen=(0x4f-SdoRevBuf.dat[0])/4+1;//check received information
						if(sdoBuf->datLen>4)
						{	
#ifdef DEDUG_SHOW
							printf("\nThis is sdo_RequestDat() function\nSDO receive data error,too length :%d\n",sdoBuf->datLen);			
#endif
							sdoBuf->datLen=0;
							return 1;
						}
						for(i=0;i<sdoBuf->datLen;i++)
						{
							sdoBuf->dat[i]=SdoRevBuf.dat[i+4];
						}
#ifdef DEDUG_SHOW
						printf("\nThis is sdo_RequestDat() function\nSDO request data success\n");			
#endif
						return 0;
					}
					else
					{
						memcpy(&err,&(SdoRevBuf.dat[4]),4);//request sdo error
						printf("\nSDO REQUEST ERROR\n");
						printf("INDEX 0x%x%x,ERROR CODE 0x%x\n",sdoBuf->index,sdoBuf->subIndex,err);
						sdo_ErrorCode(err);
						printf("\n");
						return 1;
					}
			}
			else 
			{

			}
		}
		sys_delay_us(1000);//the is wait time resolution
		timeCnt++;
		if(timeCnt>=time)
		{
			break;
		}
	}
#ifdef DEDUG_SHOW
		printf("\nThis is sdo_RequestDat() function\nSDO ACK TIME OVER\n");			
#endif
	return 1;
}

/*****************************************************************************************
* brief
*	  this function can disable pdo
*param
*		busId:can channel id
*		nodeId:the number of client node id
*		pdoObject:1800h-1A00h
*							1400h-1600h
*retval
*		1:write error
*		0:write success
*******************************************************************************************/
int disable_PDO(int busId,int nodeId,unsigned int pdoObject)
{
		struct sDO_OBJ sdoBuf;
		unsigned int frameId=0;
		//subindex 0x01
		//disable PDO
		if((pdoObject&0xff00)==0x1800)
		{
			frameId=0x180+(pdoObject-0x1800)*0x100+nodeId;
		}
		else if((pdoObject&0xfff0)==0x1400)
		{
			frameId=0x200+(pdoObject-0x1400)*0x100+nodeId;
		}
		
		sdoBuf.index=pdoObject;
		sdoBuf.subIndex=0x01;
		sdoBuf.datLen=4;
		sdoBuf.dat[0]=frameId&0x00ff;
		sdoBuf.dat[1]=frameId>>8;
		sdoBuf.dat[2]=0x00;
		sdoBuf.dat[3]=0x80;//disable PDO
		sdo_SendDat(busId,nodeId,sdoBuf,100);
		return 0;
}


/*****************************************************************************************
* brief
*	  this function can config pdo
*param
*		busId:can channel id
*		nodeId:the number of client node id
*		pdoBuf:pdo configure information
*				struct pDOCFG_STRUCT
*				{
*					int pdoIndex;								//conmmunication pdo object
*					int pdoMode;								//pdo mode,the numbers of sync
*					unsigned int datAddress[8];	//object	index subindex datalength,the total byte < 8
*					char addressNum; 						//the number of datAddress is valid
*				};
*example
*		struct pDOCFG_STRUCT pdoBuf={1800h,1,{60000120h,60000220h,60000320h},3}
*					 pdo object: 1800h
*					 pdo mode: 1 sync
*					 mapping object:60000120h
*									        60000220h
*									        60000320h
*retval
*		1:write error
*		0:write success
*******************************************************************************************/
int pdo_Config(int busId,int nodeId,const struct pDOCFG_STRUCT pdoBuf)
{
//1800h-->1A00,1400-->1600
//180h+nodeId
	int i=0,j=0;
	char datLen=0;
	int frameId;
	for(i=0;i<pdoBuf.addressNum;i++)//check the frame data length
	{
		datLen+=(pdoBuf.datAddress[i]&0x000000ff);
	}
	if(datLen>64) 
	{
#ifdef DEDUG_SHOW
		printf("\nThis is pdo_Config() function\nlonger than PDO length\n");			
#endif
		return 1; //data overflow
	}
	
	//distinguish TPDO and RPDO
	if((pdoBuf.pdoIndex&0xff00)==0x1800)
	{
		frameId=0x180+(pdoBuf.pdoIndex-0x1800)*0x100+nodeId; //client TPDO
	}
	else if((pdoBuf.pdoIndex&0xfff0)==0x1400)//client RPDO
	{
		frameId=0x200+(pdoBuf.pdoIndex-0x1400)*0x100+nodeId;
		//init the client RPDO(server TPDO) array
		for(i=0;i<CAN_CLIENT_NUM;i++)
		{
			if(nodeId==NodeId[i])
			{
				break;
			}
		}
		j=pdoBuf.pdoIndex-0x1400;
		if(j>TPDO_NUM||j<0)
		{
			printf("\narray over flow\n");
			return 1; //data overflow
		}
		TPdoBuf[j][i].frameId=frameId;
		TPdoBuf[j][i].frameType=FRAME_TYPE_DATA;
		TPdoBuf[j][i].datLen=datLen/8;
#ifdef DEDUG_SHOW
		{
			int ii=0,jj=0;
			printf("\nThis is pdo_Config() function\nshow TPDO Array\n");
			for(ii=0;ii<CAN_CLIENT_NUM;ii++)
			{
				printf("the device %d ",ii+1);
				for(jj=0;jj<TPDO_NUM;jj++)
				{
					printf("the PDO %d\n",jj);
					printf("ID %x, Len %x \n",TPdoBuf[jj][ii].frameId,TPdoBuf[jj][ii].datLen);
				}
			}			
			printf("\n");
		}
#endif	
	}
	else
	{
#ifdef DEDUG_SHOW
		printf("\nThis is pdo_Config() function\nPDO mapping address error\n");			
#endif
		return 1;
	}
	
	
	{
		struct sDO_OBJ sdoBuf;
		//subindex 0x01
		//disable PDO
		sdoBuf.index=pdoBuf.pdoIndex;
		sdoBuf.subIndex=0x01;
		sdoBuf.datLen=4;
		sdoBuf.dat[0]=frameId&0x00ff;
		sdoBuf.dat[1]=frameId>>8;
		sdoBuf.dat[2]=0x00;
		sdoBuf.dat[3]=0x80;
		sdo_SendDat(busId,nodeId,sdoBuf,100);
		
		//clear mapping
		sdoBuf.index=pdoBuf.pdoIndex+0x200;
		sdoBuf.subIndex=0x00;
		sdoBuf.datLen=1;
		sdoBuf.dat[0]=0x00;
		sdoBuf.dat[1]=0x00;
		sdoBuf.dat[2]=0x00;
		sdoBuf.dat[3]=0x00;
		sdo_SendDat(busId,nodeId,sdoBuf,100);
		
		//pdo mapping
		sdoBuf.index=pdoBuf.pdoIndex+0x200;
		sdoBuf.datLen=4;
		for(i=0;i<pdoBuf.addressNum;i++)
		{
			sdoBuf.subIndex=i+1;
			memcpy(sdoBuf.dat,&(pdoBuf.datAddress[i]),4);
			sdo_SendDat(busId,nodeId,sdoBuf,100);
		}

		//set PDO mode
		sdoBuf.index=pdoBuf.pdoIndex;
		sdoBuf.subIndex=0x02;
		sdoBuf.datLen=1;
		sdoBuf.dat[0]=pdoBuf.pdoMode; 
		sdoBuf.dat[1]=0;
		sdoBuf.dat[2]=0;
		sdoBuf.dat[3]=0;
		sdo_SendDat(busId,nodeId,sdoBuf,100);

		//set pdo mapping number
		sdoBuf.index=pdoBuf.pdoIndex+0x200;
		sdoBuf.subIndex=0x00;
		sdoBuf.datLen=1;
		sdoBuf.dat[0]=pdoBuf.addressNum;
		sdoBuf.dat[1]=0x00;
		sdoBuf.dat[2]=0x00;
		sdoBuf.dat[3]=0x00;
		sdo_SendDat(busId,nodeId,sdoBuf,100);

		//enable pdo
		sdoBuf.index=pdoBuf.pdoIndex;
		sdoBuf.subIndex=0x01;
		sdoBuf.datLen=4;
		sdoBuf.dat[0]=frameId&0x00ff;
		sdoBuf.dat[1]=frameId>>8;
		sdoBuf.dat[2]=0x00;
		sdoBuf.dat[3]=0x00;
		sdo_SendDat(busId,nodeId,sdoBuf,100);
	}
	return 0;
}

/*****************************************************************************************
* brief
*	  this function can return the servo driver status
*param
*		statusWorld:status word
*retval
*		status word
* 	UNKNOW                0x00
*	 	NOT_READY_SWITCH_ON   0x01
*		SWITCH_ON_DISABLE     0x02
*		READY_TO_SWITCH_ON    0x03
*		SWITCH_ON             0x04
*		OPERATION_ENABLE      0x05
*		QUICT_STOP_ACTIVE     0x06
*		FAULT_REACTION_ACTIVE 0x07
*		FAULT                 0x08
*******************************************************************************************/
unsigned char show_ServoStatus(unsigned short statusWord)
{

	if((statusWord&0x004f)==0x0000)
	{
#ifdef 	DEDUG_SHOW
		printf("\nNOT_READY_SWITCH_ON\n");
#endif
		return NOT_READY_SWITCH_ON;
	}
	else if((statusWord&0x004f)==0x0040)
	{
#ifdef 	DEDUG_SHOW	
		printf("\nSWITCH_ON_DISABLE\n");
#endif		
		return SWITCH_ON_DISABLE;
	}
	else if((statusWord&0x006f)==0x0021)
	{
#ifdef 	DEDUG_SHOW	
		printf("\nREADY_TO_SWITCH_ON\n");
#endif		
		return READY_TO_SWITCH_ON;
	}
	else if((statusWord&0x006f)==0x0023)
	{
#ifdef 	DEDUG_SHOW
		printf("\nSWITCH_ON\n");
#endif		
		return SWITCH_ON;
	}
	else if((statusWord&0x006f)==0x0027)
	{
#ifdef 	DEDUG_SHOW
		printf("\nOPERATION_ENABLE\n");
#endif		
		return OPERATION_ENABLE;
	}
	else if((statusWord&0x006f)==0x0007)
	{
#ifdef 	DEDUG_SHOW
		printf("\nQUICT_STOP_ACTIVE\n");
#endif		
		return QUICT_STOP_ACTIVE;
	}
	else if((statusWord&0x004f)==0x000f)
	{
#ifdef 	DEDUG_SHOW
		printf("\nFAULT_REACTION_ACTIVE\n");
#endif		
		return FAULT_REACTION_ACTIVE;
	}
	else if((statusWord&0x004f)==0x0008)
	{
#ifdef 	DEDUG_SHOW
		printf("\nFAULT\n");
#endif		
		return FAULT;
	}
	return UNKNOW;
}

/*****************************************************************************************
* brief
*	  this function can send one pdo frame
*param
*		busId:can channel id
*		nodeId:the number of client node id
*		object:pdo object,1400h,1401h,...
*		dat:pdo frame data
*		len:pdo frame length
*retval
*		0 success
*   1 error
*******************************************************************************************/
unsigned int pdo_SingleSend(unsigned int busId,int nodeId,unsigned int object,unsigned char *dat,unsigned char len)
{
	unsigned int i=0;
	for(i=0;i<CAN_CLIENT_NUM;i++)
	{
		if(nodeId==NodeId[i])
		{
			break;
		}
	}
	if((object-0x1400)>TPDO_NUM)
	{
		printf("\nPdo object error,array over flow\n");
		return 1;
	}
	memcpy(&(TPdoBuf[object-0x1400][i].dat[0]),dat,len);
	can_DatSend(busId,TPdoBuf[object-0x1400][i]);
	return 0;
}


/*****************************************************************************************
* brief
*	  using sdo to change the status of server driver
*param
*		busId:can channel id
*		nodeId:the number of client node id
*		statusSet:set status
*		statusCheck:check status
*retval
*		1:error
*		0:success
*******************************************************************************************/
int sdo_DriverStatusChange(int busId, int nodeId,const short statusSet,short statusCheck)
{
	int cnt=0;
	struct sDO_OBJ sdoBuf={0x6040,0x00,{0,0},2};
  sdoBuf.dat[0]=statusSet&0x00ff;
	sdoBuf.dat[1]=statusSet>>8;
	if(sdo_SendDat(busId,nodeId,sdoBuf,1000))//send control word
	{
		return 1;
	}
	
	while(1)
	{
		struct sDO_OBJ sdoBuf;
		short statusworld;
		sdoBuf.index=0x6041;//read status word
		sdoBuf.subIndex=0x00;
		if(!(sdo_RequestDat(busId,nodeId,&sdoBuf,1000)))
		{
			memcpy(&statusworld,sdoBuf.dat,2);
			if(statusCheck==show_ServoStatus(statusworld))
			{
				return 0;
			}
		}
		sys_delay_us(10000);//waitting status change 
		cnt++;
		if(cnt>1000)
		{
			return 1; //time out
		}
	}
}


/*****************************************************************************************
* brief
*	  this can check driver fault and reset fault
*		fault:status word  x0xx 1000
*         control word 0x00-->0x08
*param
*		busId:can channel id
*		nodeId:the number of client node id
*retval
*		1:error
*		0:success
*******************************************************************************************/
unsigned char fault_Check(int busId,int nodeId)
{
	struct sDO_OBJ sdoBufStatus={0x6041,0x00,{0,0},2};
	struct sDO_OBJ sdoBufControl={0x6040,0x00,{0x00,0x00},2};
	short statusworld;
	unsigned int cnt=0;
	while(1)
	{
		if(!(sdo_RequestDat(busId,nodeId,&sdoBufStatus,1000)))
		{
			memcpy(&statusworld,sdoBufStatus.dat,2);
			if(FAULT==show_ServoStatus(statusworld))
			{
				if(cnt>200)
				{
					printf("\nfault reset error!!!\n");
					return 1;
				}	
				sdoBufControl.dat[0]=0x00;
				sdo_SendDat(busId,nodeId,sdoBufControl,1000);//send control word
				sdoBufControl.dat[0]=0x80;
				sdo_SendDat(busId,nodeId,sdoBufControl,1000);//send control word
			}
			else
			{
				break;
			}
		}
		sys_delay_us(10000);//waitting status change 
		cnt++;
	}
	return 0;
}

/*****************************************************************************************
* brief
*	  using PDO to change the status of server driver
*param
*		deviceIndex and pdoNum:accordance about "TPdoBuf[PDO_NUM][CAN_CLIENT_NUM]"
*		syncCnt:the numbers of SYNC
*		StatusCheck: check status
*retval
*		1:error
*		0:success
*******************************************************************************************/
int pdo_StatusCheck(unsigned char deviceIndex,unsigned char pdoNum,unsigned char syncCnt,short StatusCheck)
{
	int cnt=0,i=0;
	short statusworld=0;
	while(1)
	{
		for(i=0;i<syncCnt;i++)//send SYNC frame
		{
			sync_Send(0);	
			sys_delay_us(1000);
		}
		while(1)//waittimg client send PDO
		{
			sys_delay_us(100);
			if(RPdoBuf[pdoNum][deviceIndex].flag==1) //chenck status
			{
				memcpy(&statusworld,RPdoBuf[pdoNum][deviceIndex].dat,2);	
				RPdoBuf[pdoNum][deviceIndex].flag=0;
				if(StatusCheck==show_ServoStatus(statusworld))
				{
					return 0;//success
				}
				break;//the status not change
			}
		}
		sys_delay_us(1000);
			
		cnt++;
		if(cnt>1000)
		{
			return 1; //time out
		}
	}
}


/*****************************************************************************************
* brief
*	  the function can init NMT device
*param
*		busId:can channel id
*retval
*		none
*******************************************************************************************/
void communication_Init(int busId)
{
	nmt_StateResetApplication(busId,0);
	sys_delay_ms(1000);
	sys_delay_ms(1000);
	sys_delay_ms(1000);
	sys_delay_ms(1000);
	nmt_StateResetCommunication(busId,0);
	sys_delay_ms(1000);
	nmt_StatePreOperational(busId,0);
	sys_delay_ms(1000);
}

/*****************************************************************************************
* brief
*	  this function can set heartbeats time,client send
*param
*		busId:can channel id
*		nodeId:the number of client node id
*		time:the time of client to send heartbeat(ms)
*retval
*		1:error
*		0:success
*******************************************************************************************/
unsigned char heartbeat_ClientSend(int busId,int nodeId,unsigned int time)
{
	struct sDO_OBJ sdoBuf={0x1017,0x00,{0,0},2};
	sdoBuf.dat[0]=time&0x00ff;
	sdoBuf.dat[1]=time>>8;
	return sdo_SendDat(busId,nodeId,sdoBuf,1000);
}

/*****************************************************************************************
* brief
*	  this function can set heartbeats time,master send
*param
*		busId:can channel id
*		nodeId:the number of client node id
*		time:the time of client to receive heartbeat(ms)
*retval
*		1:error
*		0:success
*******************************************************************************************/
unsigned char heartbeat_MasterSend(int busId,int nodeId,unsigned int time)
{
	struct sDO_OBJ sdoBuf={0x1016,0x01,{0,0,0,0},4};
	sdoBuf.dat[0]=time&0x00ff;
	sdoBuf.dat[1]=time>>8;
	sdoBuf.dat[2]=MASTER_NODE;
	sdoBuf.dat[3]=0x00;
	return sdo_SendDat(busId,nodeId,sdoBuf,1000);
}

/*****************************************************************************************
* brief
*	  this function can set master send heartbeat
*param
*		busId:can channel id
*retval
*		none
*******************************************************************************************/
void master_HeartbeatSend(int busId)
{
	struct cAN_FRAME canDatBuf;
	canDatBuf.frameId=0x700+MASTER_NODE;
	canDatBuf.frameType=0;
	canDatBuf.datLen=1;
	canDatBuf.dat[0]=0x05;
	can_DatSend(busId,canDatBuf);//send frame
}


/*****************************************************************************************
* brief
*	  this function can set the run mode of driver
*param
*		busId:can channel id
*		nodeId:the number of client node id
*		mode: 1 position mode
*					3 velocity mode
*					4 torque mode
*retval
*		1:error
*		0:success
*******************************************************************************************/
unsigned char driver_RunMode(int busId,int nodeId,unsigned char mode)
{
	struct sDO_OBJ sdoBuf={0x6060,0x00,{0,0},2};
	sdoBuf.dat[0]=mode;
		return sdo_SendDat(busId,nodeId,sdoBuf,1000);
}


/*****************************************************************************************
* brief
*	  this function can set the max velocity of motor(RPM)
*param
*		busId:can channel id
*		nodeId:the number of client node id
*		maxV:max profile velocity ,0-5000
*retval
*		1:error
*		0:success
*******************************************************************************************/


unsigned char position_RangeLimitMin(int busId,int nodeId,int minP)
{
		struct sDO_OBJ sdoBuf={0x607B,0x01,{0,0,0,0},4};
		
		memcpy(sdoBuf.dat,&minP,4);
	
		if(sdo_SendDat(busId,nodeId,sdoBuf,1000))
		{
			return 1;
		}
		return 0;
}


unsigned char position_RangeLimitMax(int busId,int nodeId,int maxP)
{
		struct sDO_OBJ sdoBuf={0x607B,0x02,{0,0,0,0},4};
		
		memcpy(sdoBuf.dat,&maxP,4);
	
		if(sdo_SendDat(busId,nodeId,sdoBuf,1000))
		{
			return 1;
		}
		return 0;
}


unsigned char position_SoftwareLimitMin(int busId,int nodeId,int minP)
{
		struct sDO_OBJ sdoBuf={0x607D,0x01,{0,0,0,0},4};
		
		memcpy(sdoBuf.dat,&minP,4);
	
		if(sdo_SendDat(busId,nodeId,sdoBuf,1000))
		{
			return 1;
		}
		return 0;
}

unsigned char position_SoftwareLimitMax(int busId,int nodeId,int maxP)
{
		struct sDO_OBJ sdoBuf={0x607D,0x02,{0,0,0,0},4};
		
		memcpy(sdoBuf.dat,&maxP,4);
	
		if(sdo_SendDat(busId,nodeId,sdoBuf,1000))
		{
			return 1;
		}
		return 0;
}


unsigned char speed_MotorMax(int busId,int nodeId,unsigned int maxV)
{
		struct sDO_OBJ sdoBuf={0x6080,0x00,{0,0,0,0},4};
		
		memcpy(sdoBuf.dat,&maxV,4);
	
		if(sdo_SendDat(busId,nodeId,sdoBuf,1000))
		{
			return 1;
		}
		return 0;
}


unsigned char veolcity_profileMAX(int busId,int nodeId,unsigned int velocity)
{
		struct sDO_OBJ sdoBuf={0x607f,0x00,{0,0,0,0},4};
		
		memcpy(sdoBuf.dat,&velocity,4);
	
		if(sdo_SendDat(busId,nodeId,sdoBuf,1000))
		{
			return 1;
		}
		return 0;
}
unsigned char veolcity_profile(int busId,int nodeId,unsigned int velocity)
{
		struct sDO_OBJ sdoBuf={0x6081,0x00,{0,0,0,0},4};
		
		memcpy(sdoBuf.dat,&velocity,4);
	
		if(sdo_SendDat(busId,nodeId,sdoBuf,1000))
		{
			return 1;
		}
		return 0;
}


unsigned char acceleration_profile(int busId,int nodeId,unsigned int acceleration)
{
		struct sDO_OBJ sdoBuf={0x6083,0x00,{0,0,0,0},4};
		
		memcpy(sdoBuf.dat,&acceleration,4);
	
		if(sdo_SendDat(busId,nodeId,sdoBuf,1000))
		{
			return 1;
		}
		return 0;
}

unsigned char deceleration_profile(int busId,int nodeId,unsigned int deceleration)
{
		struct sDO_OBJ sdoBuf={0x6084,0x00,{0,0,0,0},4};
		
		memcpy(sdoBuf.dat,&deceleration,4);
	
		if(sdo_SendDat(busId,nodeId,sdoBuf,1000))
		{
			return 1;
		}
		return 0;
}

unsigned char deceleration_QuickStop(int busId,int nodeId,unsigned int deceleration)
{
		struct sDO_OBJ sdoBuf={0x6085,0x00,{0,0,0,0},4};
		
		memcpy(sdoBuf.dat,&deceleration,4);
	
		if(sdo_SendDat(busId,nodeId,sdoBuf,1000))
		{
			return 1;
		}
		return 0;
}


unsigned char acceleration_Max(int busId,int nodeId,unsigned int acceleration)
{
		struct sDO_OBJ sdoBuf={0x60C5,0x00,{0,0,0,0},4};
		
		memcpy(sdoBuf.dat,&acceleration,4);
	
		if(sdo_SendDat(busId,nodeId,sdoBuf,1000))
		{
			return 1;
		}
		return 0;
}

unsigned char deceleration_Max(int busId,int nodeId,unsigned int deceleration)
{
		struct sDO_OBJ sdoBuf={0x60C6,0x00,{0,0,0,0},4};
		
		memcpy(sdoBuf.dat,&deceleration,4);
	
		if(sdo_SendDat(busId,nodeId,sdoBuf,1000))
		{
			return 1;
		}
		return 0;
}





unsigned char toeque_Max(int busId,int nodeId,unsigned short maxTorque)
{
		struct sDO_OBJ sdoBuf={0x6072,0x00,{0,0},2};
		
		memcpy(sdoBuf.dat,&maxTorque,2);
	
		if(sdo_SendDat(busId,nodeId,sdoBuf,1000))
		{
			return 1;
		}
		return 0;
}

unsigned char current_Max(int busId,int nodeId,unsigned short maxCurrent)
{
		struct sDO_OBJ sdoBuf={0x6073,0x00,{0,0},2};
		
		memcpy(sdoBuf.dat,&maxCurrent,2);
	
		if(sdo_SendDat(busId,nodeId,sdoBuf,1000))
		{
			return 1;
		}
		return 0;
}


unsigned char current_MotorRated(int busId,int nodeId,unsigned short ratedCurrent)
{
		struct sDO_OBJ sdoBuf={0x6075,0x00,{0,0},2};
		
		memcpy(sdoBuf.dat,&ratedCurrent,2);
	
		if(sdo_SendDat(busId,nodeId,sdoBuf,1000))
		{
			return 1;
		}
		return 0;
}


unsigned char torque_MotorRated(int busId,int nodeId,unsigned short ratedTorque)
{
		struct sDO_OBJ sdoBuf={0x6076,0x00,{0,0},2};
		
		memcpy(sdoBuf.dat,&ratedTorque,2);
	
		if(sdo_SendDat(busId,nodeId,sdoBuf,1000))
		{
			return 1;
		}
		return 0;
}
