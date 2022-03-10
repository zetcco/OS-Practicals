/*

    This program demonstrates the race condition when using multithreading.
    In here, small routine will increment the x 1000 times, so by the time first two threads finished their executions, output x value will be 2000.
    Because, In here, by the time the second thread is getting created, the first one will finish it's execution and x will be incremented to 1000.
    So the two threads will not get executed parallely because it is too fast.
    Then after the second one starts, it will increment x from 1000 to 2000. So the output is correct.

    BUT, when a large routine is used as the thread function, the result will not be doubled. 
    Because, unlike the fist scenario, in here by the time the second thread is started, the first one might not yet completed it's execution.
    So then the second one starts midway (x's value is between 0 and 100000).
    When in these conditions, one of threads might overwrite a wrong increment to x.

    This scenario is knows as a Race Condition.

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void * routine_small();
void * routine_large();

int x = 0;

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, &routine_small, NULL);
    pthread_create(&t2, NULL, &routine_small, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("Value of x after small routine: %d\n", x);

    x = 0;
    pthread_create(&t1, NULL, &routine_large, NULL);
    pthread_create(&t2, NULL, &routine_large, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("Value of x after large routine: %d\n", x);
}

void * routine_large() {
    int i = 0;
    for(; i < 100000 ; i++)
        x++;
}

void * routine_small() {
    int i = 0;
    for(; i < 1000 ; i++)
        x++;
}