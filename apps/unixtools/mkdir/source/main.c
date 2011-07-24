#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[])
{
  int i;
  int error = 0;
  if(argc == 1)
  {
    fprintf(stderr, "Not enough arguments\n");
    return 1;
  }

  for(i = 1; i < argc; i++)
  {
    if(mkdir(argv[i], 0777))
    {
      fprintf(stderr, "mkdir: '%s': %s\n", argv[i], strerror(errno));
      error = 1;
    }
  }

  return error;
}
