/* This code is based off of RFC 1321 - The MD5 Message Digest Algorithm */

typedef unsigned int u32;
typedef unsigned char u8;

typedef struct
{
  u32 bits[2];
  u32 scratch[4];
  u8  buffer[64];
  u8  digest[16];
} md5Context;

void md5Initialize(md5Context *m);
void md5Update(md5Context *m, u8 *inBuf, unsigned int inLen);
void md5Finalize(md5Context *m);

