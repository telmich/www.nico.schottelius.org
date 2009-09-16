/***********************************************************************
 *
 *    2005-2006 Nico Schottelius (nico-linux-cinit at schottelius.org)
 *
 *    part of cLinux/cinit
 *
 *    How to halt the system
 */

#include <unistd.h>
#include <sys/reboot.h>

void cinit_halt(void)
{
   reboot(RB_HALT_SYSTEM);
}
