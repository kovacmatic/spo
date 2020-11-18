/**
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
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>

#define BUFFER 1024
#define ARGS_MAX_COUNT 64

/* True if an exit was requested in the last three seconds. */
bool allowExit = false;

void signalHandler(int signalNo);

int main(int argc, char const *argv[])
{
    pid_t pid;
    char buff[BUFFER];
    char *argsArray[ARGS_MAX_COUNT];
    char *input;
    int counter;

    // signal handling
    if (signal(SIGINT, signalHandler) == SIG_ERR) {
        perror("signal (SIGINT) error");
    }
    if (signal(SIGALRM, signalHandler) == SIG_ERR) {
        perror("signal (SIGALRM) error");
    }

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
        } else { // executing parent process logic
            if (wait(NULL) == -1) { // wait to child completes
                perror("wait");
            }
        }
    }

    return 0;
}

void signalHandler(int signalNo) {
    switch (signalNo) {
        case SIGINT:
            if (allowExit) {
                printf("\nNasvidenje!\n");
                exit(1); // exit program
            } else {
                // ask and start counting for 3 seconds
                allowExit = true;
                printf("\nZa izhod iz programa še enktrat pritisnite CTRL+C v roku 3 sekund.\n");
                alarm(3);
            }
        break;
        case SIGALRM:
            // alarm is finshed - reset
            allowExit = false;
        break;
    }
    return;
}