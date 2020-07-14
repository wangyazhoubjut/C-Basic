// AsyncTask.cpp : 定义控制台应用程序的入口点。
//


/*
      在前面工程代码中 我们定义了各种线程的操作。但是使用不是很方便啊

	  比如我希望获取线程函数的返回结果的时候，我们不能通过join返回结果

	  我们必须在外面定义变量 然后线程函数取给变量赋值然后join
	  最后得到结果啊，这个过程比较繁琐

	  C++11提供了异步接口std::async，通过这个变量它返回一个std::future，
	  这个future中存储了线程函数返回的结果，
	  当我们需要线程函数的结果时，直接从future中获取

	  解耦了线程的创建和执行。可以将线程进行延迟加载

	  future：std::future提供了一种访问异步操作结果的机制
	  1：同步等待 2：查询future状态获取异步操作的结果







*/


#include "stdafx.h"
#include <iostream>

int main()
{
	//std::cout << _cplusplus << std::endl;
    return 0;
}

