/***********************************************************************
 * 
 *    (c) 2005 Marcus Przyklink (downhill-clinux (at) burningchaos.org)
 *    2006 Nico Schottelius (nico-linux-cinit at schottelius.org)
 * 
 *    part of cLinux/cinit
 * 
 *    Search in the list
 */

#include <stdio.h>         /* NULL              */
#include <string.h>        /* strcmp            */
#include "cinit.h"
#include "svc.h"

/* search for an entry by path, reverse: from end to the beginning */
struct listitem *list_search(char *path)
{
   struct listitem *tmp;

   if( list == NULL ) { /* think positive */
      return NULL;
   } else {
      tmp = list;
   }

   do {
      if( !strcmp(path, tmp->abs_path) ) {
         return tmp;
      }
      tmp = tmp->prev;
   } while(tmp != list);
   
   return NULL;
}
