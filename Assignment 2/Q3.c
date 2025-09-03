// Implement Peterson’s algorithm to solve the critical section problem for two processes.
// Demonstrate mutual exclusion and absence of race conditions.
// a. Use two flags and a turn variable.
// b. Simulate two threads accessing a shared counter

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#define NUM_ITERATIONS 100

int turn;
bool flag[2] = {false, false};

int shared_counter = 100;

void * proc(void * arg){
    int i = *(int *) arg;

    for (int m = 0; m < NUM_ITERATIONS; m++){
        flag[i] = true;
        turn = 1-i;
        while (turn == 1-i && flag[1-i] == true);

        int temp = shared_counter;
        shared_counter = temp + 1;

        flag[i]=false;
    }
    return NULL;
}

int main(){
    pthread_t p0, p1;
    int id0 = 0, id1 = 1;
    pthread_create(&p0, NULL, &proc, &id0);
    pthread_create(&p1, NULL, &proc, &id1);

    pthread_join(p0, NULL);
    pthread_join(p1, NULL);

    printf("final value = %d", shared_counter);
    return 0;
}
