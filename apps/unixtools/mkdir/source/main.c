#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

void usage(const char *prog) {
  fprintf(stderr, "Usage: %s [OPTIONS] dir1 [dir2 ...]\n", prog);
  fprintf(stderr, "  OPTIONS can be any of the following:\n");
  fprintf(stderr, "  -v, --verbose\n");
  fprintf(stderr, "    print a message for each created directory\n");
  fprintf(stderr, "  -p, --parents\n");
  fprintf(stderr, "    no error if existing, make parent directories as needed\n");
}

int extract(int *argc, char **argv[], const char *str) {
  int i, j;
  int found = 0;

  for(i = 1; i < *argc; i++) {
    if(strcmp(str, (*argv)[i]) == 0) {
      found = 1;
      /* remove from the arguments */
      (*argc)--;
      for(j = i; j < *argc; j++)
        (*argv)[j] = (*argv)[j+1];
      i = 1;
    }
  }

  return found;
}

int mkdirp(char *path, int mode, int verbose) {
  char *end = &path[strlen(path)];
  char *c;

  for(c = path; c < end; c++)
    if(*c == '/') *c = 0;

  c = path;
  do {
    if(*path && mkdir(path, mode)) {
      if(errno != EEXIST) {
        fprintf(stderr, "mkdir: '%s': %s\n", path, strerror(errno));
        for(c = path; c < end; c++) {
          if(*c == 0) {
            while(c < end && *c == 0)
              *c++ = '/';
          }
        }
        return 1;
      }
    }
    else if(*path && verbose)
      printf("mkdir: created directory '%s'\n", path);

    while(c < end && *c != 0)
      c++;
    if(c != end)
      *c = '/';
  } while(c < end);

  return 0;
}

int main(int argc, char *argv[]) {
  int i;
  int error = 0;
  int verbose = 0;
  int parents = 0;

  /* look for 'verbose' option */
  while(extract(&argc, &argv, "-v"))
    verbose = 1;
  while(extract(&argc, &argv, "--verbose"))
    verbose = 1;

  /* look for 'verbose' option */
  while(extract(&argc, &argv, "-p"))
    parents = 1;
  while(extract(&argc, &argv, "--parents"))
    parents = 1;

  if(argc == 1) {
    usage(argv[0]);
    return 1;
  }

  for(i = 1; i < argc; i++) {
    if(parents) {
      if(mkdirp(argv[i], 0777, verbose))
        error = 1;
    }
    else if(mkdir(argv[i], 0777)) {
      if(parents && errno == EEXIST) {
        fprintf(stderr, "mkdir: '%s': %s\n", argv[i], strerror(errno));
        error = 1;
      }
    }
    else if(verbose)
      printf("mkdir: created directory '%s'\n", argv[i]);
  }

  return error;
}
