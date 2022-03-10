/*

    This explains how to handle signals using the signal() function.

    signalhandler_t signal(int signum, signalhandler_t handler_funciton);  User 'man signal', 'man 7 kill' to see additional details.

    In here, we first set what to do when certain signals arrive at the process using the signal funtion. signal function has two parameters,
    first one defines the signal that needs to be handled. the second one is used to point to a function (function pointer) that defines what to do when 
    that specified signal is arrived at the process. 
    **  As the second parameter, we can also use SIG_IGN and SIG_DFL. 
        SIG_IGN -  Used to ignore the specified signal. (Does nothing)
        SIG_DFL - Used to set the default action set by OS to that process if that specified signal is arrived.

    If signal() encountered any error, it will return SIG_ERR.

    ** SIGQUIT is initiated when pressing Ctrl+\

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handle_sig_keyinterrupt(int sig);
void handle_sig_quit(int sig);

int main() {
    if (signal(SIGINT, &handle_sig_keyinterrupt) == SIG_ERR)
        printf("Cannot catch keyboard interrupt\n");
    
    if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
        printf("Cannot catch quit interrupt\n");

    while (1) {
        printf("Inside main function...\n");
        sleep(1);
    }
}

/* 
    This function will work only once, heres why,
    After this function gets executed after the process encountered Ctrl+C, it will print the print statement, then it
    executes the signal(SIGINT, SIG_DFL) function, that sets the SIGINT to do its default task. So in other words,
        Next time the process encounters Ctrl+C, it will do the default task which is to terminate the process.
*/
void handle_sig_keyinterrupt(int sig) {
    printf("You pressed Ctrl+C\nInside keyboard_interrupt handler function, with signal : %d\n", sig);
    signal(SIGINT, SIG_DFL);
}