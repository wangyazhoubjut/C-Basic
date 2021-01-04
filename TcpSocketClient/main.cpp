#include<iostream>
#include "AppSdk.h"
#include <windows.h>
#include <thread>
using namespace std;

//回调函数，接收服务器发来的数据
void static NetRecvCallback(APP_SDK_ALARMER *pAlarmer, char *buffer, unsigned int dwSize)
{
	int nPort;
	char sDeviceIP[16];				//服务器IP地址
	strncpy_s(sDeviceIP, pAlarmer->sDeviceIP, 16);
	// 	if (dwSize == sizeof(Send_INFO))
	// 	{

	//	}
	//下面处理数据  
	//char *buffer, 
	//unsigned int dwSize
	return;
}

void InitTcp()
{
	//初始化SDK
	APP_SDK_Init();
	APP_SDK_NET_SetRecvCallBack(NetRecvCallback);

	//登录服务器
	APP_SDK_NET_ClientLogin("127.0.0.1", 9396);

}

bool static TCPThread()
{

	while (1)
	{
		APP_SDK_NET_ClientDataSend(2," ", 1);
		Sleep(10000);
	}
	return false;

}



int main()
{
	InitTcp();

     std::thread heartThread(TCPThread);
	 heartThread.detach();
	//CreateThread(0, 0, TCPThread, 0, 0, 0);


	cout << "Hello Socket" << endl;

	return 0;

}