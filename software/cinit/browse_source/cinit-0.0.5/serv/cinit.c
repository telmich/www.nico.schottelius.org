/* 
 * (c) 2005 Nico Schottelius (nico-linux at schottelius.org)
 * cinit.c
 * part of cLinux/cinit
 */

/* *stat() */
#include <sys/stat.h>
#include <unistd.h>

/* open */
#include <fcntl.h>

/* siggnal */
#include <signal.h>

/* PATH_MAX */
#include <limits.h>

/* str* */
#include <string.h>

/* sockets */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

/* mount */
#include <sys/mount.h>

#include <stdio.h>
#include <fcntl.h>

#include <time.h>    /* nanosleep */

#include "cinit.h"

/* global variable */
struct listitem *list;
int sock, logfd;

/***********************************************************************
 * the main procedure
 */

#ifdef TIME_ME
time_t start;
#endif

int main()
{
   struct stat sbuf;
   pid_t pid;
   struct sockaddr_un addr;
   struct timespec ts;
   
#ifdef TIME_ME
   start = time(NULL);
#endif

   set_signals(ACT_SERV);  /* set signal handlers */
   logfd = 1;              /* write to stdout */
   list = NULL;            /* list of services is empty currently */

   /* stat, checkdir */
   if( stat(CINIT_INIT,&sbuf) ) {
      perror("PANIC ACTION");
      panic();
   }
   if( ! S_ISDIR(sbuf.st_mode) ) {
      LOG("PANIC ACTION: init is not a dir");
      panic();
   }
   if( chdir(CINIT_INIT) == -1) {
      perror("PANIC ACTION");
      panic();
   }

   /* count of started processes */
   // svc_list.process = 0; 

   /******************** TMPDIR  **********************/
   if(mount(C_TMPMOUNT,CINIT_TMNT,C_TMPFS,0,NULL) == -1) {
      perror("mount");
      panic();
    }

   /******************** begin socket **********************/
   sock = socket(AF_UNIX,SOCK_STREAM,0); /* create socket */
   if( sock == -1 ) {
      perror("socket");
      panic();
   }
   
   /* tell the socket, _we_ want to get SIGIO! */
   pid = getpid();
   if(fcntl(sock,F_SETOWN,pid) == -1) {
      perror("fcntl");
      panic();
   }

   /* O_SYNC: SIGIO will be called, O_NONBLOCK: don't block cinit */
   if ( fcntl(sock,F_SETFL,O_ASYNC|O_NONBLOCK) == -1) {
      perror("fcntl");
      panic();
   }
   
   memset(&addr,0,sizeof(addr)); /* clear addr */
   strcpy(addr.sun_path, CINIT_SOCK);
   addr.sun_family   = AF_UNIX;
   

   if(bind(sock,(struct sockaddr *)&addr,sizeof(addr)) == -1) {
      perror("bind");
      panic();
   }
   
   /* start listening */
   if(listen(sock,SOCK_QUEUE) == -1) {
      perror("listen");
      panic();
   }
   /******************** end socket **********************/

   /* initial run, only if we are 'real' init' */
//   if( pid == 1) {
      run_init_svc();
//   }

   D_PRINTF("fertig gestartet");
   
   /* the main loop: sleep until a signal comes in */
   ts.tv_sec = -1;
   if(ts.tv_sec < 0) {
      D_PRINTF("is signed");
      ts.tv_sec = 1 << ((sizeof(ts.tv_sec) * 8)-1);
      ts.tv_sec = ts.tv_sec - 1;
   }

   while(1) {
      nanosleep(&ts,NULL);
   }
}
