/*

    fork() uses COW, so the resulting two processes will have two different address space. So what if we want to have some data that is 
    common to both processes? This can be done using 'Shared Memory'.
    Shared memory can be implemented using shmget() and mmap(). 
    Our focus is only mmap().

    Look at the man page of mmap (man mmap).

    void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);

    addr    -   Starting address location of the memory. If we do not have a space prior, we can set it to NULL. Kernel will allocate and give it to us.
    length  -   Size of the shared memory that must be allocated
    prot    -   Memory protection, PROT_WRITE means shared memory is writable, PROT_READ means shared memory is readable
    flags   -   Whether to share the memory between other processes or not. 
                MAP_SHARED - Share it between other child processes
                MAP_ANONYMOUS - No file (actual file that can be used to share memory) is used. If this is given, fd can be -1, offset can be 0
    fd      -   File Descriptor
    offset  -   Offset of the file

    This returns a void * that can be casted (like malloc, calloc)

    We do not have to use a file, so we can use function as,
        mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED)

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>

#define ITER 10

int main() {
    int pid;
    int * shared_value;
    if ((shared_value = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED) {
        perror("Failed to allocate shared memory\n");
        exit(1);
    }
    printf("Address of shared memory: %p\n", shared_value);
    if ((pid = fork()) == -1) {
        perror("Fork failed\n");
        exit(1);
    } else if (pid == 0) {
        printf("Child process begins as Reader\n");
        int i = 0;
        for (i ; i < ITER ; i++) {
            sleep(2);
            printf("Child report (of 'shared_value'): %d\n", *shared_value);
        }
        printf("Child execution completed\n");
    } else {
        printf("Parent process begins as writer\n");
        int i = 0;
        for (i ; i < ITER ; i++) {
            *shared_value = i*i;
            printf("Parent updated 'shared_value' to %d\n", *shared_value);
            sleep(2);
        }
        wait(NULL);
        printf("Parent process execution completed\n");
    }
}