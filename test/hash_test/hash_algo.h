#pragma once

#define MAXFILENAME 255     // 最大文件名长度
#define MAXTABLELEN 1024    // 默认哈希索引表大小
struct MPQHASHTABLE
{
	long nHashA;
	long nHashB;
	bool bExists;
};

class CHashAlgo
{
	public:
		CHashAlgo( const long nTableLength = MAXTABLELEN )// 创建指定大小的哈希索引表，不带参数的构造函数创建默认大小的哈希索引表
		{
			prepareCryptTable();
			m_tablelength = nTableLength;

			m_HashIndexTable =new MPQHASHTABLE[nTableLength];
			for ( int i =0; i < nTableLength; i++ )
			{
				m_HashIndexTable[i].nHashA =-1;
				m_HashIndexTable[i].nHashB =-1;
				m_HashIndexTable[i].bExists =false;
			}
		}
		void prepareCryptTable();                                              // 对哈希索引表预处理
		unsigned long HashString(const char*lpszFileName, unsigned long dwHashType); // 求取哈希值
		unsigned long GetHashTablePos(const char* lpszString );                      // 得到在定长表中的位置
		bool SetHashTable( const char*lpszString );                                  // 将字符串散列到哈希表中
		unsigned long GetTableLength(void){return m_tablelength;}
		void SetTableLength( const unsigned long nLength ){m_tablelength = nLength;}
		~CHashAlgo()
		{
			if ( nullptr != m_HashIndexTable )
			{
				delete []m_HashIndexTable;
				m_HashIndexTable = nullptr;
				m_tablelength =0;
			}
		}
	protected:
	private:
		unsigned long cryptTable[0x500];
		unsigned long m_tablelength;    // 哈希索引表长度
		MPQHASHTABLE *m_HashIndexTable;
};

