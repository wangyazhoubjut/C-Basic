//#include "stdafx.h"
#include "Network.h"

#include "AppSdk.h"

#include <WS2tcpip.h>

#include <windows.h>
#include <process.h>
fNetRecvCallBack g_fNetworkCallBack = NULL;

//TCP Socket发送和接收数据，用于字符识别，船舶类型识别，救生衣识别
NetworkTransmission::NetworkTransmission(void)
{
	//Client
	m_pClientNetworkParam = new LOCAL_NETWORK_PARAM;
	memset(m_pClientNetworkParam, 0, sizeof(LOCAL_NETWORK_PARAM));
}

NetworkTransmission::~NetworkTransmission()
{
	//Client
	if (m_pClientNetworkParam != NULL)
	{
		delete m_pClientNetworkParam;
		m_pClientNetworkParam = NULL;
	}
}

//-----------------------------------------------------------------------------
//检查数据合法性
//-----------------------------------------------------------------------------
int check_magic(unsigned char *sBuffer, int nBytes)
{
	int i;
	int nSize;
	LOCAL_DATA_PACKAGE *pstruDataPackage;

	for (i = 0; i < nBytes; i++)
	{
		if ((sBuffer[i] == 0x0FF) && (sBuffer[i + 1] == 0x0FF) && (sBuffer[i + 2] == 0x055) && (sBuffer[i + 3] == 0x0AA)) //magic
		{
			nSize = nBytes - i;

			if (nSize >= MAX_RECV_SIZE)
			{
				pstruDataPackage = (LOCAL_DATA_PACKAGE *)sBuffer;

				//检查数据合法性
				if ((pstruDataPackage->dwHead == htonl(NET_MAGIC_HEAD)) && (pstruDataPackage->dwTail == htonl(NET_MAGIC_TAIL)) && (pstruDataPackage->dwSize <= MAX_DATA_SIZE))
				{
					return i;
				}
			}
			else       //数据长度不够，继续接收
			{
				return i;
			}
		}
	}

	return -1;
}

/*********************************************************
Function:	SetConnectThread
Desc:		set socket connect thread
Input:		pParam, parameter pointer
Output:
Return:
**********************************************************/
static void *SetClientConnectThread(void *lpParam)
{
	//int i;
	int nIndex;
	int err;
	int nSize;
	int nPort;
	int nOffset;

	int connect_sock;
	sockaddr_in hostaddr;
	unsigned int addr;
	char sIPAddr[16];

	unsigned int dwDataLen;
	unsigned int dwAisDataLen;

	unsigned char sRecvBuf[MAX_RECV_SIZE * 2];
	unsigned char byteDataBuf[MAX_RECV_SIZE * 4];
	unsigned char byteCallData[MAX_RECV_SIZE * 2];		//回调数据

	APP_SDK_ALARMER struAlarmer;

	LOCAL_NETWORK_PARAM *pLocalNetworkClientParam;
	LOCAL_DATA_PACKAGE *pstruDataPackage;

	if (lpParam == NULL)
	{
		_endthread();
		return NULL;
	}

	pLocalNetworkClientParam = (LOCAL_NETWORK_PARAM *)lpParam;

	nIndex = pLocalNetworkClientParam->nIndex;

	nPort = pLocalNetworkClientParam->struServerSock[nIndex].nServerPort;
	addr = pLocalNetworkClientParam->struServerSock[nIndex].dwServerAddr;
	connect_sock = pLocalNetworkClientParam->struServerSock[nIndex].nServerSock;

	inet_ntop(AF_INET, &addr, sIPAddr, 16);		//ip地址

	memset(&hostaddr, 0, sizeof(hostaddr));
	hostaddr.sin_family = AF_INET;
	hostaddr.sin_port = htons(nPort);
	hostaddr.sin_addr.S_un.S_addr = addr;

	//设置报警设备IP
	struAlarmer.nIndex = 0;
	struAlarmer.nPort = nPort;
	strncpy_s(struAlarmer.sDeviceIP, 16, sIPAddr, 16);

	while (pLocalNetworkClientParam->struServerSock[nIndex].bEnable)
	{
		if (!pLocalNetworkClientParam->struServerSock[nIndex].bBeatFlag)
		{
			connect_sock = socket(AF_INET, SOCK_STREAM, 0);
			if (connect_sock < 0)
			{
				closesocket(connect_sock);
				pLocalNetworkClientParam->struServerSock[nIndex].bBeatFlag = FALSE;
				continue;
			}

			err = connect(connect_sock, (sockaddr *)&hostaddr, sizeof(sockaddr_in));
			if (err < 0)
			{
				closesocket(connect_sock);
				pLocalNetworkClientParam->struServerSock[nIndex].bBeatFlag = FALSE;
				Sleep(500);
				continue;
			}

			pLocalNetworkClientParam->struServerSock[nIndex].nServerSock = connect_sock;		//socket
			pLocalNetworkClientParam->struServerSock[nIndex].bBeatFlag = TRUE;				//连接上服务端
		}

		dwDataLen = 0;

		while (1)
		{
			nSize = recv(connect_sock, (char *)sRecvBuf, MAX_RECV_SIZE, 0);

			if (nSize <= 0)
			{
				closesocket(connect_sock);
				pLocalNetworkClientParam->struServerSock[nIndex].bBeatFlag = FALSE;
				break;
			}

			memcpy(&byteDataBuf[dwDataLen], sRecvBuf, nSize);
			dwDataLen += nSize;

			nOffset = check_magic(byteDataBuf, dwDataLen);//'\n'

			if ((dwDataLen - nOffset) >= MAX_RECV_SIZE) //找到有效数据
			{
				memcpy(byteCallData, byteDataBuf + nOffset, MAX_RECV_SIZE);

				dwDataLen -= (nOffset + MAX_RECV_SIZE);
				if (dwDataLen != 0)
				{
					memcpy(byteDataBuf, byteDataBuf + nOffset + MAX_RECV_SIZE, dwDataLen);
				}

				if (g_fNetworkCallBack != NULL)
				{
					pstruDataPackage = (LOCAL_DATA_PACKAGE *)byteCallData;

					//运行回调函数
					g_fNetworkCallBack(&struAlarmer, (char *)pstruDataPackage->byteBuffer, pstruDataPackage->dwSize);
				}
			}
			else
			{
				if (nOffset > 0)	//数据长度不够，继续接收
				{
					memcpy(byteDataBuf, byteDataBuf + nOffset, dwDataLen - nOffset);
				}
				else if (nOffset < 0)//清除无效数据
				{
					dwDataLen = 0;
				}
			}
		}
	}

	_endthread();
	return NULL;
}

