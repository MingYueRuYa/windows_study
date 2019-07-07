#include <stdio.h>
#include <stdlib.h>

#define NUMBER_COUNT 100

void add(int &nVar)
{
	nVar += 1;
}

int main(int argc, char *argv[])
{
	printf("%d %s\n", argc, argv);

	return 0;
}

int main02()
{
	int i=0;	
	int array[] = {1,2,3,4,5};

	for (int j=0;array[j] < 5; ++j) {
		printf("array[%d]=%d", j, array[j]);	
	}

	system("pause");
	return 0;
}

/*
	测试const常量
*/
int const_main()
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
	// 此处编译器会进行优化
//	const int constvar = 100;
//	int &var = const_cast<int &>(constvar);
//	var += 1;
//	
//	int tempvar = constvar;
//
//	printf("constvar=%d\n", constvar);
	// 此处打印的值还是100
/****************************/

/****************************/
	// 此处编译器不会进行优化
	int i = 0;
	scanf_s("%d", &i);

	const int j = i;

	int &tempj = const_cast<int &>(j);
	tempj += 1;
	printf("j=%d, tempj = %d\n", j, tempj);
	// 此处打印的j值就是tempj+1后的值
	
/****************************/
	system("pause");

	return 0;
}

int main01()
{
	int var1 = 5, var2 = 10;
	// 变量 - 变量
	var2 = var1 - var2;
	printf("var2 = %d\n", var2);
	// 变量 - 常量
	var1 = var1 - 10;
	printf("var1 = %d\n", var1);

	// 常量 - 常量
	var1 = 20 - 10;

	printf("var1 = %d\n", var1);
	return 0;
}