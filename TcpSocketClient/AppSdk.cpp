/*
* File   name��AppSdk.cpp
*/
#include <stdio.h>
#include <errno.h>
#include <string>
#include <Winsock2.h>
#include "Network.h"
#include "AppSdk.h"
#include <map>
#include <iostream>
#include <string>
#pragma comment( lib, "ws2_32.lib" )  
extern fNetRecvCallBack g_fNetworkCallBack;
std::map<std::string, int> m_mapLoginID;

NetworkTransmission *g_pTcpDataComm = NULL;

//-----------------------------------------------------------------------------
//��ʼ��
//return:         APP_SDK_NOERROR
//-----------------------------------------------------------------------------
APP_SDK_API int __stdcall APP_SDK_Init(void)
{
	//�����׽��ֿ� 
	int err;
	WORD wVersionRequested;
	WSADATA wsaData;

	g_pTcpDataComm = new NetworkTransmission();

	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);

	if (err != 0)
	{
		return APP_SDK_INIT_ERROR;
	}

	return APP_SDK_NOERROR;
}

//-----------------------------------------------------------------------------
//�ͷ�
//return:         APP_SDK_NOERROR
//-----------------------------------------------------------------------------
APP_SDK_API int __stdcall APP_SDK_Cleanup(void)
{

	//��ֹ�׽��ֿ��ʹ��  
	WSACleanup();

	if (g_pTcpDataComm != NULL)
	{
		delete g_pTcpDataComm;
		g_pTcpDataComm = NULL;
	}

	return APP_SDK_NOERROR;
}

//-----------------------------------------------------------------------------
//���ݻص�����(����ˣ��ͻ��˹���)
//return:         APP_SDK_NOERROR
//-----------------------------------------------------------------------------
APP_SDK_API int __stdcall APP_SDK_NET_SetRecvCallBack(fNetRecvCallBack cbNetcallback)
{
	g_fNetworkCallBack = cbNetcallback;

	return APP_SDK_NOERROR;
}

//-----------------------------------------------------------------------------
//�ͻ��˵�¼
//char *sIPAddr     [in] sIPAddr   �����IP��ַ������:"192.168.1.64"
//int nPort         [in] nPort     ����˶˿ڣ�����: 5678   
//return:         APP_SDK_NOERROR
//-----------------------------------------------------------------------------
APP_SDK_API int __stdcall APP_SDK_NET_ClientLogin(char *sIPAddr, int nPort)
{
	return g_pTcpDataComm->ClientLogin(sIPAddr, nPort);

	//return APP_SDK_NOERROR;
}

//-----------------------------------------------------------------------------
//�ͻ��˵ǳ�
//return:         APP_SDK_NOERROR
//-----------------------------------------------------------------------------
APP_SDK_API int __stdcall APP_SDK_NET_ClientLogout(void)
{
	std::map<std::string, int>::iterator iter;
	for (iter = m_mapLoginID.begin(); iter != m_mapLoginID.end(); iter++)
	{

		g_pTcpDataComm->ClientLogout(iter->second);
		//APP_SDK_NET_ClientDataSend(iter->first, " ", 1);
		//GH_LOG_INFO("%d, %s", iter->first, iter->second);
	}

	return APP_SDK_NOERROR;
}

//-----------------------------------------------------------------------------
//�ͻ��˷�������
//char *sBuffer  [in] sBuffer    ���͵�����
//int  nSize     [in] nSize      ���ݵĳ���
//return:         APP_SDK_NOERROR
//-----------------------------------------------------------------------------
APP_SDK_API int __stdcall APP_SDK_NET_ClientDataSend(int loginID, char *sBuffer, int nSize)
{
	return g_pTcpDataComm->ClientDataSend(loginID, sBuffer, nSize); //asia

																	//return APP_SDK_NOERROR;
}

