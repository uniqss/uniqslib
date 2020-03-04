#include <map>
#include <iostream>
#include <chrono>
#include <string>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

extern chrono::system_clock::time_point last_time;

#define PRINT_TIME(str) do { auto curr_time = std::chrono::high_resolution_clock::now();\
	std::cout << str << " : " << std::chrono::duration_cast<std::chrono::milliseconds>(curr_time - last_time).count() << endl;\
	last_time = curr_time; } while (false);

int g_nTestTmp = 0;

template<class the_map, class Obj>
void map_test(the_map mapObjects[], const unsigned nTestObjectCount, const unsigned nTestMapCount, Obj& obj)
{

	PRINT_TIME("construct");

	for (unsigned idx = 0; idx < nTestObjectCount; ++idx)
	{
		obj.ID = idx;
		memset(obj.szTest, 0, sizeof(obj.szTest));

		for (size_t mapid = 0; mapid < nTestMapCount; ++mapid)
		{
			mapObjects[mapid][idx] = obj;
		}
	}
	PRINT_TIME("add cost");

	for (unsigned idx = 0; idx < nTestObjectCount; ++idx)
	{
		obj.ID = idx;
		memset(obj.szTest, 0, sizeof(obj.szTest));

		for (size_t mapid = 0; mapid < nTestMapCount; ++mapid)
		{
			typename the_map::iterator it = mapObjects[mapid].find(idx);
			if (it->second.ID == g_nTestTmp)
			{
				g_nTestTmp = 1;
			}
		}
	}
	PRINT_TIME("find cost");

	for (unsigned idx = 0; idx < nTestObjectCount; ++idx)
	{
		for (size_t mapid = 0; mapid < nTestMapCount; ++mapid)
		{
			mapObjects[mapid].erase(idx);
		}
	}
	PRINT_TIME("erase cost");


	for (unsigned idx = 0; idx < nTestObjectCount; ++idx)
	{
		obj.ID = idx;
		memset(obj.szTest, 0, sizeof(obj.szTest));

		for (size_t mapid = 0; mapid < nTestMapCount; ++mapid)
		{
			mapObjects[mapid][idx] = obj;
		}
	}
	PRINT_TIME("add cost");

	for (unsigned idx = 0; idx < nTestObjectCount; ++idx)
	{
		obj.ID = idx;
		memset(obj.szTest, 0, sizeof(obj.szTest));

		for (size_t mapid = 0; mapid < nTestMapCount; ++mapid)
		{
			typename the_map::iterator it = mapObjects[mapid].find(idx);
			if (it->second.ID == g_nTestTmp)
			{
				g_nTestTmp = 1;
			}
		}
	}
	PRINT_TIME("find cost");

	for (unsigned idx = 0; idx < nTestObjectCount; ++idx)
	{
		for (size_t mapid = 0; mapid < nTestMapCount; ++mapid)
		{
			mapObjects[mapid].erase(idx);
		}
	}
	PRINT_TIME("erase cost");


	for (unsigned idx = 0; idx < nTestObjectCount; ++idx)
	{
		obj.ID = idx;
		memset(obj.szTest, 0, sizeof(obj.szTest));

		for (size_t mapid = 0; mapid < nTestMapCount; ++mapid)
		{
			mapObjects[mapid][idx] = obj;
		}
	}
	PRINT_TIME("add cost");

	for (unsigned idx = 0; idx < nTestObjectCount; ++idx)
	{
		obj.ID = idx;
		memset(obj.szTest, 0, sizeof(obj.szTest));

		for (size_t mapid = 0; mapid < nTestMapCount; ++mapid)
		{
			typename the_map::iterator it = mapObjects[mapid].find(idx);
			if (it->second.ID == g_nTestTmp)
			{
				g_nTestTmp = 1;
			}
		}
	}
	PRINT_TIME("find cost");

	for (unsigned idx = 0; idx < nTestObjectCount; ++idx)
	{
		for (size_t mapid = 0; mapid < nTestMapCount; ++mapid)
		{
			mapObjects[mapid].erase(idx);
		}
	}
	PRINT_TIME("erase cost");
}
