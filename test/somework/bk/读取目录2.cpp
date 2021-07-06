#include <iostream>
#include <io.h>
 
using namespace std;
 
void listFiles(string dir);
 
int main() {
	//目录路径
	string dir = "C:\\Users\\111\\Desktop\\测试文件夹";
	listFiles(dir);
	system("pause");
	return 1;
}
 
void listFiles(string dir) {
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
		return;
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
		else{
			string file_path = dir + "\\" + fileinfo.name;
			cout << file_path.c_str() << endl;
		}
		
	} while (!_findnext(handle, &fileinfo));
 
	_findclose(handle);
	return;
}

