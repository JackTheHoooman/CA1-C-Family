#include "msString.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef DEFAULT_CHUNK_SIZE
#define DEFAULT_CHUNK_SIZE 2048
#endif
/*
To allow for large files and decrease IO opertations: they are read in chunks.
I thought it would be more interesting than doing it char by char / reading the
entire thing but that functionality can be done by this i guess?

I have tested it with files up to 5gb (with multiple lines) on a poor raspberry
pi. Use head/tail to view a file like that. Do not use vim, i learnt the hard
way. Also change the block/chunk size if you do or it will take a while.
*/

void printBytes(void *ptr, int numBytes);

void reverseArrayC(char *arr, int len);

void reverseFile(char *inFile[],
                 char *outFile[]); /* Calls reverseFileByChunk */
void reverseFileByChunk(char *inFile[], char *outFile[], int chunkSize);

int main(int argc, char **argv) {
  printf("--- Task 1 ---\n");
  /* "You should call that function with a couple of different
  values from within the main function." */

  unsigned char test1[5] = {10, 20, 30, 145, 5};
  printBytes(&test1, 4);
  /* 145 > CHAR_MAX */
  printf("\n");

  unsigned char test2[] = {10, 20, 30, 145, 50, 30, 145};
  printBytes(&test2, -10);
  printf("\n");

  printBytes(&test2, 6);

  /*char test3[] = {10,20,30,145,50,30,145};
  reverseArrayC(test3, 6);
  printBytes(&test3, 6); */

  printf("\n--- Task 2 ---\n");
  if (argc == 3) {
    printf("Input file is : '%s' \nOutput file is : '%s'\n\n", argv[1],
           argv[2]);

    reverseFile(&argv[1], &argv[2]);

  } else if (argc >= 4) {
    int chunkSize;
    if (sscanf(argv[3], "%i", &chunkSize) != 1 || chunkSize < 1) {
      perror("Error with chunk size");
      exit(1);
      /*
      This provides more error information than the example used in lecture.
      It also works still with:
      $ ./coursework infile.txt outfile.txt >output.lis 2>err.lis
      Since is does send error messages to sterr.
      */
    } else {
      printf("Input file is : '%s' \nOutput file is : '%s'\n\n", argv[1],
             argv[2]);
      reverseFileByChunk(&argv[1], &argv[2], chunkSize);
    }

  } else {
    fprintf(stderr,
            "Wrong number of arguments for task 2. At least 2 should be "
            "provided:\n- %s inFile outFile\n- %s inFile outFile "
            "chunkSize(bytes)\n",
            argv[0], argv[0]);
  }

  printf("\n--- Task 3 ---\n");

  msString ms = msSetString("Hello");
  msString ms2 = msSetString("World!");
  msString mscopy = NULL;

  char *tempString = msGetString(ms);
  printf("String        (ms) |%s| is %li characters long (%p).\n", tempString,
         msLength(ms), &ms);
  free(tempString);
  /*
  Since msGetString must use malloc to create a char[] with a null char
  it must be freed
  */

  msCopy(&mscopy, ms);
  tempString = msGetString(mscopy);
  printf("Copied string (mscopy) |%s| is %li characters long (%p).\n",
         tempString, msLength(mscopy), mscopy);
  free(tempString);

  tempString = msGetString(ms2);
  printf("Second string (ms2) |%s| is %li characters long (%p) \n", tempString,
         msLength(ms2), ms2);
  free(tempString);

  printf("Compare ms with mscopy : %s \n",
         (msCompare(ms, mscopy) ? "True" : "False"));
  printf("Compare ms with ms2 : %s \n",
         (msCompare(ms, ms2) ? "True" : "False"));
  printf("Compare ms with Hello : %s \n",
         (msCompareString(ms, "Hello")) ? "True" : "False");
  printf("Compare ms with HelloX : %s \n",
         (msCompareString(ms, "HelloX") ? "True" : "False"));
  printf("Compare ms with Hella : %s \n",
         (msCompareString(ms, "Hella") ? "True" : "False"));
  /*

  msConcatenate(&mscopy, ms2);
  tempString = msGetString(mscopy);
  printf(" Concatenated string |%s| is %d characters long (%p).\n ", tempString,
         msLength(mscopy), mscopy);

  free(tempString);
  */
  free(ms);
  free(ms2);
  free(mscopy);

  return 0;
}

void printBytes(void *ptr, int numBytes) {
  printf("Starting at memory address %p:\n", ptr);

  int i; /* The example starts at 001 */
  for (i = 1; i <= numBytes; i++) {
    /*printf("%03d: %4hhu (%c)\n", i, *(char*)ptr,*(char*)ptr); */
    printf("%03d: %4hhu\n", i, *(char *)ptr);
    /* ^^ The return format type is not defined so I used unsigned char */
    ptr += 1;
  }
}

void reverseArrayC(char *arr, int len) {
  int i = len - 1; /* End index */
  int j = 0;       /* Start index */

  while (i > j) {
    char swap = arr[i]; /* arr[i], arr[j] = arr[j], arr[i] */
    arr[i] = arr[j];
    arr[j] = swap;
    i--;
    j++; /* moves the indexes closer */
  }
}

void reverseFile(char *inFile[],
                 char *outFile[]) { /* NOTE: These are references since there is
                                       no point copying the entiire string */
  reverseFileByChunk(inFile, outFile, DEFAULT_CHUNK_SIZE);
}

void reverseFileByChunk(char *inFile[], char *outFile[], int chunkSize) {
  FILE *inFStrseam = fopen(*inFile, "r");
  FILE *outFStream = fopen(*outFile, "w"); /* Will create if not exist */

  if (inFStrseam == NULL ||
      outFStream ==
          NULL) { /* The lack of standardisation with C errors is ridiculous */
    perror("Error opening files");
    exit(EXIT_FAILURE);
    /* "graceful exit": it closes any open streams */
  }

  fseek(inFStrseam, 0,
        SEEK_END); /* Puts the pointer/cursor at the end of the file */
  long position = ftell(inFStrseam); /* Gets location of cursor */

  printf("File is %li bytes long, proceeding with a chunk size of %i (%li "
         "chunks)\n",
         position, chunkSize, ((position + (chunkSize - 1)) / chunkSize));

  char *chunk;
  if (!(chunk = (char *)malloc(position < chunkSize ? position : chunkSize))) {
    perror("Error in assignment of chunk");
    exit(EXIT_FAILURE);
  }

  while (position > 0) { /* Position is used as the chunk end index */
    int currentChunkSize = position < chunkSize ? position : chunkSize;

    fseek(inFStrseam, position - currentChunkSize,
          SEEK_SET); /* Go to SEEK_SET+(position-crntChnkSze), SEEK_SET : File
                        start pos */
    int bytesRead = fread(chunk, 1, currentChunkSize, inFStrseam);

    /*printBytes(chunk, currentChunkSize);*/
    reverseArrayC(chunk, currentChunkSize);
    /*printBytes(chunk, currentChunkSize);*/

    int bytesWritten = fwrite(chunk, 1, bytesRead, outFStream);

    if (bytesRead != bytesWritten) {
      perror("Error in file read/write");
      exit(EXIT_FAILURE);
    }

    position -= currentChunkSize;
  }

  free(chunk);
  fclose(inFStrseam);
  fclose(outFStream);
}