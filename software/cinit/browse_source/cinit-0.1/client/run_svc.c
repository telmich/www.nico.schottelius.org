/* 
 * (c) 2005 Nico Schottelius (nico-linux at schottelius.org)
 * run_svc
 * part of cinit
 */

/* headers are clean */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include <time.h>

#include "cinit.h"

/***********************************************************************
 * run_svc: run a service and beforeo all dependencies
 */
int run_svc(char *rpath)
{
   int tmp;
   pid_t pid;
   char abspath[PATH_MAX], pathtmp[PATH_MAX]; /* pathtmp = use-it-for-all bitch*/
   struct stat buf;
   struct timespec ts;
   
   D_PRINTF(rpath);

   /******************* absolute PATH ***************/
   /* get current working dir */
   if(! (int) getcwd(pathtmp,PATH_MAX)) {
      perror(pathtmp);
      return 0;
   }

   /* change to rpath */
   if(chdir(rpath) == -1) {
      perror(rpath);
      return 0;
   }

   /* get absolute name of rpath */
   if(! (int) getcwd(abspath,PATH_MAX)) {
      perror(abspath);
      return 0;
   }

   /* change back */
   if(chdir(pathtmp) == -1) {
      perror(pathtmp);
      return 0;
   }
   D_PRINTF(abspath);
   
   /******************* REGISTER SERVICE ***************/
   do {
      tmp = msg_svc_on_off(abspath,1);    /* check status */
      
      switch(tmp) {
         case 0: /* failed to communicate */
         case ST_FAIL: /* somebody failed, we won't retry */
            D_PRINTF("fail");
            return 0;
            break;
         case ST_TMP: /* someone is working on it */
            D_PRINTF("schon tmp");
            ts.tv_sec = SLEEP_RERUN;
            nanosleep(&ts,NULL);
            break;
         case ST_ONCE: /* somebody did our work */
         case ST_RESPAWN: /* somebody does our work */
            D_PRINTF("schon erledigt");
            return 1;
            break;
         case ST_TMPNOW: /* we are on it! */
            D_PRINTF("wir sind dran");
            break;
      }
   } while(tmp != ST_TMPNOW);

   /******************* BEGIN DEPENDENCIES ***************/
   strcpy(pathtmp,abspath);
   strcat(pathtmp,SLASH);
   strcat(pathtmp,C_NEEDS);
   
   D_PRINTF(pathtmp);

   if( stat(pathtmp,&buf) == 0 ) {
      if( ! run_run_svcs(pathtmp) ) {
         LOG(abspath);
         LOG(LOG_NEED_FAIL);
         msg_change_status(abspath, ST_FAIL, 0);
         return 0;
      }
   }
   
   /********** WANTS ************/
   strcpy(pathtmp,abspath);
   strcat(pathtmp,SLASH);
   strcat(pathtmp,C_WANTS);

   D_PRINTF(pathtmp);

   if( stat(pathtmp,&buf) == 0 ) {
      run_run_svcs(pathtmp);  /* don't care about what happens with the want svc */
   }

   /******************* execute services ***************/
   strcpy(pathtmp,abspath);
   strcat(pathtmp,SLASH);
   strcat(pathtmp,C_RESPAWN);
   
   if( stat(pathtmp,&buf) == 0) {
      pid = respawn_svc(abspath);
      tmp = ST_RESPAWN;
   } else {
      pid = exec_svc(abspath, 1);
      tmp = ST_ONCE;
   }

   if(!pid) {
      msg_change_status(abspath, ST_FAIL, pid);
      return 0;
   }

   if(!msg_change_status(abspath, tmp, pid) ) {
      return 0;
   }

   return 1;
}
