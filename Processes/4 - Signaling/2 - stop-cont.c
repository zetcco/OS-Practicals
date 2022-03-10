#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    int pid, stopSec;
    if ((pid = fork()) == -1) {
        printf("Fork failed. Exiting..\n");
        exit(1);
    } else if (pid == 0) {
        while (1) {
            printf("Child executing..\n");
            sleep(1);
        }
    } else {
        while (1) {
            kill(pid, SIGSTOP);
            printf("Enter number of seconds: ");
            scanf(" %d", &stopSec);
            if (stopSec > 0) {
                kill(pid, SIGCONT);
                sleep(stopSec);
                kill(pid, SIGSTOP);
            } else {
                printf("Please enter a positive number\n");
            }
        }
    }
}