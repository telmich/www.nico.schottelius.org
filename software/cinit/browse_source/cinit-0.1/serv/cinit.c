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

#include <sys/poll.h>  /* poll */

#include "cinit.h"

/* global variables */
struct listitem *list;
int sock;
pid_t cpid;

/***********************************************************************
 * the main procedure
 */

int main(int argc, char **argv)
{
   struct sockaddr_un addr;
   struct pollfd plist;
   char  *initdir;

   list = NULL;            /* list of services is empty currently */
   initdir = CINIT_INIT;   /* default init dir */

   set_signals(ACT_SERV);  /* set signal handlers */

   /* tell the world we are there */
   LOG(MSG_CINIT);

   /* read args, profile support */
   while(argc > 1) {
#ifdef DEBUG
   printf("argc=%d\n, argv=%s",argc,argv[argc-1]);
#endif
      D_PRINTF(argv[argc-1]);
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
   D_PRINTF(initdir);

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
   
   memset(&addr, 0, sizeof(addr) ); /* clear addr */
   strcpy(addr.sun_path, CINIT_SOCK);
   addr.sun_family = AF_UNIX;
   
   if ( fcntl(sock,F_SETFL,O_ASYNC|O_NONBLOCK) == -1) {
      perror(MSG_FCNTL);
      panic();
   }


   if(bind(sock,(struct sockaddr *)&addr,sizeof(addr)) == -1) {
      perror(MSG_BIND);
      panic();
   }
   
   /* start listening */
   if(listen(sock,SOCK_QUEUE) == -1) {
      perror(MSG_LISTEN);
      panic();
   }

   /* start init or profile */
   run_init_svc(initdir);

   /* free, if we malloc()ed before */
   if(initdir != CINIT_INIT) {
      free(initdir);
   }

   /* our life is polling a socket */
   plist.fd = sock;
   plist.events = POLLIN | POLLPRI;
   while(1) {
      if(poll(&plist, 1, -1) != -1) {
         if( (plist.revents & POLLIN)  == POLLIN ||
             (plist.revents & POLLPRI) == POLLPRI) {
            /* look if a packet reached us */
            D_PRINTF("sigio starten");
            sigio(sock);
         }
      }
   }
   D_PRINTF("hier sollte man nicht sein");
}
