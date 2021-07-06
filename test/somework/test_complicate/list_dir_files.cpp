#include "list_dir_files.h"

#include <iostream>
#include <io.h>

using namespace std;

int _listFiles(const string& dir, std::list<std::string>& files) {
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
			_listFiles(newDir.c_str(), files);
		}
		else
		{
			// 这就是一个文件了，只要对它加密就万事大吉了
			string file_path = dir + "\\" + fileinfo.name;
			cout << file_path.c_str() << endl;
			files.push_back(file_path);
		}

	} while (!_findnext(handle, &fileinfo));

	_findclose(handle);
	return 0;
}

int list_dir_files(const std::string& dir, std::list<std::string>& files)
{
	return _listFiles(dir, files);
}

