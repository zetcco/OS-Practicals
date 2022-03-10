#include <stdio.h>
#include <unistd.h>

/* This code represent behavior of the pipe, 
	Pipe is used to commiunicate with other processes, when reading from an empty pipe, program holds until something is written to
	that pipe.. In this case.. child process will write something to the pipe after 5 seconds, until that, in parent's process
	read will be waiting... after 5 seconds has been elapsed, val will be written to pipe. Then, in parent's process reading will
	be continued and program will complete.
*/

int main() {
	int fd[2], val, pid;

	if (pipe(fd) == -1) return -1;

	if ((pid = fork()) == 0) {
		sleep(5);
		printf("Attempting to write from pipe..\n");
		printf("Write: %ld\n", write(fd[1], &val, sizeof(int)));	
		close(fd[1]);
	} else {
		printf("Attempting to read from pipe..\n");
		printf("Read: %ld\n", read(fd[0], &val, sizeof(int)));
		close(fd[0]);
	}
}
