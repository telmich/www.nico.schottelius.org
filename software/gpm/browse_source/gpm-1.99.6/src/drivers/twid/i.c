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

#include "types.h"                  /* Gpm_type         */
#include "mice.h"                   /* check_no_argv     */
#include "daemon.h"                 /* which_mouse       */
#include "drivers.h"

int twiddler_key_init(void); /* FIXME: from twiddler */

Gpm_Type *I_twid(int fd, unsigned short flags, struct Gpm_Type *type, int argc, char **argv)
{

   if (check_no_argv(argc, argv)) return NULL;

   if (twiddler_key_init() != 0) return NULL;
   /*
   * the twiddler is a serial mouse: just drop dtr
   * and run at 2400 (unless specified differently)
   */
   if((which_mouse->opt_baud)==DEF_BAUD) (which_mouse->opt_baud) = 2400;
   argv[1] = "dtr"; /* argv[1] is guaranteed to be NULL (this is dirty) */
   return I_serial(fd, flags, type, argc, argv);
}

