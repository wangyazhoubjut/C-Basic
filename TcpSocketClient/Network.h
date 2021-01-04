//-------------------------------------------------------------------------------------------------
//Network.h
//-------------------------------------------------------------------------------------------------

#ifndef _NETWORK_H_
#define _NETWORK_H_

//-------------------------------------------------------------------------------------------------
//ȫ�ִ����붨��
//-------------------------------------------------------------------------------------------------
#define NET_NOERROR 					0	//û�д���
#define NET_INIT_ERROR				   -1	//API��ʼ������
#define NET_DATA_TOO_LONG			   -2	//����̫��(����1024�ֽ�)

#define MAX_LISTEN_NUM				    5
#define MAX_DATA_SIZE				 1024	//��������ֽ���
#define MAX_RECV_SIZE  MAX_DATA_SIZE + 16	//�������ֽ���

#define NET_MAGIC_STATIC	   0xFFFF55AA	//ħ��(����)_�̶�ֵ
#define NET_MAGIC_HEAD		   0x48454144	//ħ��(����)ͷ"HEAD"
#define NET_MAGIC_TAIL		   0x5441494C	//ħ��(����)β"TAIL"

//-------------------------------------------------------------------------------------------------
//�ṹ�嶨��
//-------------------------------------------------------------------------------------------------
//����˲���
typedef struct tag_LOCAL_SERVER_SOCKET
{
	int nServerSock;						//Server Socket
	int nServerPort;						//Server Port
	unsigned int dwServerAddr;				//Server IP
	bool bEnable;							//��������
	bool bBeatFlag;							//������־
} LOCAL_SERVER_SOCKET;

//�������
typedef struct tag_LOCAL_NETWORK_PARAM
{
	bool bHeartBeatEnable;					//�����������
	bool bEnable;							//��������(clientģʽ��)

	bool bBeatFlag;							//������־
	int nBeatTimeOut;						//��������ʱ�䣨0-���������, ����0-����ʱ�䣨�룩��
	int nServerSock;						//Server Socket
	int nServerPort;						//Server Port
	unsigned int dwServerAddr;				//Server IP

	int nIndex;								//socket���
	LOCAL_SERVER_SOCKET struServerSock[50];	//���ӵ����з����
} LOCAL_NETWORK_PARAM;

//�ײ����ݰ�
typedef struct tag_LOCAL_DATA_PACKAGE
{
	unsigned int dwMagic;					//���� 0xFF 0xFF 0x55 0xAA
	unsigned int dwHead;					//ͷ   0x48 0x45 0x41 0x44 

	unsigned int dwSize;					//byteBuffer���ݵĳ���
	unsigned char byteBuffer[MAX_DATA_SIZE];//ʵ������

	unsigned int dwTail;					//����β 0x54 0x41 0x49 0x4C
} LOCAL_DATA_PACKAGE;

//-------------------------------------------------------------------------------------------------
//TCP Socket���ͺͽ������ݣ������ַ�ʶ�𣬴�������ʶ�𣬾�����ʶ��
class NetworkTransmission
{
public:
	NetworkTransmission(void);
	~NetworkTransmission();
public:

	//��¼�豸
	int ClientLogin(char *sIPAddr, int nPort);

	//�ǳ��豸
	int ClientLogout(int nUserID);

	//�ͻ��˷�������
	int ClientDataSend(int nUserID, char *sBuffer, int nSize);

public:

	//TCP�ͻ���
	LOCAL_NETWORK_PARAM *m_pClientNetworkParam;
};

#endif
