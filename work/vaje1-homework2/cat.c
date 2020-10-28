#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define BUFFER 100
#define error(message) { fprintf(stderr, "%s\n", message); exit(1); }

int openFile(char const *fileName);
void writeStdinToStdout();
void writeFilesToStdout();
void writeToStdout(char const *buf, int size);

int main(int argc, char const *argv[])
{
    if (argc < 2) {
        writeStdinToStdout();
    } else {
        writeFilesToStdout(argc, argv);
    }

    return 0;
}

void writeStdinToStdout() {
    int size;
    char buf[BUFFER];
    while ((size = read(STDIN_FILENO, buf, BUFFER)) > 0) {
        writeToStdout(buf, size);
    }
}

void writeFilesToStdout(int argsCount, char const *argv[]) {
    int fd, size;
    char buf[BUFFER];
    for (int i = 1; i < argsCount; i++) { // skip first argument
        fd = openFile(argv[i]);
        while ((size = read(fd, buf, BUFFER)) > 0) {
            writeToStdout(buf, size);
        }

        close(fd);
    }
}

void writeToStdout(char const *buf, int size) {
    if (write(STDOUT_FILENO, buf, size) != size) {
        error("write to stdout error!");
    }
}

int openFile(char const *fileName) {
    int fd;
    if ((fd = open(fileName, O_RDONLY)) < 0) {
        perror("open");
        exit(1);
    }
    return fd;
}
