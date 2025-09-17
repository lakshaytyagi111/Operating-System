#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_THREADS 5
#define TOTAL_TICKETS 10

int tickets = TOTAL_TICKETS;
sem_t ticket_mutex;

void* book_ticket(void* arg) {
    int id = *((int*)arg);
    while (1) {
        sem_wait(&ticket_mutex);
        if (tickets > 0) {
            printf("User %d booked ticket %d\n", id, tickets);
            tickets--;
            sem_post(&ticket_mutex);
            sleep(rand() % 2);
        } else {
            sem_post(&ticket_mutex);
            printf("User %d: Tickets sold out!\n", id);
            break;
        }
    }
    return NULL;
}

int main() {
    pthread_t users[NUM_THREADS];
    int ids[NUM_THREADS];

    sem_init(&ticket_mutex, 0, 1);

    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i + 1;
        pthread_create(&users[i], NULL, book_ticket, &ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(users[i], NULL);
    }

    sem_destroy(&ticket_mutex);
    return 0;
}
