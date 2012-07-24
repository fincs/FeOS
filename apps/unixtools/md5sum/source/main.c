/* This code is based off of RFC 1321 - The MD5 Message Digest Algorithm */

#ifdef FEOS
#include <feos.h>
#elif defined(ARM9)
#include <nds.h>
#else
#include <stdint.h>
typedef uint8_t  u8;
typedef uint32_t u32;
#endif
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "md5.h"

u8 data[8192];

static inline int md5sum(char *filename) {
  FILE *inFile;
  MD5_CTX mdContext;
  int bytes;
  int i;
  u8  digest[16];

  if((inFile = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "md5sum: '%s': %s\n", filename, strerror(errno));
    return 1;
  }

  if(!MD5_Init(&mdContext)) {
    fprintf(stderr, "md5sum: '%s': Failed to initialize\n", filename);
    fclose(inFile);
    return 1;
  }
  while((bytes = fread (data, 1, sizeof(data), inFile)) != 0)
    MD5_Update(&mdContext, data, bytes);
  if(!MD5_Final(digest, &mdContext)) {
    fprintf(stderr, "md5sum: '%s': Failed to finalize\n", filename);
    fclose(inFile);
    return 1;
  }

  for(i = 0; i < 16; i++)
    printf("%02x", digest[i]);
  printf("\t%s\n", filename);

  if(fclose(inFile)) {
    fprintf(stderr, "md5sum: '%s': %s\n", filename, strerror(errno));
    return 1;
  }

  return 0;
}

int main(int argc, char *argv[]) {
  int i;
  int error = 0;

  if (argc < 2) {
    fprintf(stderr, "%s\n", strerror(EINVAL));
    return 1;
  }
  for (i = 1; i < argc; i++)
    error |= md5sum(argv[i]);

  return error;
}
