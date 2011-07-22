#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

char cwd[1024];

int main(int argc, char *argv[])
{
  if(getcwd(cwd, 1024))
    printf("%s\n", cwd);
  else
  {
    fprintf(stderr, "pwd: %s\n", strerror(errno));
    return 1;
  }

  return 0;
}
