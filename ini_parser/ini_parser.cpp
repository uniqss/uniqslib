// cross-platform ini parser by uniqs
#include "ini_parser.h"

#include <string.h>
#include <stdlib.h>

#include <fstream>

CINIParser::CINIParser()
{
	m_mapSectionKeyValues.clear();
}

CINIParser::~CINIParser()
{
	m_mapSectionKeyValues.clear();
}

bool CINIParser::ReadFromFile(const char* pszFile)
{
	std::ifstream ifs(pszFile);

	const int nTmpBufSize = 2048;
	char szTmp[nTmpBufSize];
	std::string strSection;
	while (ifs.getline(szTmp, nTmpBufSize))
	{
		std::string strTmp = szTmp;
		if (strTmp.empty())
		{
			continue;
		}

		std::string::size_type pos_valid_end = strTmp.find(';');
		if (pos_valid_end != std::string::npos)
		{
			while (pos_valid_end > 0 && IsBlank(strTmp[pos_valid_end]))
			{
				--pos_valid_end;
			}
			strTmp = strTmp.substr(0, pos_valid_end);
		}
		else
		{
			pos_valid_end = strTmp.size() - 1;
		}
		std::string::size_type pos_valid_begin = 0;
		while (pos_valid_begin < pos_valid_end && IsBlank(strTmp[pos_valid_begin]))
		{
			++pos_valid_begin;
		}
		if (pos_valid_begin >= pos_valid_end)
		{
			continue;
		}

		std::string::size_type pos_section1 = strTmp.find('[');
		if (pos_section1 != std::string::npos)
		{
			std::string::size_type pos_section2 = strTmp.find(']');
			if (pos_section2 == std::string::npos)
			{
				ifs.close();
				return false;
			}
			if (pos_section2 < pos_section1)
			{
				ifs.close();
				return false;
			}
			strSection = strTmp.substr(pos_section1 + 1, pos_section2 - pos_section1 - 1);
			m_mapSectionKeyValues[strSection] = MAPKeyValues();
			continue;
		}

		std::string::size_type pos_split = strTmp.find('=');
		if (pos_split != std::string::npos)
		{
			std::string::size_type pos_key1 = 0;
			while (pos_key1 < pos_split && IsBlank(strTmp[pos_key1]))
			{
				++pos_key1;
			}
			std::string::size_type pos_key2 = pos_split - 1;
			while (pos_key2 > pos_key1 && IsBlank(strTmp[pos_key2]))
			{
				--pos_key2;
			}
			std::string strKey = strTmp.substr(pos_key1, pos_key2 + 1);

			std::string::size_type len = strTmp.size();
			std::string::size_type pos_value1 = pos_split + 1;
			while (pos_value1 < len - 1 && IsBlank(strTmp[pos_value1]))
			{
				++pos_value1;
			}
			std::string::size_type pos_value2 = len - 1;
			while (pos_value2 > pos_value1 && IsBlank(strTmp[pos_value2]))
			{
				--pos_value2;
			}
			std::string strValue = strTmp.substr(pos_value1, pos_value2);
			m_mapSectionKeyValues[strSection][strKey] = strValue;
			continue;
		}
		ifs.close();
		return false;
	}
	ifs.close();

	return true;
}

bool CINIParser::GetIniFileString(const char* pszSection, const char* pszKey, char szValue[], int nSize, const char* pszDefault)
{
	if (pszSection&& pszKey)
	{
		std::string strSection = pszSection;
		std::string strKey = pszKey;
		MAPSectionKeyValues::const_iterator itSection = m_mapSectionKeyValues.find(strSection);
		if (itSection != m_mapSectionKeyValues.end())
		{
			MAPKeyValues::const_iterator itKV = itSection->second.find(strKey);
			if (itKV != itSection->second.end())
			{
				strncpy(szValue, itKV->second.c_str(), nSize);

				return true;
			}
		}
	}

	strncpy(szValue, pszDefault, nSize);
	return false;
}

int CINIParser::GetIniFileInt(const char* pszSection, const char* pszKey, int nDefault)
{
	if (pszSection&& pszKey)
	{
		std::string strSection = pszSection;
		std::string strKey = pszKey;
		MAPSectionKeyValues::const_iterator itSection = m_mapSectionKeyValues.find(strSection);
		if (itSection != m_mapSectionKeyValues.end())
		{
			MAPKeyValues::const_iterator itKV = itSection->second.find(strKey);
			if (itKV != itSection->second.end())
			{
				return atoi(itKV->second.c_str());
			}
		}
	}

	return nDefault;
}
