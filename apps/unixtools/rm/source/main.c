#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{
  int i;
  int error = 0;

  if(argc == 1)
  {
    fprintf(stderr, "Not enough arguments\n");
  }

  for(i = 1; i < argc; i++)
  {
    if(remove(argv[i]))
    {
      fprintf(stderr, "rm: '%s': %s\n", argv[i], strerror(errno));
      error = 1;
    }
  }

  return error;
}
