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

void setSignalHandlers();
void signalHandler(int signalNo);
void executeCmd(char *input);
void parsePipeCmd(char* input, char **leftCmdPart, char **rightCmdPart);
void executeParent(pid_t childPid);
void executeChild(char *input);
void executeGrandsom();
void executeChildCommandIfPipe(int fd[2], pid_t grandsomPid, char* rightCmdPart);

int main(int argc, char const *argv[]) {
    pid_t childPid;
    char buff[BUFFER], *input;
    
    setSignalHandlers();

    while ((input = fgets(buff, BUFFER, stdin)))
    {
        input[strlen(input)-1] = '\0'; // making '\n' '\0' - remove last space
        
        // create child
        if ((childPid = fork()) < 0) {
            perror("fork");
            exit(1);
        }

        if (childPid == 0) {
            executeChild(input);
        } else {
            executeParent(childPid);            
        }
    }

    return 0;
}

void executeParent(pid_t childPid) {
    if (waitpid(childPid, NULL, 0) < 0) { // wait child to completes
        perror("waitpid");
        exit(1);
    }
}

void executeChild(char *input) {
    pid_t grandsomPid;
    char *leftCmdPart = 0, *rightCmdPart = 0;
    int fd[2];

    // parse pipe arguments
    parsePipeCmd(input, &leftCmdPart, &rightCmdPart);

    if (rightCmdPart == 0) { // pipe NOT exists in cmd
        executeCmd(input);
    } else { // pipe EXISTS in cmd

        // make pipe
        if (pipe(fd) < 0) {
            perror("pipe");
            exit(1);
        }
        
        // create grandson
        if ((grandsomPid = fork()) < 0) {
            perror("fork (grandsom)");
            exit(1);                
        }

        if (grandsomPid == 0) {
            executeGrandsom(fd, leftCmdPart);
        } else {
            executeChildCommandIfPipe(fd, grandsomPid, rightCmdPart);
        }
    }
}

void executeGrandsom(int fd[2], char* leftCmdPart) {
    close(fd[0]);

    // STDOUT to write (fd[1])
    if (dup2(fd[1], STDOUT_FILENO) != STDOUT_FILENO) {
        perror("dup2 (STDOUT -> write)");
        exit(1);
    }
    executeCmd(leftCmdPart);
}

void executeChildCommandIfPipe(int fd[2], pid_t grandsomPid, char* rightCmdPart) {
    close(fd[1]);

    // wait grandson to completes
    if (waitpid(grandsomPid, NULL, 0) < 0) { 
        perror("waitpid (grandsome)");
        exit(1);
    }
    
    // duplicate fd & execute right part of command                  
    if (dup2(fd[0], STDIN_FILENO) != STDIN_FILENO) {
        perror("dup2 (read -> STDIN_FILENO)");
        exit(1);
    }
    executeCmd(rightCmdPart);
}

void parsePipeCmd(char* input, char **leftCmdPart, char **rightCmdPart) {
    char *parseSign = ">", *leftResult = 0, *rightResult = 0;

    // parse pipe sign (->)
    leftResult = strtok(input, parseSign);

    // check if "->"" exists
    if (leftResult[strlen(leftResult) - 1] == '-') {
        leftResult[strlen(leftResult) - 1] = 0; // remove last character '-'
        rightResult = strtok(NULL, parseSign);
    }

    *rightCmdPart = rightResult;
    *leftCmdPart = leftResult;
}

void setSignalHandlers() {
    if (signal(SIGINT, signalHandler) == SIG_ERR) {
        perror("signal (SIGINT) error");
        exit(1);
    }
    if (signal(SIGALRM, signalHandler) == SIG_ERR) {
        perror("signal (SIGALRM) error");
        exit(1);
    }
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
 
void executeCmd(char *arguments) {
    char *argsArray[ARGS_MAX_COUNT], *parseSign = " ";
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
}
