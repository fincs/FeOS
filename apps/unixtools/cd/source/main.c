#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[])
{
  int error = 0;
  if(argc != 2)
  {
    fprintf(stderr, "Please provide exactly one parameter\n");
    return 1;
  }

  if(chdir(argv[1]))
  {
    fprintf(stderr, "chdir: '%s': %s\n", argv[1], strerror(errno));
    error = 1;
  }

  return error;
}
