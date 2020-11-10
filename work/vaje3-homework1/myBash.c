/**
Napišite program, ki deluje kot preprosta ukazna vrstica in v neskončni
zanki izvaja vnesene ukaze.
    ▪ Program se zaključi ob pritisku CTRL+D.
    ▪ Celoten ukaz preberite s standardnega vhoda v znakovni niz. Lahko
predpostavite, da ukazni niz ni daljši od 1024 znakov. Za branje lahko
uporabite fgets() ali pa kar read().
    ▪ Za posamezen ukaz naj proces s klicem fork() ustvari otroka, ki s
klicem execv() ali execvp() izvrši ukaz.
    ▪ Omogočite izvajanje ukazov z argumenti.
    • Ukaz razčlenite po presledkih: presledke zamenjajte z '\0', na začetke podnizov pa
naj kažejo kazalci, ki jih boste podali kasneje funkciji execvp().
    • Namesto ročnega razčlenjevanja lahko uporabite tudi funkcijo strtok() iz knjižnice
string.h.
    • Lahko predpostavite, da argumentov ne bo več kot 64.
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER 1024
#define ARGS_MAX_COUNT 64

int main(int argc, char const *argv[])
{
    pid_t pid;
    char buff[BUFFER];
    char *argsArray[ARGS_MAX_COUNT];
    char *input;
    int counter;

    while ((input = fgets(buff, BUFFER, stdin)))
    {
        input[strlen(input)-1] = '\0'; // making '\n' '\0' - remove last space

        // create child process
        pid = fork();

        if (pid == 0) // executing child process logic
        { 
            // parse data (commands and arguments) by spaces
            counter = 0;
            argsArray[counter] = strtok(input, " ");

            // utill exists
            while (argsArray[counter] != NULL)
            {
                argsArray[++counter] = strtok(NULL, " ");                
            }

            argsArray[++counter] = NULL; // null for last parameter is required - determinate size of array

            execvp(argsArray[0], argsArray);
            perror(argsArray[0]);
        } else { // executing main process logic
            wait(NULL); // wait to child completes
        }
    }

    return 0;
}