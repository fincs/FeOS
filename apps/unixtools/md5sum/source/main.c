/* This code is based off of RFC 1321 - The MD5 Message Digest Algorithm */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "md5.h"

static inline int md5sum(char *filename)
{
  FILE *inFile;
  md5Context mdContext;
  int bytes;
  int i;
  u8 data[1024];

  if((inFile  = fopen(filename, "r")) == NULL)
  {
    printf("md5sum: '%s': %s\n", filename, strerror(errno));
    return 1;
  }

  md5Initialize(&mdContext);
  while((bytes = fread (data, 1, 1024, inFile)) != 0)
    md5Update(&mdContext, data, bytes);
  md5Finalize(&mdContext);

  for(i = 0; i < 16; i++)
    printf("%02x", mdContext.digest[i]);
  printf(" %s\n", filename);

  if(fclose(inFile))
  {
    fprintf(stderr, "md5sum: '%s': %s\n", filename, strerror(errno));
    return 1;
  }

  return 0;
}

int main(int argc, char *argv[])
{
  int i;
  int error = 0;

  if (argc == 1)
  {
    fprintf(stderr, "%s\n", strerror(EINVAL));
    return 1;
  }
  for (i = 1; i < argc; i++)
    error |= md5sum(argv[i]);

  return error;
}

