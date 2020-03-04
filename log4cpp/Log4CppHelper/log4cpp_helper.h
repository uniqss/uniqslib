#pragma once

#include <log4cpp/Portability.hh>

#if (defined(WIN32)||defined(WINDOWS))
#include <windows.h>
#endif
#ifdef LOG4CPP_HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <iostream>
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <log4cpp/HierarchyMaintainer.hh>

#include <stdlib.h>
#include <crtdbg.h>

#include <memory>

class Log4CppHelper
{
public:
	/* config file name: log4cpp.strProcessName.property
	log file name:strProcessName.log */
	Log4CppHelper(const std::string& strProcessName);
	~Log4CppHelper();

public:
	log4cpp::Category* m_pCategoryDebug;
	log4cpp::Category* m_pCategoryNotice;
	log4cpp::Category* m_pCategoryWarning;
	log4cpp::Category* m_pCategoryError;
	log4cpp::Category* m_pCategoryCritical;
};

// must be initialized in your process.
extern std::unique_ptr<Log4CppHelper> g_pLog4CppHelperInstance;

void Log4CppHelper_Init(const char* pszProcessName);

#define ULOG_DEBUG(format, ...) {if(g_pLog4CppHelperInstance->m_pCategoryDebug)\
{std::string fmt = "[%d %s]"; fmt += format;\
g_pLog4CppHelperInstance->m_pCategoryDebug->debug(fmt.c_str(), __LINE__, __FUNCTION__, __VA_ARGS__);}}

#define ULOG_NOTICE(format, ...) {if(g_pLog4CppHelperInstance->m_pCategoryNotice)\
{std::string fmt = "[%d %s]"; fmt += format;\
g_pLog4CppHelperInstance->m_pCategoryNotice->notice(fmt.c_str(), __LINE__, __FUNCTION__, __VA_ARGS__);}}

#define ULOG_WARNING(format, ...) {if(g_pLog4CppHelperInstance->m_pCategoryWarning)\
{std::string fmt = "[%d %s]"; fmt += format;\
g_pLog4CppHelperInstance->m_pCategoryWarning->warn(fmt.c_str(), __LINE__, __FUNCTION__, __VA_ARGS__);}}

#define ULOG_ERROR(format, ...) {if(g_pLog4CppHelperInstance->m_pCategoryError)\
{std::string fmt = "[%d %s]"; fmt += format;\
g_pLog4CppHelperInstance->m_pCategoryError->error(fmt.c_str(), __LINE__, __FUNCTION__, __VA_ARGS__);}}

#define ULOG_CRITICAL(format, ...) {if(g_pLog4CppHelperInstance->m_pCategoryCritical)\
{std::string fmt = "[%d %s]"; fmt += format;\
g_pLog4CppHelperInstance->m_pCategoryCritical->crit(fmt.c_str(), __LINE__, __FUNCTION__, __VA_ARGS__);}}
