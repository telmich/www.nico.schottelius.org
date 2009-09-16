/* 
 * (c) 2005 Nico Schottelius (nico-linux at schottelius.org)
 * chk_svc: check whether service exists
 * part of cinit
 */

#include <string.h>
#include "cinit.h"

/* we are called from sigio */
/* return SID, if service exists, else -1 */
int chk_svc(char *svc)
{
   int tmp = 0;

   while(tmp < svc_list.process) {
      if( ! strcmp(svc,svc_list.svca[tmp].abs_path) ) {
         return tmp;
      }
      tmp++;
   }
   return -1;
}
