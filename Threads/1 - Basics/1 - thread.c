/*
    Threads are executed paralelly inside a function, sort of like sub processes inside a process. (Observe the result of printf functions which prints PPID and PID)
    This example shows basic usage of threads in c.

    pthread library is used to implement multi threading features on c. (#include <pthread.h>)
    Task assigned to a threads should be defined by using a function, and in order to run it as a thread,
        1. Create pthread_t structure datatype variables. They are used to specify additional attributes when running that thread.
        2. Then using that created structure instance, and a specified function, we can start a thread using pthread_create() function. (man pthread_create)
            Arguments to that function are,
                1 - Address of that created structure. (This function will set other imporant members on that structure)
                2 - Takes pthread_attr_t *, Used to specifiy additional attributes when running the thread.
                3 - Pointer to the required function
                4 - Parameters to that functions. Passed using a structure pointer. 
                    (Create some structure, set it's memebers then set function's input argument as that created structure address. then use it as needed)
            Then, this will start executing the thread.
            *** THAT FUNCTION MUST RETURN AN ADDRESS ***
        3. Next we can wait until that thread is finished if needed. To do that pthread_join() is used.

        This code also further demonstrates the differences between threads and multi processing.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>

void * job_1();
void * job_2();

int x = 1;

int main() {
    pthread_t j1, j2;
    pthread_create(&j1, NULL, &job_1, NULL);
    pthread_create(&j2, NULL, &job_2, NULL);
    sleep(1);
    pthread_join(j2, NULL);
    pthread_join(j1, NULL);


    // Code below only demonstrates COW feature of fork() to demonstrate difference between threads and multi processing.
    int pid;
    printf("Forking.., Value of x: %d\n", x);
    if ((pid = fork()) == 0) {
        printf("Inside child process, PID: %d, PPID: %d, Value of x: %d\n", getpid(), getppid(), x);
        x++;
        printf("Inside child process, PID: %d, PPID: %d, Value of x after modification in child: %d\n", getpid(), getppid(), x);
    } else {
        wait(NULL);
        printf("Inside Parent process, PID: %d, PPID: %d, Value of x after modification in child: %d\n", getpid(), getppid(), x);
    }
    return 0;
}

void * job_1() {
    x++;
    printf("Started job 1, PID: %d, Parent PID: %d, Value of X: %d\n", getpid(), getppid(), x);
    sleep(2);
    printf("Ended job 1\n");
}

void * job_2() {
    printf("Started job 2, PID: %d, Parent PID: %d, Value of X: %d\n", getpid(), getppid(), x);
    sleep(2);
    printf("Ended job 2\n");
}