KOPIE VON RUN_SVC

/* 
 * (c) 2005 Nico Schottelius (nico-linux at schottelius.org)
 * run_svc
 * part of cinit
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>

#include "cinit.h"

/***********************************************************************
 * run_svc: run a service and beforeo all dependencies
 */
void kill_svc(char *rpath, pid_t pid)
{
   int tmp;
   char svc[PATH_MAX], svcparams[PATH_MAX];
   struct stat buf;

   D_PRINTF("starte kill_svc");
   D_PRINTF(rpath);

   /******************* absolute PATH ***************/
   /* get current working dir */
   if(! (int) getcwd(pathtmp,PATH_MAX)) {
      perror("getcwd");
      return 0;
   }

   /* change to rpath */
   if(chdir(rpath) == -1) {
      perror("chdir");
      return 0;
   }

   /* get absolute name of rpath */
   if(! (int) getcwd(abspath,PATH_MAX)) {
      perror("getcwd2");
      return 0;
   }

   /* change back */
   if(chdir(pathtmp) == -1) {
      perror("chdir2");
      return 0;
   }
   
   /******************* REGISTER SERVICE ***************/
   D_PRINTF(abspath);
   tmp = msg_start_svc(abspath);    /* mark us as temporary */
   if(tmp == -1) return 1;          /* already started */

   /******************* BEGIN DEPENDENCIES ***************/
   D_PRINTF("Starte needs");
   strcpy(pathtmp,abspath);
   strcat(pathtmp,"/");
   strcat(pathtmp,C_NEEDS);
   if( ! run_run_svcs(pathtmp) ) {
      /* FIXME: add log + service name here */
      D_PRINTF("some NEEDED services failed)");
      msg_change_status(abspath, ST_FAIL, 0);
      return 0;
   }
   
   D_PRINTF("Starte wants");
   strcpy(pathtmp,abspath);
   strcat(pathtmp,"/");
   strcat(pathtmp,C_WANTS);
   run_run_svcs(pathtmp);  /* don't care about what happens with the want svc */

   /******************* execute services ***************/
   strcpy(pathtmp,abspath);
   strcat(pathtmp,"/");
   strcat(pathtmp,C_RESPAWN);
   
   if( stat(pathtmp,&buf) == 0) {
      D_PRINTF("Respawning");
      pid = respawn_svc(abspath);
      tmp = ST_RESPAWN;
   } else {
      D_PRINTF("exec_unce");
      pid = exec_svc(abspath);
      tmp = ST_ONCE;
   }

   if(!pid) {
      msg_change_status(abspath, ST_FAIL, pid);
      return 0;
   }

   D_PRINTF("Melde status");
   if(!msg_change_status(abspath, tmp, pid) ) {
      return 0;
   }

   return 1;
}
