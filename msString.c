#include "msString.h"

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

char *msGetString(msString ptr) {
  long len = msLength(ptr);

  char *str;
  if (!(str = malloc(len + 1))) {
    msError("Error in memmory allocation");
  }

  memcpy(str, ptr + sizeof(long), len);
  *(str + len) = '\0';

  return str;
}

void msCopy(msString *dest, msString src) {
  if (!(*dest = malloc(sizeof(long) + msLength(src)))) {
    msError("Error in memory allocation for copy");
  }

  memcpy(*dest, src, sizeof(long) + msLength(src));
}

void msConcatenate(msString *dest, msString ptr2) {
  long destLen = msLength(*dest);
  long ptr2Len = msLength(ptr2);

  /* I would use below but we are not allowed to change definitions to add limits.h 
  if ((ptr2Len > 0 && destLen > LONG_MAX - ptr2Len) || (destLen > 0 &&ptr2Len > LONG_MAX - destLen)) { 
    msError("Resultant string from concatenation is > long int");
  }
  The requirements should also be for UNSIGNED long int */

  long len = destLen + ptr2Len;

  msString newStr;
  if (!(newStr = malloc(sizeof(long) + len))) {
    msError("Error in memmory allocation");
  }

  memcpy(newStr, &len, sizeof(long));
  memcpy((newStr + sizeof(long)), (*dest + sizeof(long)), destLen);
  memcpy((newStr + sizeof(long) + destLen), (ptr2 + sizeof(long)), ptr2Len);

  free(*dest);
  *dest = newStr;
}

long int msLength(msString ptr) { return *(long *)ptr; }

int msCompare(msString ptr1, msString ptr2) {
  long int i;
  if (msLength(ptr1) != msLength(ptr2)) {
    return 1;
  }

  char *ptr1Str = ptr1 + sizeof(long);
  char *ptr2Str = ptr2 + sizeof(long);

  for (i = 0; i <= msLength(ptr1); i++) {
    if (*(ptr1Str + i) != *(ptr2Str + i)) {
      return 1;
    }
  }

  return 0;
}

int msCompareString(msString ptr, char *str) {
  long int ptrLen = msLength(ptr);
  if (strlen(str) != ptrLen) {
    return 1;
  }

  char *ptrStr = ptr + sizeof(long);
  long int i;
  for (i = 0; i <= ptrLen; i++) {
    if (*(ptrStr + i) != *(str + i)) {
      return 1;
    }
  }

  return 0;
}

void msError(char *errMsg) {
  fprintf(stderr, "Err:%s\n", errMsg);
  exit(EXIT_FAILURE);

  /* Would also use errno like in the main file but definitions can not be
   * changed */
}