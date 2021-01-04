/*
* File   name：AppSdk.cpp
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
//初始化
//return:         APP_SDK_NOERROR
//-----------------------------------------------------------------------------
APP_SDK_API int __stdcall APP_SDK_Init(void)
{
	//加载套接字库 
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
//释放
//return:         APP_SDK_NOERROR
//-----------------------------------------------------------------------------
APP_SDK_API int __stdcall APP_SDK_Cleanup(void)
{

	//终止套接字库的使用  
	WSACleanup();

	if (g_pTcpDataComm != NULL)
	{
		delete g_pTcpDataComm;
		g_pTcpDataComm = NULL;
	}

	return APP_SDK_NOERROR;
}

//-----------------------------------------------------------------------------
//数据回调函数(服务端，客户端共用)
//return:         APP_SDK_NOERROR
//-----------------------------------------------------------------------------
APP_SDK_API int __stdcall APP_SDK_NET_SetRecvCallBack(fNetRecvCallBack cbNetcallback)
{
	g_fNetworkCallBack = cbNetcallback;

	return APP_SDK_NOERROR;
}

//-----------------------------------------------------------------------------
//客户端登录
//char *sIPAddr     [in] sIPAddr   服务端IP地址，例如:"192.168.1.64"
//int nPort         [in] nPort     服务端端口，例如: 5678   
//return:         APP_SDK_NOERROR
//-----------------------------------------------------------------------------
APP_SDK_API int __stdcall APP_SDK_NET_ClientLogin(char *sIPAddr, int nPort)
{
	return g_pTcpDataComm->ClientLogin(sIPAddr, nPort);

	//return APP_SDK_NOERROR;
}

//-----------------------------------------------------------------------------
//客户端登出
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
//客户端发送数据
//char *sBuffer  [in] sBuffer    发送的数据
//int  nSize     [in] nSize      数据的长度
//return:         APP_SDK_NOERROR
//-----------------------------------------------------------------------------
APP_SDK_API int __stdcall APP_SDK_NET_ClientDataSend(int loginID, char *sBuffer, int nSize)
{
	return g_pTcpDataComm->ClientDataSend(loginID, sBuffer, nSize); //asia

																	//return APP_SDK_NOERROR;
}

