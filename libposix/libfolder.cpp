#include "libfolder.h"
#include<cstring>
#include<unistd.h>
#include<iostream>

int CreateDir(const char *sPathName)
{
	char DirName[256];
	strcpy(DirName, sPathName);
	int i,len = strlen(DirName);
	if(DirName[len-1]!='/')
		strcat(DirName, "/");

	len = strlen(DirName);

	for(i=1; i<len;i++)
	{
		if(DirName[i]=='/')
		{
			DirName[i] = 0;
			if(access(DirName, 0)!=0)
			{
				if(mkdir(DirName, 0755)==-1)
				{
					std::cout<<"mkdir error"<<std::endl;
					return -1;
				}
			}
			DirName[i] = '/';
		}
	}

	return 0;
}

