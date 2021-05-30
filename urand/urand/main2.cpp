#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <random>
#include <cmath>
#include <chrono>

int main(int argc, const char** argv)
{
	int nMax = INT_MAX;
	int nMin = INT_MIN;
	if (argc >= 2)
	{
		nMax = atoi(argv[1]);
		if (nMax > 0)
		{
			nMin = 0;
		}
	}
	if (argc >= 3)
	{
		nMin = atoi(argv[2]);
		if (nMin == nMax)
		{
			printf("%d\n", nMax);
			return 0;
		}
		if (nMin > nMax)
		{
			int tmp = nMin;
			nMin = nMax;
			nMax = tmp;
		}
	}

	auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();

	std::mt19937_64 generator(seed);
	std::uniform_int_distribution<int> dis(nMin, nMax);
	//for (int i = 0;i < 1000;i++)
	{
		auto result = dis(generator);
		std::cout << result << std::endl;
	}
}