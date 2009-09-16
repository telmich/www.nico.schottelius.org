/* 
 * (c) 2005 Nico Schottelius (nico-linux at schottelius.org)
 * panic: if cinit fails.
 * part of cinit
 */

#include "cinit.h"
#include <unistd.h>

void panic(void)
{
   _exit(1);
}
