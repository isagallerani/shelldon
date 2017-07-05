/* fork.c - Fork example. 

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

#include <posixeg/debug.h>

#include <stdio.h>		/* printf etc. */
#include <stdlib.h>		/* EXIT_SUCCESS */
#include <unistd.h>		/* fork */
#include <sys/types.h>		/* wait */
#include <sys/wait.h>		/* wait */

int
main (int argc, char **argv)
{
  pid_t pid;
  int count;
  int status;

  count = 10;

  /* Here we create a child process. */

  pid = fork ();		/* From this point we (may) have 2 processes.*/
  sysfatal(pid<0);
  
  if (pid >0)			/* In the parent process pid == child's PID. */
    {
      printf ("Father. My pid is %d. My child is  %d\n", 
	      getpid (), pid);
      
      while (count)		
	{
	  printf ("F %d\n", count--);
	  sleep (1);
	}

      printf ("F: waiting child\n");

      /* If parent terminates first, it quits. 
       Or, we can make it wait for any of it's child to terminate.*/

      wait (&status);		/* Block until any child terminates. */

      printf ("F: done. Child returned %d\n", WEXITSTATUS(status));

    }
  else				/* In the child process pid is zero. */
    {
      printf ("Child.  My pid is %d. My father is %d\n", 
	      getpid (), getppid() );

      while (count)
	{
	  printf ("C %d\n", count--);
	  sleep (2);
	}

      printf ("C: done\n");

    }

  return EXIT_SUCCESS;
}

/* Notes.  

   In this example, the parent process complete the loop first
   and then waits for the child.  Try to remove the call to wait()
   and see what happens. Then, invert the sleeping times of
   parent and child so that child terminates first. You can see
   what is happening if you open another shell terminal and
   issue the command

   $ while true; do ps uf; sleep 1; clear; done

   to see the processes tree during the experiment.

*/
