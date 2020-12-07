/*
POSREDNIK:
    • Ustvari novo sporočilno vrsto. Pridobi identifikatorja semaforja in deljenega pomnilnika.
    • Na eno sekundo dostopa do deljenega pomnilnika
        in prebere vsebino. Dostop je usklajen z
        generatorjem s semaforji.
    • Vsebino nato pošlje v sporočilno vrsto, vendar
        samo, če je bilo drugačno od prejšnjega. S tem
        želimo zagotoviti, da bodo v sporočilni vrsti samo
        unikatna sporočila.
    • Zanka branja in pošiljanja sporočil se zaključi, ko
        prebere prazno sporočilo (5. sporočilo
        generatorja). Prazno sporočilo prav tako pošlje
        naprej v sporočilno vrsto. Nato izbriše semaforno
        polje in deljeni pomnilnik ter se konča.
*/

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define SEM_KEY 1000
#define SHM_KEY 2000
#define MSG_KEY 3000
#define SHM_SIZE 1024

void semaphoreLock(int semId, struct sembuf *op);
void semaphoreUnLock(int semId, struct sembuf *op);
void setOperationsLocked(struct sembuf *op);
void setOperationsUnLocked(struct sembuf *op);

int main(int argc, char const *argv[]) {

    int msgId, shmId, semId;
    char *shmAddr;
    struct sembuf operations[1];

    struct mymsg {
        long mtype;
        char mtext[SHM_SIZE];
    } message;
    message.mtype = 1;

    // ustvari novo spročilo vrsto
    if ((msgId = msgget(MSG_KEY, 0644 | IPC_CREAT)) < 0) {
        perror("msgget");
        exit(1);
    }

    // odpiranje semaforja
    if ((semId = semget(SEM_KEY, 1, 0664)) < 0) {
        perror("semget");
        exit(1);
    }

    // odpiranje deljenega pomnilnika
    if ((shmId = shmget(SHM_KEY, SHM_SIZE, 0644)) < 0) {
        perror("shmget");
        exit(1);
    }

    // priklop (attach) na deljeni pomnilnik
    if ((shmAddr = (char *) shmat(shmId, NULL, 0)) < 0) {
        perror("shmat");
        exit(1);
    }

    for (;;) {
        
        // zakleni semafor
        semaphoreLock(semId, &operations[0]);

        // preveri če je različen
        if (strcmp(shmAddr, message.mtext) != 0) {

            // pošlji sporočilo v vrsto
            printf("Pošiljam sporočilo v vrsto: %s\n", shmAddr);
            if (msgsnd(msgId, &message, strlen(message.mtext) + 1, 0) < 0) {
                perror("msgsnd");
                exit(1);
            }
            
            // kopiraj message za primerjavo
            strcpy(message.mtext, shmAddr);
        }

        // odkleni semafor        
        semaphoreUnLock(semId, &operations[0]);

        // preveri če zadnje spročilo in prekini operacijo!
        if (shmAddr[0] == '\0') {
            break;
        }

        sleep(1);
    }
    
    // odklop od deljeni pomnilnik
    if (shmdt(shmAddr) < 0) {
        perror("shmdt");
        exit(1);
    }

    // odstrani segment deljenega pomnilnika
    if (shmctl(shmId, 0, IPC_RMID) < 0) {
        perror("shmctl");
        exit(1);
    }

    // izbriši semaforno vrsto
    if (semctl(semId, 0, IPC_RMID) < 0) {
        perror("semctl");
        exit(1);
    }
    
    return 0;
}

void setOperationsLocked(struct sembuf *op) {
    op->sem_num = 0;
    op->sem_op = -1;
    op->sem_flg = 0;
}

void setOperationsUnLocked(struct sembuf *op) {
    op->sem_num = 0;
    op->sem_op = +1;
    op->sem_flg = 0;
}

void semaphoreLock(int semId, struct sembuf *op) {
    setOperationsLocked(op);
    if (semop(semId, op, 1) < 0) {
        perror("semop: lock");
        exit(1);
    }
}

void semaphoreUnLock(int semId, struct sembuf *op) {
    setOperationsUnLocked(op);
    if (semop(semId, op, 1) < 0) {
        perror("semop: unlock");
        exit(1);
    }
}