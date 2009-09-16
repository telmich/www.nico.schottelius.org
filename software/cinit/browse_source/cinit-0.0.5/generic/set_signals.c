/* 
 * (c) 2005 Nico Schottelius (nico-linux at schottelius.org)
 * cinit.c
 * part of cLinux/cinit
 */

#include <signal.h>
#include <stdio.h>
#include "cinit.h"

void set_signals(int action)
{
   struct sigaction sa;

   if(action == ACT_SERV) {
      sa.sa_handler=SIG_IGN;
   } else {
      sa.sa_handler=SIG_DFL;
   }
   sigaction(SIGINT,&sa,NULL);      /* ignore ctr+c */
   sigaction(SIGPIPE,&sa,NULL);     /* what todo when pipe/fifo closed */
   sigaction(SIGCHLD,&sa,NULL);     /* what todo when child exited */
   sigaction(SIGUSR2,&sa,NULL);     /* USR2 */
   sigaction(SIGQUIT,&sa,NULL);     /* QUIT... */
   sigaction(SIGTTIN,&sa,NULL);
   sigaction(SIGTTOU,&sa,NULL);

   /* sigio is called to act on the socket */
   if(action == ACT_SERV) {
      sa.sa_handler=sigio;
   }
   sigaction(SIGIO,&sa,NULL);

   /* signal handlers to do special things with: reboot */
   if(action == ACT_SERV) {
      sa.sa_handler=sig_reboot;
   }
//   sigaction(SIGINT,&sa,NULL);      /* FIXME: Testing only: ctr+c = REBOOT!!! */
   sigaction(SIGUSR1,&sa,NULL);     
   sigaction(SIGTERM,&sa,NULL);
   sigaction(SIGHUP,&sa,NULL);

   /* sigstop can't be ignored, do the manpages say   */
   /* sigaction(SIGSTOP,&sa,NULL); */
}
