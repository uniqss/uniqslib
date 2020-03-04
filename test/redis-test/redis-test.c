#include <stdio.h>  
#include <stdlib.h>  
#include <stddef.h>  
#include <stdarg.h>  
#include <string.h>  
#include <assert.h>  
#include <hiredis/hiredis.h>  

void doTest()  
{  
	redisContext* c = redisConnect("127.0.0.1", 6379);  
	if ( c->err)  
	{  
		redisFree(c);  
		printf("Connect to redisServer faile\n");  
		return ;  
	}  
	printf("Connect to redisServer Success\n");  

#define TEST_BUF_SIZE 1024
	char szTest[TEST_BUF_SIZE];
	redisReply* r = NULL;

#if 1
	memset(szTest, 0, sizeof(szTest));
	{
		const char* command1 = "SADD player_nick_cache %llu %llu %llu";  
		snprintf(szTest, TEST_BUF_SIZE, command1, 123456, 123, 456);
	}
	printf("command:%s\n", szTest);
	r = (redisReply*)redisCommand(c, szTest);  
	if( NULL == r)  
	{  
		printf("NULL == r command[%s]\n", szTest);  
		redisFree(c);  
		return;  
	}
	printf("r->type:%d r->integer:%llu\n", r->type, r->integer);
	if(r->type == REDIS_REPLY_ERROR)
	{  
		printf("r->type == REDIS_REPLY_ERROR command:[%s]\n",szTest);  
		freeReplyObject(r);  
		redisFree(c);  
		return;  
	}
	freeReplyObject(r);  
	printf("========================================Succeeded to execute command[%s]\n", szTest);  
#endif
#if 1
	memset(szTest, 0, sizeof(szTest));
	{
		const char* command1 = "SADD player_nick_cache %llu";  
		snprintf(szTest, TEST_BUF_SIZE, command1, 789); 
	}
	printf("command:%s\n", szTest);
	r = (redisReply*)redisCommand(c, szTest);  
	if( NULL == r)  
	{  
		printf("NULL == r command[%s]\n", szTest);  
		redisFree(c);  
		return;  
	}
	printf("r->type:%d r->integer:%llu\n", r->type, r->integer);
	if(r->type == REDIS_REPLY_ERROR)
	{  
		printf("r->type == REDIS_REPLY_ERROR command:[%s]\n",szTest);  
		freeReplyObject(r);  
		redisFree(c);  
		return;  
	}
	freeReplyObject(r);  
	printf("========================================Succeeded to execute command[%s]\n", szTest);  
#endif
#if 1
	memset(szTest, 0, sizeof(szTest));
	{
		const char* command1 = "SADD player_nick_cache %llu";  
		snprintf(szTest, TEST_BUF_SIZE, command1, 123456);
	}
	printf("command:%s\n", szTest);
	r = (redisReply*)redisCommand(c, szTest);  
	if( NULL == r)  
	{  
		printf("NULL == r command[%s]\n", szTest);  
		redisFree(c);  
		return;  
	}
	printf("r->type:%d r->integer:%llu\n", r->type, r->integer);
	if(r->type == REDIS_REPLY_ERROR)
	{  
		printf("r->type == REDIS_REPLY_ERROR command:[%s]\n",szTest);  
		freeReplyObject(r);  
		redisFree(c);  
		return;  
	}
	freeReplyObject(r);  
	printf("========================================Succeeded to execute command[%s]\n", szTest);  
#endif

#if 1
	memset(szTest, 0, sizeof(szTest));
	{
		const char* command1 = "SISMEMBER player_nick_cache %llu";  
		snprintf(szTest, TEST_BUF_SIZE, command1, 123456);
	}
	printf("command:%s\n", szTest);
	r = (redisReply*)redisCommand(c, szTest);  
	if( NULL == r)  
	{  
		printf("NULL == r command[%s]\n", szTest);  
		redisFree(c);  
		return;  
	}  
	printf("r->type:%d r->integer:%llu\n", r->type, r->integer);
	if(r->type == REDIS_REPLY_ERROR)
	{  
		printf("r->type == REDIS_REPLY_ERROR command:[%s]\n",szTest);  
		freeReplyObject(r);  
		redisFree(c);  
		return;  
	}     
	freeReplyObject(r);  
	printf("========================================Succeeded to execute command[%s]\n", szTest);  
#endif
#if 1
	memset(szTest, 0, sizeof(szTest));
	{
		const char* command1 = "SISMEMBER player_nick_cache %llu";  
		snprintf(szTest, TEST_BUF_SIZE, command1, 7777);
	}
	printf("command:%s\n", szTest);
	r = (redisReply*)redisCommand(c, szTest);  
	if( NULL == r)  
	{  
		printf("NULL == r command[%s]\n", szTest);  
		redisFree(c);  
		return;  
	}  
	printf("r->type:%d r->integer:%llu\n", r->type, r->integer);
	if(r->type == REDIS_REPLY_ERROR)
	{  
		printf("r->type == REDIS_REPLY_ERROR command:[%s]\n",szTest);  
		freeReplyObject(r);  
		redisFree(c);  
		return;  
	}     
	freeReplyObject(r);  
	printf("========================================Succeeded to execute command[%s]\n", szTest);  
#endif

#if 1
	memset(szTest, 0, sizeof(szTest));
	{
		const char* command1 = "SREM player_nick_cache %llu";  
		snprintf(szTest, TEST_BUF_SIZE, command1, 7777);
	}
	printf("command:%s\n", szTest);
	r = (redisReply*)redisCommand(c, szTest);  
	if( NULL == r)  
	{  
		printf("NULL == r command[%s]\n", szTest);  
		redisFree(c);  
		return;  
	}  
	printf("r->type:%d r->integer:%llu\n", r->type, r->integer);
	if(r->type == REDIS_REPLY_ERROR)
	{  
		printf("r->type == REDIS_REPLY_ERROR command:[%s]\n",szTest);  
		freeReplyObject(r);  
		redisFree(c);  
		return;  
	}     
	freeReplyObject(r);  
	printf("========================================Succeeded to execute command[%s]\n", szTest);  
#endif
#if 1
	memset(szTest, 0, sizeof(szTest));
	{
		const char* command1 = "SREM player_nick_cache %llu %llu %llu %llu %llu";  
		snprintf(szTest, TEST_BUF_SIZE, command1, 123456, 123, 456, 789, 22222);
	}
	printf("command:%s\n", szTest);
	r = (redisReply*)redisCommand(c, szTest);  
	if( NULL == r)  
	{  
		printf("NULL == r command[%s]\n", szTest);  
		redisFree(c);  
		return;  
	}  
	printf("r->type:%d r->integer:%llu\n", r->type, r->integer);
	if(r->type == REDIS_REPLY_ERROR)
	{  
		printf("r->type == REDIS_REPLY_ERROR command:[%s]\n",szTest);  
		freeReplyObject(r);  
		redisFree(c);  
		return;  
	}     
	freeReplyObject(r);  
	printf("========================================Succeeded to execute command[%s]\n", szTest);  
#endif


	redisFree(c);  
}  

int main()  
{  
	doTest();  
	return 0;  
}  

