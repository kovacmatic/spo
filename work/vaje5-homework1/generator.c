/*
Napišite tri programe: generator, posrednik in
zapisovalec.
Programi uporabljajo deljeni pomnilnik in sporočilno vrsto
za posredovanje sporočil drug drugemu: 
    • Deljeni pomnilnik uporabljata programa generator in
    posrednik za izmenjavo sporočil. Dostop do deljenega
    pomnilnika mora biti sinhroniziran s pomočjo
    semaforjev.
    • Sporočilno vrsto uporabljata programa posrednik in
    zapisovalec, in sicer posrednik pošilja sporočila
    zapisovalcu.
    • Generator in posrednik morata uporabljati isti ključ za
    dostop do deljenega pomnilnika in semaforja. Prav tako
    posrednik in zapisovalec uporabljata isti ključ za
    sporočilno vrsto (uporabite ftok() ali si izmislite
    poljubno število)

GENERATOR
    • Ustvari kos deljenega pomnilnika in semafor. Tega
        tudi nastavi na začetno vrednost.
    • Na vsaki 2 sekundi zapiše sporočilo v deljeni
        pomnilnik. Sporočilo naj ima lastno številko in
        nekaj poljubnega besedila. Primer dveh sporočil: 
            • ID 1: To je sporocilo. 
            • ID 2: To je sporocilo.
    • Pošlje naj 5 sporočil. Zadnje (5.) naj bo prazno ('\0').
    • Pred pisanjem v deljeni pomnilnik mora program
        zakleniti semafor in po pisanju ga mora odkleniti.
        Pred odklepom naj čaka še 2 sekundi. Torej:
        program 2 sekundi preživi izven semaforjev in 2
        sekundi znotraj semaforjev (varovanega območja).
*/

#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <string.h>

#define SEM_KEY 1000
#define SHM_KEY 2000
#define SHM_SIZE 1024

void semaphoreLock(int semId, struct sembuf *op);
void semaphoreUnLock(int semId, struct sembuf *op);
void setOperationsLocked(struct sembuf *op);
void setOperationsUnLocked(struct sembuf *op);

int main(int argc, char const *argv[]) {

    int shmId, semId;
    char *shmAddr;
    char message[SHM_SIZE];
    struct sembuf operations[1];

    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    } arg;

    // ustvarimo semafor
    if ((semId = semget(SEM_KEY, 1, 0664 | IPC_CREAT)) < 0) {
        perror("semget");
        exit(1);
    }

    // začetna vrednost semaforja
    arg.val = 1;
    if (semctl(semId, 0, SETVAL, arg) < 0) {
        perror("semctl");
        exit(1);
    }

    // naredi deljeni pomnilnik
    if ((shmId = shmget(SHM_KEY, SHM_SIZE, 0644 | IPC_CREAT)) < 0) {
        perror("shmget");
        exit(1);
    }

    // priklop na deljeni pomnilnik    
    if ((shmAddr = (char *) shmat(shmId, NULL, 0)) < 0) {
        perror("shmat");
        exit(1);
    }

    // 5 sporočil
    for (int i = 1; i<=5; i++) {

        // zakleni semafor
        semaphoreLock(semId, &operations[0]);

        // generiraj spročilo
        if (i == 5) {
            message[0] = '\0';
        } else {
            sprintf(message, "ID %d: To je sporočilo!", i);
        }

        // kopiraj na deljeni pomnilnik
        strncpy(shmAddr, message, SHM_SIZE);
        printf("Message sent! %s\n", message);

        // počakaj 2 sekundi
        sleep(2);

        // odkleni semafor
        semaphoreUnLock(semId, &operations[0]);

        // počakaj 2 sekundi
        sleep(2);
    }

    // odklop od deljenega pomnilnika
    if (shmdt(shmAddr) < 0) {
        perror("shmdt");
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