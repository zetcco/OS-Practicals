/*

    In order to solve the Race Condition, one solution is to use Mutex.
    Mutex is like a door/lock, and the data (in here 'x') is like a room. Problem arises when both threads access the 
    room at once and tries to change it. So, we can use a door (in other words a mutex).
    Once a thread arrives at the door and if the door is open, it enters the room, we tell it to close it.
    Then do whatever it wants to the data and reopen the door and exit.
    Then other threads can enter the room. 

    In order to implement that, we must use,

    Initialize a global pthread_mutex_t struct type mutex as the lock/door. Then initialize it using 'pthread_mutex_init'.
    And then, set 'pthread_mutex_lock' before the data manipulation, and 'pthread_mutex_unlock' after data manipulation.
    All the waiting of other processes are handled by the mutex.

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void * routine_large();
pthread_mutex_t mutex;

int x = 0;
int door = 1;

int main() {
    pthread_t t1, t2;
    pthread_mutex_init(&mutex, NULL);
    pthread_create(&t1, NULL, &routine_large, NULL);
    pthread_create(&t2, NULL, &routine_large, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_mutex_destroy(&mutex);
    printf("Value of x: %d\n", x);
}

void * routine_large() {
    int i = 0;
    for (i = 0 ; i < 100000 ; i++) {
        pthread_mutex_lock(&mutex);
        x++;
        pthread_mutex_unlock(&mutex);
    }
}