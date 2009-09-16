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
   if(!begin_msg(CMD_START_SVC)) return 0;
   if(!do_start_svc(sock,svc,ACT_CLIENT)) return 0;
   return (int) do_result(sock,NULL);
}
