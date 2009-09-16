/***********************************************************************
 *
 *    2007 Nico Schottelius (nico-cinit at schottelius.org)
 *
 *    part of cLinux/cinit
 *
 *    enables a service
 */


#include ""       /* include global header */

int cinit_enable_service(char *name, int how)
{
   int tmp;
   
   /* every client has to connect to cinit first */
   tmp = cinit_ipc_logon();
   if(!tmp) return 0;

   switch(how) {
      case 'p':   /* power off */
      break;

      default:
         break;
      }
   }

   return 1;
}
