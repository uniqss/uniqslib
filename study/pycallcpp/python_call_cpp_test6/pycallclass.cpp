
#if defined(WIN32)||defined(WINDOWS)
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif



#include <iostream>
#include<map>
#include<stdint.h>
#include<cstring>
using namespace std;
typedef unsigned char BYTE;

struct ByteArray_20
{
	BYTE e1;
	BYTE e2;
	BYTE e3;
};

class TestLib
{
	BYTE m_array[3];
public:
	TestLib()
	{
		memset(this, 0, sizeof(*this));
	}
	public:
		void LogicFunc(BYTE ret[]);
		void print();
};
void TestLib::LogicFunc(BYTE ret[]) {
	for(int nIdx = 0;nIdx < 3;++nIdx)
	{
		m_array[nIdx] = ret[nIdx];
	}

	cout<<"TestLib::LogicFunc"<<endl;
}
void TestLib::print()
{
	for(int nIdx = 0;nIdx < 3;++nIdx)
	{
		std::cout<<(int)m_array[nIdx]<<"\t";
	}
	std::cout<<endl;
}

extern "C" {
typedef std::map<int, TestLib> MAPGAMELOGICS;
	static MAPGAMELOGICS g_GameLogics;
	int DLLEXPORT init(int nIdx) {
		cout<<"cpp init func begin"<<endl;
		if (g_GameLogics.find(nIdx) != g_GameLogics.end()) return -1;
		TestLib oTmp;
		g_GameLogics[nIdx] = oTmp;
		cout<<"cpp init func end"<<endl;
		return 0;
	}
	int DLLEXPORT test_cpp(int nIdx, BYTE pret[3], int64_t int64test1, int64_t int64test2, int64_t* int64ret1, int64_t* int64ret2) {
		cout<<"cpp test_cpp func begin "<<int64test1<<" "<<int64test2<<endl;
		MAPGAMELOGICS::iterator it = g_GameLogics.find(nIdx);
		if (it == g_GameLogics.end()) return -1;
		it->second.LogicFunc(pret);
		*int64ret1 = 11223344556677;
		*int64ret2 = 22334455667788;
		cout<<"cpp test_cpp func end "<<int64test1<<" "<<int64test2<<endl;
		return 0;
	}
	int DLLEXPORT p(int nIdx)
	{
		cout<<"cpp print begin "<<nIdx<<endl;
		MAPGAMELOGICS::iterator it = g_GameLogics.find(nIdx);
		if (it == g_GameLogics.end()) return -1;
		it->second.print();
		cout<<"cpp print end "<<nIdx<<endl;
		return 0;
	}
}
