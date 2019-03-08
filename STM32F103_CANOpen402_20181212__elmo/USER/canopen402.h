#ifndef _CANOPEN402_H_
#define _CANOPEN402_H_



//#define DEDUG_SHOW 1


//pdo配置时，对应的对象字典地址只能按顺序使用，最大的地址为objAddress+TPDO_NUM
//如TPDO_NUM为3，则可使用的PDO对象字典地址为1400h-1402h，对RPDO_NUM同理

#define CAN_CLIENT_NUM 4  //user define:how many CAN client device
#define TPDO_NUM 4         //user define:how many TPDO used(server send)
#define RPDO_NUM 4         //user define:how many RPDO used(client send)
#define MASTER_NODE 0x40	//user define: master node id



#define HEARTBEAR_TIME 3000 //ms


#define FRAME_TYPE_DATA 0
#define REAME_TYPE_REMOTE 1

#define CAN1000K 0x1400
#define CAN800K  0x1600
#define CAN500K  0x1c00
#define CAN250K  0x1c01
#define CAN100K  0x2f43
#define CAN50K   0x2f47


//NMT状态
#define NMT_OPERATIONAL			0x01
#define NMT_STOP		    		0x02
#define NMT_PREOPERATIONAL	0x80
#define NMT_RESETAPP				0x81 //reset-application,reset object 0x2000--0xffff
#define NMT_RESETCOMM				0x82 //reset-communication，reset object 0x1000--0x1fff

//control word
#define SERVO_2  0x06
#define SERVO_3  0x07
#define SERVO_4  0x0f
#define SERVO_5  0x07
#define SERVO_6  0x06
#define SERVO_7  0x00
#define SERVO_8  0x06
#define SERVO_9  0x00
#define SERVO_10 0x02
#define SERVO_11 0x02
#define SERVO_12 0x00
#define SERVO_13 
#define SERVO_14 
#define SERVO_15 
#define SERVO_16 0x0f


//status word
#define UNKNOW                0x00
#define NOT_READY_SWITCH_ON   0x01
#define SWITCH_ON_DISABLE     0x02
#define READY_TO_SWITCH_ON    0x03
#define SWITCH_ON             0x04
#define OPERATION_ENABLE      0x05
#define QUICT_STOP_ACTIVE     0x06
#define FAULT_REACTION_ACTIVE 0x07
#define FAULT                 0x08


//redefine CAN frame struct
//unsigned int frame
//unsigned frameType data:0  remote:1
//unsigned char data[8] data
//unsigned char dataLen data length
struct cAN_FRAME{
	unsigned int frameId;
	unsigned char frameType;
	unsigned char dat[8];
	unsigned char datLen;
	unsigned char flag;
};

//heartbeat struct
//如果接收到心跳报文，则清空标志位，更新结构体的时间，错误标志清零
//如果未接收到心跳报文，则计算累计时间
//    如果超过报文发送时间，则错误标志位加一
//    如果没有超过报文发送时间，则跳过该次轮询
struct cAN_HEARTBEAT_FRAME
{
	unsigned int frameId;
	unsigned char state;
	unsigned long int useTime;
	unsigned char flag;
	unsigned char errCnt;
};


/*********************************/
/***define send sdo struct**/
/*********************************/
//index    
//subIndex 
//dat      data
//datLen   data length
struct sDO_OBJ
{
	unsigned int index;
	unsigned int subIndex;
	unsigned char dat[4];
	unsigned char datLen;
};


//define PDO data struct
struct pDO_DATA
{
	unsigned char dat[8];
	unsigned char flag;
};

//define PDO configure struct
struct pDOCFG_STRUCT
{
	int pdoIndex;
	int pdoMode;
	unsigned int datAddress[8];
	char addressNum;
};


extern struct cAN_FRAME SdoRevBuf;
extern struct cAN_HEARTBEAT_FRAME HeartBeatBuf[CAN_CLIENT_NUM];
extern int NodeId[CAN_CLIENT_NUM];
extern char EmergencyBuff[CAN_CLIENT_NUM][8];
extern struct pDO_DATA RPdoBuf[RPDO_NUM][CAN_CLIENT_NUM];
extern struct cAN_FRAME TPdoBuf[TPDO_NUM][CAN_CLIENT_NUM];


void show_FrameInfo(unsigned char *str,unsigned int id,unsigned char len,unsigned char *dat);
int clientNodeInfo(unsigned char *nodeId,unsigned char num);
int can_DatReceive(int busId,struct cAN_FRAME canBuf);
int can_DatSend(int busId, const struct  cAN_FRAME canDatBuf);
void nmt_StateSet(int busId,int nodeId,int nodeState);
void nmt_StateResetApplication(int busId,int nodeId);
void nmt_StateResetCommunication(int busId,int nodeId);
void nmt_StatePreOperational(int busId,int nodeId);
void nmt_StateOperational(int busId,int nodeId);
void nmt_StateStop(int busId,int nodeId);
void sync_Send(int busId);
int sdo_SendDat(int busId, int nodeId,const struct sDO_OBJ sdoBuf,unsigned long  int time);
int sdo_RequestDat(int busId, int nodeId,struct sDO_OBJ *sdoBuf, unsigned long int time);
int disable_PDO(int busId,int nodeId,unsigned int pdoObject);
int pdo_Config(int busId,int nodeId,const struct pDOCFG_STRUCT pdoBuf);
unsigned char show_ServoStatus(unsigned short statusWord);
unsigned int pdo_SingleSend(unsigned int busId,int nodeId,unsigned int object,unsigned char *dat,unsigned char len);
int sdo_DriverStatusChange(int busId, int nodeId,const short statusSet,short statusCheck);
int pdo_StatusCheck(unsigned char deviceIndex,unsigned char pdoNum,unsigned char syncCnt,short StatusCheck);
unsigned char fault_Check(int busId,int nodeId);


void communication_Init(int busId);
unsigned char heartbeat_ClientSend(int busId,int nodeId,unsigned int time);
unsigned char heartbeat_MasterSend(int busId,int nodeId,unsigned int time);
void master_HeartbeatSend(int busId);
unsigned char driver_RunMode(int busId,int nodeId,unsigned char mode);

unsigned char position_RangeLimitMin(int busId,int nodeId,int minP);
unsigned char position_RangeLimitMax(int busId,int nodeId,int maxP);
unsigned char position_SoftwareLimitMin(int busId,int nodeId,int minP);
unsigned char position_SoftwareLimitMax(int busId,int nodeId,int maxP);
unsigned char speed_MotorMax(int busId,int nodeId,unsigned int maxV);
unsigned char veolcity_profileMAX(int busId,int nodeId,unsigned int velocity);
unsigned char veolcity_profile(int busId,int nodeId,unsigned int velocity);
unsigned char acceleration_profile(int busId,int nodeId,unsigned int acceleration);
unsigned char deceleration_profile(int busId,int nodeId,unsigned int deceleration);
unsigned char deceleration_QuickStop(int busId,int nodeId,unsigned int deceleration);
unsigned char acceleration_Max(int busId,int nodeId,unsigned int acceleration);
unsigned char deceleration_Max(int busId,int nodeId,unsigned int deceleration);


unsigned char toeque_Max(int busId,int nodeId,unsigned short maxTorque);
unsigned char current_Max(int busId,int nodeId,unsigned short maxCurrent);
unsigned char current_MotorRated(int busId,int nodeId,unsigned short ratedCurrent);
unsigned char torque_MotorRated(int busId,int nodeId,unsigned short ratedTorque);



#endif
