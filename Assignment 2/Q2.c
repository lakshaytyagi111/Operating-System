// Implement a C program using POSIX semaphores to simulate two processes accessing a
// shared resource. Use sem_wait and sem_post to control access.
// a. Initialize a binary semaphore with value 1.
// b. Use two threads to increment a shared variable.
// c. Ensure only one thread accesses the shared variable at a time.

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

