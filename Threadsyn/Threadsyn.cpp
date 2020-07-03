// Threadsyn.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

/*
        线程同步方式你知道的有哪几种？

		1：互斥量  简单点：一个锁就是一个互斥量，既然是锁
		   通过加锁>>>操作共享数据>>>解锁的方式，实现保护共享数据。
		   不加锁乱套  按照次序来输出 mutex.lock mutex.unlock
		2: std::lock_guard<mutex> myguard (my_mutex)来代替lock()和unlock(),
		 其原理是，在std::lock_guard构造函数里执行了lock(),在析构函数里执行
		 了unlock()。

        3:死锁  两把锁 线程A获取到了锁1尝试获取锁2 
		               线程B获取到了锁2尝试获取锁1

         解决方法：保持互斥量加锁顺序一致。或者使用lock（mutex1，mutex2）
		 同时上锁

*/

#include <iostream>
#include <mutex>
using namespace std;
int num = 0;
mutex my_mutex1;         //创建一个互斥量
mutex my_mutex2; //验证死锁

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
	thread thread_1(print1);      //打印0-49
	thread thread_2(print2);      //打印49-99
	thread_1.join();
	thread_2.join();
	system("pause");

    return 0;
}

