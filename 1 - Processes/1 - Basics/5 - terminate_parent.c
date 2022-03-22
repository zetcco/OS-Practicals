/*

	What if parent prcoess got terminated before child completes?
	This example demonstrates above scenario. 

	In here, Parent will terminate before child. After 1 second, Child will terminate. And when terminating both processs' parent 
	process's PID will get printed. If child got terminated before parent, then that print statement will show,
		When child gets terminated, it will print Child's PID with Parent's PID
		When parent gets terminated, it will print Parent's PID and It's parent's PID (usually it's parent is 'systemd'
	
	However, if the child got terminated after it's parent is terminated, then that child will be considered as 'Zombie Process'.
	So what will be the output of print statement?
		First, it will print parent's PID (let's say it is 'x') with it's parent's PID (usually systemd)
		Then, it will print child's PID with some other PID that is not it's original parent's PID ('x').

*/

#include <stdio.h>
#include <unistd.h>

int main() {
	int pid = fork();
	if (pid == 0) {
		sleep(1);
	}
	printf("Parent PID: %d, PID: %d\n", getppid(), getpid());
	return 0;
}
