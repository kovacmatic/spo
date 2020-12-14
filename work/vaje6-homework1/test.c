#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define N_THR 5

int a = 0;
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;

void *funkcija_niti(void *arg)
{
    int tid;
    tid = (int)arg;
    pthread_mutex_lock(&mymutex);
    printf("Increase\n");
    a++;
    pthread_mutex_unlock(&mymutex);
    printf("Pozdrav od niti %d!\n", tid);
    pthread_exit(NULL);
}
int main(int argc, char *argv[])
{
    pthread_t id[N_THR];
    int i;
    int ret;
    pthread_mutex_init(&mymutex, NULL);
    for (i = 0; i < N_THR; i++)
    {
        ret = pthread_create(&id[i], NULL, funkcija_niti, (void *)i);
        if (ret)
        {
            printf("Napaka (%d)\n", ret);
            exit(1);
        }
    }

    for(i = 0; i < N_THR; i++) {
        pthread_join(id[i], NULL);
    }

    pthread_mutex_destroy(&mymutex);

    printf("a: %d\n", a);
    pthread_exit(NULL);
}