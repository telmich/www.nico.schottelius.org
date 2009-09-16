/* 
 * cinit
 * (c) 2005 Nico Schottelius (nico-linux at schottelius.org)
 * run initial service
 */

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

#include "cinit.h"

#include <time.h>

/*********************************************************************** 
 * start the first service
 */

extern time_t start;

int run_init_svc()
{
   pid_t pid;
   int status;
   time_t stop;

   /* leave cinit alone */
   pid = fork();
   
   if(pid == -1) { /* err */
      perror(MSG_ERR_FORK);
      return 0;
   } else if(pid == 0) { /* child */
      logfd = 1;  /* FIXME: shouldn't this be some kind of log, like /dev/log? */
      set_signals(ACT_CLIENT);
      /* FIXME: open stderr, stdin, stdout to files / syslog / logable */
      if ( run_svc(CINIT_INIT) )
         _exit(0);
      else
         _exit(1);
   } else { /* parent */
      waitpid(pid, &status,0);
      stop = time(NULL);
      printf("*** cinit needed %d seconds to start ****\n",(int) stop-start);
      return WIFEXITED(status);
   }
}
