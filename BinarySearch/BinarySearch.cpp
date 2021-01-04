// BinarySearch.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream>
int data[] = { 2,3,5,8,20,50 }; //定义一个有序数组

int BinarySearch(int *a, int size, int target)
{
	int left = 0;
	int right = size - 1;

	int middle = (left + right) / 2;

//if (target == *(a+middle))
//{
//	return middle;
//}

while (left<= right)
{
	if (*(a+middle) > target)
	{
		right = middle - 1;
		middle = (left + right) / 2;
		//break;
	}
	if (*(a + middle) < target)
	{

		left = middle + 1;
		middle = (left + right) / 2;
		//break;
	}
	if (*(a + middle) == target)
	{
		break;
	}

}
if (left > right)
{
	std::cout << "not found" << std::endl;
}
return 0;

}

int main()
{
	int size = sizeof(data);

	BinarySearch(data, sizeof(data) / sizeof(int), 50);

    return 0;
}

