/* 
 * cinit
 * (c) 2005 Nico Schottelius (nico-linux at schottelius.org)
 * handle client requests
 */

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <signal.h>

#include "cinit.h"

/***********************************************************************
 * sigio: client handling
 */

/* we are called, if one or _more_ connections are waiting */
void sigio(int socket)
{
   int               tmp, nsock;
   char              buf[PATH_MAX+1], status;
   struct listitem   *list_tmp; 
   pid_t             pid;
   
   while (
      (nsock = accept(socket,(struct sockaddr *) NULL, (socklen_t *) NULL) )
         != -1) {

      D_PRINTF("sigio...");

      if( read(nsock,&buf[0],1) == -1) {
         perror(MSG_ERR_READ);
         close(nsock);
         continue;
      } 

#ifdef DEBUG
      printf("CMD: %d\n",buf[0]);
#endif
      switch(buf[0]) {
      
      /********************** START SERVICE *******************/
      case CMD_START_SVC:
         tmp = do_svc_name(nsock,buf,ACT_SERV);
         if(!tmp) break;

         buf[tmp] = 0;
         D_PRINTF(buf);
         list_tmp = list_search(buf);

         if(list_tmp != NULL) {   /* service already exists, return status */
            status = list_tmp->status;
            do_result(nsock,&status);
            break;
         }

         tmp = list_insert(buf,ST_TMP); /* add service */
         if(!tmp) { /* failed */
            LOG(MSG_ERR_ADD_SVC);
            status = 0;
         } else {
            status = ST_TMPNOW;
         }
         do_result(nsock,&status);
      break;

      /********************** STOP RESPAWNING *******************/
      case CMD_STOP_SVC:
         tmp = do_svc_name(nsock,buf,ACT_SERV);
         if(!tmp) break;
         buf[tmp] = 0;
         D_PRINTF(buf);
         list_tmp = list_search(buf);
         
         if(list_tmp != NULL) {   /* service exists */
            if(list_tmp->status == ST_RESPAWN) {
               D_PRINTF("respawner ausschalten");
               kill(list_tmp->pid,SIGTERM);

#ifdef DEBUG
               printf("pid=%d\n",list_tmp->pid);
#endif
               /* wait for watcher to terminate */
               waitpid(list_tmp->pid,&tmp,0);

               D_PRINTF("liste modifizieren");
               status = ST_OFF;
               if(!list_modify(buf,status,list_tmp->pid)) {
                  D_PRINTF("mod-aus fail");
                  status = ST_FAIL;
               } else { /* return status */
                  D_PRINTF("kein respawn");
                  status = list_tmp->status;
               }
            }
         } else { /* no service there */
            status = ST_OFF;
         }
         do_result(nsock,&status);
      break;
      /********************** CHANGE SERVICE STATUS *******************/
      case CMD_CHG_STATUS:
         tmp =  do_change_status(buf,&status,&pid,nsock,ACT_SERV);
         if(!tmp) break;
         buf[tmp] = 0;  /* terminate buf */

         D_PRINTF(buf);

         if(!list_modify(buf,status,pid)) {
            LOG(MSG_ERR_MODIFY);
            status = 0;
         }
         do_result(nsock,&status);
         break;

      /********************** SPECIAL ACTIONS *******************/
      case CMD_REBOOT:
         sig_reboot(SIGHUP);
         break;
      case CMD_POWEROFF:
         sig_reboot(SIGTERM);
         break;
      case CMD_HALT:
         sig_reboot(SIGUSR1);
         break;
      case CMD_RESCUE:
         sig_reboot(SIGUSR2);
         break;
      case CMD_UPDATE:
         sig_reboot(SIGCONT);
         break;
      default:
         LOG(MSG_CMD_UNKNOWN);
         break;
      }
      close(nsock);
   }

   if( errno != EAGAIN ) { /* report, but don't panic */
      perror(MSG_ERR_ACCEPT);
   }

   D_PRINTF("ENDE");
}
