/* exec.c - Exec example. 

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

#include <stdio.h>		/* printf */
#include <stdlib.h>		/* EXIT_SUCCESS */
#include <unistd.h>		/* sleep, exec */


int
main (int argc, char **argv)
{
  int count;
  char *args[] = {"./loop", NULL};

  count = 10;

  while (count)
    {
      printf ("p1 %d\n", count--);
      sleep (1);
    }

  /* If the call to exec is successfull, the current process is replaced by the
     program speficied by the first argument.  If exec fails nothing happens.

     The firs argument is the path to the program.  The second argument 
     is an array of strings in the format: path-to-program argument-1 
     argument-2.... NULL */

  execv (args[0], args);	

  /* We'll reach this point of the code only if exec fails. */

  printf ("Exec failed\n");

  return EXIT_SUCCESS;
}
