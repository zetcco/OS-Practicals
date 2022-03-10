/*

	This program demonstrates the wait() with exit codes.

	wait(int *) expects int pointer, the pointed int (let's call it 'wstatus' will be replaced with exit status code from the child process.

	Then, we can use WIFEXITED(wstatus) macro to check if the child process got exited on it's own or terminated by another program.
	If it is exited on it's own, then we can use WEXITSTATUS(wstatus) to get the actual exit code returned by the child process.
	In below example, if 'execlp()' got failed because of the execution program (in here it is 'ping') , then it's (ping command's) exit 
	code will be returned. But, if execlp failed during it's function call (for example, bad program name.. using 'ing' instead of 'ping'
	will result in execlp function returning -1. That -1 however is not a exit code.. It is a function return code.. So in below example,
	if that occurs we will manually return 1 as exit code.

	Refer 'man wait' for more info on wait macros.


*/
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
	int pid;

	if ((pid = fork()) == 0) {
		int excode;
		if ((excode = execlp("ing", "ping", "-c", "1", "google.com", NULL)) == -1) {
			printf("Cound't find program name, execode:%d\n", excode);
			exit(1);
		}
	} else {
		int wstatus = -1;
		wait(&wstatus);
		if (WIFEXITED(wstatus)) {
			if (WEXITSTATUS(wstatus) == 0)
				printf("Finished successfully\n");
			else
				printf("Failure, execode: %d\n", WEXITSTATUS(wstatus));
		}

	}
}
