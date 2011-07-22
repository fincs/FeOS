#include <stdio.h>
#include <string.h>
#include <errno.h>

char buf[4096];

int main(int argc, char *argv[])
{
  int i, bytes;
  if(argc == 1)
  {
    fprintf(stderr, "Not enough arguments\n");
    return -1;
  }

  for(i = 1; i < argc; i++)
  {
    FILE* fp = fopen(argv[i], "r");
    if(fp == NULL)
      fprintf(stderr, "fopen: '%s': %s\n", argv[i], strerror(errno));
    else
    {
      while((bytes = fread(buf, 1, 4096, fp)) > 0)
        printf("%*s", bytes, buf);
      if(bytes < 0)
        fprintf(stderr, "fread: '%s': %s\n", argv[i], strerror(errno));
      if(fclose(fp))
        fprintf(stderr, "fclose: '%s': %s\n", argv[i], strerror(errno));
    }
  }

  return 0;
}
