#include "../../common_header/mersenne_rand.h"

#include <chrono>
#include <thread>

int main(int argc, const char** argv)
{
	int nMax = 2147483647;
	int nMin = -2147483648;
	if (argc >= 2)
	{
		nMax = atoi(argv[1]);
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

	long long seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();

	//printf("seed:%llu \n", seed);

	mtsrand(seed);

	if (argc == 1)
	{
		auto result = mtrandi();
		printf("%llu\n", result);
		return 0;
	}
	else if (argc == 2)
	{
		auto result = mtrandi() % nMax;
		printf("%llu\n", result);
		return 0;
	}
	else
	{
		auto result = mtrandi() % (nMax - nMin) + nMin;
		printf("%llu\n", result);
		return 0;
	}

	return 0;
}
