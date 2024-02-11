# system-programing-exercices
Programs made through collage System Programing course labortory exercices 

## CopyPaste

- cp

The C program is designed to copy the contents of one file to another using system calls for file operations. It verifies the correct number of arguments are provided, opens both the source and destination files, copies data from the source to the destination in chunks, and closes the files upon completion.


## DirectoryTraverse
- ls

This C program lists the contents of a directory provided as a command-line argument, distinguishing between directories and files. It skips the entries for the current directory (.), parent directory (..), and any file named .DS_Store. The program utilizes opendir(), readdir(), and closedir() functions to traverse the directory, and lstat() to retrieve information about each entry. Finally, it prints the total number of directories and files found within the specified directory.

- rc

This C program recursively traverses the current directory, calculating the number of files larger than 1 kB and identifying the oldest file based on modification time. It utilizes opendir(), readdir(), and closedir() functions for directory traversal, lstat() to obtain file information, and time() to initialize the oldest file time. The function traverse_directory() recursively explores subdirectories, updating the count of big files and the oldest file's modification time. Finally, it prints the total number of big files and the date of the oldest file found.

## MultiThread

- Matrices

This C program computes the sum of elements in a matrix using both parallel and sequential approaches. It initializes a 100x100 matrix with values calculated based on mathematical operations involving sine and cosine functions.


## ClientServer

- client

This C program implements a basic TCP client that connects to a server at the address "127.0.0.1" on port 3000. Here's a breakdown of the code:

- server

This C program implements a basic TCP server that can handle multiple clients concurrently. Here's an overview of its functionality:
