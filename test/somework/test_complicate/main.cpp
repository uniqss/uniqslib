#include "simple_crypt.h"
#include "list_dir_files.h"


int main()
{

	std::string dir = "C:\\Users\\uniqs\\Desktop\\²âÊÔÎÄ¼ş¼Ğ";
	std::list<std::string> files;
	list_dir_files(dir, files);
	for (std::list<std::string>::iterator it = files.begin();it != files.end();++it)
	{
		simple_crypt(*it);
	}

	return 0;
}
