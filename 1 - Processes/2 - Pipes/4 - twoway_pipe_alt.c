#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

/* This code will generate different results each time (run it several times, using,
		for i in {1..20} ; do ./test ; done
	), because of the dependancy on the CPU scheduler. Because, in parent's code segment.. There is a chance that it will read from the pipe as
	-soon as 5 is written to the pipe.. That will result a infinite wait in the child process because after the parent reads the 5 (that was
	-written on the pipe just before) that read value will be removed from the pipe... So child waits for something to be added to the pipe.. yet,
	-nothing will be added because it was too late.. Then the parent will get executed and finished, child will become a zombie process and 
	-will be killed.
	BUT this is not always the case.. If child managed to read the 5 before parent reads it, the program will output the result correctly.

	In order to make it work,
		1. Send 5 from parent to child thru the pipe
		2. Child must read 5 from the pipe (If child got executed earlier than the parent, it won't affect because child will be waiting for
			-parent's pipe write..)
		3. Do the calculation on the child and send to parent thru the pipe
		5. Parent will read and output.. (in here also, parent might be already waiting for something from the child thru the pipe.. so it won't
			-affect the result or execution of the program)

	So, in order to correctly get the output always correctly, we must wait until the child sends the result after sending 5 to it.. so 
	we must add a wait(NULL) in parent between the writing to the pipe and the reading from the pipe.


	ALTERNATIVE solution would be to use two pipes
*/

int main() {
	int pid, fd1[2], fd2[2];

	if (pipe(fd1) == -1)
		return -1;

	if (pipe(fd2) == -1)
		return -1;

	if ((pid = fork()) == 0) {
		int val;

		close(fd1[0]);
		close(fd2[1]);

		printf("Attempting to read from child process\n");
		printf("Read code: %ld\n", read(fd2[0], &val, sizeof(int)));
		
		val *= 4;

		write(fd1[1], &val, sizeof(int));
		
		close(fd2[0]);
		close(fd1[1]);
	} else {
		int val = 5;
		printf("Value: %d\n", val);

		close(fd2[0]);
		close(fd1[1]);

		write(fd2[1], &val, sizeof(int));
		read(fd1[0], &val, sizeof(int));

		printf("Value: %d\n\n", val);

		close(fd1[1]);
		close(fd2[0]);

		wait(NULL);
	}

}
