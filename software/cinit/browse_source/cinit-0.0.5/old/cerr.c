/* 
 * cinit
 * (c) 2005 Nico Schottelius (nico-linux at schottelius.org)
 * print error to stderr and exist
 */

#include <unistd.h>
#include "cinit.h"

void cerr(char *string, int status)
{
   char *p;

   p = string;
   while ( *p != '\0') {
      write(2,p,1);
      p++;
   }
   write(2,"\n",1);

   switch(status) {
      case RT_CHLD_FAIL: 
      case RT_CHLD_OK: 
         _exit(status);
         ;;
      case RT_PAR_FAIL: /* FIXME: if status is RT_PAR_FAIL, start sulogin */
         _exit(23);
         ;;
      case RT_PAR_OK:
         return;
         ;;
   }

/* FIXME: return if in cinit, _exit on fork() */
//   if(...) 
}
