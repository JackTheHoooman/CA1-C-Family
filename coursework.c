/* #include <limits.h> */
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_CHUNK_SIZE 2048
/* 
To allow for large files, they are read in chunks. This is the used size. 
It also prevents many small IO operations by grouping them, which i assume is more efficient.
*/


void printBytes(void *ptr, int numBytes);

void reverseArrayC(char *arr, int len);

void reverseFile(char *inFile[], char *outFile[]); /* Calls reverseFileByChunk */
void reverseFileByChunk(char *inFile[], char *outFile[], int chunkSize);


int main (int argc, char **argv) {
    printf("--- Task 1 ---\n");
    /* "You should call that function with a couple of different 
    values from within the main function." */

    unsigned char test1[5] = {10,20,30,145,5}; 
    printBytes(&test1, 4); 
    /* 145 > CHAR_MAX */
    printf("\n");

    unsigned char test2[] = {10,20,30,145,50,30,145}; 
    printBytes(&test2, -10); 
    printf("\n");

    printBytes(&test2, 6); 
    
    /*char test3[] = {10,20,30,145,50,30,145}; 
    reverseArrayC(test3, 6);
    printBytes(&test3, 6); */

    


    printf("\n--- Task 2 ---\n");
    if (argc == 3) {
        printf("Input file is : '%s' \nOutput file is : '%s'\n\n", argv[1], argv[2]);
    
        reverseFile(&argv[1], &argv[2]);

    } else if (argc >= 4) {
        int chunkSize;
        if (sscanf (argv[3], "%i", &chunkSize) != 1 || chunkSize < 1) {
            perror("Chunk size input is not an int or is too small");
            exit(1);
        } else {
            printf("Input file is : '%s' \nOutput file is : '%s'\n\n", argv[1], argv[2]);
            reverseFileByChunk(&argv[1], &argv[2], chunkSize);
        }

    } else {
        printf("Wrong number of arguments for task 2. At least 2 should be provided\n");
    }

    return 0;
}





void printBytes(void *ptr, int numBytes) {
    printf("Starting at memory address %p:\n", ptr);

    int i;
    for (i=1; i<=numBytes; i++) {
        /*printf("%03d: %4hhu (%c)\n", i, *(char*)ptr,*(char*)ptr); */
        printf("%03d: %4hhu\n", i, *(char*)ptr); 
        /* ^^ The return format type is not defined so I used unsigned char */
        ptr += 1;
    }
}

void reverseArrayC(char *arr, int len) {
    int i = len-1; 
    int j = 0;

    while (i > j) {
        char swap = arr[i];
        arr[i] = arr[j];
        arr[j] = swap;
        i--;
        j++;
    }
}


void reverseFile(char *inFile[], char *outFile[]) { /* NOTE: These are references since there is no point copying the entiire string */
    reverseFileByChunk(inFile, outFile, DEFAULT_CHUNK_SIZE);
}

void reverseFileByChunk(char *inFile[], char *outFile[], int chunkSize) { 
    FILE *inFStrseam = fopen(*inFile, "r");
    FILE *outFStream = fopen(*outFile, "w"); /* Will create if not exist */

    if (inFStrseam == NULL || outFStream == NULL) { /* The lack of standardisation with C errors is ridiculous */
        perror("Error opening file");
        exit(EXIT_FAILURE);
        /* "graceful exit": it closes any open streams */
    }

    fseek(inFStrseam, 0, SEEK_END); /* Puts the pointer/cursor at the end of the file */
    long position = ftell(inFStrseam); /* Gets location of cursor */
    
    printf("File is %li bytes long, proceeding with a chunk size of %i (%li chunks)\n", position, chunkSize, ((position+(chunkSize-1))/chunkSize));

    char chunk[position < chunkSize ? position : chunkSize];

    while (position > 0) { /* Position is used as the chunk end index */
        int currentChunkSize = position < chunkSize ? position : chunkSize; 

        fseek(inFStrseam, position - currentChunkSize, SEEK_SET); /* Go to SEEK_SET+(position-crntChnkSze), SEEK_SET : File start pos */
        int bytesRead = fread(chunk, 1, currentChunkSize, inFStrseam);

        /*printBytes(chunk, currentChunkSize);*/
        reverseArrayC(chunk, currentChunkSize);

        int bytesWritten = fwrite(chunk, 1, bytesRead, outFStream);

        if (bytesRead != bytesWritten) {
            perror("Error in file read/write");
            exit(EXIT_FAILURE);
        }

        position -= currentChunkSize;
    }

    fclose(inFStrseam);
    fclose(outFStream);
}