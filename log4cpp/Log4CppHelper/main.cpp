#include "log4cpp_helper.h"

class FFF
{
public:
	void Test()
	{
		ULOG_NOTICE("alksdjflkasjdf");
	}
};

int main()
{
	Log4CppHelper_Init("FFF");

#if 1
	ULOG_DEBUG("this is debug log hello %s", "ffff");
	ULOG_DEBUG("this is debug log world %s %d", "ggggg", 111);
	ULOG_NOTICE("this is notice log asdffsdaf %s %d", "ggggg", 111);
	ULOG_WARNING("this is warning log ULOG_WARNING");
	ULOG_ERROR("this is error log ULOG_ERROR");
	ULOG_CRITICAL("this is critical log ULOG_CRITICAL");
	FFF oTmp;
	oTmp.Test();
#endif

	//g_pLog4CppHelperInstance = nullptr;
	return 0;
}

