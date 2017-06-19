#include <unistd.h> /* fork() */
#include <stdio.h>
#include <error.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{

	int count = 5;
	char *args[] = {"./loop", NULL}

	while(count) {
		printf("loop: %d\n", count--);
		sleep(2);
	}
	return EXIT_SUCCESS;
}
