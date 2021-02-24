#include <thread>
#include <chrono>
#include <stdlib.h>

int main(int argc, const char** argv)
{
	int64_t sleepMS = 1000;
	if (argc >= 2)
	{
		sleepMS = atoll(argv[1]);
		if (sleepMS == 0)
		{
			sleepMS = 1000;
		}
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(sleepMS));

	return 0;
}
