#include <stdio.h>

typedef struct Naslov {
    char* ulica;
} Naslov;

int main(int argc, char const *argv[])
{
    Naslov naslov;
    naslov.ulica = "Smoletova ulica 12a";
    printf("ulica: %s\n", naslov.ulica);
    printf("argv: %p\n", argv);
    printf("argv: %p\n", argv[0]);
    printf("argv: %p\n", argv[1]);
    printf("argv: %p\n", argv[2]);
    return 0;
}
