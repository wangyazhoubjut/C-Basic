#include<iostream>
#include "AppSdk.h"
#include <windows.h>
#include <thread>
using namespace std;

//�ص����������շ���������������
void static NetRecvCallback(APP_SDK_ALARMER *pAlarmer, char *buffer, unsigned int dwSize)
{
	int nPort;
	char sDeviceIP[16];				//������IP��ַ
	strncpy_s(sDeviceIP, pAlarmer->sDeviceIP, 16);
	// 	if (dwSize == sizeof(Send_INFO))
	// 	{

	//	}
	//���洦������  
	//char *buffer, 
	//unsigned int dwSize
	return;
}

void InitTcp()
{
	//��ʼ��SDK
	APP_SDK_Init();
	APP_SDK_NET_SetRecvCallBack(NetRecvCallback);

	//��¼������
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