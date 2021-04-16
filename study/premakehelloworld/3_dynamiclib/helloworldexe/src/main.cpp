#include <iostream>
using namespace std;

#include "mathutility.h"

int main()
{
	int i = 123;
	int j = 111;
	int result = add(i, j);
	if (result != i + j)
	{
		cout << "hello world! Something went wrong!" << endl;
	}
	else
	{
		cout << "hello world! It works!" << endl;
	}
	return 0;
}
