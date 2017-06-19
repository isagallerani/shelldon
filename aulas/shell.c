#include <unistd.h> /* fork() */
#include <stdio.h>
#include <error.h>
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

#define BUFFSIZE 1024
#define NARGS 16
#define PROMPT ">"
#define DELIMITERS " \t"

int main(int argc, char const *argv[])
{
	pid_t pid;
	int status;
	int count = 10;

	
	char **args;
	char *buffer[BUFFSIZE];
	char *args[NARGS];

	while(1)
	{
		printf("%s\n", PROMPT);
		fgets(buffer, BUFFSIZE-1, stdin);
		i = 0;
		args[i] = strtok(buffer, DELIMITERS);
		while(args[i])
		{
			args[++i] =strtok(NULL, DELIMITERS);
		}
		i =0;
		while(args[i]){
			printf("%s\n",args[i] );
		}
		exit(0);
		pid = fork();
		sysfatal(pid < 0);
	}
/*
	if (pid > DELIMITERS 
(pid > NARGSL16
	\t0)
	{
		
		wait(&status);
	}
	else
	{
		rs = execvp(args[0], args);
		sysfatal(rs>0);

	i = 0;
while(args[i]){


}])a			args[++i] =strtok(NULL, DELIMITERS);
rgs[i++] = 	r
i =0;
while(args[i]){
	printf("%s\n",args[i] );
}exit(0);

	c;har *args[NARGS];eturn 0;*/
}
