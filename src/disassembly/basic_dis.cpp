#include <stdio.h>

#define NUMBER_COUNT 100

void add(int &nVar)
{
	nVar += 1;
}

int main()
{
/****************************/
//	int nVar = 0x12345678;
//
//	int &nVarType = nVar;
//
//	add(nVarType);
//
//	const int number_count = NUMBER_COUNT;
//
//	printf("number count=%d\n", NUMBER_COUNT);
/****************************/

/****************************/
	// �˴�������������Ż�
//	const int constvar = 100;
//	int &var = const_cast<int &>(constvar);
//	var += 1;
//	
//	int tempvar = constvar;
//
//	printf("constvar=%d\n", constvar);
	// �˴���ӡ��ֵ����100
/****************************/

/****************************/
	// �˴���������������Ż�
	int i = 0;
	scanf_s("%d", &i);

	const int j = i;

	int &tempj = const_cast<int &>(j);
	tempj += 1;
	printf("j=%d, tempj = %d\n", j, tempj);
	// �˴���ӡ��jֵ����tempj+1���ֵ
	
/****************************/

	return 0;
}

int main01()
{
	int var1 = 5, var2 = 10;
	// ���� - ����
	var2 = var1 - var2;
	printf("var2 = %d\n", var2);
	// ���� - ����
	var1 = var1 - 10;
	printf("var1 = %d\n", var1);

	// ���� - ����
	var1 = 20 - 10;

	printf("var1 = %d\n", var1);
	return 0;
}