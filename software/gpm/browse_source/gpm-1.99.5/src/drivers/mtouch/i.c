/*
 * general purpose mouse (gpm)
 *
 * Copyright (c) 2008        Nico Schottelius <nico-gpm2008 at schottelius.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
 *
 ********/

#include <stdio.h>   /* NULL */

#include <unistd.h>                 /* usleep, write     */
#include <termios.h>                /* termios           */

#include "types.h"                  /* Gpm_type         */


Gpm_Type *I_mtouch(int fd, unsigned short flags, struct Gpm_Type *type, int argc, char **argv)
{
   struct termios tty;

   flags = argc = 0; /* FIXME: 1.99.13 */
   argv = NULL; 


   /* Set speed to 9600bps (copied from I_summa, above :) */
   tcgetattr(fd, &tty);
   tty.c_iflag = IGNBRK | IGNPAR;
   tty.c_oflag = 0;
   tty.c_lflag = 0;
   tty.c_line = 0;
   tty.c_cc[VTIME] = 0;
   tty.c_cc[VMIN] = 1;
   tty.c_cflag = B9600|CS8|CREAD|CLOCAL|HUPCL;
   tcsetattr(fd, TCSAFLUSH, &tty);


   /* Turn it to "format tablet" and "mode stream" */
   write(fd,"\001MS\r\n\001FT\r\n",10);

   return type;
}

