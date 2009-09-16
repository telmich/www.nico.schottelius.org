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

   D_PRINTF("starte run_svc");
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
   
   if( stat(pathtmp,&buf) == 0 ) {
      if( ! run_run_svcs(pathtmp) ) {
         /* FIXME: add log + service name here */
         D_PRINTF("some NEEDED services failed");
         msg_change_status(abspath, ST_FAIL, 0);
         return 0;
      }
   }
   
   strcpy(pathtmp,abspath);
   strcat(pathtmp,"/");
   strcat(pathtmp,C_WANTS);

   if( stat(pathtmp,&buf) == 0 ) {
      D_PRINTF("Starte wants");
      run_run_svcs(pathtmp);  /* don't care about what happens with the want svc */
   }

   /******************* execute services ***************/
   /*FIXME: check for C_ON, otherwise we are done! */

   strcpy(pathtmp,abspath);
   strcat(pathtmp,"/");
   strcat(pathtmp,C_RESPAWN);
   
   if( stat(pathtmp,&buf) == 0) {
      pid = respawn_svc(abspath);
      tmp = ST_RESPAWN;
   } else {
      tmp = 1;
      pid = exec_svc(abspath, tmp);
      tmp = ST_ONCE;
   }

   D_PRINTF("Melde status");
   if(!pid) {
      msg_change_status(abspath, ST_FAIL, pid);
      return 0;
   }

   if(!msg_change_status(abspath, tmp, pid) ) {
      return 0;
   }

   return 1;
}
