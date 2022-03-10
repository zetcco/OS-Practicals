#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	int pid;
	printf("Pid: %d\n", getpid());
	int x = 10;
	if ((pid = fork()) == 0) {
		printf("Child process is here, value of x: %d\n", x);
		x++;
		printf("Child process modified the value, after that x: %d\n", x);
	} else {
		printf("My pid: %d\n", pid);
		printf("Parent process here, value of x: %d\n", x);
		wait(NULL);
		printf("Parent process here, after child modified the value of x: %d\n", x);
	}
}
