#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define READ_BUFFER 100
#define DEFAULT_LINES 5
#define error(message) { fprintf(stderr, "%s\n", message); exit(1); }

void validateArgsCount(int argc);
void printLines(int linesCount, int linesArgvPosition, int fd);
int getLinesCount(int argc, char const *argv[], int *position);
int openFile(const char *fileName);
const char* getFileName(int linesArgvPosition, char const *argv[]);

int main(int argc, char const *argv[])
{
    int linesCount, fd, linesArgvPosition = -1;
    validateArgsCount(argc);
    linesCount = getLinesCount(argc, argv, &linesArgvPosition);
    fd = openFile(getFileName(linesArgvPosition, argv));
    printLines(linesCount, linesArgvPosition, fd);
    return 0;
}

void printLines(int linesCount, int linesArgvPosition, int fd) {
    off_t currentOffset, position = 0;
    int foundedNewLines = 0, size = 0;
    char buf[1];
    char readBuff[READ_BUFFER];
    
    do {
        currentOffset = lseek(fd, --position, SEEK_END); // move one char

        // read one char
        if (read(fd, buf, 1) == -1) {
            perror("read");
            exit(1);
        }

        // increase founded new lines
        // last character must be new line
        if (buf[0] == '\n' || position == -1) {
            foundedNewLines++;

            // last new line char was found, skip it
            if (foundedNewLines > linesCount) {
                currentOffset++;
            }
        }

    } while (currentOffset > 0 && foundedNewLines <= linesCount);

    // move to offset from start
    lseek(fd, currentOffset, SEEK_SET);
    do {        
        if ((size = read(fd, readBuff, READ_BUFFER)) < 0) {
            perror("read");
            exit(1);
        }

        if (write(STDOUT_FILENO, readBuff, size) < 0) {
            perror("write");
            exit(1);
        }
    } while (size > 0);

    close(fd);
}

const char* getFileName(int linesArgvPosition, char const *argv[]) {
    if (linesArgvPosition == 1) {
        return argv[2];
    }

    return argv[1];
}

int openFile(const char *fileName) {
    int fd;
    if ((fd = open(fileName, O_RDWR, 0777)) < 0) {
        perror("open");
        exit(1);
    }

    return fd;
}

void validateArgsCount(int argc) {
    if (argc < 2 || argc > 3) {
        error("Invalid arguments count!");
    }
}

int getLinesCount(int argc, char const *argv[], int *position) {
    for (int i = 1; i < argc; i++) {
        switch (argv[i][0])
        {
            case '-':
                // check second character (must be n)
                if (argv[i][1] != 'n') {
                    error("Invalid input arguments.")
                }

                *position = i;
                return atoi(&argv[i][2]); // get address and cast
                break;
        }
    }

    return DEFAULT_LINES;
}
