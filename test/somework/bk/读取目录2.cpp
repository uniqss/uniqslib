#include <iostream>
#include <io.h>
 
using namespace std;
 
void listFiles(string dir);
 
int main() {
	//Ŀ¼·��
	string dir = "C:\\Users\\111\\Desktop\\�����ļ���";
	listFiles(dir);
	system("pause");
	return 1;
}
 
void listFiles(string dir) {
	//��Ŀ¼�������"\\*.*"���е�һ������
	string newDir = dir + "\\*.*";
	//���ڲ��ҵľ��
	intptr_t handle;
	struct _finddata_t fileinfo;
	//��һ�β���
	handle = _findfirst(newDir.c_str(), &fileinfo);
 
	if (handle == -1) {
		cout << "���ļ�" << endl;
		system("pause");
		return;
	}
 
	do
	{
		if (fileinfo.attrib & _A_SUBDIR) {//���Ϊ�ļ��У������ļ���·�����ٴα���
			if (strcmp(fileinfo.name, ".") == 0 || strcmp(fileinfo.name, "..") == 0)
				continue;
 
			// ��Ŀ¼�������"\\"����������Ŀ¼��������һ������
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

