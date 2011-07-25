#include <stdio.h>
#include <string.h>
#include <errno.h>

char buf[4096];

int main(int argc, char *argv[])
{
  int i;
  int bin, bout;
  int pos;
  int error = 0;
  if(argc == 1)
  {
    fprintf(stderr, "Not enough arguments\n");
    return 1;
  }

  for(i = 1; i < argc; i++)
  {
    FILE* fp = fopen(argv[i], "r");
    if(fp == NULL)
      fprintf(stderr, "fopen: '%s': %s\n", argv[i], strerror(errno));
    else
    {
      while((bin = fread(buf, 1, 4096, fp)) > 0)
      {
        pos = 0;
        while(pos < bin && (bout = fwrite(&(buf[pos]), 1, bin-pos, stdout)) > 0)
          pos += bout;

        if(bout == 0 && pos != bin) //didn't complete writing
        {
          fprintf(stderr, "fwrite: Failed to write data\n");
          error = 1;
          break;
        }
        else if(bout < 0) //error writing
        {
          fprintf(stderr, "fwrite: %s\n", strerror(errno));
          error = 1;
          break;
        }
      }
      if(bin < 0)
      {
        fprintf(stderr, "fread: '%s': %s\n", argv[i], strerror(errno));
        error = 1;
      }
      if(fclose(fp))
      {
        fprintf(stderr, "fclose: '%s': %s\n", argv[i], strerror(errno));
        error = 1;
      }
    }
  }

  return error;
}
