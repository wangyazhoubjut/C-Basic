// Static.cpp : 定义控制台应用程序的入口点。
//

/*
        C++ static的作用
		
	1：	当同时编译多个文件时，
		所有未加static前缀的全局变量和函数都具有全局可见性。
		但是前提是你在本源文件要声明下哦  变量可以用extern  函数直接放外面

		如果定义为static的话 那么你在其他文件声明也不行啊  提示找不到定义
		你必须要定义 且可以和之前定义的static函数名字相同哈。

		这就是static的隐藏性

	2：C++标准定为全局或静态对象是有首次用到时才会进行构造
	   局部静态变量（一般为函数内的静态变量）
	   在第一次使用时分配内存并初始化  和C有点不同？？？

	   static局部变量在函数内定义，它的生存期为整个源程序，
	   但是其作用域仍与自动变量相同	

   3：默认初始化为0

   4：C++类成员函数声明为static   绑定线程需要哦
      a：无this指针 仅能访问类的静态数据和静态成员函数
	  b:不能将静态成员函数定义为虚函数  
	  虚函数依靠vptr和vtable来处理。vptr是一个指针，在类的构造函数中创建生成，
	  并且只能用this指针来访问它，因为它是类的一个成员，并且vptr指向保存虚函数地址的vtable.
	  对于静态成员函数，它没有this指针，所以无法访问vptr.
	  虚函数的调用关系：this -> vptr -> vtable ->virtual function
	  
	5：为什么类的成员函数不能作为回调函数？

	   由于this指针的作用，使得将一个CALLBACK型的成员函数作为回调函数安装
	   时就会因为隐含的this指针使得函数参数个数不匹配，
	   从而导致回调函数安装失败


	   创建线程传递的回调的话 其实就是回调函数啦 
	   1：声明为静态函数
	   2：在类内startthread  将this传进去


	  
	  */


#include "stdafx.h"
#include <iostream>

void msg()
{

	std::cout << "nihao" << std::endl;

}
int main()
{
	//extern char a; // extern variable must be declared before use
	//printf("%c ", a);
	(void)msg();
    return 0;
}

