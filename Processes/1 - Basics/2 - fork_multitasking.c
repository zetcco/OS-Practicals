#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
	int arr[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 210, 220, 230, 240, 250, 260, 270, 280, 290, 300};
	int fd[2];
	int size = sizeof(arr)/sizeof(int);
	int pid;

	if (pipe(fd) == -1)
		return -1;

	if ((pid = fork()) == 0) {
		int sum = 0;
		for (int i = 0 ; i < size/2 ; i++)
			sum += arr[i];

		close(fd[0]);
		write(fd[1], &sum, sizeof(int));
		close(fd[1]);
	} else {
		int sum = 0;

		for (int i = size/2+1 ; i < size ; i++)
			sum += arr[i];

		close(fd[1]);
		read(fd[0], &sum, sizeof(int));
		close(fd[0]);

		printf("Sum : %d\n", sum);

	}
}
