/* 
 * (c) 2005 Nico Schottelius (nico-linux at schottelius.org)
 * add_svc
 * part of cinit
 */

#include <string.h>

#include "cinit.h"

/* we return the service id or -1 on error*/
int add_mod_svc(char *svc, char status, pid_t pid)
{
   int tmp = 0;
   
   /* check if service exists, if so, set new status, return sid */
   tmp = chk_svc(svc);
   if( tmp != -1 ) {
      svc_list.svca[tmp].status = status;
      return tmp;
   }
   
   /* still here? than there is no such service */
   if(svc_list.process < MAX_SVC) {
      printf("[%d]: %s (%d)\n",svc_list.process,svc,pid);
      svc_list.svca[svc_list.process].abs_path = strdup(svc);

      if(svc_list.svca[svc_list.process].abs_path == NULL) {
         LOG("strdup failed!");
         return -1;
      }
      svc_list.svca[svc_list.process].pid = pid;
      /* inkrement later, as we count argv[0..n] */
      (svc_list.process)++;
   } else {
      LOG("too many services");
      return -1;
   }
   
   /* return _our_ service id */
   return (svc_list.process -1);
}
