/* 
 * (c) 2005 Nico Schottelius (nico-linux at schottelius.org)
 * tell cinit that I want to start a service
 * part of cinit
 */

#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "cinit.h"

int msg_start_svc(char *svc)
{
   D_PRINTF(svc);

   D_PRINTF("writing message begin");
   if(!begin_msg(CMD_START_SVC)) return 0;
   D_PRINTF("writing service infos");
   if(!do_start_svc(sock,svc,ACT_CLIENT)) return 0;
   D_PRINTF("reading result");
   if(!do_result(sock,NULL)) return 0;
   D_PRINTF("gutes ende");

   return 1;
}
