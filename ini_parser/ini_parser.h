// cross-platform ini parser

#include <string>

#if 0
#define INI_FILE_USE_C11
#endif
#ifdef INI_FILE_USE_C11
#include <unordered_map>
#else
#include <map>
#endif

class CINIParser
{
#ifdef INI_FILE_USE_C11
	typedef std::unordered_map<std::string, std::string> MAPKeyValues;
	typedef std::unordered_map < std::string, MAPKeyValues > MAPSectionKeyValues;
#else
	typedef std::map<std::string, std::string> MAPKeyValues;
	typedef std::map < std::string, MAPKeyValues > MAPSectionKeyValues;
#endif

	MAPSectionKeyValues m_mapSectionKeyValues;

	bool IsBlank(char c){ return c == ' ' || c == '\t'; }

public:
	CINIParser();
	~CINIParser();

public:
	bool ReadFromFile(const char* pszFile);
	bool GetIniFileString(const char* pszSection, const char* pszKey, char szValue[], int nSize, const char* pszDefault);
	int GetIniFileInt(const char* pszSection, const char* pszKey, int nDefault);
};
