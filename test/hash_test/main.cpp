#define TESTNUM (50 * 51 * 52 * 53)
#define TESTLOOP 100

#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include "hash_algo.h"
#include <string>
#include <sys/time.h>

#include <city.h>
#include "murmurhash.h"

using namespace std;

long getTimeUS()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}

int main( int argc, char**argv )
{
	long uslast = getTimeUS();
	long uscurr = getTimeUS();

#define PRINTTIME(str) uscurr = getTimeUS();\
	cout<<str<<uscurr<<"us diff:"<<uscurr - uslast<<"us"<<endl;\
	uslast = uscurr;

	PRINTTIME("before initialization:");

	vector<string> dic;
	do{
		string sks = "我人有的和主产不为这工要在地一上是中国经以发了民同金八月白禾言立水火之啊木大土王目日口田给又女子已山";
		cout<<sks.size()<<endl;
		for(int i = 0;i < sks.size() / 3;++i)
		{
			dic.push_back(sks.substr(i*3, 3));
		}
	}while(false);
#if 0
	for(int i = 0;i < dic.size();++i)
	{
		cout<<dic[i]<<endl;
	}
#endif

	list<string> ks;

	for(int i = 0;i < dic.size();++i)
	{
		/*
		   ks.push_back(dic[i]);
		   */
		for(int j = 0;j < dic.size();++j)
		{
			/*
			   string s2 = dic[i] + dic[j];
			   ks.push_back(s2);
			   */
			for(int k = 0;k < dic.size();++k)
			{
				/*
				   string s3 = dic[i] + dic[j] + dic[k];
				   ks.push_back(s3);
				   */
				for(int l = 0;l < dic.size();++l)
				{
					string s4 = dic[i] + dic[j] + dic[k] + dic[l];
					ks.push_back(s4);
					/*
					*/
					/*
					   for(int m = 0;m < dic.size();++m)
					   {
					   string s5 = dic[i] + dic[j] + dic[k] + dic[l] + dic[m];
					   ks.push_back(s5);
					 *                        for(int n = 0;n < dic.size();++n)
					 *                                               {
					 *                                                                      string s6 = dic[i] + dic[j] + dic[k] + dic[l] + dic[m] + dic[n];
					 *                                                                                             ks.push_back(s6);
					 *                                                                                                                    }
					 }
					 *                                                                                                                    */
				}
			}
		}
	}

	cout<<"size:"<<ks.size()<<endl;

	PRINTTIME("after initialization:");

#if 1
	for(auto idx = 0;idx < TESTLOOP;++idx)
		for(auto it = ks.begin();it != ks.end();++it)
			MurmurHash64A((*it).c_str(), (*it).size(), 32);

	PRINTTIME("MurmurHash64A:");
#endif

#if 1
	for(auto idx = 0;idx < TESTLOOP;++idx)
		for(auto it = ks.begin();it != ks.end();++it)
			CityHash64((*it).c_str(), (*it).size());

	PRINTTIME("CityHash64:");
#endif

#if 1
	CHashAlgo hash_test( ks.size() );

	PRINTTIME("blizzard hash initialize:");

	for(auto idx = 0;idx < TESTLOOP;++idx)
		for(auto it = ks.begin();it != ks.end();++it)
		{
			/*
			   bool is_success = hash_test.SetHashTable( (*it).c_str() );
			   if ( is_success )
			   {
			   }
			   else
			   {
			   cout <<"hash failed!"<< endl;
			   }
			   */
			hash_test.HashString( (*it).c_str(), 0 );
		}

	PRINTTIME("blizzard hash:");
#endif
#if 0
	bool is_success = hash_test.SetHashTable( "test" );
	if ( is_success )
	{
		cout <<"散列结果一：成功！"<< endl;
	}
	else
	{
		cout <<"散列结果一：失败！"<< endl;
	}

	is_success = hash_test.SetHashTable( "测试" );
	if ( is_success )
	{
		cout <<"散列结果二：成功！"<< endl;
	}
	else
	{
		cout <<"散列结果二：失败！"<< endl;
	}

	long pos = hash_test.GetHashTablePos( "test" );
	cout <<"查找测试字符串：\"test\" 的散列位置："<< pos << endl;
	pos = hash_test.GetHashTablePos( "测试" );
	cout <<"查找测试字符串：“测试” 的散列位置："<< pos << endl;

	for ( int i =0; i < TESTNUM; i++ )
	{
		char buff[32];
		sprintf(buff, "abcdefg%d.", i);
		is_success = hash_test.SetHashTable(buff);
		is_success ? cout << buff <<"散列结果：成功！位置："<< endl : cout << buff <<"散列结果：失败！"<< endl;
	}
#endif
#if 0
	int iii;
	cin>> iii;
	for ( int i =0; i < TESTNUM; i++ )
	{
		char buff[32];
		sprintf(buff, "abcdefg%d.", i);
		pos = hash_test.GetHashTablePos( buff );
		pos !=-1?  cout <<"查找测试字符串："<< buff <<" 的散列位置："<< pos << endl : cout << buff <<"存在冲突！"<< endl;
	}

	cin>> iii;
#endif

	return 0;
}

