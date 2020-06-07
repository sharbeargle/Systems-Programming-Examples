/*
	Contrived example of using pipes for IPC.
	Parent process reads in a file, sends data to child via a pipe. Child prints data to stdout.
*/

#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
	int newPid;
	int pipefd[2];

	if (argc < 2) {
		return -1;
	}

	if (pipe(pipefd) == -1) {
		return -1;
	}

	newPid = fork();
	if (newPid == -1) {
		return -1;
	}


	if (newPid == 0) {
		close(pipefd[1]);

		char rdBuffer[80];
		while (read(pipefd[0], rdBuffer, 80) > 0) {
			write(STDOUT_FILENO, rdBuffer, 80);
		}

		write(STDOUT_FILENO, "\n", 1);
		close(pipefd[0]);
		return 0;
	}


	// In Parent
	close(pipefd[0]);

	struct stat filestat;

	if (stat(argv[1], &filestat) == -1) {
		return -1;
	}

	int filefd;
	filefd = open(argv[1], O_RDONLY);

	char wrBuffer[80];
	int filelen = filestat.st_size;
	int readcnt = 0;
	int readin = 0;

	while (readcnt <= filelen) {
		readin = read(filefd, wrBuffer, 80);
		
		if (readin <= 0) {
			break;
		}

		readcnt += readin;

		int i;
		for (i = readin; i < 80; i++) {
			wrBuffer[i] = '\0';
		}

		write(pipefd[1], wrBuffer, 80);
	}

	close(pipefd[1]);
	wait(NULL);

	return 0;
}
