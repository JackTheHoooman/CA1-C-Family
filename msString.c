#include "msString.h"


void readBytes(void *ptr, int numBytes) {
  printf("Starting at memory address %p:\n", ptr);

  int i;
  for (i = 1; i <= numBytes; i++) {
    /*printf("%03d: %4hhu (%c)\n", i, *(char*)ptr,*(char*)ptr); */
    printf("%03d: %4hhu\n", i, *(char *)ptr);
    /* ^^ The return format type is not defined so I used unsigned char */
    ptr += 1;
  }
}


extern msString msSetString(char *string) {
    
}

extern char *msGetString(msString);
extern void msCopy(msString *, msString);
extern void msConcatenate(msString *, msString);
extern long int msLength(msString);
extern int msCompare(msString, msString);
extern int msCompareString(msString, char *);
static void msError(char *);