// ��ӡ��hello world
/*
void print()
{
	// TODO �ڴ˴���д�����ӡ��hello word
}

void main()
{
}

*/

// ����˼·�����Ǹı�exe����ڵ�ַ��������ӡhello world ���
// ��ַ���ݵ�main��������ͨ��mainCRTStartup����main����
// ���ֱ����print�к�������printf�����������ʧ����Ϊio�ȵ�
// ��û������ʼ���Ĳ���

#include <iostream>

extern "C" int __cdecl mainCRTStartup(void);

int print()
{
	#pragma comment(linker, "/entry:print")	// ������ڴ�
	#pragma comment(linker, "/SECTION:.text,ERW")	//���ô���ɶ���д��ִ��
	#pragma comment(lib, "msvcrt.lib")	// ����c����ʱ��
	int mainCRTStartup();	//���� mainCRTStartup main ����
	void main();

	__asm
	{
		MOV EAX, OFFSET main
		MOV BYTE PTR[EAX], 0xB8 //MOV EAX, 0x      //mov
		MOV DWORD PTR[EAX + 1], OFFSET SHOWSTRING    //��printf����ַ����eax+1��
		MOV WORD PTR[EAX + 5], 0xE0FF // JMP EAX
	}

	mainCRTStartup();

	__asm
	{
		leave
		ret
	}

SHOWSTRING:
	printf("hello,world!\n");
	__asm
	{
		xor eax, eax
		ret
	}
}

int main()
{
}