/*
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
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER 1024
#define MAX_ARGS_COUNT 64
#define error(message) { fprintf(stderr, "%s\n", message); exit(1); }

int main(int argc, char const *argv[])
{
    int readSize, argCount;
    char buff[BUFFER];
    char *args[MAX_ARGS_COUNT];
    pid_t pid;

    do {
        readSize = read(STDIN_FILENO, buff, BUFFER);
        if (readSize < 0) {
            perror("read");
            exit(1);
        }

        // create child
        if ((pid = fork()) == 0) { // child
            
            // new line to \0
            buff[readSize - 1] = '\0';

            // parse arguments
            argCount = 0;
            long len = strlen(buff);
            int lastIndex = 0;
            for (int i = 0; i < len; i++)
            {
                if (buff[i] == ' ') { // new space new operation
                    buff[i] = '\0';  // set as new string

                    args[argCount++] = buff + lastIndex; // start of string
                    lastIndex = i + 1;
                }
            }

            // last string
            args[argCount++] = buff + lastIndex;
            args[argCount] = NULL;

            execvp(args[0], args);
            perror(args[0]);
            exit(127);            
        } else {
            if (waitpid(pid, NULL, 0) == -1) {
                perror("waitpit");
                exit(1);
            }
        }

        
    } while (readSize > 0);

    return 0;
}