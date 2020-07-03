// ThreadCustomer.cpp : �������̨Ӧ�ó������ڵ㡣
//
/*
          ��������C++11ʵ����һ�����������ߵ��������ߵ�ģ��

		  �������õ���uniqeMutex��convitional_varible�����̵߳�ͬ������/



*/




#include "stdafx.h"
#include <iostream>           
#include <queue>
#include <thread>             
#include <mutex>              
#include <condition_variable> 

using namespace std;

mutex mtx;
condition_variable produce, consumer;//��������ʹ��һ��ͬ�����ƣ�
                                     //Ҫ��mutex�Լ�lockһ��ʹ��
queue<int> q;     // shared value by producers and consumers, which is the critical section
int maxSize = 500;

void consumerThread()
{
	while (true)
	{
		//this_thread::sleep_for(chrono::milliseconds(1000));//�͹�����Ϣ��
		unique_lock<mutex> lck(mtx); //RAII���������е���block�����棨������һ��whileѭ��֮ǰ��
			                         //����Դ���ڴ棩�Զ��ͷ�
		consumer.wait(lck, [] {return q.size() != 0; });

		//wait(block) consumer until q.size() != 0 is true
		cout << "consumer " << this_thread::get_id() << ": ";

		cout << q.front() << endl;
		q.pop();
		cout << q.size() << '\n';
		produce.notify_all();
		//nodify(wake up) producer when q.size() != maxSize is true
	}


}

void producerThread()
{
	for (int i=0;i<100;i++)
	{
		//this_thread::sleep_for(chrono::milliseconds(900));
		// producer is a little faster than consumer  
		unique_lock<mutex> lck(mtx);
		produce.wait(lck, [] {return q.size() != maxSize; });
		//wait(block) producer until q.size() != maxSize is true
		cout << "-> producer " << this_thread::get_id() << ": ";
		q.push(i);
		cout << q.size() << '\n';
		consumer.notify_all();
		// notify(wake up) consumer when q.size() != 0 is true
	}
	
}


int main()
{
	


	thread produces(producerThread);
	
    thread consumers(consumerThread);
	

	produces.join();
	consumers.join();


	//thread producers();

	// spawn 2 consumers and 2 producers:
	/*for (int i = 0; i < 2; ++i)
	{
		consumers[i] = thread(consumerThread);
		producers[i] = thread(producerThread, i + 1);
	}*/

	// join them back: (in this program, never join...)
	/*for (int i = 0; i < 2; ++i)
	{
		producers[i].join();
		consumers[i].join();
	}*/
	system("pause");
    return 0;
}

