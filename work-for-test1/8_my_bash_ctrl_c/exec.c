/*
    Napišite nadgradnjo programa iz prejšnje vaje (ukazna vrstica), in sicer tako,
    da bo odporna na pritisk kombinacije CTRL+C, ki trenutnemu procesu pošlje
    signal SIGINT.
        • Ko uporabnik pritisne kombinacijo tipk CTRL+C, naj se izpiše opozorilo, da
    bo signal SIGINT upoštevan, če uporabnik znova pritisne to kombinacijo v
    roku 3 sekund.
        • Če v roku treh sekund program ponovno ne dobi signala SIGINT, se stanje
    ponastavi in sta potrebna zopet dva pritiska CTRL+C v razmaku manj kot 3
    sekund, da prekinemo izvajanje procesa.
        • Namig: funkcija alarm().
        • Primer
    $ ./ukaznaVrstica
    >>> ^C
    Za izhod iz programa se enkrat pritisnite CTRL+C v roku 3 sekund.
    ^C
    Nasvidenje!
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdbool.h>

#define BUFFER 1024
#define MAX_ARGS_COUNT 64
#define error(message) { fprintf(stderr, "%s\n", message); exit(1); }

void signalHandler(int signalNo);

bool allowExit = false;

int main(int argc, char const *argv[])
{
    int readSize, argCount;
    char buff[BUFFER];
    char *args[MAX_ARGS_COUNT];
    pid_t pid;

    // signal handling
    if (signal(SIGINT, signalHandler) == SIG_ERR) {
        perror("signal (SIGINT) error");
    }
    if (signal(SIGALRM, signalHandler) == SIG_ERR) {
        perror("signal (SIGALRM) error");
    }

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

void signalHandler(int signalNo) {
    switch (signalNo)
    {
        case SIGINT:
            if (allowExit) {
                printf("\nNasvidenje.\n");
                exit(1); // exit program
            } else {
                printf("\nZa izhod iz programa se enkrat pritisnite CTRL+C v roku 3 sekund.\n");
                allowExit = true;
                alarm(3); // after three seconds reset allow exit...
            }
            break;
        case SIGALRM:
            allowExit = false;
            break;
        
        default:
            break;
    }
}