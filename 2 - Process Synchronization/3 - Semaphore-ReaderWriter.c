/*
    This is the same Reader/Writer problem as the 14th Assignemnt question. The only difference is that the solution is to use Semaphores.

    There is a buffer, Writer writes to that buffer, Reader reads from that buffer. Buffer is a circular queue. (Circular queue is used because
    we can run many test runs (TEST_RUNS to define how much) as we want without worrying about the size of the buffer. ). So there must be 
    two pointers, front and rear. So, circular queue buffer, front, rear should be sharable accross the writer and the reader. So Shared memory technique 
    mmap is used.

    How to synchronize the two?

    We have to use semaphores. Actualy in this case, two semaphores. Why? 
    Because, There are two processes and there are conditions when one process should wait, while other finish some work.
    When a semaphore is 0, waiting occurs until it is incremented. So If there are only one semaphore, and that semaphore is 0, One process will wait. What about 
    the other? Well since the semaphore is 0 and there is only one semaphore, it will also wait. So that won't work and we have to use 2 semaphores.
    One semaphore will represent number of free spaces available in the buffer, the other will represent the number of used buffer spaces.

    How to get semaphores?
    Using semget(), then initialize them using semctl(). And use semop() to do operations (incremention, waits, decrementions) on them.
    semget() will return a set of semaphores (how much we need can be defined). Semaphore id is used to represent that set. And how to use
    a certain semaphore among that set? Array indexing. If we get semaphore id with 4 semaphores. 0,1,2,3 will represent the four of them respectively.
    In this example, we needed 2. And we have two variables to represent them(0 and 1) (EMPTY_SEM = 0) and (FULL_SEM = 1).

    What are P() and V()?
    P() will wait if the given semaphore is 0, and if not, then it will decrement the semaphore by one and enter the critical section to perform the operations.
    V() will increment the given semaphore by 1.

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/mman.h>

#define BUFF_LEN 5      // size of the buffer
#define TEST_RUNS 10    // Number of test runs
#define EMPTY_SEM 0     // Semaphore number to represent empty buffer spaces
#define FULL_SEM 1      // Semaphore number to represent full buffer spaces

int * allocate_sharedMem(size_t size);                  // Allocate shared memory spaces
void deallocate_sharedMem(void * addr, size_t size);    // Deallocate shared memory spaces
void increment(int * iter , int size);                  // Increment front, rear pointers so that it works for circular queues
void consume(int * buffer, int * front, int iter);      // Consume a full buffer space, creating a new empty buffer space
void produce(int * buffer, int * rear, int iter);       // Produce a full buffer space, removing a empty buffer space
int get_semaphore();                                    // Get two semaphore, one to represent Number of empty buffer spaces, other to represent full buffer spaces
void P(int sem_id, int sem_num);                        // Wait if given sem_num is 0, otherwise continue to do the operation and decrement the semaphore
void V(int sem_id, int sem_num);                        // Increment the semaphore

int main() {
    int pid;                                            // PID for child process (Reader process)
    int *buffer, *front, *rear;                         // Pointers for circular queue buffer (buffer - starting location, front - To store front pointer value, rear - to store rear pointer value)
    int sem_id;                                         // Semaphore ID to represent set of semaphores

    /* Allocated shared memory for the buffer and the front, rear and initialize front, rear */
    buffer = allocate_sharedMem(BUFF_LEN*sizeof(int));
    front = buffer + BUFF_LEN;
    rear = buffer + BUFF_LEN + 1;
    *front = *rear = 0;

    /* Creating and Initializng semaphore */
    sem_id = get_semaphore();

    /* Reader/Writer process */
    if ((pid = fork()) == -1) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        printf("Reader initiated\n");
        int i = 0;
        for (; i < TEST_RUNS ; i++) {

            P(sem_id, FULL_SEM);        // Wait on this line for Full buffer spaces if No. of full buffer spaces are 0 (i.e. FULL_SEM = 0), if not continue
            /* Critical Section */
            consume(buffer, front, i);
            increment(front, BUFF_LEN);
            /* Critical Section End */
            V(sem_id, EMPTY_SEM);       // Increment number of Empty buffer spaces

        }
    } else {
        printf("Writer initiated\n");
        int i = 0;
        for (; i < TEST_RUNS ; i++) {

            P(sem_id, EMPTY_SEM);       // Wait on this line for Full buffer spaces if No. of empty buffer spaces are 0 (i.e. EMPTY_SEM = 0), if not continue
            /* Critical Section */
            produce(buffer, rear, i);
            increment(rear, BUFF_LEN);
            /* Critical Section End */
            V(sem_id, FULL_SEM);        // Increment number of Full buffer spaces

            if (i%4 == 0) sleep(10);    // This is to show the semaphore process in action. Observe the stdout. Reader pauses after reading all available full buffer spaces.

        }
    }

    /* Deallocate shared memory */
    deallocate_sharedMem(buffer, BUFF_LEN*sizeof(int));

}

