#include <stdlib.h>
#include <string.h>

char* strdup(const char* str) {
  int  size;
  char *rc;

  if(str == NULL)
    return NULL;

  size = strlen(str)+1;

  rc = malloc(size);
  if(rc == NULL)
    return NULL;

  memcpy(rc, str, size);
  return rc;
}

