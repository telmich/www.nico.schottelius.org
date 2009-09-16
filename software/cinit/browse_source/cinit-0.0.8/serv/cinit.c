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
#include <stdlib.h>
#include <fcntl.h>

#include <time.h>    /* nanosleep */

#include "cinit.h"

/* global variable */
struct listitem *list;
int sock, logfd;
pid_t cpid;

/***********************************************************************
 * the main procedure
 */

int main(int argc, char **argv)
{
   struct stat sbuf;
   struct sockaddr_un addr;
   struct timespec ts;
   char *initdir;
   
   set_signals(ACT_SERV);  /* set signal handlers */
   logfd = 1;              /* write to stdout */
   list = NULL;            /* list of services is empty currently */

   initdir = CINIT_INIT;

   /* profile support */
   while(argc > 1) {
      if( !strncmp(PROFILE, argv[argc-1], strlen(PROFILE)) ) {
         initdir = (char *) malloc(
                              strlen(CINIT_DIR) +
                              strlen(&argv[argc-1][strlen(PROFILE)]) + 2
                              );
         if(initdir == NULL) {
            panic();
         }
         strcpy(initdir,CINIT_DIR);
         strcat(initdir,SLASH);
         strcat(initdir,&argv[argc-1][strlen(PROFILE)]);
         break;
      }
      argc--;
   }

   /* stat, checkdir */
   if( stat(CINIT_INIT,&sbuf) ) {
      perror(MSG_INIT_MISS);
      panic();
   }
   if( ! S_ISDIR(sbuf.st_mode) ) {
      LOG(MSG_NOT_DIR);
      panic();
   }
   if( chdir(CINIT_INIT) == -1) {
      perror(MSG_CHDIR);
      panic();
   }

   /******************** TMPDIR  **********************/
   if( mount(C_TMPMOUNT,CINIT_TMNT,C_TMPFS,0,NULL) == -1 ) {
      perror(MSG_ERR_MOUNT);
      panic();
    }

   /******************** begin socket **********************/
   sock = socket(AF_UNIX,SOCK_STREAM,0); /* create socket */
   if( sock == -1 ) {
      perror(MSG_SOCKET);
      panic();
   }
   
   /* tell the socket, _we_ want to get SIGIO! */
   cpid = getpid();
   if(fcntl(sock,F_SETOWN,cpid) == -1) {
      perror(MSG_FCNTL);
      panic();
   }

   /* O_SYNC: SIGIO will be called, O_NONBLOCK: don't block cinit */
   if ( fcntl(sock,F_SETFL,O_ASYNC|O_NONBLOCK) == -1) {
      perror(MSG_FCNTL);
      panic();
   }
   
   memset(&addr, 0, sizeof(addr) ); /* clear addr */
   strcpy(addr.sun_path, CINIT_SOCK);
   addr.sun_family = AF_UNIX;
   
   if(bind(sock,(struct sockaddr *)&addr,sizeof(addr)) == -1) {
      perror(MSG_BIND);
      panic();
   }
   
   /* start listening */
   if(listen(sock,SOCK_QUEUE) == -1) {
      perror(MSG_LISTEN);
      panic();
   }
   /******************** end socket **********************/

   /* initial run, only if we are 'real' init': not needed */
//   if( cpid == 1) {
//   if(! run_init_svc() ) {
//      panic();
//   }
//   }
   
   run_init_svc(initdir);

   /* free, if we malloc()ed before */
   if(initdir != CINIT_INIT) {
      free(initdir);
   }

   ts.tv_sec = SLEEP_INIT;
   while(1) {
      nanosleep(&ts,NULL);
   }
}