int * allocate_sharedMem(size_t size) {
    int *buffer;

    if ((buffer = (int *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED) {
        perror("Shared memory allocation failed");
        exit(2);   
    }

    return buffer;
}

void deallocate_sharedMem(void * addr, size_t size) {
    if (munmap(addr, size) == -1) {
        perror("Shared memory deallocation failed");
        exit(3);
    }
}

void increment(int * iter , int size) {
    *iter = (*iter + 1) % size;
}

void consume(int * buffer, int * front, int iter) {
    printf("[-] Read: %d\tIteration: %d\n", *(buffer + *front), iter);
}

void produce(int * buffer, int * rear, int iter) {
    *(buffer + *rear) = iter*iter;
    printf("[-] Write: %d\tIteration: %d\n", *(buffer + *rear), iter);
}

int get_semaphore() {
    int sem_id;

    // IPC_PRIVATE means new set of semaphore, 2 means number of semaphores | 0600 Means Read and Write for Owner, No permission for others... See man page for this
    if ((sem_id = semget(IPC_PRIVATE, 2, IPC_CREAT | 0600)) == -1) {    
        perror("Getting semaphore error");
        exit(5);
    }

    /* Initialize the two semaphores. */

    // Set no of empty buffer semaphore to BUFF_LEN (i.e. At start there are no any full buffer spaces, all are empty)
    /*
        First argument - Semaphore ID, (which obtained from above)
        Second arguement - Semaphore number to tell which semaphore 
        Third arguement - A flag to tell that we are goint to initialize a value to the semaphore
        Fourth arguement - Value to set
    */
    if (semctl(sem_id, EMPTY_SEM, SETVAL, BUFF_LEN) == -1) {        // First arugement i
        perror("Initializing Empty Semaphore to BUF_LEN error");
        exit(5);
    }

    // Set no of full buffer semaphore to 0, (i.e. At start there are no any full bffer spaces)
    if (semctl(sem_id, FULL_SEM, SETVAL, 0) == -1) {
        perror("Initializing Empty Semaphore to 0 error");
        exit(5);
    }
}

void V(int sem_id, int sem_num) {
    struct sembuf semOperation[1];                  // Array of 'struct sembuf' used to define the operation to be done to the semaphore

    semOperation[0].sem_num = sem_num;              // Give the semaphore number
    semOperation[0].sem_op = 1;                     // Give any positive number, to indicate incremention
    semOperation[0].sem_flg = 0;                    // Give 0 flag. (see man page example)

    if (semop(sem_id, semOperation, 1) == -1) {     // Give the semaphore id, above struct array and the length of the above struct array
        perror("Incrementing semaphore error");
        exit(6);
    }
}

void P(int sem_id, int sem_num) {
    struct sembuf semOperation[1];                  // Array of 'struct sembuf' used to define the operation to be done to the semaphore

    semOperation[0].sem_num = sem_num;              // Give the semaphore number
    semOperation[0].sem_op = -1;                    // Give any negative number, to indicate decremention
    semOperation[0].sem_flg = 0;                    // Give 0 flag. (see man page example)

    if (semop(sem_id, semOperation, 1) == -1) {     // Give the semaphore id, above struct array and the length of the above struct array
        perror("Decrementing semaphore error");
        exit(6);
    }
}