/* 
 * (c) 2005 Nico Schottelius (nico-linux at schottelius.org)
 * run_svc
 * part of cinit
 */

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>


#include "cinit.h"

/* run a service, gets (relative or absolute) path */
pid_t respawn_svc(char *abspath)
{
   pid_t pid;
   int status;
   int pipefd[2];
   
   if(pipe(pipefd) == -1) {
      perror("pope");
      return 0;
   }
      
   pid = fork();
   if(pid == -1) {
      perror("forke");
      return 0;
   }

   /******************** PARENT ********************/
   if(pid > 0) {
      /* if we read anything, our child succeded */
      if( read(pipefd[0],&status,sizeof(status)) == -1) {
         perror("read pope");
         return 0;
      }
      return pid; /* return watchers pid */
   }

   /******************** CHILD ********************/
   /* FIXME: add signal handler for shut down */
   
   status = 1;
   if( write(pipefd[1],&status,sizeof(status)) == -1) {
      perror("read pope");
      _exit(1);
   }

   while( status ) {
      D_PRINTF(abspath);
      pid = exec_svc(abspath, status);
      
      waitpid(pid,&status,0);
      
      if( ! WIFEXITED(status) ) {
         if( WEXITSTATUS(status) ) {
            /* sleep conf/c_sleep seconds on error exit */
            sleep(SLEEP_SVC);
            D_PRINTF("schlecht gestorben");
         }
      }
   }

   _exit(0);
}
