// osnove C
#include <stdio.h>

// char je en byte
// vsi stringi - char[] se zaključijo z '\0' - kar pomeni da je rezerviran en byte več
// sizeof - koliko bajtov zaseda nek podatkovni tip - za trenutno arhitekturo (hardware)
// conditions - 0 je false; vse ostalo true (več kot 0)
// n%r = [0, r-1]
// (tidla) ~ - obrne vse bite
// << - zamane vse bite levo
// >> - zamakne vse bite desno
// #define - complie replaca vse z vrednostmi
// #define se določi na predporceseor levelu - ko se stvar complia - sepravi prevajalnik vse replaca - uporabi define namesto const
// #define makro - replaca

int main(int argc, char* argv[]) {

    for (int i = 0; i < argc; i++)
    {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    return 0;
}