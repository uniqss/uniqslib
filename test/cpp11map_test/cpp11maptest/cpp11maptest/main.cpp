#include <map>
#include <unordered_map>
#include <iostream>
#include <chrono>
#include <string>

#include "map_test.h"

using namespace std;

class PoolObj
{
public:
	unsigned ID;
	char szTest[8824];
};

chrono::system_clock::time_point last_time;

int main()
{
#define TEST_OBJECT_COUNT 60000
#define TEST_MAP_COUNT 10

	//std::chrono::time_point<std::chrono::high_resolution_clock> t1 = std::chrono::high_resolution_clock::now();
	last_time = std::chrono::high_resolution_clock::now();

	PoolObj obj;
	cout << "std::map begin TEST_OBJECT_COUNT:" << TEST_OBJECT_COUNT << " TEST_MAP_COUNT:" << TEST_MAP_COUNT << endl;
	{
		map<unsigned, PoolObj> mapObjects[TEST_MAP_COUNT];

		map_test(mapObjects, TEST_OBJECT_COUNT, TEST_MAP_COUNT, obj);
	}
	cout << "std::map end =================================" << endl << endl << endl;


	cout << "std::unordered_map begin TEST_OBJECT_COUNT:" << TEST_OBJECT_COUNT << " TEST_MAP_COUNT:" << TEST_MAP_COUNT << endl;
	{
		unordered_map<unsigned, PoolObj> mapObjects[TEST_MAP_COUNT];
		map_test(mapObjects, TEST_OBJECT_COUNT, TEST_MAP_COUNT, obj);
	}
	cout << "std::unordered_map end =================================" << endl;

	return 0;
}
