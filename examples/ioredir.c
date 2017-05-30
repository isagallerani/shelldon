/* io-redir.c - Example of I/O redirection. 

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
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#include <posixeg/debug.h>

int main (int argc, char **argv)
{

  int fd;			/* A file descriptor. */
  int temp;

  /* We open the file for read and write; create the file if it
     already exists. */
  
  fd = open ("output.txt", O_CREAT | O_TRUNC | O_RDWR,  S_IRUSR | S_IWUSR);
  sysfatal(fd<0);
  
  /* Then we close the file descriptor associated to the
     standard output.  By default, it's file descriptor number 1
     (POSIX guaranteed) and is connected to the user terminal. */
  
  close (1);			/* Now fd 1 is empty. */
  
  /* The dup syscall duplicates the the file descriptor fd,
     by choosing the first available file descriptor.  Remember that
     the file descriptor 1 is now empty, therefore it's choosen. */
  
  temp = dup (fd);
  sysfatal (temp <0);
  
  /* We don't need two file descriptors associated to the open
     file, therefore we close fd.  Leaving it open, it's not 
     good practice. */

  close (fd);

  /* Now if we call printf, the standard output... */

  printf ("Foo\n");		/* the output is redirected. */
      
  return EXIT_SUCCESS;
}
