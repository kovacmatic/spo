/*
Naloga 1

Napišite program v C-ju, ki kopira eno datoteko v drugo. Imeni
obeh datotek sta podani kot argumenta ukazne vrstice.
Kopirajte datoteko po en blok (512 bajtov) naenkrat.

Če ponorna (ciljna) datoteka ne obstaja, naj jo program
ustvari. Če obstaja, naj jo prepiše.

Pred kopiranjem preverite:
• ali ima program 2 argumenta,
• ali je prva datoteka berljiva (jo lahko odpremo za branje?) in
• ali je druga datoteka zapisljiva (jo lahko opremo za pisanje?),
• sicer izpišite ustrezno sporočilo in zaključite program.
*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER 300
#define REQUIRED_ARGS_COUNT 3
#define error(message) { fprintf(stderr, "%s\n", message); exit(1); }

void validateArgs(int argc);
void writeToNewFile(char const *newFileName, int fd);
int openFile(char const *fileName, int oFlags, char const *error);

int main(int argc, char const *argv[])
{
    int fd;

    validateArgs(argc);
    fd = openFile(argv[1], O_RDONLY, "open");
    writeToNewFile(argv[2], fd);
    close(fd);

    return 0;
}

/** Function opens file and return file descriptor. */
int openFile(char const *fileName, int oFlags, char const *error) {
    int fd;
    if ((fd = open(fileName, oFlags, 0777)) < 0) { // all users persmission 0777 - only for debugging purposes 
        perror(error);
        exit(1);
    }

    return fd;
}

/** Function validates input arguments. */
void validateArgs(int argcCount) {
    if (argcCount != REQUIRED_ARGS_COUNT) {
        error("Invalid arguments count!");
    }
}

/** Read existing file and write content into new file. */
void writeToNewFile(char const *newFileName, int originalFd) {
    int newFd, n;
    char buff[BUFFER];

    newFd = openFile(newFileName, O_CREAT | O_WRONLY | O_TRUNC, "create");
    while ((n = read(originalFd, buff, BUFFER)) > 0) 
    {
        if (write(newFd, buff, n) != n) {
            error("Something wrong on write file content. Not all data has been successfuly written.");
        }
    }

    close(newFd);
}