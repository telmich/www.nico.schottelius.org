#include <sys/types.h>
#include <sys/wait.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#include "cinit.h"

/* we are called, when a child exited */
void sig_chld(int status)
{
   int   pstat, i = 0;
   pid_t pid;

   /* get pid of child and its status */
   pid = waitpid(chpid,&pstat,0);

   /* the child was either a respawed or once running process */
   
   while(i < svc_list.process) {
      if(pid == svc_list.svca[i].pid) { /* found service */

      }
      i++;
   }
   
   if( WIFEXITED(pstat) ) printf("Kind gut gestorben\n");

   printf("kind tot %d ...\n", status);
}
