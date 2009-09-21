/* 
 * cinit
 * (c) 2005 Nico Schottelius (nico-linux at schottelius.org)
 * handle client requests
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include "cinit.h"

/***********************************************************************
 * sigio: client handling
 */

/* we are called, if one or _more_ connections are waiting */
void sigio(int signal)
{
   int               tmp, nsock;
   char              buf[PATH_MAX+1], status;
   struct listitem   *list_tmp; 
   pid_t             pid;


   D_PRINTF("sigio() startet");
   
   do {
      /* create new socket */
      nsock = accept(sock,(struct sockaddr *) NULL, (socklen_t *) NULL);
   
      if( nsock == -1) {
         if (errno != EAGAIN) { /* report, but don't panic */
            perror("accept");
         }

         return;
      }
      
      if( read(nsock,&buf[0],1) == -1) {
         perror("read-command");
         close(nsock);
         continue;
      } 
#ifdef DEBUG
      printf("DEEEEEBUG: command: %d\n",buf[0]);
#endif
      switch(buf[0]) {
      
      /********************** START SERVICE *******************/
      case CMD_START_SVC:
         tmp = do_start_svc(nsock,buf,ACT_SERV);
         if(!tmp) break;

         buf[tmp] = 0;

         list_tmp = list_search(buf);

         if(list_tmp != NULL) {   /* service already exists */
            buf[0] = 0;
            do_result(nsock,&buf[0]);
            break;
         }

         tmp = list_insert(buf,ST_TMP); /* add service */
         if(!tmp) { /* failed */
            LOG("Adding service failed!");
            buf[0] = 0;
            do_result(nsock,&buf[0]);
         }
      break;

      /********************** CHANGE SERVICE STATUS *******************/
      case CMD_CHG_STATUS:
         tmp =  do_change_status(buf,&status,&pid,nsock,ACT_SERV);

         if(!tmp) break;

         buf[tmp] = 0;  /* terminate buf */

#ifdef DEBUG
         printf("DEBUG: %s (%d) = %d\n",buf,pid,status);
#endif
         
         if(!list_modify(buf,status,pid)) {
            D_PRINTF("MOD.ERR");
            buf[0] = 0;
         } else {
            buf[0] = 1;
         }
         do_result(nsock,buf);
         break;

      default:
         LOG("Unknown command!");
         break;
      }
      close(nsock);
   } while ( 1 );

   D_PRINTF("ENDE:SIGIO");
}