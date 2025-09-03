// Write a program to simulate the Producer-Consumer problem using semaphores and a
// bounded buffer. Ensure synchronization between producer and consumer threads

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define total_goods 20
#define buffer_size 10

int buffer[buffer_size];
int in=0, out=0;

sem_t empty;
sem_t full;

pthread_mutex_t mutex;


void *consumer(void* arg){

}

void *producer(void* arg){

}

int main(){
    pthread_t cons, prod;

    sem_init(&empty, 0, buffer_size);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&cons, NULL, consumer, total_goods);
    pthread_create(&prod, NULL, producer, total_goods);

    pthread_join(cons, NULL);
    pthread_join(prod, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex, NULL);

    return 0;
}
