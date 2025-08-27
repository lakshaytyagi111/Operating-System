// Write a C program using pthread_mutex to increment a shared counter variable using two
// threads. Ensure mutual exclusion so that the final value of the counter is correct.
// a. Use pthread_create to spawn two threads.
// b. Protect the critical section using pthread_mutex_lock and pthread_mutex_unlock.

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_ITERATIONS 1000000

int counter = 1;
pthread_mutex_t lock;

void* increament_counter(void* arg){
    for (int i = 0; i < NUM_ITERATIONS; i++){
        pthread_mutex_lock(&lock);
        counter++;
        pthread_mutex_unlock(&lock);
    }
}

int main(){
    pthread_t thread1;
    pthread_t thread2;

    if (pthread_mutex_init(&lock, NULL) != 0){
        printf("Failed to initialise the mutex");
        return 1;
    }
    pthread_create(&thread1, NULL, increament_counter, NULL);
    pthread_create(&thread2, NULL, increament_counter, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_mutex_destroy(&lock);

    printf("Final counter Value : %d", counter);
    return 0;
}