/*

	File Descriptor - Handle (int) to a input/output resource. A file descriptor is unique per process. So same file descriptor value
	could be in two different processes but they refer to two different resources. But, same file descriptor cannot be in same process.
	The following descriptors are reserved,
		0 - STDIN
		1 - STDOUT
		2 - STDERR
	
	dup2(fd_1, fd_2) - fd_1's IO resource will be pointed to fd_2, so fd_2 will also point to fd_1's resource.

	After dup2(file, STDOUT_FILENO), all the STDOUT will be printed to text file.
	After duplication of the descriptor, file descriptor 1 (earlier pointed to STDOUT) will point to file opened. That results in redirecting
	all the stdout outputs to that file.

*/
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

int main() {
	int pid;

	if ((pid = fork()) == 0) {
		int code, file;

		printf("Opening file..\n");
		file = open("test.txt", O_WRONLY | O_CREAT);
		perror("Error ");
		dup2(file, STDOUT_FILENO);
		printf("came here\n");

		if ((code = execlp("ping", "ping", "google.com", "-c", "1",  NULL)) == -1)
			exit(1);
	} else {
		int wstatus;
		wait(&wstatus);
		if (WIFEXITED(wstatus))
			printf("Finished execution, execode: %d\n", WEXITSTATUS(wstatus));		
	}
}
