#include "time.h"
#include "stdio.h"
int main(void)
{
    struct tm *local;
    time_t t;
    t=time(NULL);
    local=localtime(&t);
    printf("YYMMDD is: %4d%02d%02d",local->tm_year + 1900, local->tm_mon + 1, local->tm_mday);
    local=gmtime(&t);
    printf("UTC hour is: %d",local->tm_hour);
    return 0;
}