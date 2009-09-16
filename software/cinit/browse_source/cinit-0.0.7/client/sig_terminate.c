/* 
 * (c) 2005 Nico Schottelius (nico-linux at schottelius.org)
 * terminate: terminate our watching pid
 * part of cinit
 */

#include "cinit.h"
#include <unistd.h>

void sig_terminate(int signal)
{
   char *nargv[2];

   nargv[0] = SULOGIN;
   nargv[1] = NULL;

   execv(SULOGIN,nargv);
   _exit(1);
}
