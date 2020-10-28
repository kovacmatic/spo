#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define error(message) { fprintf(stderr, "%s\n", message); exit(1); }

void validateArgs(int count);
void writeReversed(int newFd, int fd);
int openFile(char const *fileName);
int createFile(char const *fileName);
int lseekFile(int fd, int offset, int whence);

int main(int argc, char const *argv[])
{
    int fd, newFd;
    validateArgs(argc);
    fd = openFile(argv[1]);
    newFd = createFile(argv[2]);
    writeReversed(newFd, fd);
    close(fd);
    close(newFd);
    return 0;
}

void writeReversed(int newFd, int fd) {
    off_t position, currentOffset;
    char buf[1]; // read one char

    // gets file end offset
    position = 0;

    do {
        // move to one position closer to file start
        currentOffset = lseekFile(fd, --position, SEEK_END);

        // read one char
        if (read(fd, buf, 1) == -1) {
            perror("read");
            exit(1);
        }

        // write to new file
        if (write(newFd, buf, 1) == -1) {
            perror("write");
            exit(1);
        }
    }
    while (currentOffset > 0); // read until start offset
}

int lseekFile(int fd, int offset, int whence) {
    int resOffset;
    if ((resOffset = lseek(fd, offset, whence)) == -1) {
        perror("lseek");
        exit(1);
    }

    return resOffset;
}

void validateArgs(int count) {
    if (count != 3) {
        error("Invalid arguments count.");
    }
}

int createFile(char const *fileName) {
    int fd;
    if ((fd = open(fileName, O_CREAT | O_WRONLY | O_TRUNC, 0777)) < 0) { // all users persmission 0777 - only for debugging purposes 
        perror(fileName);
        exit(1);
    }

    return fd;
}

int openFile(char const *fileName) {
    int fd;
    if ((fd = open(fileName, O_RDONLY)) < 0) {
        perror(fileName);
        exit(1);
    }

    return fd;
}
