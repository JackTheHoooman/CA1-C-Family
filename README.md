# Task 1 
Function printBytes that accepts a void pointer and int. Prints relevant bytes of memory in specified format.

# Task 2 
Reverse contents of a file and put it in another. File is one line long and of "unknown length".

*NOTE:* Vim likes to add a new line char at the end

## Usage 
- `$ ./coursework inFile.txt outFile.txt`
- `$ ./coursework inFile.txt outFile.txt chunkLen`
    - Files are read in *"chunks"*/sections to allow for large files 
    - Default chunk size of 2048 bytes.

# Task 3 
- Implement `msString.h` without changing signatures.
- Fix memory leak in provided test code.
