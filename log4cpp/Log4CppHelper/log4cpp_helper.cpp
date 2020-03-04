#include "stdafx.h"
#include "log4cpp_helper.h"

std::unique_ptr<Log4CppHelper> g_pLog4CppHelperInstance;

Log4CppHelper::Log4CppHelper(const std::string& strProcessName)
{
	std::string strCfgFileName = "./cfg_log4cpp/log4cpp.";
	strCfgFileName += strProcessName;
	strCfgFileName += ".property";
	log4cpp::PropertyConfigurator::configure(strCfgFileName);

	m_pCategoryDebug = &log4cpp::Category::getInstance("cat_debug");
	m_pCategoryNotice = &log4cpp::Category::getInstance("cat_notice");
	m_pCategoryWarning = &log4cpp::Category::getInstance("cat_warning");
	m_pCategoryError = &log4cpp::Category::getInstance("cat_error");
	m_pCategoryCritical = &log4cpp::Category::getInstance("cat_critical");
}

Log4CppHelper::~Log4CppHelper()
{
	m_pCategoryDebug = nullptr;
	m_pCategoryNotice = nullptr;
	m_pCategoryWarning = nullptr;
	m_pCategoryError = nullptr;
	m_pCategoryCritical = nullptr;
	//log4cpp::Category::shutdownForced();
	//log4cpp::Category::shutdown();
}


void Log4CppHelper_Init(const char* pszProcessName)
{
	std::string strProcessName = pszProcessName;
	std::unique_ptr<Log4CppHelper> ptmp(new Log4CppHelper(strProcessName));
	g_pLog4CppHelperInstance = std::move(ptmp);
}
