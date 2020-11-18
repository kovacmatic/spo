#include <stdio.h>

#define MAX_PARSED_ARGS 32

void parser(char *str, char *args[]);

int main(int argc, char const *argv[])
{
    char niz[] = "   To je moj niz plus tab";
    char *args[MAX_PARSED_ARGS];

    parser(niz, args);

    for (int i = 0; args[i] != (char *)0; i++) {
        printf("%s\n", args[i]);
    }

    return 0;
}

void parser(char *str, char *args[]) {    
    while (*str != '\0') { // dokler se originalni niz ne konča
        while (*str == ' ' || *str == '\t') { // iščemo vse NE znake in jih zamenjamo z null el oz. '\0'
            *str = '\0'; // nastavi na null znak
            str++; // povečaj
        }
        
        if (*str != '\0') {
            *args = str; // nastavi vrednost na kazalec od str¸
            args++; // povečaj
        }

        while (*str != ' ' && *str != '\t' && *str != '\0') { // povečaj dokler spet ne najdeš praznega znaka
            str++;
        }
    }

    *args = (char *)0; // zadnji array element naj bo null, da se ga ne izpiše
}
