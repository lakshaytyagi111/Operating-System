// Implement Peterson’s algorithm to solve the critical section problem for two processes.
// Demonstrate mutual exclusion and absence of race conditions.
// a. Use two flags and a turn variable.
// b. Simulate two threads accessing a shared counter

// Peterson's Algorithm using pthreads (Corrected Version)

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#define NUM_ITERATIONS 100

volatile int turn;
volatile bool flag[2] = {false, false};
volatile int shared_counter = 0;

void* proc(void* arg) {
    int i = *(int*)arg;
    int j = 1 - i;

    for (int k = 0; k < NUM_ITERATIONS; k++) {

        // ENTRY SECTION
        flag[i] = true;
        turn = j;

        while (flag[j] && turn == j) {
            // Busy wait
        }

        // ---- CRITICAL SECTION ----
        int temp = shared_counter;
        temp++;
        shared_counter = temp;
        // ---------------------------

        // EXIT SECTION
        flag[i] = false;

        // Remainder section (optional)
        // usleep(100); 
    }

    return NULL;
}

int main() {
    pthread_t t0, t1;
    int id0 = 0, id1 = 1;

    pthread_create(&t0, NULL, proc, &id0);
    pthread_create(&t1, NULL, proc, &id1);

    pthread_join(t0, NULL);
    pthread_join(t1, NULL);

    printf("Final Counter Value = %d\n", shared_counter);

    // Expected: 200 (100 increments by each process)

    return 0;
}
