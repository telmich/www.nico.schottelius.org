/***********************************************************************
 *
 *    2006-2007 Nico Schottelius (nico-cinit at schottelius.org)
 *
 *    part of cLinux/cinit
 *
 *    Pre calculate the service tree
 */

/* FIXME: clean headers, check:
 * getcwd */
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>


#include <limits.h>        /* PATH_MAX          */
#include <stdlib.h>        /* malloc            */
#include <string.h>        /* strcpy            */

#include "intern.h"        /* mini_printf       */
#include "messages.h"
#include "svc.h"

int check_add_deps(struct listitem *svc, int type)
{
   char              buf[PATH_MAX+1];
   char              oldpath[PATH_MAX+1];
   struct dirent     *tdirent;
   struct dep        *deps = NULL;
   struct listitem   *new_svc;
   DIR               *d_tmp;

   /* remember where we started */
   if(!getcwd(oldpath,PATH_MAX+1)) {
      print_errno(MSG_GETCWD);
      return 0;
   }

   /* FIXME remove in production */
   D_PRINTF("CAD::");
   D_PRINTF(svc->abs_path);
   D_PRINTF("\n");

   /* Create path */
   strcpy(buf,svc->abs_path);
   if(type == DEP_NEEDS) {
      if(!path_append(buf,C_NEEDS)) return 0;
   } else {
      if(!path_append(buf,C_WANTS)) return 0;
   }

   d_tmp = opendir(buf);
   if(d_tmp == NULL) {
      if(errno != ENOENT) {
         print_errno(buf);
         return 0;
      }
      return 1;   /* it's fine when there's no dependencies  */
   }

   if(chdir(buf) == -1) { /* change to needs or wants */
      print_errno(buf);
      return 0;
   }

   while((tdirent=readdir(d_tmp))!=NULL) {
      if(*(tdirent->d_name) == '.') continue; /* ignore .* */

      /* skip non-working directories / broken links
       * path_absolute reports errors on failure */
      if(!path_absolute(tdirent->d_name,buf,PATH_MAX+1)) continue;

      /* 1. create the service we depend on
       * 2. initialize its dependencies
       */
      if(!(new_svc = gen_svc_tree(buf))) return 0;

      /* We need ALL dependencies, as we are called only once
       * per service; no need to test that first!
       *
       * And the other service CANNOT know anything about us yet,
       * so we always add us to its list.
       */

      /* Dependencies:
       * - a.needs b; add b to the list of dependencies.
       * - a.needs b; add a to the list of needed by b.
       *
       * 1. check whether the dependency already exists
       * 2. otherwise add it
       * 3. do it once for needs, once for needed_by
       */

      /* create a dependency entry containing us */
      deps  = dep_create(svc);
      if(!deps) return 0;

      if(type == DEP_NEEDS) {
         dep_entry_add(&(new_svc->needed),deps);

         /* second link */
         deps  = dep_create(new_svc);
         if(!deps) return 0;
         dep_entry_add(&(svc->needs),deps);
      } else {
         dep_entry_add(&(new_svc->wanted),deps);

         /* second link */
         deps  = dep_create(new_svc);
         if(!deps) return 0;
         dep_entry_add(&(svc->wants),deps);
      }
   }
   if(chdir(oldpath) == -1) {
      print_errno(buf);
      return 0;
   }
   closedir(d_tmp);

   return 1;
}
