#include <iostream>
using namespace std;
typedef unsigned char BYTE;
#define MAX_COUNT 20

struct tagOutCardResult_py
{
	BYTE							cbCardCount;					
	BYTE							cbResultCard1;
	BYTE							cbResultCard2;
	BYTE							cbResultCard3;
	BYTE							cbResultCard4;
	BYTE							cbResultCard5;
	BYTE							cbResultCard6;
	BYTE							cbResultCard7;
	BYTE							cbResultCard8;
	BYTE							cbResultCard9;
	BYTE							cbResultCard10;
	BYTE							cbResultCard11;
	BYTE							cbResultCard12;
	BYTE							cbResultCard13;
	BYTE							cbResultCard14;
	BYTE							cbResultCard15;
	BYTE							cbResultCard16;
	BYTE							cbResultCard17;
	BYTE							cbResultCard18;
	BYTE							cbResultCard19;
	BYTE							cbResultCard20;
	int							ntest1;
	int							ntest2;
	int							ntest3;
};

struct tagOutCardResult
{
	BYTE							cbCardCount;					
	BYTE							cbResultCard[MAX_COUNT];		
	void clear()
	{
		cbCardCount = 0;
		for (int nIdx = 0;nIdx < MAX_COUNT;++nIdx)
		{
			cbResultCard[nIdx] = 0;
		}
	}	
	void topy(tagOutCardResult_py* ppy)
	{
		cout<<"topy function begin"<<endl;
		ppy->cbCardCount = cbCardCount;
		cout<<"topy function 1"<<endl;
		ppy->cbResultCard1 = cbResultCard[1 - 1];
		cout<<"topy function 2"<<endl;
		ppy->cbResultCard2 = cbResultCard[2 - 1];
		ppy->cbResultCard3 = cbResultCard[3 - 1];
		ppy->cbResultCard4 = cbResultCard[4 - 1];
		ppy->cbResultCard5 = cbResultCard[5 - 1];
		ppy->cbResultCard6 = cbResultCard[6 - 1];
		ppy->cbResultCard7 = cbResultCard[7 - 1];
		ppy->cbResultCard8 = cbResultCard[8 - 1];
		ppy->cbResultCard9 = cbResultCard[9 - 1];
		ppy->cbResultCard10 = cbResultCard[10 - 1];
		ppy->cbResultCard11 = cbResultCard[11 - 1];
		ppy->cbResultCard12 = cbResultCard[12 - 1];
		ppy->cbResultCard13 = cbResultCard[13 - 1];
		ppy->cbResultCard14 = cbResultCard[14 - 1];
		ppy->cbResultCard15 = cbResultCard[15 - 1];
		ppy->cbResultCard16 = cbResultCard[16 - 1];
		ppy->cbResultCard17 = cbResultCard[17 - 1];
		ppy->cbResultCard18 = cbResultCard[18 - 1];
		ppy->cbResultCard19 = cbResultCard[19 - 1];
		ppy->cbResultCard20 = cbResultCard[20 - 1];
		cout<<"topy function end"<<endl;
	}
};

class TestLib
{
	public:
		void display(tagOutCardResult& ret);
};
void TestLib::display(tagOutCardResult& ret) {
	ret.cbCardCount = 3;
	ret.cbResultCard[0] = 1;
	ret.cbResultCard[1] = 50;
	ret.cbResultCard[2] = 100;
	ret.ntest1 = 78901234;
	ret.ntest1 = 89012345;
	ret.ntest1 = 33333333;

	cout<<"First display aaa ";
	cout<<"hello ";
	cout<<"world ";
}

extern "C" {
	TestLib oGameLogic;
	void display(tagOutCardResult_py* ret_py) {
		tagOutCardResult oRet;
		oGameLogic.display(oRet);
		cout<<"before topy"<<endl;
		oRet.topy(ret_py);
		cout<<"after topy"<<endl;
		cout<<"in cpp:ret_py->cbCardCount:"<<ret_py->cbCardCount<<endl;
		cout<<"in cpp:ret_py->cbResultCard1:"<<ret_py->cbResultCard1<<endl;
		cout<<" this:" << ret_py << endl;
	}
}

