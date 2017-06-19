#include <unistd.h> /* fork() */
#include <stdio.h>
#include <error.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
	pid_t pid;
	pid = fork();
	int count = 10;
	int status;

	if (pid < 0)
	{
		perror("Fork failed \n");
		exit(0);
	}	
	
	else if (pid > 0)
	{
		while(count)
		{

			printf("Pai:: %d, meu pid %d\n pid do meu pai %d",count--, getpid(), getppid());
			sleep(1);
		}
		sleep(5);
		printf("Shut the head\n");
		wait(&status); //matar o zumbi, pai recolhe as informacoes do filho
		printf("Filho terminou: %s\n", WIFEXTIDED(status) ? "normal" : "anormal");
		sleep(5);

	}
	else
	{
		while(count)
		{
			printf("Filho:: %d, meu pid %d\n pid do meu pai %d",count--, getpid(), getppid());
			sleep(2);
		}

	}
	return 0;
}
