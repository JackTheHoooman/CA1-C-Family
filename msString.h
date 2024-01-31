#ifndef __MSSTRING__
#define __MSSTRING__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void * msString;

extern msString  msSetString     (char *);
extern char     *msGetString     (msString);
extern void      msCopy          (msString *, msString);
extern void      msConcatenate   (msString *, msString);
extern int       msLength        (msString);
extern int       msCompare       (msString, msString);
extern int       msCompareString (msString, char *);
static void      msError         (char *);

#endif
