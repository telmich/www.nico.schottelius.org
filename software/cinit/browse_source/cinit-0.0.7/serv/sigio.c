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
#include <limits.h>

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

   do {
      /* create new socket */
      nsock = accept(sock,(struct sockaddr *) NULL, (socklen_t *) NULL);
   
      if( nsock == -1) {
         if( errno != EAGAIN ) { /* report, but don't panic */
            perror("accept");
         }
         return;
      }
      
      if( read(nsock,&buf[0],1) == -1) {
         perror(MSG_ERR_READ);
         close(nsock);
         continue;
      } 
      switch(buf[0]) {
      
      /********************** START SERVICE *******************/
      case CMD_START_SVC:
         tmp = do_start_svc(nsock,buf,ACT_SERV);
         if(!tmp) break;

         buf[tmp] = 0;
         list_tmp = list_search(buf);

         if(list_tmp != NULL) {   /* service already exists, return status */
            D_PRINTF("Service schon da");
            buf[0] = list_tmp->status;
            do_result(nsock,&buf[0]);
            break;
         }

         tmp = list_insert(buf,ST_TMP); /* add service */
         if(!tmp) { /* failed */
            LOG("Adding service failed!");
            buf[0] = 0;
            do_result(nsock,&buf[0]);
         } else {
            buf[0] = ST_TMPNOW;
            do_result(nsock,&buf[0]);
         }
      break;

      /********************** CHANGE SERVICE STATUS *******************/
      case CMD_CHG_STATUS:
         tmp =  do_change_status(buf,&status,&pid,nsock,ACT_SERV);

         if(!tmp) break;

         buf[tmp] = 0;  /* terminate buf */

         if(!list_modify(buf,status,pid)) {
            D_PRINTF("**** MOD.ERR ***");
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
