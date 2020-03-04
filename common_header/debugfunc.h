#pragma once

#if !defined(FUNC_ENTER)
#if 0
#define FUNC_ENTER printf("Enter function:%s\n", __FUNCTION__);
#define FUNC_LEAVE printf("Leave function:%s\n", __FUNCTION__);
#else
#define FUNC_ENTER ;
#define FUNC_LEAVE ;
#endif
#endif
