#include <stdio.h>


void printBytes(void *ptr, int numBytes);

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
    printf("\n");

    


    printf("--- Task 2 ---\n");
    if (argc >= 2) {
        printf("In file is '%s' and out file is '%s'\n", argv[1], argv[2]);
    } else {
        printf("Wrong number of arguments for task 2. At least 2 should be provided\n");
    }

}


void printBytes(void *ptr, int numBytes) {
    printf("Starting at memory address %p:\n", ptr);

    int i;
    for (i=1; i<=numBytes; i++) {
        printf("%03d: %4hhu\n", i, *(char*)ptr); 
        /* ^^ The return format type is not defined so I used unsigned char */
        ptr += 1;
    }
}