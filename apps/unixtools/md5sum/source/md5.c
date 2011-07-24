/* This code is based off of RFC 1321 - The MD5 Message Digest Algorithm */
#include "md5.h"

static void Transform(u32 *buf, u32 *in);
static inline int ror(int v, int sh)
{   
  __asm__ __volatile__(
    "ror  %[result], %[operand], %[shift]"
    : [result]"=r"(v)       // Output registers
    : [operand]"r"(v), [shift]"r"(sh)
  );

  return v;
}

static u8 PADDING[64] =
{
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

#define F(x, y, z) (((x) & (y)) | ((~(x)) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~(z))))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~(z)))) 

#define FF(a, b, c, d, x, s, ac) \
  {(a) += F((b), (c), (d)) + (x) + (ac); \
   (a) = ror((a), 32-(s)); \
   (a) += (b); \
  }
#define GG(a, b, c, d, x, s, ac) \
  {(a) += G((b), (c), (d)) + (x) + (ac); \
   (a) = ror((a), 32-(s)); \
   (a) += (b); \
  }
#define HH(a, b, c, d, x, s, ac) \
  {(a) += H((b), (c), (d)) + (x) + (ac); \
   (a) = ror((a), 32-(s)); \
   (a) += (b); \
  }
#define II(a, b, c, d, x, s, ac) \
  {(a) += I((b), (c), (d)) + (x) + (ac); \
   (a) = ror((a), 32-(s)); \
   (a) += (b); \
  }

void md5Initialize(md5Context *m)
{
  m->bits[0] = m->bits[1] = 0;

  m->scratch[0] = 0x67452301;
  m->scratch[1] = 0xefcdab89;
  m->scratch[2] = 0x98badcfe;
  m->scratch[3] = 0x10325476;
}

void md5Update(md5Context *m, u8 *inBuf, u32 inLen)
{
  u32 in[16];
  int mdi;
  u32 i, ii;

  mdi = ((m->bits[0] >> 3) & 0x3F);

  if((m->bits[0] + (inLen << 3)) < m->bits[0])
    m->bits[1]++;
  m->bits[0] += (inLen << 3);
  m->bits[1] += (inLen >> 29);

  while(inLen--)
  {
    m->buffer[mdi++] = *inBuf++;

    if (mdi == 0x40)
    {
      for(i = 0, ii = 0; i < 16; i++, ii += 4)
        in[i] = ((m->buffer[ii+3]) << 24) |
                ((m->buffer[ii+2]) << 16) |
                ((m->buffer[ii+1]) << 8)  |
                (m->buffer[ii]);
      Transform(m->scratch, in);
      mdi = 0;
    }
  }
}

void md5Finalize(md5Context *m)
{
  u32 in[16];
  int mdi;
  u32 i, ii;
  u32 padLen;

  in[14] = m->bits[0];
  in[15] = m->bits[1];

  mdi = ((m->bits[0] >> 3) & 0x3F);

  padLen = (mdi < 56) ? (56 - mdi) : (120 - mdi);
  md5Update(m, PADDING, padLen);

  for(i = 0, ii = 0; i < 14; i++, ii += 4)
    in[i] = ((m->buffer[ii+3]) << 24) |
            ((m->buffer[ii+2]) << 16) |
            ((m->buffer[ii+1]) << 8)  |
            (m->buffer[ii]);
  Transform(m->scratch, in);

  for(i = 0, ii = 0; i < 4; i++, ii += 4)
  {
    m->digest[ii] = (m->scratch[i] & 0xFF);
    m->digest[ii+1] =
      ((m->scratch[i] >> 8) & 0xFF);
    m->digest[ii+2] =
      ((m->scratch[i] >> 16) & 0xFF);
    m->digest[ii+3] =
      ((m->scratch[i] >> 24) & 0xFF);
  }
}

static void Transform(u32 *buf, u32 *in)
{
  u32 a = buf[0], b = buf[1], c = buf[2], d = buf[3];

  /* Round 1 */
  FF(a, b, c, d, in[ 0],  7, 3614090360UL);
  FF(d, a, b, c, in[ 1], 12, 3905402710UL);
  FF(c, d, a, b, in[ 2], 17,  606105819UL);
  FF(b, c, d, a, in[ 3], 22, 3250441966UL);
  FF(a, b, c, d, in[ 4],  7, 4118548399UL);
  FF(d, a, b, c, in[ 5], 12, 1200080426UL);
  FF(c, d, a, b, in[ 6], 17, 2821735955UL);
  FF(b, c, d, a, in[ 7], 22, 4249261313UL);
  FF(a, b, c, d, in[ 8],  7, 1770035416UL);
  FF(d, a, b, c, in[ 9], 12, 2336552879UL);
  FF(c, d, a, b, in[10], 17, 4294925233UL);
  FF(b, c, d, a, in[11], 22, 2304563134UL);
  FF(a, b, c, d, in[12],  7, 1804603682UL);
  FF(d, a, b, c, in[13], 12, 4254626195UL);
  FF(c, d, a, b, in[14], 17, 2792965006UL);
  FF(b, c, d, a, in[15], 22, 1236535329UL);

  /* Round 2 */
  GG(a, b, c, d, in[ 1],  5, 4129170786UL);
  GG(d, a, b, c, in[ 6],  9, 3225465664UL);
  GG(c, d, a, b, in[11], 14,  643717713UL);
  GG(b, c, d, a, in[ 0], 20, 3921069994UL);
  GG(a, b, c, d, in[ 5],  5, 3593408605UL);
  GG(d, a, b, c, in[10],  9,   38016083UL);
  GG(c, d, a, b, in[15], 14, 3634488961UL);
  GG(b, c, d, a, in[ 4], 20, 3889429448UL);
  GG(a, b, c, d, in[ 9],  5,  568446438UL);
  GG(d, a, b, c, in[14],  9, 3275163606UL);
  GG(c, d, a, b, in[ 3], 14, 4107603335UL);
  GG(b, c, d, a, in[ 8], 20, 1163531501UL);
  GG(a, b, c, d, in[13],  5, 2850285829UL);
  GG(d, a, b, c, in[ 2],  9, 4243563512UL);
  GG(c, d, a, b, in[ 7], 14, 1735328473UL);
  GG(b, c, d, a, in[12], 20, 2368359562UL);

  /* Round 3 */
  HH(a, b, c, d, in[ 5],  4, 4294588738UL);
  HH(d, a, b, c, in[ 8], 11, 2272392833UL);
  HH(c, d, a, b, in[11], 16, 1839030562UL);
  HH(b, c, d, a, in[14], 23, 4259657740UL);
  HH(a, b, c, d, in[ 1],  4, 2763975236UL);
  HH(d, a, b, c, in[ 4], 11, 1272893353UL);
  HH(c, d, a, b, in[ 7], 16, 4139469664UL);
  HH(b, c, d, a, in[10], 23, 3200236656UL);
  HH(a, b, c, d, in[13],  4,  681279174UL);
  HH(d, a, b, c, in[ 0], 11, 3936430074UL);
  HH(c, d, a, b, in[ 3], 16, 3572445317UL);
  HH(b, c, d, a, in[ 6], 23,   76029189UL);
  HH(a, b, c, d, in[ 9],  4, 3654602809UL);
  HH(d, a, b, c, in[12], 11, 3873151461UL);
  HH(c, d, a, b, in[15], 16,  530742520UL);
  HH(b, c, d, a, in[ 2], 23, 3299628645UL);

  /* Round 4 */
  II(a, b, c, d, in[ 0],  6, 4096336452UL);
  II(d, a, b, c, in[ 7], 10, 1126891415UL);
  II(c, d, a, b, in[14], 15, 2878612391UL);
  II(b, c, d, a, in[ 5], 21, 4237533241UL);
  II(a, b, c, d, in[12],  6, 1700485571UL);
  II(d, a, b, c, in[ 3], 10, 2399980690UL);
  II(c, d, a, b, in[10], 15, 4293915773UL);
  II(b, c, d, a, in[ 1], 21, 2240044497UL);
  II(a, b, c, d, in[ 8],  6, 1873313359UL);
  II(d, a, b, c, in[15], 10, 4264355552UL);
  II(c, d, a, b, in[ 6], 15, 2734768916UL);
  II(b, c, d, a, in[13], 21, 1309151649UL);
  II(a, b, c, d, in[ 4],  6, 4149444226UL);
  II(d, a, b, c, in[11], 10, 3174756917UL);
  II(c, d, a, b, in[ 2], 15,  718787259UL);
  II(b, c, d, a, in[ 9], 21, 3951481745UL);

  buf[0] += a;
  buf[1] += b;
  buf[2] += c;
  buf[3] += d;
}

