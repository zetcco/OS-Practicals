/*

    ** NOTE: In assignment 14, this question is given as Readers/Writers problem, but actualy I think this must be Producer/Consumer.

    In this question,
        There is a buffer (a circular queue) where there are two entities, One who puts data into that buffer, namely 'Producer'
        and the other, who consumes data that is inputted by the 'Producer'. The catch is that, Consumer should not be able to
        consume data when there is no data available and it must wait until at least one data is added to the buffer by the producer.
        Also, Producer should not be able to put data into the buffer when there is no space availble on the buffer and must wait until
        consumer consumes at least one data in order to put at least one data to the buffer.
        So, Think Producer and Consumer as concurrently executing processes. So they must be synchronized, which is our goal.

    In order to achieve that goal,
        We have to use spinlocks. Spinlocks are basically waiting until some condition occurs.
        They can be implemented using while loops ( while (some_condition);  <- Notice that ';' is neccessary  )
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#define DATA_LEN 5
#define BUFF_LEN (DATA_LEN + 2)
#define TEST_RUNS 10

int main() {
    pid_t pid;
    int* buffer;
    if ((buffer = mmap(NULL, BUFF_LEN * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED) {
        perror("Shared Memory allocation Error");
        exit(1);
    }

    int* queue = buffer;
    int* front = buffer + DATA_LEN;
    int* rear  = buffer + DATA_LEN + 1;

    *front = *rear = 0;

    if ((pid = fork()) == -1) {
        perror("Fork failed");
        exit(2);
    } else if (pid == 0) {
        /* Reader Process */
        int i;
        for (i = 0 ; i < TEST_RUNS ; i++) {
            while (*front == *rear);        // Wait until some data is added. When no data is on the buffer, front and rear will have same index number
            printf("Child\t Read Value: %d, Index: %d\t(%p)\n", *(queue + *front), *front, queue + *front);
            *front = (*front + 1) % DATA_LEN;   // Increment front by one after some data is consumed
        }
    } else {
        /* Writer process */
        int i;
        for (i = 0 ; i < TEST_RUNS ; i++) {
            while ( *front == (*rear + 1) % DATA_LEN );     // Wait until some space are available. When no space is available, index just after rear will be front.
            *(queue + *rear) = i*i;
            printf("Parent\t Wrote Value: %d, Index: %d\t(%p)\n", *(queue + *rear), *rear, queue + *rear);
            *rear = (*rear + 1) % DATA_LEN;     // Increment rear by one after some data is added to buffer
        }
    }
}