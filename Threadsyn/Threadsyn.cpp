// Threadsyn.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

/*
        �߳�ͬ����ʽ��֪�������ļ��֣�

		1��������  �򵥵㣺һ��������һ������������Ȼ����
		   ͨ������>>>������������>>>�����ķ�ʽ��ʵ�ֱ����������ݡ�
		   ����������  ���մ�������� mutex.lock mutex.unlock
		2: std::lock_guard<mutex> myguard (my_mutex)������lock()��unlock(),
		 ��ԭ���ǣ���std::lock_guard���캯����ִ����lock(),������������ִ��
		 ��unlock()��

        3:����  ������ �߳�A��ȡ������1���Ի�ȡ��2 
		               �߳�B��ȡ������2���Ի�ȡ��1

         ������������ֻ���������˳��һ�¡�����ʹ��lock��mutex1��mutex2��
		 ͬʱ����

*/

#include <iostream>
#include <mutex>
using namespace std;
int num = 0;
mutex my_mutex1;         //����һ��������
mutex my_mutex2; //��֤����

void print1()
 {
	 for (int i = 0; i < 500; i++)
	   {
		   //my_mutex.lock();
		 std::lock_guard<mutex> myguard1(my_mutex1);


		 std::lock_guard<mutex> myguard2(my_mutex2);

		// std::lock(my_mutex1, my_mutex2);
		   cout << "thread_1:" << num++ << endl;
		  // my_mutex.unlock();
		   //my_mutex1.unlock();
		   //my_mutex2.unlock();
	   }
 }

void print2()
{
	for (int i = 0; i < 500; i++)
	{
		std::lock_guard<mutex> myguard1(my_mutex1);
		std::lock_guard<mutex> myguard2(my_mutex2);
		//std::lock(my_mutex1, my_mutex2);
		
		//my_mutex.lock();
		cout << "thread_2:" << num++ << endl;
		//my_mutex.unlock();

		//my_mutex1.unlock();
		//my_mutex2.unlock();
	}
}

int main()
{
	thread thread_1(print1);      //��ӡ0-49
	thread thread_2(print2);      //��ӡ49-99
	thread_1.join();
	thread_2.join();
	system("pause");

    return 0;
}

