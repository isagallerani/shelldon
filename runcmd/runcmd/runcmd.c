/* 
	runcmd.c - Execute a command as a subprocess. 

	Shelldon, Copyright(c) 2017, Alessandra Camargo, Alexandre Lara, Lucas Pessoa, Tiago Avellar 
	<552038@comp.ufscar.br, 587117@comp.ufscar.br, 551864@comp.ufscar.br, 551910@comp.ufscar.br>

	Shelldon is derived from POSIXeg, Copyright (c) 2017, Francisco José Monaco <moanco@icmc.usp.br>

	This file is part of Shelldon.

	Shelldon is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Shelldon is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

	Our Shelldon repository can be found at https://github.com/SO-II-2017/shelldon
*/

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>


#include <runcmd.h>
#include <debug.h>


/* 

	Executes 'command' in a subprocess. Information on the subprocess execution
	is stored in 'result' after its completion, and can be inspected with the
	aid of macros made available for this purpose. Argument 'io' is a pointer
	to an integer vector where the first, second and third positions store
	file descriptors to where standard input, output and error, respective, 
	shall be redirected; if NULL, no redirection is performed. On
	success, returns subprocess' pid; on error, returns 0. 

*/

int runcmd (const char *command, int *result, int *io)
{
  int pid, status; 
  int aux, i, tmp_result;
  char *args[RCMD_MAXARGS], *p, *cmd; 	
  FILE *fp = NULL;

  tmp_result = 0;

  /* Parse arguments to obtain an argv vector. */

  cmd = malloc ((strlen (command)+1) * sizeof(char));
  sysfail(!cmd, -1);
  p = strcpy (cmd, command);

  i=0;
  args[i++] = strtok (cmd, RCMD_DELIM);
  while ((i<RCMD_MAXARGS) && (args[i++] = strtok (NULL, RCMD_DELIM)));
  i--;

  /* Create a subprocess. */

  pid = fork();
  sysfail(pid<0, -1);

  if (pid>0)			/* Caller process (parent). */
    {
      aux = wait(&status);
      sysfail(aux<0, -1);
      

      /*
         IS_NORMTERM(result) returns true if the subprocess has terminated 
                             normally; false otherwise.
         IS_NONBLOCK(result) returns true if the subprocess has been executed
                             in nonblocking mode; false otherwise.
         IS_EXECOK(result)   returns true if 'command' has been effectively 
      		       executed; false otherwise.
                             the subprocess; false otherwise.
         EXITSTATUS(result)  returns the exit status returned by the
                             subproccess.
      */

      /* Collect termination mode - Process Completion Status   */
      if (WIFEXITED(status))
		{ 			/* This macro returns a nonzero value if the child process terminated normally with exit or _exit.*/
			tmp_result |= NORMTERM;			/* Bitwise OR assignment*/
			fp = fopen("check.txt","r");
			if(fp == NULL)
			{
				tmp_result |= WEXITSTATUS(status); /* If WIFEXITED is true of status, this macro returns the low-order 8 bits of the exit status value from the child process. */ 
				tmp_result |= EXECOK;
			}		
			else
			{
				tmp_result |= EXECFAILSTATUS & RETSTATUS;
				fclose(fp);
				remove("check.txt");
			}
		}
    }

  else				/* Subprocess (child) */
    {
      aux = execvp (args[0], args);
	if(aux <= -1)
	{
		fp = fopen("check.txt","w");
		fclose(fp);
	}
      exit (EXECFAILSTATUS);
    }

  if (result)
    *result = tmp_result;

  free (p);
  return pid;			/* Only parent reaches this point. */
}

/* If function runcmd is called in non-blocking mode, then function
   pointed by rcmd_onexit is asynchronously evoked upon the subprocess
   termination. If this variable points to NULL, no action is performed.
*/

void (*runcmd_onexit)(void) = NULL;

