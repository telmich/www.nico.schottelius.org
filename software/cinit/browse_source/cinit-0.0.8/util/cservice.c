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
#include <fcntl.h>

#include <time.h>    /* nanosleep */

#include "cinit.h"

/* global variable */
struct listitem *list;
int sock, logfd;
pid_t cpid;

/***********************************************************************
 * cservice - control cinit
 */

#define CSERVICE_BANNER    "cservice - control cinit\n\n"
#define USAGE_TEXT         "\n\nUsage:\n\ncservice -[phears] <service>\n"  \
   "\t-e\t- (ein,on) start a service (respawn if respawning is set)\n" \
   "\t-a\t- (aus,off) stop a respawing service\n\n" \
   "\t-p\t- (power off) Power off the system\n" \
   "\t-h\t- (halt) Halt the system \n" \
   "\t-r\t- (reboot) Reboot the system\n" \
   "\t-s\t- (single user/rescue) Rescue mode\n" \
   "\n-h, -p, -s and -r need the \"Magic Word\" to do their work: \"thilo\"\n" \

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
         if(msg_svc_on_off(argv[2],0) ) { 
            mini_printf("Successfully stoped.",1);
         } else {
            mini_printf("An error occured",1);
         }
         break;

      case 'p':   /* power off */
         msg_reboot(CMD_POWEROFF);
         break;

      case 'h':   /* halt */
         msg_reboot(CMD_HALT);
         break;

      case 'r':   /* reboot */
         msg_reboot(CMD_REBOOT);
         break;

      case 's':   /* rescue */
         msg_reboot(CMD_RESCUE);
         break;

      default:
         usage("Unknown parameter");
         break;
   }
   D_PRINTF("wir beenden uns");;
   return 0;
}
