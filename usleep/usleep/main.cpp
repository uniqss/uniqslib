#include <windows.h>
#include <stdlib.h>
#pragma comment(lib,"kernel32.lib")

// ����: ָ���ڶ���Ϊ512�ֽ�
#pragma comment(linker, "/align:512")

// ����: �ϲ���
// ��.data�ں�.rdata�ںϲ���.text��(�����)
#pragma comment(linker, "/merge:.data=.text")
#pragma comment(linker, "/merge:.rdata=.text")

// ����: ָ����[ϵͳ](https://www.2cto.com/os/)Ϊwindows (���Ż��޹�)
// vc������Ĭ����console,���и��ں�����CMD����,���ÿ�.��windows�ͺ���
//#pragma comment(linker, "/subsystem:windows")

// ����: ָ����ں���
// ��ϵͳΪwindows��Ĭ����ڵ�WinMain��console��Ĭ����ڵ�main,��������

//#pragma comment(linker, "/ENTRY:main")

//int WinMain(HINSTANCE current, HINSTANCE prev, LPSTR cmdline, int
//showcmd)

// ����: ȥ��������ջ֡����,������ë���:-)
// ��������ͷ��push ebp / mov ebp, esp�ͽ�β��pop ebp / retn
//__declspec(naked)
int main(int argc, const char** argv)
{
	DWORD sleepMS = 1000;
	if (argc >= 2)
	{
		sleepMS = atoi(argv[1]);
	}
	if (sleepMS == 0)
	{
		sleepMS = 1000;
	}
	Sleep(sleepMS);

	ExitProcess(0);
	return 0;
}
