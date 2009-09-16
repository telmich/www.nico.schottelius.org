/* 
 * (c) 2005 Nico Schottelius (nico-linux at schottelius.org)
 * run_svc
 * part of cinit
 */

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


#include "cinit.h"

/***********************************************************************
 * exec_svc: exec service abspath C_ON or C_OFF
 */

pid_t exec_svc(char *abspath, int on)
{
   pid_t pid;
   int tmp;
   char *p, pathtmp[PATH_MAX];
   char *sbuf = NULL, **nargv = NULL, **nenv = NULL;
   int fd, argc;
   struct stat buf;
   
   pid = fork();
   if( pid == -1 ) {
      perror("fork");
      return 0;
   }
   
   /***** PARENT ******/
   if(pid > 0) {
      waitpid(pid,&tmp,0);
      if( WIFEXITED(tmp) ) {
         D_PRINTF("EXEC SVC ERFOLGREICH");
         return pid;
      } else {
         D_PRINTF("EXEC SVC FEHLGESCHLAGEN");
         return 0;
      }
   }

   /*********** CHILD EXECUTION ***********/
   strcpy(pathtmp,abspath);
   strcat(pathtmp,"/");

   if(on) {
      strcat(pathtmp,C_ON);
   } else {
      strcat(pathtmp,C_OFF);
   }

   D_PRINTF(pathtmp);

   /* readlink retrieves real name, if it is a symlink */
   if ( (tmp = readlink(pathtmp,pathtmp,PATH_MAX) ) == -1) {

      /* nothing there? fine, exit */
      if(errno == ENOENT) {
        _exit(0);
      } else if (errno != EINVAL) {
         perror("readlink");
         _exit(1);
      }

      /* restore original name, file is not a link */
      strcpy(pathtmp,abspath);
      strcat(pathtmp,"/");

      if(on) { 
         strcat(pathtmp,C_ON);
      } else {
         strcat(pathtmp,C_OFF);
      }
      tmp = strlen(pathtmp);
   } else {
      /* add '\0', readlink forgets it */
      pathtmp[tmp] = '\0'; 
   }
   tmp++; /* add memory for \0 */

   /********** build argv0 ********/
   nargv = (char **) malloc( sizeof(char *) );
   if(nargv == NULL) _exit(1);

   *nargv = (char *) malloc( tmp );
   if(*nargv == NULL) _exit(1);

   strcpy(*nargv,pathtmp);

   /********************** read params *********************/
   strcpy(pathtmp,abspath);
   strcat(pathtmp,"/");

   if(on) {
      strcat(pathtmp,C_ONARG);
   } else {
      strcat(pathtmp,C_OFFARG);
   }

   /* open params file */
   if( !stat(pathtmp,&buf) ) {
      fd = open(pathtmp,O_RDONLY);

      if(fd == -1) {
         perror("open");
         _exit(1);
      }

      argc = 0;

      /* most likely one round */
      while ( (tmp = read(fd,pathtmp,PATH_MAX) ) != 0 ) {
         if(tmp == -1) { 
            perror("read");
            _exit(1);
         }

         sbuf = realloc(sbuf,argc + tmp + 1);
         strncpy(&sbuf[argc],pathtmp,tmp);
         argc += tmp;
      }
      close(fd);

      if(argc)
         sbuf[argc] = '\0'; /* terminate string */

   } else {
      sbuf = NULL;
   }

   /************** build string **************/
   /* The idea is to reuse the allocated memory. We'll simply
   * replace the \n with \0 and set a char* to it.
   * We'll lose the last byte (the initial \0), but we
   * don't care, as we die some seconds later with execv() */
   
   argc = 1; /* argv0 */
   while( sbuf != NULL) {
      p = strchr(sbuf,'\n');
      nargv = realloc(nargv, sizeof(char *) * (argc + 1));

      if(nargv == NULL) {
         LOG("realloc failed");
         _exit(1);
      }

      nargv[argc] = sbuf;
      *p = '\0';
      if( *(p+1) == '\0') {
         sbuf = NULL;
      } else {
         sbuf = p+1;
      }
      argc++;
   }

   /************ close argv list **************/
   nargv = realloc(nargv, sizeof(char *) * (argc + 1)); /* 1: NULL-pointer */
   if(nargv == NULL) {
      LOG("realloc failed");
      _exit(1);
   }
   nargv[argc] = NULL;  /* terminate argv list */

   /********************** read environment *********************/
   strcpy(pathtmp,abspath);
   strcat(pathtmp,SLASH);
   if(on) {
      strcat(pathtmp,C_ONENV);
   } else {
      strcat(pathtmp,C_OFFENV);
   }

   argc = 0;
   sbuf = NULL;
   if( !stat(pathtmp,&buf) ) {
      fd = open(pathtmp,O_RDONLY);

      /* if a file exists, failing to open it is an error */
      if(fd == -1) {
         perror("open");
         _exit(1);
      }

      while ( (tmp = read(fd,pathtmp,PATH_MAX) ) != 0 ) {
         if(tmp == -1) { 
            perror("read");
            _exit(1);
         }

         sbuf = realloc(sbuf,argc + tmp + 1);
         strncpy(&sbuf[argc],pathtmp,tmp);
         argc += tmp;
      }
      close(fd);
      if(argc)
         sbuf[argc] = '\0'; /* terminate string */
   }

   /************** build environment string **************/
   argc = 0;
   while( sbuf != NULL ) {
      p = strchr(sbuf,'\n');
         
      nenv = realloc(nenv, sizeof(char *) * (argc + 1));
      if(nenv == NULL) {
         LOG("realloc failed");
         _exit(1);
      }

      nenv[argc] = sbuf;
      *p = '\0';
      if( *(p+1) == '\0') {
         sbuf = NULL;
      } else {
         sbuf = p+1;
      }
      argc++;
   }

   /************ close env list **************/
   nenv = realloc(nenv, sizeof(char *) * (argc + 1));
   if(nenv == NULL) {
      LOG(MSG_ERR_ALLOC);
      _exit(1);
   }
   nenv[argc] = NULL;

   /****************** EXECUTE ********************/

   execve(nargv[0],nargv,nenv);

   /* we should never reach this block */
   perror(MSG_ERR_EXECVE);
   _exit(1);
}
