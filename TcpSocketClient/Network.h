//-------------------------------------------------------------------------------------------------
//Network.h
//-------------------------------------------------------------------------------------------------

#ifndef _NETWORK_H_
#define _NETWORK_H_

//-------------------------------------------------------------------------------------------------
//全局错误码定义
//-------------------------------------------------------------------------------------------------
#define NET_NOERROR 					0	//没有错误
#define NET_INIT_ERROR				   -1	//API初始化错误
#define NET_DATA_TOO_LONG			   -2	//数据太长(超过1024字节)

#define MAX_LISTEN_NUM				    5
#define MAX_DATA_SIZE				 1024	//最大数据字节数
#define MAX_RECV_SIZE  MAX_DATA_SIZE + 16	//最大接收字节数

#define NET_MAGIC_STATIC	   0xFFFF55AA	//魔数(幻数)_固定值
#define NET_MAGIC_HEAD		   0x48454144	//魔数(幻数)头"HEAD"
#define NET_MAGIC_TAIL		   0x5441494C	//魔数(幻数)尾"TAIL"

//-------------------------------------------------------------------------------------------------
//结构体定义
//-------------------------------------------------------------------------------------------------
//服务端参数
typedef struct tag_LOCAL_SERVER_SOCKET
{
	int nServerSock;						//Server Socket
	int nServerPort;						//Server Port
	unsigned int dwServerAddr;				//Server IP
	bool bEnable;							//允许连接
	bool bBeatFlag;							//心跳标志
} LOCAL_SERVER_SOCKET;

//网络参数
typedef struct tag_LOCAL_NETWORK_PARAM
{
	bool bHeartBeatEnable;					//心跳检测允许
	bool bEnable;							//允许连接(client模式用)

	bool bBeatFlag;							//心跳标志
	int nBeatTimeOut;						//心跳设置时间（0-不检测心跳, 大于0-心跳时间（秒））
	int nServerSock;						//Server Socket
	int nServerPort;						//Server Port
	unsigned int dwServerAddr;				//Server IP

	int nIndex;								//socket序号
	LOCAL_SERVER_SOCKET struServerSock[50];	//连接的所有服务端
} LOCAL_NETWORK_PARAM;

//底层数据包
typedef struct tag_LOCAL_DATA_PACKAGE
{
	unsigned int dwMagic;					//幻数 0xFF 0xFF 0x55 0xAA
	unsigned int dwHead;					//头   0x48 0x45 0x41 0x44 

	unsigned int dwSize;					//byteBuffer数据的长度
	unsigned char byteBuffer[MAX_DATA_SIZE];//实际数据

	unsigned int dwTail;					//幻数尾 0x54 0x41 0x49 0x4C
} LOCAL_DATA_PACKAGE;

//-------------------------------------------------------------------------------------------------
//TCP Socket发送和接收数据，用于字符识别，船舶类型识别，救生衣识别
class NetworkTransmission
{
public:
	NetworkTransmission(void);
	~NetworkTransmission();
public:

	//登录设备
	int ClientLogin(char *sIPAddr, int nPort);

	//登出设备
	int ClientLogout(int nUserID);

	//客户端发送数据
	int ClientDataSend(int nUserID, char *sBuffer, int nSize);

public:

	//TCP客户端
	LOCAL_NETWORK_PARAM *m_pClientNetworkParam;
};

#endif
