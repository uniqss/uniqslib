// cross-platform ini parser by uniqs
// main.cpp
#include "ini_parser.h"

#include <iostream>
using namespace std;

int main()
{
	CINIParser oParser;
	oParser.ReadFromFile("Scene.ini");

	const int nBufSize = 1024;
	char szTmp[nBufSize];
	if (oParser.GetIniFileString("scene", "SmallBird", szTmp, nBufSize, ""))
	{
		cout << szTmp << endl;
	}

	int ChestBird = oParser.GetIniFileInt("scene", "ChestBird", 0);

	cout << ChestBird << endl;

	return 0;
}

