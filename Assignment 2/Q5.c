#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t mutex, wrt;
int readcount = 0;

void* reader(void* arg) {
    int id = *((int*)arg);
    while (1) {
        sem_wait(&mutex);
        readcount++;
        if (readcount == 1)
            sem_wait(&wrt); // first reader locks writers
        sem_post(&mutex);

        // Reading section
        printf("Reader %d is reading\n", id);
        sleep(1); // simulate reading

        sem_wait(&mutex);
        readcount--;
        if (readcount == 0)
            sem_post(&wrt); // last reader releases writers
        sem_post(&mutex);

        sleep(rand() % 3); // simulate time before next read
    }
    return NULL;
}

void* writer(void* arg) {
    int id = *((int*)arg);
    while (1) {
        sem_wait(&wrt);
        // Writing section
        printf("Writer %d is writing\n", id);
        sleep(2); // simulate writing
        sem_post(&wrt);

        sleep(rand() % 5); // simulate time before next write
    }
    return NULL;
}

int main() {
    int i;
    pthread_t r[5], w[2];
    int r_ids[5], w_ids[2];

    sem_init(&mutex, 0, 1);
    sem_init(&wrt, 0, 1);

    for (i = 0; i < 5; i++) {
        r_ids[i] = i + 1;
        pthread_create(&r[i], NULL, reader, &r_ids[i]);
    }
    for (i = 0; i < 2; i++) {
        w_ids[i] = i + 1;
        pthread_create(&w[i], NULL, writer, &w_ids[i]);
    }

    for (i = 0; i < 5; i++) pthread_join(r[i], NULL);
    for (i = 0; i < 2; i++) pthread_join(w[i], NULL);

    sem_destroy(&mutex);
    sem_destroy(&wrt);
    return 0;
}
