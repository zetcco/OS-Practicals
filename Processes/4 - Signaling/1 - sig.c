/*
    Send signals to other processes.

    This program demonstrates kill() function, which is used to send Signals to other processes.

    kill(process_id, signal_to_send)

    In here, SIGKILL signal is sent to the child process after fork() has been initiated, SIGKILL is used to denote kill signal.
    After recieving the SIGKILL, child process will initiate it's termination thus killing itself.

    Key point in here is kill() is used to send signals, in this example it is SIGKILL. After the targeted process recieved the signal,
    it will do whatever recieved in that signal. So in simpler terms, parent do not kill the child, what happens is that parent signals the
    child to kill itself.

    Use 'man 7 signal' to view list of possible Signals that can be sent to processes.

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int main() {
    int pid;
    if ((pid = fork()) == -1)
        return -1;
    else if (pid == 0) {
        while (1) {
            printf("Inside the child\n");
            sleep(1);
        }
    } else {
        sleep(5);
        kill(pid, SIGKILL);
    }
}