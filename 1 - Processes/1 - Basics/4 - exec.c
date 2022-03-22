/*

	This is to demonstrate the use of "exec" family of functions. In here "execlp(), execv(), execvp()" will be demonstrated.
	l - List
	v - Vector
	p - path variable (program name)
	
	The way how exec works is like this, 
		Any of a exec function call will overwrite existing memory, execution, etc. of the process that called exec function.
		For example, think we added,

			..
			execlp("ls", "ls", "/home/zetcco/", "-l", NULL); 
			printf("Finished execution of ls\n");
			..

		Any of the lines after "execlp" will not be executed because exec will overwrite them, and after exec has compuleted
		it's operation it will call exit(), which then causes whole program to exit. So anything after exec will not get executed.

		So in order to prevent that, we can use exec in child processes (using fork()).. Below demonstration will show how
		exec function works with their neccessary arguments, etc. and also how to use fork() with them..

*/

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	if (fork() == 0) {
		if (fork() == 0) {
			execlp("ls", "ls", "/home/zetcco/os/", "-l",  NULL);
			printf("Finished execution on (pid: %d, ppid: %d)\n", getpid(), getppid()); // This do net get executed
		} else {
			wait(NULL);
			char * arguments[] = {"ls", "/home/zetcco/os/", "-l", NULL};
			execvp("ls", arguments);
			printf("Finished execution on (pid: %d, ppid: %d)\n", getpid(), getppid()); // This do net get executed
		}
	} else {
		wait(NULL);
		char * arguments[] = {"ls", "/home/zetcco/os/", "-l", NULL};
		execv("/bin/ls", arguments);
		printf("Finished execution on (pid: %d, ppid: %d)\n", getpid(), getppid()); // This do net get executed
	}
}
