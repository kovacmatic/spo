/*
Naloga 3
    ▪ Napišite program v C-ju, ki vsebino datoteke izpiše v obratnem vrstnem redu v drugo datoteko.
    ▪ Imeni obeh datotek sta podani kot argumenta programa.
    ▪ Pri vsakem sistemskem klicu preverite, ali se lahko izvede, sicer izpišite sporočilo o napaki in končajte program.
    ▪ Program mora delovati za „poljubno“ dolge datoteke: ni dovoljeno prebrati celotne datoteke v delovni spomin. Uporabite lseek.
    ▪ Test programa:
    ./naloga1 a.txt b.txt
    ./naloga1 b.txt c.txt
    diff a.txt c.txt
*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define error(message) { fprintf(stderr, "%s\n", message); exit(1); }

void validateArgsCount(int argc);
int openFile(const char *fileName, int flags);
void writeReversed(int originalFd, const char *fileName);

int main(int argc, char const *argv[])
{
    int fd;
    validateArgsCount(argc);
    fd = openFile(argv[1], O_RDONLY);
    writeReversed(fd, argv[2]);
    return 0;
}

void validateArgsCount(int argc) {
    if (argc != 3) {
        error("Invalid argument count.")
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

void writeReversed(int originalFd, const char *fileName) {
    int fd;
    off_t offset = 0, currentOffset;
    char buff[1];

    // create file
    fd = openFile(fileName, O_CREAT | O_WRONLY | O_TRUNC);

    // read till end
    do {

        // move one char left
        currentOffset = lseek(originalFd, --offset, SEEK_END);
        if (currentOffset == -1) {
            perror("lseek");
            exit(1);
        }

        // read char
        if (read(originalFd, buff, 1) != 1) {
            perror("read");
            exit(1);
        }

        // write
        if (write(fd, buff, 1) != 1) {
            perror("write");
            exit(1);
        }
    } while (currentOffset > 0);
}

