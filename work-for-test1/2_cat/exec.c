/**
 Naloga 2
    ▪   Napišite svojo verzijo terminalskega ukaza cat s pomočjo
        read, write, open in close.
    ▪   Program naj na standardni izhod (stdout) izpiše vsebino
        datotek podanih preko argumentov ukazne vrstice.
    ▪   Če je program klican brez argumentov, kopira stdin v
        stdout.
    ▪   Lahko je podanih tudi več vhodnih argumentov (datotek).
 * */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER 512
#define error(message) { fprintf(stderr, "%s\n", message); exit(1); }

void printStdinToStdout();
void printFiles(int argc, char const *argv[]);
int openFile(const char *fileName, int flags);

int main(int argc, char const *argv[])
{
    // check arguments
    if (argc < 2) {
        printStdinToStdout();
    } else {
        printFiles(argc, argv);
    }

    return 0;
}

void printStdinToStdout() {
    int readSize;
    char buff[BUFFER];
    do {
        readSize = read(STDIN_FILENO, buff, BUFFER);
        if (readSize < 0) {
            perror("read");
            exit(1);
        }

        if (write(STDOUT_FILENO, buff, readSize) != readSize) {
            perror("write STDIN -> STDOUT");
            exit(1);
        }
        
    } while (readSize > 0);
}

void printFiles(int argc, char const *argv[]) {
    int fd, readSize;
    char buff[BUFFER];

    for (int i = 1; i < argc; i++) {
        
        // open
        if ((fd = open(argv[i], O_RDONLY)) < 0) {
            perror("open file");
            exit(1);
        }

        do {
            // read block
            readSize = read(fd, buff, BUFFER);
            if (readSize < 0) {
                perror("read file");
                exit(1);
            }

            // write block
            if (write(STDOUT_FILENO, buff, readSize) != readSize) {
                perror("write file -> STDOUT");
                exit(1);
            }
        } while (readSize > 0);

        close(fd);
    }
}

int openFile(const char *fileName, int flags) {
    int fd;
    if ((fd = open(fileName, flags, 0777)) < 0) { // all premissions - 0777
        perror("open");
        exit(1);
    }
    return fd;
}