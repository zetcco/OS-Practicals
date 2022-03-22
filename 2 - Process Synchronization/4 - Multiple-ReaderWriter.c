#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/sem.h>

#define NO_READERS 5
#define NO_WRITERS 6
#define READER_TEST_RUNS 12
#define WRITER_TEST_RUNS 10
#define BUFF_LEN 10
#define mutex 0
#define empty_slot 1
#define full_slot 2

void increment(int* iter, int size);
void produce(int* buffer, int* rear, int i, int proc_num);
void consume(int* buffer, int* front, int i, int proc_num);
int get_semaphore(int count);
void init_semaphore(int sem_id, int sem_num, int init_value);
void P(int sem_id, int sem_num);
void V(int sem_id, int sem_num);

int main() {
    int pid, iSpawn, process[NO_READERS + NO_WRITERS];
    int* buffer, *front, *rear;
    int sem_id;

    if ((buffer = mmap(NULL, BUFF_LEN*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED) {
        perror("Shared memory allocation failed");
        exit(1);
    }
    front = buffer + BUFF_LEN;
    rear = front + 1;
    *front = *rear = 0;

    sem_id = get_semaphore(3);
    init_semaphore(sem_id, mutex, 1);
    init_semaphore(sem_id, empty_slot, BUFF_LEN);
    init_semaphore(sem_id, full_slot, 0);

    for (iSpawn = 0 ; iSpawn < NO_READERS ; iSpawn++) {
        if ((pid = fork()) == -1) {
            perror("Failed to spawn reader");
        } else if (pid == 0) {
            printf("Spawned reader\n");
            int i = 0;
            for (; i < READER_TEST_RUNS ; i++) {
                P(sem_id, full_slot);
                P(sem_id, mutex);
                consume(buffer, front, i, iSpawn); 
                increment(front, BUFF_LEN);
                V(sem_id, mutex);
                V(sem_id, empty_slot);
                if ((i+iSpawn)%5 == 0) sleep(1);
            }
            printf("Reader done.\n");
            exit(0);
        } else {
            process[iSpawn] = pid;
        }
    }

    printf("Completed pawning all the reader processes\n");

    for (iSpawn = 0 ; iSpawn < NO_WRITERS ; iSpawn++) {
        if ((pid = fork()) == -1) {
            perror("Failed to spawn writer");
        } else if (pid == 0) {
            printf("Spawned writer\n");
            int i = 0;
            for (; i < WRITER_TEST_RUNS; i++) {
                P(sem_id, empty_slot);
                P(sem_id, mutex);
                produce(buffer, rear, i, iSpawn);
                increment(rear, BUFF_LEN);
                V(sem_id, mutex);
                V(sem_id, full_slot);
            }
            printf("Writer done.\n");
            exit(0);
        } else {
            process[iSpawn + NO_READERS] = pid;
        }
    }

    printf("Completed pawning all the writer processes\n");

    for (iSpawn = 0 ; iSpawn < NO_READERS + NO_WRITERS ; iSpawn++) 
        waitpid(process[iSpawn], NULL, 0);

    if (munmap(buffer, BUFF_LEN*sizeof(int)) == -1) {
        perror("Shared memory deallocation failed");
        exit(1);
    }
}

void increment(int* iter, int size) {
    *iter = (*iter + 1) % size;
}

void produce(int* buffer, int* rear, int i, int proc_num) {
    *(buffer + *rear) = 100*proc_num + i;
}

void consume(int* buffer, int* front, int i, int proc_num) {
    // printf("Iteration:%d\tConsumed: %d\n", i, );
    printf ("Reader %d: item %2d == %2d\n", proc_num, i, *(buffer + *front));
}

int get_semaphore(int count) {
    int sem_id;

    if ((sem_id = semget(IPC_PRIVATE, count, IPC_CREAT | 0600)) == -1) {
        perror("Getting semaphore error");
        exit(2);
    }

    return sem_id;
}

void init_semaphore(int sem_id, int sem_num, int init_value) {
    if (semctl(sem_id, sem_num, SETVAL, init_value) == -1) {
        perror("Error initializing semaphore");
        exit(3);
    }
}

void P(int sem_id, int sem_num) {
    struct sembuf sem_operations[1];
    sem_operations[0].sem_num = sem_num;
    sem_operations[0].sem_op = -1;
    sem_operations[0].sem_flg = 0;

    if (semop(sem_id, sem_operations, 1) == -1) {
        perror("Semaphore decrement operation failed");
    }
}

void V(int sem_id, int sem_num) {
    struct sembuf sem_operations[1];
    sem_operations[0].sem_num = sem_num;
    sem_operations[0].sem_op = 1;
    sem_operations[0].sem_flg = 0;

    if (semop(sem_id, sem_operations, 1) == -1) {
        perror("Semaphore increment operation failed");
    }
}