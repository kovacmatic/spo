/*
ZAPISOVALEC:
    • Odpre datoteko za pisanje – ime dobi kot
        vhodni argument. Pridobi indentifikator
        sporočilne vrste.
    • Bere sporočila iz vrste in jih zapisuje v
        datoteko.
    • Ko prejme prazno sporočilo, zapre datoteko,
        pobriše sporočilno vrsto in se konča.

    Koristno:
    terminalska ukaza ipcs (status) in ipcrm (brisanje)
*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define error(message) { fprintf(stderr, "%s\n", message); exit(1); }
#define ARGS_COUNTR_REQUIRED 2
#define MSG_KEY 3000
#define MSG_SIZE 1024

void validateArgsCount(int argc);
int openFile(const char *fileName, int flags);

int main(int argc, char const *argv[]) {

    int fd, msgId;
    
    struct mymsg {
        long mtype;
        char mtext[MSG_SIZE];
    } message;
    message.mtype = 0;

    validateArgsCount(argc);

    // open/create file for writing
    fd = openFile(argv[1], O_CREAT | O_WRONLY | O_TRUNC);

    // ustvari novo spročilo vrsto
    if ((msgId = msgget(MSG_KEY, 0644)) < 0) {
        perror("msgget");
        exit(1);
    }

    for (;;) {
        // pridobi sporočilo iz vrste
        if (msgrcv(msgId, &message, MSG_SIZE, 1, 0) < 0) {
            perror("msgrcv");
            exit(1);
        }

        printf("Dobil sem sporočilo: %s\n", message.mtext);

        if (message.mtext[0] == '\0') {
            break;
        }
    }

    if (close(fd) < 0) {
        perror("close");
        exit(1);
    }

    return 0;
}


void validateArgsCount(int argc) {
    if (argc != ARGS_COUNTR_REQUIRED) {
        error("Invalid argument count. Example: ./zapisovalec <file-name>")
    }
}

int openFile(const char *fileName, int flags) {
    int fd;
    if ((fd = open(fileName, flags, 0777)) < 0) { // all premissions - 0777
        perror("open");
        exit(1);
    }

    return fd;
}