#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_USERS 5

sem_t printer_mutex;

void* print_job(void* arg) {
    int id = *((int*)arg);
    printf("User %d wants to print\n", id);
    sem_wait(&printer_mutex);
    printf("User %d is printing\n", id);
    sleep(2); // simulate printing time
    printf("User %d finished printing\n", id);
    sem_post(&printer_mutex);
    return NULL;
}

int main() {
    pthread_t users[NUM_USERS];
    int ids[NUM_USERS];

    sem_init(&printer_mutex, 0, 1);

    for (int i = 0; i < NUM_USERS; i++) {
        ids[i] = i + 1;
        pthread_create(&users[i], NULL, print_job, &ids[i]);
    }

    for (int i = 0; i < NUM_USERS; i++) {
        pthread_join(users[i], NULL);
    }

    sem_destroy(&printer_mutex);
    return 0;
}
