#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

char target[1024];

int list(char *path)
{
  DIR *dp;
  struct dirent *dent;
  struct stat buf;
  int error = 0;

  if(stat(path, &buf))
  {
    fprintf(stderr, "ls: '%s': %s\n", path, strerror(errno));
    return 1;
  }
  else if(S_ISDIR(buf.st_mode))
  {
    printf("%s:\n", path);
    dp = opendir(path);
    if(dp == NULL)
    {
      fprintf(stderr, "ls: '%s': %s\n", path, strerror(errno));
      return 1;
    }

    while((dent = readdir(dp)) != NULL)
    {
      if(strcmp(dent->d_name, ".") && strcmp(dent->d_name, ".."))
      {
        strncpy(target, path, 1024);
        target[1023] = 0;
        strncat(target, "/", 1023-strlen(target));
        strncat(target, dent->d_name, 1023-strlen(target));
        if(stat(target, &buf))
        {
          fprintf(stderr, "ls: '%s': %s\n", target, strerror(errno));
          error = 1;
        }
        else if(S_ISDIR(buf.st_mode))
          printf(" <%s>\n", dent->d_name);
        else
          printf(" %s\n", dent->d_name);
      }
    }

    if(closedir(dp))
    {
      fprintf(stderr, "ls: '%s': %s\n", path, strerror(errno));
      return 1;
    }
  }
  else
    printf("%s\n", path);

  return error;
}

int main(int argc, char *argv[])
{
  int i;
  int error = 0;

  if(argc == 1 && list("."))
    error = 1;
  for(i = 1; i < argc; i++)
    if(list(argv[i]))
      error = 1;

  return error;
}
