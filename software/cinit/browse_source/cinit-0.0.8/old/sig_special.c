/* 
 * cinit
 * (c) 2005 Nico Schottelius (nico-linux at schottelius.org)
 * We are called by signal to reboot
 */

#include <signal.h>

#include "cinit.h"

/***********************************************************************
 * sig_special
 * react on special signals:
   o SIGINT: ctrl + alt + del
   o SIGWINCH: keyboard request
 */

void sig_special(int signal)
{
   /* not used */
}
