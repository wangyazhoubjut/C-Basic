/*

二分查找方法的思想：将数列按有序化(递增或递减)排列，查找过程中采用跳跃式方式查找，
即先以有序数列的中点位置为比较对象，如果要找的元素值小 于该中点元素，则将待查序列
缩小为左半部分，否则为右半部分。通过一次比较，将查找区间缩小一半。
最好的情况是：待查关键字刚好位于序列中间，第一次即查到。
最坏的情况是：第一次查找，还剩下n/2个元素需要比较；第二次，还剩n/4……第i次查找，
还剩下n/2^i个元素需要比较，直到剩余最后一个元素，查找结束。
最后n/2^i=1，即n=2^i，i为查找的次数（长度），i=log2n，所以选D。

判定树的高度，也就是为最坏一次查找时，需要比较的次数，所以为[log2n+1]

编程题：
给定一个整数sum,从有N个有序元素的数组中寻找元素a,b,
使得a+b的结果最接近sum,最快的平均时间复杂度是：

思想类似于两端向中间扫描
1、设定两个指针P1、P2，分别指向数组开始和结尾，即P1指向最小值，P2指向最大值；
2、计算 *P1+*P2 的值为 SUM，与 sum 比较，记录它们的差值 DIF 和 SUM，若 SUM<sum，P1++，
若SUM>sum，P2--;
3、重复以上过程直到DIF最小
*/
#include "stdafx.h"
#include<iostream>
#include <math.h>
using namespace std;
int computeTheResult(int intarray[], int arraysize, int sum)//输入sum及一个数组  然后返回最相近的结果
{
	 //1:为什么传长度啊
	int size = sizeof(intarray) / sizeof(int); //输出是1啊老弟 仅仅将数组首地址传递给f(),无法求出长度	                                        //（不知道何时结束
	cout << size << endl;

	int low = 0, high = arraysize - 1;
	int bestLow = low;
	int bestHigh = high;

	int tempSum = 0;
	int error = 0;
	int besrError = error;
	int diffError = 0;
	while (low<high)
	{
		tempSum = intarray[low] + intarray[high];
		error = tempSum - sum;
		if (abs(error)<abs(diffError))
		{
			bestHigh = high;
			bestLow = low;
			besrError = error;
		}
		if (error<0)
		{
			low++;
		}
		if (error ==0)
		{
			break;
		}
		if (error>0)
		{
			high--;
		}

		diffError = error;
	}
	cout << bestHigh << bestLow << endl;
	tempSum = intarray[bestHigh] + intarray[bestLow];
	return tempSum;
}

int binarySearch(int x[],int length, int n)
{
	cout << sizeof(x)  << endl;

	int left = 0; int right = length - 1;

	while (left<=right)
	{
		int mid = (left + right) / 2;
		if (n == x[mid])
		{
			return x[mid];
		}
		if (n>x[mid])
		{
			left = mid + 1;
		}
		else
		{
			right = mid - 1;
		}

	}
	return -1;

}

int  main()
{

	int array[] = {2, 3, 5,7};
	//cout << computeTheResult(array, 4, 7) << endl;;
	
	cout << binarySearch(array,4,7);
	return 0;
}