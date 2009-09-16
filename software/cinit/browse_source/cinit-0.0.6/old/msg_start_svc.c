/* 
 * cinit
 * (c) 2005 Nico Schottelius (nico-linux at schottelius.org)
 * tell cinit we start a service
 */

#include <unistd.h>

/* return status */
int msg_start_svc(char *string, int respawn)
{
   char *p;
   
   p = string;
   while ( *p != '\0') {
      write(2,p,1);
      p++;
   }

/* FIXME: return if in cinit, _exit on fork() */
//   if(...) 
   _exit(status);
}
