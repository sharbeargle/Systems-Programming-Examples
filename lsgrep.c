/*
	Example to demonstrate IO redirection.
	This program does the same thing as $ ls -la <dir> | grep <txt>
	Syntax: $ lsgrep <dir> <txt>
*/

#include <unistd.h>
#include <fcntl.h>

int main( int argc, char* argv[]){
	int ls_pid, grep_pid;
	int pipefd[2];

	if( argc < 3){
		return -1;
	}

	// Create pipe
	if(pipe(pipefd) == -1){
		return -1;
	}

	// Fork for grep
	grep_pid = fork();

	if( grep_pid == -1) {
		return -1;
	}

	// In grep child process
	if( grep_pid == 0) {

		// Implictly close stdin fd and dup read end of pipe in its place
		if(dup2(pipefd[0], STDIN_FILENO) == -1) {
			return -1;
		}

		close(pipefd[0]);
		close(pipefd[1]);

		char *new_argv[] = {"/bin/grep", argv[2], 0};

		execv(new_argv[0], &new_argv[0]);

		return -1;
	} 

	// Fork for ls
	ls_pid = fork();

	if( ls_pid == -1) {
		return -1;
	}

	// In ls child proc
	if( ls_pid == 0) {

		// Implicitly close stdout fd and dup write end of pipe in its place
		if(dup2(pipefd[1], STDOUT_FILENO) == -1) {
			return -1;
		}

		close(pipefd[0]);
		close(pipefd[1]);

		char *new_argv[] = {"/bin/ls", "-la", argv[1], 0};

		execv(new_argv[0], &new_argv[0]);

		return -1;
	}

	// In parent
	close(pipefd[0]);
	close(pipefd[1]);

	// Wait for all children, disregard exit status
	while(wait(NULL) > 0);

	return 0;
}
