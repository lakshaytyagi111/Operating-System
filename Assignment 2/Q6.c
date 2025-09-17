#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5
sem_t mutex;

void* process(void* arg) {
    int id = *((int*)arg);
    while (1) {
        sem_wait(&mutex); // enter critical section
        printf("Process %d is in critical section\n", id);
        sleep(1); // simulate critical section
        printf("Process %d leaving critical section\n", id);
        sem_post(&mutex); // leave critical section
        sleep(rand() % 3); // non-critical section
    }
    return NULL;
}

int main() {
    pthread_t threads[N];
    int ids[N];
    sem_init(&mutex, 0, 1); // binary semaphore for mutual exclusion

    for (int i = 0; i < N; i++) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, process, &ids[i]);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&mutex);
    return 0;
}
