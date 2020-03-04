
#include "hash_algo.h"
#include <ctype.h> // for toupper
#include <stdio.h> // for printf

void CHashAlgo::prepareCryptTable()
{
	unsigned long seed = 0x00100001, index1 = 0, index2 = 0, i;

	for( index1 =0; index1 < 0x100; index1++ )
	{
		for( index2 = index1, i = 0; i < 5; i++, index2 += 0x100 )
		{
			unsigned long temp1, temp2;
			seed = (seed * 125 + 3) % 0x2AAAAB;
			temp1 = (seed & 0xFFFF) << 0x10;
			seed = (seed * 125 + 3) % 0x2AAAAB;
			temp2 = (seed & 0xFFFF);
			cryptTable[index2] = ( temp1 | temp2 );
		}
	}
}

unsigned long CHashAlgo::HashString(const char*lpszFileName, unsigned long dwHashType)
{
	unsigned char*key = (unsigned char*)lpszFileName;
	unsigned long seed1 = 0x7FED7FED, seed2 = 0xEEEEEEEE;
	int ch;

	while(*key !=0)
	{
		//ch = toupper(*key++);
		ch = *key++;

		seed1 = cryptTable[(dwHashType << 8) + ch] ^ (seed1 + seed2);
		seed2 = ch + seed1 + seed2 + (seed2 << 5) +3;
	}
	return seed1;
}

unsigned long CHashAlgo::GetHashTablePos(const char*lpszString)
{
	const unsigned long HASH_OFFSET = 0, HASH_A = 1, HASH_B = 2;
	unsigned long nHash = HashString(lpszString, HASH_OFFSET);
	unsigned long nHashA = HashString(lpszString, HASH_A);
	unsigned long nHashB = HashString(lpszString, HASH_B);
	unsigned long nHashStart = nHash % m_tablelength;
	unsigned long nHashPos = nHashStart;

	while ( m_HashIndexTable[nHashPos].bExists)
	{
		if (m_HashIndexTable[nHashPos].nHashA == nHashA && m_HashIndexTable[nHashPos].nHashB == nHashB)
			return nHashPos;
		else
			nHashPos = (nHashPos + 1) % m_tablelength;

		if (nHashPos == nHashStart)
			break;
	}

	return -1; //没有找到
}
bool CHashAlgo::SetHashTable( const char* lpszString )
{
	const unsigned long HASH_OFFSET = 0, HASH_A = 1, HASH_B = 2;
	unsigned long nHash = HashString(lpszString, HASH_OFFSET);
	unsigned long nHashA = HashString(lpszString, HASH_A);
	unsigned long nHashB = HashString(lpszString, HASH_B);
	unsigned long nHashStart = nHash % m_tablelength,
				  nHashPos = nHashStart;

	int __debug__conflict = 0;
	while ( m_HashIndexTable[nHashPos].bExists)
	{
		++__debug__conflict;
		nHashPos = (nHashPos + 1) % m_tablelength;
		if (nHashPos == nHashStart)
		{
			return false;
		}
	}

	if(__debug__conflict > 0) printf("conflict %lu, %lu\n", nHashStart, __debug__conflict);

	m_HashIndexTable[nHashPos].bExists = true;
	m_HashIndexTable[nHashPos].nHashA = nHashA;
	m_HashIndexTable[nHashPos].nHashB = nHash;

	return true;
}

