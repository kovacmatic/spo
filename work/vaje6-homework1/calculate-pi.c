#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdbool.h>

#define error(message) { fprintf(stderr, "%s\n", message); exit(1); }

#define NR_THR 3 // število niti
#define N_THREAD_COUNT 10000 // koliko poiskusov naredi vsaka nit, preden posodobi globalno stanje
#define N_MAX_COUNT 100000000 // maksimalno število poiskusov, preden se računanje konča

pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;

// global variables
int Zs = 0; // skupno število v krog zadetih poiskusov 
int Ns = 0; // skupno število poiskusov

// functions
double get_rand_number();
bool is_hit_inside_circle(double x, double y);
void *try_hit_circle(void *arg);
double calculate_pi();

int main(int argc, char const *argv[])
{
    pthread_t id[NR_THR];

    // ustvarimo niti
    for(int i=0; i < NR_THR; i++) {
        pthread_create(&id[i], NULL, try_hit_circle, NULL);
    }

    // počakamo vse niti
    for(int i=0; i < NR_THR; i++) {
        pthread_join(id[i], NULL);
    }

    // prikaži izračunan PI
    printf("Zs: %d\n", Zs);
    printf("Ns: %d\n", Ns);
    printf("PI: %f\n", calculate_pi());
    return 0;
}

void *try_hit_circle(void *arg) {

    int n = 0;
    int z = 0;

    for (;;) {

        // preveri ali je MAX št hitov preseženo
        if (Ns >= N_MAX_COUNT) {
            pthread_exit(NULL);
        }

        // update local stanje
        n++;
        if (is_hit_inside_circle(get_rand_number(), get_rand_number())) {
            z++;
        }

        // posodobi globalno stanje
        if (n == N_THREAD_COUNT) {
            pthread_mutex_lock(&mymutex);
            Ns += n;
            Zs += z;
            pthread_mutex_unlock(&mymutex);

            // reset
            n = 0;
            z = 0;
        }
    }
}

double get_rand_number() {
    struct timespec myTime;
    clock_gettime(CLOCK_REALTIME, &myTime); // program prevedi s stikalom -lrt
    unsigned int seed = myTime.tv_nsec;
    return ((double) rand_r(&seed)) / RAND_MAX; // rand() ni „thread-safe“
}

bool is_hit_inside_circle(double x, double y) {
    return (x*x) + (y*y) <= 1;
}

double calculate_pi() {
    return ((double)(4*Zs))/Ns;
}