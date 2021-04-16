
#if defined(WIN32)||defined(WINDOWS)||defined(_WIN32)
//#define UDLLEXPORT __declspec(dllexport) 
#define UDLLEXPORT 
#else
#endif