//-------------------------------------------------------------------------------------------------
//PC端TCP Client模式
//-------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//客户端模式登录
//-----------------------------------------------------------------------------
int NetworkTransmission::ClientLogin(char *sIPAddr, int nPort)
{
	int i;
	int nUserID;
	int err;
	int connect_sock;
	sockaddr_in hostaddr;
	in_addr addr;
	LOCAL_NETWORK_PARAM *lpParam = m_pClientNetworkParam;

	inet_pton(AF_INET, sIPAddr, (void *)&addr);

	memset(&hostaddr, 0, sizeof(hostaddr));
	hostaddr.sin_family = AF_INET;
	hostaddr.sin_port = htons(nPort);
	hostaddr.sin_addr.S_un.S_addr = addr.S_un.S_addr;

	connect_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (connect_sock < 0)
	{
		closesocket(connect_sock);
		return -1;
	}

	err = connect(connect_sock, (sockaddr *)&hostaddr, sizeof(sockaddr_in));
	if (err < 0)
	{
		closesocket(connect_sock);
		return -2;
	}

	nUserID = -1;

	for (i = 0; i < 50; i++)
	{
		if (m_pClientNetworkParam->struServerSock[i].dwServerAddr == 0)
		{
			nUserID = i;
			m_pClientNetworkParam->nIndex = i;
			m_pClientNetworkParam->struServerSock[i].nServerSock = connect_sock;
			m_pClientNetworkParam->struServerSock[i].dwServerAddr = addr.S_un.S_addr; //IP addr
			m_pClientNetworkParam->struServerSock[i].nServerPort = nPort;
			m_pClientNetworkParam->struServerSock[i].bEnable = TRUE;
			m_pClientNetworkParam->struServerSock[i].bBeatFlag = TRUE;
			break;
		}
	}

	_beginthread((void(__cdecl *)(void *))SetClientConnectThread, 0, lpParam);

	return nUserID;
}

//-----------------------------------------------------------------------------
//客户端模式登出
//-----------------------------------------------------------------------------
int NetworkTransmission::ClientLogout(int nUserID)
{
	m_pClientNetworkParam->struServerSock[nUserID].bEnable = FALSE;				//允许断开
	closesocket(m_pClientNetworkParam->struServerSock[nUserID].nServerSock);
	m_pClientNetworkParam->struServerSock[nUserID].dwServerAddr = 0;			//IP addr
	m_pClientNetworkParam->struServerSock[nUserID].nServerSock = 0;			//AppSock
	m_pClientNetworkParam->struServerSock[nUserID].nServerPort = 0;
	m_pClientNetworkParam->struServerSock[nUserID].bBeatFlag = FALSE;

	return NET_NOERROR;
}

//-----------------------------------------------------------------------------
//客户端发送数据
//-----------------------------------------------------------------------------
int NetworkTransmission::ClientDataSend(int nUserID, char *sBuffer, int nSize)
{
	int ret;
	int nLength;
	int nServerSock;
	unsigned int addr;

	LOCAL_DATA_PACKAGE struDataPackage;

	if (nSize > MAX_DATA_SIZE)
	{
		return NET_DATA_TOO_LONG;
	}

	addr = m_pClientNetworkParam->struServerSock[nUserID].dwServerAddr;			//IP addr

	struDataPackage.dwMagic = htonl(NET_MAGIC_STATIC);	//nMagic
	struDataPackage.dwHead = htonl(NET_MAGIC_HEAD);	//"HEAD"
	struDataPackage.dwSize = nSize;					//nSize
	struDataPackage.dwTail = htonl(NET_MAGIC_TAIL);	//"TAIL"

	nLength = sizeof(LOCAL_DATA_PACKAGE);

	memset(&struDataPackage.byteBuffer[0], 0, MAX_DATA_SIZE);
	memcpy(&struDataPackage.byteBuffer[0], sBuffer, nSize);

	if (addr != 0)
	{
		if (m_pClientNetworkParam->struServerSock[nUserID].bBeatFlag)
		{

			nServerSock = m_pClientNetworkParam->struServerSock[nUserID].nServerSock;

			ret = send(nServerSock, (char *)&struDataPackage, nLength, 0);
		}
	}

	return NET_NOERROR;
}
