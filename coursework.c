#include <stdio.h>


void printBytes(void *ptr, int numBytes);

int main () {
    char test[5] = {10,2,3,4,5};

    printBytes(&test, 4);

}


void printBytes(void *ptr, int numBytes) {
    printf("Starting at memory address %p:\n", ptr);

    int i;
    for (i=1; i<=numBytes; i++) {
        printf("%03d: %4u\n", i, *(char*)ptr);
        // Requirement for formatting was unclear, it looked like 4
        ptr = ptr + 1;
    }
}