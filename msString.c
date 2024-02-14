#include "msString.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

msString msSetString(char *str) {
  size_t len = strlen(str); /* Not including null char */

  msString newStr;
  if (!(newStr = malloc(sizeof(long) + len))) {
    msError("Error in memmory allocation");
  }

  memcpy((newStr), &len, sizeof(long));
  memcpy((newStr + sizeof(long)), str, len);
  /* memcpy avoids copying over the null char vs strcpy */

  /*
  I would use structs, however the requirements say that it
  should point to long int, char[] instead of long int, *char, char[]:

  struct msStringStruct {
    long int len;
    char *str;
  }
  */

  return newStr;
}

char* msGetString(msString ptr) {
  long len = msLength(ptr);

  char *str;
  if (!(str = malloc(len+1))) {
    msError("Error in memmory allocation");
  }

  memcpy(str, ptr+sizeof(long), len);

  return str;
}

void msCopy(msString *dest, msString src) {
  if (!(*dest = malloc(sizeof(long) + msLength(src)))) {
    msError("Error in memory allocation for copy");
  }

  memcpy(*dest, src, sizeof(long) + msLength(src));
}

void msConcatenate(msString *string1, msString string2) {}

long int msLength(msString ptr) { 
  return *(long *)ptr;
}

int msCompare(msString string1, msString string2) { return 1; }

int msCompareString(msString string1, char *string2) { return 1; }

void msError(char *errMsg) {
  fprintf(stderr, "Err:%s\n", errMsg);
  exit(EXIT_FAILURE);
}