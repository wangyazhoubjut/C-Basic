// Thread.cpp : 定义控制台应用程序的入口点。
//

//******************************************************//
/*
     本个文件主要是演示了c++的线程相关内容；首先普及下程序运行时候
	 的内存空间分布。

	 1：程序运行时候 内存管理以页为单位；程序使用的地址都为虚拟地址
	    当程序需要使用内存时候，操作系统把虚拟地址映射到真实的物理内
		存，以虚拟地址来看，数据或者代码是一块块存在内存中的；各种数
		据存放到不同的段中；
		a）bss段 该段用来存放没有被初始化或者初始化为0的全局变量
		b）data段  初始化过的全局变量数据段
		c）rodata段 存放常量数据
		d）代码段 存放程序代码
		e）栈段 保存临时变量和函数参数 通常像下增长
		f）堆段  通常从低地址像高地址分配

	2：启动一个线程 我们需要往里面传递一些参数啊 那么如何在线程里面
	   正确的传递对象或者参数呢？


*/

#include "stdafx.h"
#include <Thread>
#include <iostream>
#include <chrono>
#include <string>
using namespace std;
int num = 0;

struct MyStruct
{
	int a;
	char b;
}m_struct; //m_struct其实是一个结构体变量

int ThreadFun1()
{
	std::cout << "Fun1 Thread ID" << endl << std::this_thread::get_id() << endl;;
	while (num<10000)
	{
		num++;

	  std::cout << "Fun1 Num" << num << endl;
	  std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	return 0;
}

int ThreadFun2()
{
	std::cout <<endl<< "Fun2 Thread ID" << endl << std::this_thread::get_id()<<endl;
	while (num < 10000)
	{
		num++;
		std::cout << "Fun2 Num" << num << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	return 0;
}

int ThreadWithParam(int& param,MyStruct& s)//参数中不能传递引用？？？
{
	cout << "I get Struct,I can use Struct" << s.a << endl;
	param = s.a*10;
	return 0;
}

//采用引用传递效果
void changeName(string& strOldName, const string& strNewName)
{
	std::cout << "Name passed in"<<strOldName<<endl;
	strOldName = strNewName;
	cout << "Now name changed to : " << strOldName << endl;
}

class Person
{
public:
	Person() = default;
	Person(const Person& rhs)
	{
		cout << "copy Constructer called!" << endl;

		m_strName = rhs.m_strName;
		m_nAge = rhs.m_nAge;

		cout << "copy from Person info,name:" << m_strName << ",age:" << m_nAge << endl;

	}
	~Person() = default;
public:
	string m_strName;
	int m_nAge;

};

void ChangePerson(Person& objPerson, const string& strNewName)
{
	auto tHandle = std::thread
	(
		[&]()
	{
		objPerson.m_strName = strNewName;
	}
	);
	tHandle.join();
		//auto thHandle = std::thread(ChangePerson, (rechar), "Lily");
}

int main()
{
	//std::thread t1(ThreadFun1); //立即执行了
	//std::thread t2(ThreadFun2); //立即执行了


	//外部定义结构体
	//m_struct.a = 999;
	//m_struct.b = 's';

	//int m_expectValue; //传递指针 然后主线程可以使用条件变量；子线程处理完之后发送信号唤醒主线程来取值。
	//std::thread t3(ThreadWithParam, std::ref(m_expectValue),m_struct);
	
	//t3.join();
	//cout << "expectValue is " << m_expectValue;

	//t1.join(); //等待线程1执行结束
	//t2.join(); //等待线程2 执行结束
	
	//string strName("Old Name");
	//auto thHandle = std::thread(changeName, strName, "New name");
	//thHandle.join();
	//cout << "After name changed in thread, now become : " << strName << endl;
	////什么？没有改变  几个意思


	Person rechar;
	rechar.m_strName = "rhical";
	rechar.m_nAge = 27;
	ChangePerson(rechar, "Liily");
	
	//线程执行对引用参数的修改实际上是对象的副本进行的修改。
	/*
	    实际上一个进程中多个线程 共用相同的地址空间，但是它们拥有各自独立的栈空间，一个线程是不能直接访问
		另外一个线程的栈空间的。以上例子中，线程执行函数ChangePerson中，实际上是在新的线程栈空间。而我们想
		要修改的Person对象rechar这个对象的实际地址是在主线程的栈上因此，当线程在ChangePerson函数中执行时，
		它并不能访问到原始的rechar对象，所以只能采用拷贝的方式。
	
	
	  因此正确的传输方式为：1 std::ref来传递参数外，2我们还可以采用指针传递的方式
	  3：将线程创建放到一个函数栈里面 没有进行参数传递过程，使用lamda表达式
	*/
	//还可以将


	//thHandle.join();

	system("pause");
    return 0;
}

/*
   总结：

   本文告诉主要讲了以下三点关于线程执行体参数的传递问题：
   1）直接使用引用传递不能达到预期目的，需要使用std::ref来封装需要引用传递的对象；
   2）使用指针可以达到我们期望的目的；
   3）线程使用lambda表达式同样可以达到我们期望的目的。
*/