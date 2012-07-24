/* This code is based off of RFC 1321 - The MD5 Message Digest Algorithm */

typedef void* MD5_CTX;

int MD5_Init(MD5_CTX *c);
int MD5_Update(MD5_CTX *c, const void *data, unsigned long len);
int MD5_Final(unsigned char *md, MD5_CTX *c);
