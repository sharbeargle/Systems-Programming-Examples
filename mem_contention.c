#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void loop(char *buf, int len) {	
	while (1) {
		int i;
		for (i=0; i<len; i++) {
			int j = i * 4096;
			buf[j] = 'a';
		}
		sleep(10);	
	}
}

int main(int argc, char **argv) {
	if (argc != 2) {
		printf("Syntax: command <number of 4k pages greater than 0>\n");
		return 0;
	}

	int pages = atoi(argv[1]);

	if (pages <= 0) {
		printf("Syntax: command <number of 4k pages greater than 0>\n");
		return 0;
	}

	printf("Creating buffer of %d 4k pages\n", pages);

	int bufsize = pages * 4096;
	char *buf = (char *) malloc(bufsize);

	loop(buf, pages);

	return 0;
}
