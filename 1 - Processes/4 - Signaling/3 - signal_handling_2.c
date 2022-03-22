#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handle_sighup(int sig);
void handle_sigint(int sig);
void handle_sigquit(int sig);

int main() {
    int pid;
    if ((pid = fork()) == -1) {
        printf("Fork failed.\n");
    } else if (pid == 0) {
        printf("Inside child process\n");
        signal(SIGHUP, &handle_sighup);
        signal(SIGINT, &handle_sigint);
        signal(SIGQUIT, &handle_sigquit);

        for(;;);
    } else {
        sleep(5);
        printf("Sending hangup signal\n");
        kill(pid, SIGHUP);
        sleep(5);
        printf("Sending interrupt signal\n");
        kill(pid, SIGINT);
        sleep(5);
        printf("Sending quit signal\n");
        kill(pid, SIGQUIT);
    }
}

void handle_sighup(int sig) {
    printf("Signal Handling hangup\n");
}

void handle_sigint(int sig) {
    printf("Signal Handling interrupt\n");
}

void handle_sigquit(int sig) {
    printf("Signal Handling Quit\n");
}