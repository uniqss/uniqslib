#pragma once


typedef unsigned long int UINT4;

typedef struct {
	UINT4 i[2];
	UINT4 buf[4];
	unsigned char in[64];
	unsigned char digest[16];
} MD5_CTX;

void MD5Init(MD5_CTX *mdContext);
void MD5Update(MD5_CTX *mdContext, unsigned char *inBuf, unsigned int inLen);
void MD5Final(MD5_CTX *mdContext);
static void Transform(UINT4 *buf, UINT4 *in);
char *MD5_file(const char *path, int md5_len);
