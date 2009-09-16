/* 
 * (c) 2005 Nico Schottelius (nico-linux at schottelius.org)
 * cservice: control cinit
 * part of cLinux/cinit
 */

/* *stat() */
#include <sys/stat.h>
#include <unistd.h>

/* open */
#include <fcntl.h>

/* siggnal */
#include <signal.h>

/* PATH_MAX */
#include <limits.h>

/* str* */
#include <string.h>

/* sockets */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

/* mount */
#include <sys/mount.h>

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "cinit.h"

/* global variable */
struct listitem *list;
int sock;
pid_t cpid;

/***********************************************************************
 * cservice - control cinit
 */

#define CSERVICE_BANNER    "cservice - control cinit\n\n"
#define USAGE_TEXT         "\n\nUsage:\n\ncservice -[phearsu] <service>\n"  \
   "\t-e\t- (ein,on) start a service (respawn if respawning is set)\n" \
   "\t-a\t- (aus,off) stop a respawing service\n\n" \
   "\t-p\t- (power off) Power off the system\n" \
   "\t-h\t- (halt) Halt the system \n" \
   "\t-r\t- (reboot) Reboot the system\n" \
   "\t-s\t- (single user/rescue) Rescue mode\n" \
   "\t-u\t- (update) Hot-Reboot system and update cinit\n" \
   "\n-h, -p, -r, -s and -u need the \"Magic Word\" to do their work: \"thilo\"\n" \

/***********************************************************************
 * fuzzy_path: allow other kinds of path specification ..
 */
char *fuzzy_path(char *rpath)
{
   char pathtmp[PATH_MAX];
   char *re = NULL;

   /* change to rpath */
   if(chdir(rpath) == -1) {
      perror(rpath);
      return NULL;
   }

   /* get absolute name of rpath */
   if(! (int) getcwd(pathtmp,PATH_MAX)) {
      perror(pathtmp);
      return NULL;
   }
   
   re = malloc( strlen(pathtmp) + 1);
   if(re == NULL) return NULL;

   strcpy(re,pathtmp);

   return re;
}

/***********************************************************************
 * usage: tell the user what's wrong and a help text
 */
void usage(char *stext)
{
   write(1,CSERVICE_BANNER,strlen(CSERVICE_BANNER));
   write(1,stext,strlen(stext));
   write(1,USAGE_TEXT,strlen(USAGE_TEXT));
   _exit(1);
}



/***********************************************************************
 * cservice: the main part for controlling cinit from outside
 */
int main(int argc, char **argv)
{
   /* argv */
   if(argc != 3) usage("Too less arguments!");
   if(argv[1][0] != '-') usage("Wrong arguments");
   if(strlen(argv[1]) != 2) usage("Wrong parameter length");

   switch(argv[1][1]) {
      case 'e':   /* ein */
         if(run_svc(argv[2])) {
            mini_printf("Successfully started.",1);
         } else {
            mini_printf("An error occured",1);
         }
         break;

      case 'a':   /* aus */
         if( msg_svc_on_off( fuzzy_path(argv[2]),0 ) ) { 
            mini_printf("Successfully stoped.",1);
         } else {
            mini_printf("An error occured",1);
         }
         break;

      case 'p':   /* power off */
         mini_printf("Powering off...",1);
         msg_reboot(CMD_POWEROFF);
         break;

      case 'h':   /* halt */
         mini_printf("Halting...",1);
         msg_reboot(CMD_HALT);
         break;

      case 'r':   /* reboot */
         mini_printf("Rebooting...",1);
         msg_reboot(CMD_REBOOT);
         break;

      case 's':   /* rescue */
         mini_printf("Rescue mode...",1);
         msg_reboot(CMD_RESCUE);
         break;

      case 'u':   /* update */
         mini_printf("Update and hot reboot in progress...",1);
         msg_reboot(CMD_UPDATE);
         break;

      default:
         usage("Unknown parameter");
         break;
   }
   D_PRINTF("wir beenden uns");;
   return 0;
}
