#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

char buffer[4096];
char target[1024];

int cp(char *source, char *target)
{
  int pos;
  int bin;
  int bout;
  int error = 0;
  struct stat buf;
  FILE *fpin, *fpout;

  if((fpin = fopen(source, "r")) == NULL)
  {
    fprintf(stderr, "cp: source '%s': %s\n", source, strerror(errno));
    return 1;
  }
  else if(stat(target, &buf) == 0) //target must not exist
  {
    fprintf(stderr, "cp: target '%s': %s\n", target, strerror(EEXIST));
    if(fclose(fpin))
      fprintf(stderr, "cp: source '%s': %s\n", source, strerror(errno));
    return 1;
  }
  else if((fpout = fopen(target, "w")) == NULL)
  {
    fprintf(stderr, "cp: target '%s': %s\n", target, strerror(errno));
    if(fclose(fpin))
      fprintf(stderr, "cp: source '%s': %s\n", source, strerror(errno));
    return 1;
  }

  while((bin = fread(buffer, 1, 4096, fpin)) > 0) //read from input file
  {
    pos = 0;
    while(pos < bin && (bout = fwrite(&(buffer[pos]), 1, bin-pos, fpout)) > 0) //write to output file
      pos += bout;

    if(bout == 0 && pos != bin) //didn't complete writing file
    {
      fprintf(stderr, "cp: target '%s': Failed to write data\n", target);
      error = 1;
      break;
    }
    else if(bout < 0) //error writing file
    {
      fprintf(stderr, "cp: target '%s': (%d)%s\n", target, errno, strerror(errno));
      error = 1;
      break;
    }
  }
  if(bin < 0) //error reading file
  {
    fprintf(stderr, "cp: source '%s': (%d)%s\n", source, errno, strerror(errno));
    error = 1;
  }
  if(fclose(fpin)) //error closing source file
  {
    fprintf(stderr, "cp: source '%s': (%d)%s\n", source, errno, strerror(errno));
    error = 1;
  }
  if(fclose(fpout)) //error closing target file
  {
    fprintf(stderr, "cp: target '%s': (%d)%s\n", target, errno, strerror(errno));
    error = 1;
  }
  if(error && remove(target)) //delete target due to failure
    fprintf(stderr, "cp: unlink '%s': (%d)%s\n", target, errno, strerror(errno));

  return error;
}

int main(int argc, char *argv[])
{
  int i;
  int error = 0;
  struct stat buf;

  if(argc < 3)
  {
    fprintf(stderr, "Not enough parameters\n");
    return 1;
  }

  if(stat(argv[argc-1], &buf) == 0 && S_ISDIR(buf.st_mode)) //target is a directory
  {
    for(i = 1; i < argc-1; i++) //copy each source file into the target
    {
      strncpy(target, argv[argc-1], 1023);
      target[1023] = 0;
      strncat(target, "/", 1023-strlen(target));
      strncat(target, argv[i], 1023-strlen(target));

      if(cp(argv[i], target))
        error = 1;
    }
  }
  else if(errno == ENOENT) //file-to-file copy
  {
    if(argc == 3) //must be only two arguments for file-to-file copy
    {
      if(cp(argv[1], argv[2]))
        error = 1;
    }
    else
    {
      fprintf(stderr, "cp: target '%s' is not a directory\n", argv[argc-1]);
      error = 1;
    }
  }
  else //other error when stat(target)
  {
    fprintf(stderr, "cp: target '%s': %s\n", argv[argc-1], strerror(errno));
    error = 1;
  }

  return error;
}

