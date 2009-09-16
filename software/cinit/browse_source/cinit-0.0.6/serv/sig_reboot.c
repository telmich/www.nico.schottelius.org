/* 
 * cinit
 * (c) 2005 Nico Schottelius (nico-linux at schottelius.org)
 * We are called by signal to reboot
 */

#include <sys/mount.h>
#include <sys/types.h>
#include <sys/reboot.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <time.h>

#include <stdlib.h>

#include "cinit.h"

/***********************************************************************
 * sig_reboot
 * Shutdown the system
 */

void sig_reboot(int signal)
{
   struct listitem *tmp;
   struct timespec ts;
   char **cmd;
   int   i;

   D_PRINTF("sig_reboot() startet");

   tmp = list->before;
   do {
      /* kill service watcher, which will kill processes itself */
      if( tmp->status == ST_RESPAWN) {
         D_PRINTF("Bringing down respawn watcher");
         kill(tmp->pid,SIGTERM);
      }

      /* execute shutdown service */
      D_PRINTF("Executing off-service");
      exec_svc(tmp->abs_path,0);
      tmp = tmp->before;
   } while( tmp != list );
   
   /* Let's disappear */
   close(sock);
   if(umount(CINIT_TMNT) == -1) {
      perror("umount");
   }

   /* now: all services are down, lets kill all other processes */
   if( kill(-1,SIGTERM) == -1) {
      perror(MSG_TERMKILL);
   }
   D_PRINTF("SIGTERM GESENDET");

   ts.tv_sec = SLEEP_KILL; /* defined in conf/ */
   nanosleep(&ts,NULL);


   if( kill(-1,SIGKILL) == -1) {
      perror(MSG_KILLBILL);
   }
   D_PRINTF("SIGKILL GESENDET");
   /* now: we are completly alone. umount everything, go to bed */

/* THIS IS FUCKING UNCLEAN AND SHOULD BE CLEANED UP VERY MUCH!!! */

#define CMD_CNT 5
#define SWAP_OFF_CMD    "/sbin/swapoff"
#define SWAP_OFF_ARG1   "-a"

#define UMOUNT_CMD    "/bin/umount"
#define UMOUNT_ARG1   "-a"

#define MOUNT_CMD    "/bin/mount"
#define MOUNT_ARG1   "-o"
#define MOUNT_ARG2   "remount,ro"
#define MOUNT_ARG3   "/"

   cmd = (char **) malloc( (CMD_CNT+1) * ( sizeof (char*) ) );
   if(cmd) {
//      cmd[0] = (char *) malloc(strlen(SWAP_OFF_CMD) +1);
//      cmd[1] = (char *) malloc(strlen(SWAP_OFF_CMD) +1);
      /*  /sbin/swapoff -a */
      cmd[0] = SWAP_OFF_CMD;
      cmd[1] = SWAP_OFF_ARG1;
      cmd[2] = NULL;

      i = fork();
      if(i == 0) {
         execv(cmd[0],cmd);
         _exit(1);   /* if exec fails in child */
      }
      /*  /bin/umount -a */
      cmd[0] = UMOUNT_CMD;
      cmd[1] = UMOUNT_ARG1;
      cmd[2] = NULL;

      i = fork();
      if(i == 0) {
         execv(cmd[0],cmd);
         _exit(1);   /* if exec fails in child */
      }

      /* /bin/mount -o remount,ro / */
      cmd[0] = MOUNT_CMD;
      cmd[1] = MOUNT_ARG1;
      cmd[2] = MOUNT_ARG1;
      cmd[3] = MOUNT_ARG1;
      cmd[4] = NULL;

      i = fork();
      if(i == 0) {
         execv(cmd[0],cmd);
         _exit(1);   /* if exec fails in child */
      }
  }

   switch(signal) {
      case SIGUSR1: /* halt */
         reboot(RB_HALT_SYSTEM);
         break;
      case SIGTERM: /* power off */
         reboot(RB_POWER_OFF);
         break;
      case SIGHUP: /* reboot */
         reboot(RB_AUTOBOOT);
         break;
   }
   _exit(0);
}
