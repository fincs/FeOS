#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[])
{
  if(argc != 3)
  {
    fprintf(stderr, "mv: %s\n", strerror(EINVAL));
    return 1;
  }

  if(rename(argv[1], argv[2]))
  {
    fprintf(stderr, "mv: %s\n", strerror(errno));
    return 1;
  }

  return 0;
}
