#include <unistd.h> /* fork() */
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PROGRAM_NAME "exec"

#define sysfatal(exp) do{ \
if (exp) \
	{ \
		fprintf( "%s, %s, %d, %s\n", PROGRAM_NAME, __FILE__, __LINE__, strerror(errno));\
		exit(EXIT_FAILURE); \
	}while(0)

int main(int argc, char const *argv[])
{

	int count = 5, rs;
	char *args[] = {"./loop", NULL};

	while(count) {
		printf("loop: %d\n", count--);
		sleep(1);
	}
	rs = execvp(args[0], args);//programa, lista de argumentos que eu quero passar para o programa c/nome do prog.
	sysfatal(rs);
	/*if (rs)
	{
		printf("Erro na exec %d\n", errno); //seta o erro nessa variavel
		printf("%s\n", strerror(errno));
		exit(1);
	}*/

	printf("exec: done\n");

	return EXIT_SUCCESS;
}
