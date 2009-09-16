/* 
 * cinit
 * (c) 2005 Nico Schottelius (nico-linux at schottelius.org)
 * run services parallel
 */
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "cinit.h"


/*********************************************************************** 
 * parallel run forked() run_svc()
 */

int run_run_svcs(char *abspath)
{
   DIR *d_tmp = NULL;
   struct dirent *tdirent;
   char pathbuf[PATH_MAX];
   pid_t pids[MAX_DEPS];
   int status, i=0, ret = 1;

   D_PRINTF(abspath);
  
   d_tmp = opendir(abspath);
   
   /* if there is no such dir, we are finished */
   if(d_tmp == NULL) {
      return 1;
   }

   while( (tdirent = readdir(d_tmp) ) != NULL) {
      /* ignore . and .. and everything with a . at the beginning */
      if ( *(tdirent->d_name) == '.') continue;

      if(i < MAX_DEPS) {
         pids[i] = fork();
         i++;
      } else {
         LOG("to many dependencies");
         break;
      }

      if(pids[i-1] == -1) { /* err */
         LOG("fork failed");
         return 0;
      }

      if(pids[i-1] == 0) { /* child */
         strcpy(pathbuf,abspath);
         strcat(pathbuf,"/");
         strcat(pathbuf,tdirent->d_name);
         if ( run_svc(pathbuf) )
            _exit(0);
         else
            _exit(1);
      }
   }

   closedir(d_tmp);
   
   /* wait for pids */
   while(i >= 0) {
      waitpid(pids[i], &status, 0);

      if( ! WIFEXITED(status)) {
         ret = 0;
      }
      i--;
   }
   return ret;
}
