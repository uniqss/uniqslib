#include <stdlib.h>
#include "umd5.h"

#include <iostream>


int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cout << "umd5sum filename [32|16]" << std::endl;
		return 0;
	}
	int md5_len = 32;
	if (argc >= 3)
	{
		std::string strTmp = argv[2];
		if (strTmp == "16")
		{
			md5_len = 16;
		}
	}

	char *md5;
	md5 = MD5_file(argv[1], md5_len);
	std::cout << md5 << std::endl;
	free(md5);

	return 0;
}
