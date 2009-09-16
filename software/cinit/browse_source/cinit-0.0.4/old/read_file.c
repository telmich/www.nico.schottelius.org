/* 
 * (c) 2005 Nico Schottelius (nico-linux at schottelius.org)
 * run_svc
 * part of cinit
 */

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <unistd.h>
#include <stdio.h>


#include "cinit.h"

/***********************************************************************
 * read_file: return file content, each line a char*
 */

char **read_file(char *file)
{
   int tmp;
   int fd, argc;

   char buf[BUFSIZE];
   char *p, *sbuf = NULL;
   char  **nargv = NULL;

   struct stat stbuf;
   
   D_PRINTF("Lese Datei");
   D_PRINTF(file);

   if( !stat(file,&stbuf) ) {
      fd = open(file,O_RDONLY);

      if(fd == -1) {
         LOG("params exists, but open params failed");
         return NULL;
      }

      D_PRINTF("open ist ok");
   
      argc   = 0;

      /* most likely one round */
      while ( (tmp = read(fd,buf,BUFSIZE) ) != 0 ) {
         if(tmp == -1) { 
            perror("read");
            return NULL;
         }

         sbuf = realloc(sbuf,argc + tmp + 1);
         strncpy(&sbuf[argc],buf,tmp);
         argc += tmp;
      }
      close(fd);
   }

   sbuf[argc] = '\0';

   /************** build string **************/
   /* The idea is to reuse the allocated memory. We'll simply
   * replace the \n with \0 and set a char* to it.
   * We'll lose the last byte (the initial \0), but we
   * don't care, as we die some seconds later with execv() */

   argc = 0;

   while( (p = strchr(sbuf,'\n')) ) {
      nargv = realloc(nargv, sizeof(char *) * (argc + 1));

      if(nargv == NULL) {
            LOG("realloc failed");
            return NULL;
      }

      nargv[argc] = sbuf;
      *p = '\0';
      sbuf = p+1; /* is ok, as behind the last \n is a \0 */
      argc++;
   }

   /************ close argv list **************/
   nargv = realloc(nargv, sizeof(char *) * (argc + 1));
   if(nargv == NULL) {
      LOG("realloc failed");
      return NULL;
   }

   nargv[argc] = NULL;  /* terminate argv list */
   
   return nargv;
}
