/**
    Svojo ukazno vrstico sedaj razširite tako, da bo znala izvajati ukaze, ki so
    med sabo „povezani“ s cevjo.
        ▪ Namesto znaka |, bomo za cev uporabili niz ->
        ▪ Primer:
        ps -ef -> grep root
    Izvede naj se program ps z argumentom –ef in vse, kar izpiše na
    standardni izhod, se preusmeri na pisalni konec cevi. Bralni
    konec cevi pa se preveže na standardni vhod, iz katerega bere
    program grep.
        ▪ Omejite se na eno cev, torej: <ukaz1> -> <ukaz2>
        ▪ Cev obstaja med roditeljem in otrokom; roditelj naj izvede desno stran
        (<ukaz2>), otrok pa levo stran (<ukaz1>). Namig: proces „roditelj“ je
        otrok procesa, ki bere uporabnikove nize iz std. vhoda (glej vajo 3).
        ▪ Uporabite dup2, da preusmerite std. vhod in izhod
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
void execChild(char *buff, ssize_t readSize);
void execGransdom(int fd[2], char* leftPart);
void executeCmd(char *arguments);

bool allowExit = false;

int main(int argc, char const *argv[])
{
    ssize_t readSize;
    char buff[BUFFER];
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

        if ((pid = fork()) == 0) {
            execChild(buff, readSize);                
        } else { // parent
            if (waitpid(pid, NULL, 0) == -1) {
                perror("waitpit");
                exit(1);
            }
        }
    } while (readSize > 0);

    return 0;
}

void tryGetPipeCmds(char *buff, char **leftPart, char **rightPart) {
    char *left = 0, *right = 0, *pipeChar = ">";

    // try find pipeChar
    left = strtok(buff, pipeChar);

    // check if previous char is '-'
    if (left[strlen(left) - 1] == '-') {
        left[strlen(left) - 1] = 0; // remove last character '-'
        right = strtok(NULL, pipeChar);
    }

    *leftPart = left;
    *rightPart = right;
}

void execChild(char *buff, ssize_t readSize) {
    char *leftPart, *rightPart;
    int fd[2];
    pid_t pid;

    // try get pipe left and right cmd
    tryGetPipeCmds(buff, &leftPart, &rightPart);

    if (rightPart != 0) {
        // make pipe
        if (pipe(fd) == -1) {
            perror("pipe");
            exit(1);
        }

        // make grandsom
        if ((pid = fork()) == 0) {
            execGransdom(fd, leftPart);
        } else {
            // wait grandsom
            if (waitpid(pid, NULL, 0) == -1) {
                perror("waitpit (grandsom)");
                exit(1);
            }
            
            // read -> STDIN
            if (dup2(fd[0], STDIN_FILENO) != STDIN_FILENO) {
                perror("dup2 (read -> STDIN)");
                exit(1);
            }

            /*char buff[10000];
            ssize_t sizeRead;
            while ((sizeRead = read(STDIN_FILENO, buff, 10000)) > 0) {
                write(STDOUT_FILENO, buff, sizeRead);
            }*/

            executeCmd(rightPart);
        }

    } else { // single cmd (no pipe)
        executeCmd(buff);
    }  
}

void execGransdom(int fd[2], char* leftPart) {
    close(fd[0]);

    // STDOUT -> write pipe
    if (dup2(fd[1], STDOUT_FILENO) != STDOUT_FILENO) {
        perror("dup2 (STDOUT -> write)");
        exit(1);
    }

    // execute command
    executeCmd(leftPart);
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

void executeCmd(char *arguments) {
    printf("%s\n", arguments );
    char *argsArray[MAX_ARGS_COUNT], *parseSign = " ";
    int counter = 0;
    
    // parse data (commands and arguments) by spaces
    argsArray[counter] = strtok(arguments, parseSign);
    while (argsArray[counter] != NULL) {
        argsArray[++counter] = strtok(NULL, parseSign);             
    }

    // execute
    argsArray[++counter] = NULL; // null for last parameter is required - determinate size of array
    execvp(argsArray[0], argsArray);
    perror(argsArray[0]);
    exit(127);
}
