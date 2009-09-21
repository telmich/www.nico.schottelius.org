/***********************************************************************
 *
 *    2006 Nico Schottelius (nico-linux-cinit at schottelius.org)
 *
 *    part of cLinux/cinit
 *
 *    Return whether should respawn or not
 */

#include "svc.h"

/* checking for existence is done before! */
int svc_should_respawn(struct listitem *li)
{
   return (li->status & ST_SH_RESPAWN);
}