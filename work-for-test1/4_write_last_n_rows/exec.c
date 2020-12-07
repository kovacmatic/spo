/*
Naloga 4
    ▪ Napišite C program rep, ki izpiše zadnjih n vrstic besedilne datoteke, katere ime je podano kot vhodni argument.
    ▪ Program naj deluje podobno kot tail.
    ▪ Primer klica programa: rep -n4 a.txt
    Privzeta vrednost za n naj bo 5.
*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> // open
#include <unistd.h> // read, lseek

#define error(message) { fprintf(stderr, "%s\n", message); exit(1); }
#define DEFAULT_ROWS 5
#define ARGUMENTS_REQUIRED 3
#define READ_BUFFER 128

void validateArgsCount(int argc);
int getPrintRowsCount(const char *argument);
int openFile(const char *filePath);
void printRowsCount(int rowsCount, const char *filePath);
void printRows(int rowsCount, int fd);

int main(int argc, char const *argv[])
{
    int printRowsCount, fd;
    validateArgsCount(argc);
    printRowsCount = getPrintRowsCount(argv[1]);
    fd = openFile(argv[2]);
    printRows(printRowsCount, fd);
    close(fd);

    return 0;
}

void validateArgsCount(int argc) {
    if (argc != ARGUMENTS_REQUIRED) {
        error("Invalid argument count.")
    }
}

int getPrintRowsCount(const char *argument) {
    if (argument[0] != '-' || (argument[1] != 'n' && argument[1] != 'N')) {
        return DEFAULT_ROWS;
    }
    return strtol(&argument[2], NULL, 10);
}

int openFile(const char *filePath) {
    int fd;
    
    if ((fd = open(filePath, O_RDONLY, 0777)) < 0) {
        perror("open");
        exit(1);
    }

    return fd;
}

void printRows(int rowsCount, int fd) {
    off_t offset = 0, currentFileOffset;
    int linesFoundCount = 0;
    ssize_t readSize;
    char buff[1], readBuffer[READ_BUFFER];

    do {

        // move to last element
        currentFileOffset = lseek(fd, --offset, SEEK_END);
        if (currentFileOffset == -1) {
            perror("lseek");
            exit(1);
        }

        // read char
        if (read(fd, buff, 1) != 1) {
            perror("read");
            exit(1);
        }

        // try find new line character
        // last char (end of file) must be new line char; if not simulate it
        if (buff[0] == '\n' || offset == -1) {
            linesFoundCount++;
        }
    } while (currentFileOffset > 0 && rowsCount >= linesFoundCount);

    // skip last founded new line character
    if (currentFileOffset > 0) {
        currentFileOffset++;
    }

    lseek(fd, currentFileOffset, SEEK_SET);

    // print
    do {
        if ((readSize = read(fd, readBuffer, READ_BUFFER)) < 0) {
            perror("read");
            exit(1);
        }

        write(STDOUT_FILENO, readBuffer, readSize);
    } while (readSize > 0);
}