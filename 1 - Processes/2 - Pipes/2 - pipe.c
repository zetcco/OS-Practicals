#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* In here strlen(str)+1 is used because strlen(str) gives the length of the string plus the null terminator, so if we used
 * it as it is, reading will read extra bytes because it doesn't see EOF, because write writes strlen(str) size of bytes. So we need to add +1 
 * in order to send EOF also. When printing there is also a line break, i think this is because EOF causes this
*/

int main() {
	int fd[2], pid;
	char str[20];

	if (pipe(fd) == -1) {
		printf("Error occured when initializing the pipe\n");
	}

	pid = fork();
	if (pid == 0) {
		close(fd[0]);
		printf("Enter a string: ");
		fgets(str, 20, stdin);
		printf("Wrote: %ld\n", write(fd[1], str, strlen(str)+1));
		close(fd[1]);
	} else {
		char pstr[20];
		close(fd[1]);
		read(fd[0], pstr, 20);
		close(fd[0]);
		printf("Entered: %s\n", pstr);	
	}
}
