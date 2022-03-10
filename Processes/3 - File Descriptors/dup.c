/*

	File Descriptor - Handle (int) to a input/output resource. A file descriptor is unique per process. So same file descriptor value
	could be in two different processes but they refer to two different resources. But, same file descriptor cannot be in same process.
	The following descriptors are reserved,
		0 - STDIN
		1 - STDOUT
		2 - STDERR
	
	nfd = dup(fd); fd is the original file descriptor that points to some IO resource, then dup() will duplicate fd with the next possible
	lowest fd. Then it will be returned, that will be assigned to nfd.
	So, both fd,nfd will point to the same IO resource.

*/
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main() {
	int fd, nfd;
	if ((fd = open("test.txt", O_CREAT | O_WRONLY)) == -1) {
		printf("Error opening file\n");
		return -1;
	}
	else {
		printf("File opened successfully with fd: %d\n", fd);
		char * str1 = "This is a test string from Original FD\n";
		printf("Writing: %ld\n", write(fd, str1, strlen(str1)+1));
	}

	printf("Attempting to duplicate the descriptor (id:%d)\n", fd);
	if ((nfd = dup(fd)) == -1) {
		printf("Failed to duplicate the file descriptor\n");
		return -1;
	}
	else {
		printf("New file descriptor: %d\n", nfd);

		char * str2 = "This is a test string from New FD\n";
		printf("Writing: %ld\n", write(nfd, str2, strlen(str2)+1));
	}

}
