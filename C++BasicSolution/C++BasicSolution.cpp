/*

���ֲ��ҷ�����˼�룺�����а�����(������ݼ�)���У����ҹ����в�����Ծʽ��ʽ���ң�
�������������е��е�λ��Ϊ�Ƚ϶������Ҫ�ҵ�Ԫ��ֵС �ڸ��е�Ԫ�أ��򽫴�������
��СΪ��벿�֣�����Ϊ�Ұ벿�֡�ͨ��һ�αȽϣ�������������Сһ�롣
��õ�����ǣ�����ؼ��ָպ�λ�������м䣬��һ�μ��鵽��
�������ǣ���һ�β��ң���ʣ��n/2��Ԫ����Ҫ�Ƚϣ��ڶ��Σ���ʣn/4������i�β��ң�
��ʣ��n/2^i��Ԫ����Ҫ�Ƚϣ�ֱ��ʣ�����һ��Ԫ�أ����ҽ�����
���n/2^i=1����n=2^i��iΪ���ҵĴ��������ȣ���i=log2n������ѡD��

�ж����ĸ߶ȣ�Ҳ����Ϊ�һ�β���ʱ����Ҫ�ȽϵĴ���������Ϊ[log2n+1]

����⣺
����һ������sum,����N������Ԫ�ص�������Ѱ��Ԫ��a,b,
ʹ��a+b�Ľ����ӽ�sum,����ƽ��ʱ�临�Ӷ��ǣ�

˼���������������м�ɨ��
1���趨����ָ��P1��P2���ֱ�ָ�����鿪ʼ�ͽ�β����P1ָ����Сֵ��P2ָ�����ֵ��
2������ *P1+*P2 ��ֵΪ SUM���� sum �Ƚϣ���¼���ǵĲ�ֵ DIF �� SUM���� SUM<sum��P1++��
��SUM>sum��P2--;
3���ظ����Ϲ���ֱ��DIF��С
*/
#include "stdafx.h"
#include<iostream>
#include <math.h>
using namespace std;
int computeTheResult(int intarray[], int arraysize, int sum)//����sum��һ������  Ȼ�󷵻�������Ľ��
{
	 //1:Ϊʲô�����Ȱ�
	int size = sizeof(intarray) / sizeof(int); //�����1���ϵ� �����������׵�ַ���ݸ�f(),�޷��������	                                        //����֪����ʱ����
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