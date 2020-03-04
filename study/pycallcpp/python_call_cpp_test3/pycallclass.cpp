#include <iostream>
using namespace std;
typedef unsigned char BYTE;
#define MAX_COUNT 20

struct ByteArray_20
{
	BYTE e1;
	BYTE e2;
	BYTE e3;
	BYTE e4;
	BYTE e5;
	BYTE e6;
	BYTE e7;
	BYTE e8;
	BYTE e9;
	BYTE e10;
	BYTE e11;
	BYTE e12;
	BYTE e13;
	BYTE e14;
	BYTE e15;
	BYTE e16;
	BYTE e17;
	BYTE e18;
	BYTE e19;
	BYTE e20;
};
struct ByteArray_20_3
{
	ByteArray_20 e1;
	ByteArray_20 e2;
	ByteArray_20 e3;
};

class TestLib
{
	public:
		void LogicFunc(ByteArray_20_3& ret);
};
void TestLib::LogicFunc(ByteArray_20_3& ret) {
	ret.e1.e1 = 3;
	ret.e1.e2 = 1;
	ret.e1.e3 = 50;
	ret.e2.e1 = 100;
	ret.e2.e2 = 200;
	ret.e2.e3 = 20;

	cout<<"TestLib::LogicFunc"<<endl;
}

extern "C" {
	TestLib oGameLogic;
	void display(ByteArray_20_3* pret) {
		cout<<"cpp display func begin"<<endl;
		oGameLogic.LogicFunc(*pret);
		cout<<"cpp display func end"<<endl;
	}
}
