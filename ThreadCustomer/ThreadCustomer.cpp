// ThreadCustomer.cpp : 定义控制台应用程序的入口点。
//
/*
          本例利用C++11实现了一个单个生产者单个消费者的模型

		  其中利用到了uniqeMutex和convitional_varible进行线程的同步操作/



*/




#include "stdafx.h"
#include <iostream>           
#include <queue>
#include <thread>             
#include <mutex>              
#include <condition_variable> 

using namespace std;

mutex mtx;
condition_variable produce, consumer;//条件变量使是一种同步机制，
                                     //要和mutex以及lock一起使用
queue<int> q;     // shared value by producers and consumers, which is the critical section
int maxSize = 500;

void consumerThread()
{
	while (true)
	{
		//this_thread::sleep_for(chrono::milliseconds(1000));//客官先休息下
		unique_lock<mutex> lck(mtx); //RAII，程序运行到此block的外面（进入下一个while循环之前）
			                         //，资源（内存）自动释放
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

