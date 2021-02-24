#include <windows.h>
#include <stdlib.h>
#pragma comment(lib,"kernel32.lib")

// 作用: 指定节对齐为512字节
#pragma comment(linker, "/align:512")

// 作用: 合并节
// 将.data节和.rdata节合并到.text节(代码节)
#pragma comment(linker, "/merge:.data=.text")
#pragma comment(linker, "/merge:.rdata=.text")

// 作用: 指定子[系统](https://www.2cto.com/os/)为windows (和优化无关)
// vc编译器默认是console,会有个黑糊糊的CMD窗口,不好看.用windows就好了
//#pragma comment(linker, "/subsystem:windows")

// 作用: 指定入口函数
// 子系统为windows的默认入口点WinMain和console的默认入口点main,都会引入

//#pragma comment(linker, "/ENTRY:main")

//int WinMain(HINSTANCE current, HINSTANCE prev, LPSTR cmdline, int
//showcmd)

// 作用: 去掉函数的栈帧代码,纯属吹毛求疵:-)
// 即函数开头的push ebp / mov ebp, esp和结尾的pop ebp / retn
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
