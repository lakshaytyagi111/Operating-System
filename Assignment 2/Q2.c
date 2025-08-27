// Implement a C program using POSIX semaphores to simulate two processes accessing a
// shared resource. Use sem_wait and sem_post to control access.
// a. Initialize a binary semaphore with value 1.
// b. Use two threads to increment a shared variable.
// c. Ensure only one thread accesses the shared variable at a time.

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#define NUM_ITERATIONS 10000

int counter = 0;
sem_t sem;

void* decrement_function(void* arg){
    for (int i = 0; i < NUM_ITERATIONS; i++){
        sem_wait(&sem);
        counter++;
        sem_post(&sem);
    }
    return NULL;
}

int main(){
    pthread_t thread1;
    pthread_t thread2;
    if (sem_init(&sem, 0, 1) != 0){
        printf("Failed to initialise the semaphore");
        return 1;
    }

    pthread_create(&thread1, NULL, decrement_function, NULL);
    pthread_create(&thread2, NULL, decrement_function, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    sem_destroy(&sem);
    printf("Final result : %d", counter);
    return 0;
}
