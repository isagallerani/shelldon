/* crash.c - A trivial shell. 
 
   Copyright (c) 2015, Monaco F. J. <moanco@icmc.usp.br>

   This file is part of POSIX.

   POSIX is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>		
#include <sys/types.h>		
#include <sys/wait.h>
#include <posixeg/debug.h>

#define MAX_COMMAND_LINE_SIZE 1024 /* Fixed command-line length. */

int cont;			/* Controls the main loop. */

/* Built in function. */
void help (void)
{
  printf ("Help: a famous Beatles' song.\n");
}


/* Return 1 if command is executed as a builtin function;
   return 0 otherwise. */

int exec_builtin (char *command)
{
  if (!strcmp (command, "help"))
    {
      help();
      return 1;
    }
  if (!strcmp (command, "exit"))
    {
      cont = 0;
      return 1;
    }
  return 0;
}

/* Main function. */

int main (int argc, char **argv)
{
  char command[MAX_COMMAND_LINE_SIZE]; /* Command line. */
  int pid;			       /* Subprocess' ID */
  int status;			       /* Subprocess' exit status */
  char *args[2];		       /* How arguments are passed to exec. */

  cont = 1;

  while (cont)			/* Main loop. */
    {
      printf ("@: ");		/* Prompt. */
      scanf ("%s", command);

      if (! exec_builtin  (command)) /* Try to execute as built-in. */
	{

	  pid = fork ();	/* Fork */
	  sysfatal(pid<0);

	  if ( pid > 0 )	/* Parent. */
	    {
	      wait (&status);	/* Just wait for child. */
	    }
	  else			/* Child */
	    {
	      args[0] = command;
	      args[1] = NULL;
	      execvp (args[0], args);           /* Replaces subprocess' image */

	      printf ("%s: %s\n", command, strerror(errno)); /* Exec failed, for sure. */
	      exit (EXIT_FAILURE);
	    }

	}
    }

  printf ("Bye bye\n.");

  return EXIT_SUCCESS;
}
