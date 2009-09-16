/* 
 * (c) 2005 Nico Schottelius (nico-linux at schottelius.org)
 * run_svc
 * part of cinit
 */

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
//#include <signal.h>


#include "cinit.h"

/* run a service, gets (relative or absolute) path */
pid_t respawn_svc(char *abspath)
{
   pid_t pid;
   int status;
   int pipefd[2];
//   struct sigaction sa;
   
   if(pipe(pipefd) == -1) {
      perror(MSG_ERR_PIPE);
      return 0;
   }
      
   pid = fork();
   if(pid == -1) {
      perror(MSG_ERR_FORK);
      return 0;
   }

   /******************** PARENT ********************/
   if(pid > 0) {
      /* if we read anything, our child succeded */
      if( read(pipefd[0],&status,sizeof(status)) == -1) {
         perror(MSG_ERR_IO);
         return 0;
      }
      return pid; /* return watchers pid */
   }

   /******************** CHILD ********************/
/* FIXME: add handling for SIGTERM! */
//   sa.sa_handler= ( (void*)(int) ) sig_terminate;
//   sigaction(SIGTERM,&sa,NULL);      /* term = kill respawing process */
   
   status = 1;
   if( write(pipefd[1],&status,sizeof(status)) == -1) {
      perror(MSG_ERR_IO);
      _exit(1);
   }

   while( status ) {
      D_PRINTF(abspath);
      pid = exec_svc(abspath, status);
      
      waitpid(pid,&status,0);
      
      if( WIFEXITED(status) ) {
         if( WEXITSTATUS(status) ) {   /* non-zero exit status */
            /* sleep conf/c_sleep seconds on error exit */
            sleep(SLEEP_SVC);
            D_PRINTF("schlecht gestorben");
         }
      } else {
         sleep(SLEEP_SVC);
         D_PRINTF("nicht normal gestorben");
      }
   }

   _exit(0);
}
