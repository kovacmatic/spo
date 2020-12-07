/**
 Naloga 1
▪ Napišite program v C-ju, ki kopira eno datoteko v drugo. Imeni
obeh datotek sta podani kot argumenta ukazne vrstice.
Kopirajte datoteko po en blok (512 bajtov) naenkrat.
▪ Če ponorna (ciljna) datoteka ne obstaja, naj jo program
ustvari. Če obstaja, naj jo prepiše.
▪ Pred kopiranjem preverite:
• ali ima program 2 argumenta,
• ali je prva datoteka berljiva (jo lahko odpremo za branje?) in
• ali je druga datoteka zapisljiva (jo lahko opremo za pisanje?),
• sicer izpišite ustrezno sporočilo in zaključite program.
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define error(message) { fprintf(stderr, "%s\n", message); exit(1); }
#define ARGS_COUNTR_REQUIRED 3
#define READ_BUFFER 512

void validateArgsCount(int argc);
int openFile(const char *fileName, int flags);
void writeFile(int originalFd, const char *fileName);

int main(int argc, char const *argv[])
{
    int fd;

    validateArgsCount(argc);
    fd = openFile(argv[1], O_RDONLY);
    writeFile(fd, argv[2]);
    close(fd);

    return 0;
}

void validateArgsCount(int argc) {
    if (argc != ARGS_COUNTR_REQUIRED) {
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

void writeFile(int originalFd, const char *fileName) {
    int fd, readSize;
    char buff[READ_BUFFER];

    // open file for write
    fd = openFile(fileName, O_CREAT | O_WRONLY | O_TRUNC);

    do {
        // read block
        readSize = read(originalFd, buff, READ_BUFFER);
        if (readSize < 0) {
            perror("read");
            exit(1);
        }

        // write block
        if (write(fd, buff, readSize) != readSize) {
            perror("write");
            exit(1);
        }
    } while (readSize > 0);

    close(fd);
}