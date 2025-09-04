// Write a program to simulate the Producer-Consumer problem using semaphores and a
// bounded buffer. Ensure synchronization between producer and consumer threads

#include <bits/stdc++.h>
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

void *producer(void* arg){
    int item;
    for (int i = 0; i < total_goods; i++){

        item = rand() % 100;
        printf("Producer Produced: %d\n", item);

        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
                
        buffer[in] = item;
        in = (in +1) % buffer_size;

        sem_post(&full);
        pthread_mutex_unlock(&mutex);

    }
    return NULL;
}

void *consumer(void* arg){
    int item;
    for (int i = 0; i < total_goods; i++){
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
                
        item = buffer[out];
        out = (out +1) % buffer_size;
        
        sem_post(&empty);
        pthread_mutex_unlock(&mutex);

        printf("Consumer consumed: %d\n", item);

    }
    return NULL;
}

int main(){
    pthread_t cons, prod;

    sem_init(&empty, 0, buffer_size);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&cons, NULL, consumer, NULL);
    pthread_create(&prod, NULL, producer, NULL);

    pthread_join(cons, NULL);
    pthread_join(prod, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}
