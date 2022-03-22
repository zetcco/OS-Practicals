/*
    pthread_exit() is used to exit from a thread.
    What if we called it in the main() ?

    > Usually, if we called exit or return in main, the process will exit while killing it's threads even though they are not done. If we want to 
      terminate the main process, but we do not need to kill it's threads, then we can use pthread_exit()

*/

#include <stdio.h> 
#include <stdlib.h> 
#include <pthread.h> 
#include <unistd.h>
 
#define NUM_THREADS 5 

void* job(void * val);
 
int main() {
    pthread_t threads[NUM_THREADS];

    int i = 0;
    for (; i < NUM_THREADS ; i++) {
        if (pthread_create(&threads[i], NULL, job, &i) != 0) {
            printf("Error when creating thread[%d]\n", i);
        }
    }
    printf("Completed forging threads\n");
    pthread_exit(NULL);                             // If this is called, the main process will terminate instantly without killing it's threads and let them run.
    // exit(0);                                     // If this is called, then main process will terminate instantly while killing it's all the threads.
    printf("After calling exit, pthread_exit\n");
}

void* job(void * val) {
    int id = *(int *)val;
    printf("Thread[%d] started.\n", id);
    sleep(5);
    printf("Thread[%d] completed.\n", id);
    pthread_exit(NULL);
}