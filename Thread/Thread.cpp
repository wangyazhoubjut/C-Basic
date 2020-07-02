// Thread.cpp : �������̨Ӧ�ó������ڵ㡣
//

//******************************************************//
/*
     �����ļ���Ҫ����ʾ��c++���߳�������ݣ������ռ��³�������ʱ��
	 ���ڴ�ռ�ֲ���

	 1����������ʱ�� �ڴ������ҳΪ��λ������ʹ�õĵ�ַ��Ϊ�����ַ
	    ��������Ҫʹ���ڴ�ʱ�򣬲���ϵͳ�������ַӳ�䵽��ʵ��������
		�棬�������ַ���������ݻ��ߴ�����һ�������ڴ��еģ�������
		�ݴ�ŵ���ͬ�Ķ��У�
		a��bss�� �ö��������û�б���ʼ�����߳�ʼ��Ϊ0��ȫ�ֱ���
		b��data��  ��ʼ������ȫ�ֱ������ݶ�
		c��rodata�� ��ų�������
		d������� ��ų������
		e��ջ�� ������ʱ�����ͺ������� ͨ����������
		f���Ѷ�  ͨ���ӵ͵�ַ��ߵ�ַ����

	2������һ���߳� ������Ҫ�����洫��һЩ������ ��ô������߳�����
	   ��ȷ�Ĵ��ݶ�����߲����أ�


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
}m_struct; //m_struct��ʵ��һ���ṹ�����

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

int ThreadWithParam(int& param,MyStruct& s)//�����в��ܴ������ã�����
{
	cout << "I get Struct,I can use Struct" << s.a << endl;
	param = s.a*10;
	return 0;
}

//�������ô���Ч��
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
	//std::thread t1(ThreadFun1); //����ִ����
	//std::thread t2(ThreadFun2); //����ִ����


	//�ⲿ����ṹ��
	//m_struct.a = 999;
	//m_struct.b = 's';

	//int m_expectValue; //����ָ�� Ȼ�����߳̿���ʹ���������������̴߳�����֮�����źŻ������߳���ȡֵ��
	//std::thread t3(ThreadWithParam, std::ref(m_expectValue),m_struct);
	
	//t3.join();
	//cout << "expectValue is " << m_expectValue;

	//t1.join(); //�ȴ��߳�1ִ�н���
	//t2.join(); //�ȴ��߳�2 ִ�н���
	
	//string strName("Old Name");
	//auto thHandle = std::thread(changeName, strName, "New name");
	//thHandle.join();
	//cout << "After name changed in thread, now become : " << strName << endl;
	////ʲô��û�иı�  ������˼


	Person rechar;
	rechar.m_strName = "rhical";
	rechar.m_nAge = 27;
	ChangePerson(rechar, "Liily");
	
	//�߳�ִ�ж����ò������޸�ʵ�����Ƕ���ĸ������е��޸ġ�
	/*
	    ʵ����һ�������ж���߳� ������ͬ�ĵ�ַ�ռ䣬��������ӵ�и��Զ�����ջ�ռ䣬һ���߳��ǲ���ֱ�ӷ���
		����һ���̵߳�ջ�ռ�ġ����������У��߳�ִ�к���ChangePerson�У�ʵ���������µ��߳�ջ�ռ䡣��������
		Ҫ�޸ĵ�Person����rechar��������ʵ�ʵ�ַ�������̵߳�ջ����ˣ����߳���ChangePerson������ִ��ʱ��
		�������ܷ��ʵ�ԭʼ��rechar��������ֻ�ܲ��ÿ����ķ�ʽ��
	
	
	  �����ȷ�Ĵ��䷽ʽΪ��1 std::ref�����ݲ����⣬2���ǻ����Բ���ָ�봫�ݵķ�ʽ
	  3�����̴߳����ŵ�һ������ջ���� û�н��в������ݹ��̣�ʹ��lamda���ʽ
	*/
	//�����Խ�


	//thHandle.join();

	system("pause");
    return 0;
}

/*
   �ܽ᣺

   ���ĸ�����Ҫ����������������߳�ִ��������Ĵ������⣺
   1��ֱ��ʹ�����ô��ݲ��ܴﵽԤ��Ŀ�ģ���Ҫʹ��std::ref����װ��Ҫ���ô��ݵĶ���
   2��ʹ��ָ����Դﵽ����������Ŀ�ģ�
   3���߳�ʹ��lambda���ʽͬ�����Դﵽ����������Ŀ�ġ�
*/