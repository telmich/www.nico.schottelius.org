/* 
 * cinit
 * (c) 2005 Nico Schottelius (nico-linux at schottelius.org)
 * print string
 */

#include <unistd.h>

void mini_printf(char *str,int fd)
{
   char *p;

   p = str;
   while ( *p != '\0') {
      write(fd,p,1);
      p++;
   }
   
   write(fd,"\n",1); /* FIXME: only when console */
}
