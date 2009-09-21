/***********************************************************************
 *
 *    2006-2007 Nico Schottelius (nico-cinit at schottelius.org)
 * 
 *    part of cLinux/cinit
 *
 *    Service status (also exposed to the outside)
 */ 


#ifndef __CINIT_SVC_H
#define __CINIT_SVC_H

/***********************************************************************
 * The real status types a service may have (ignore above)
 * We have 32 Bits, we should use them ;-)
 */
enum cinit_svc_status {
   /* first define basics */
   CINIT_ST_SH_ONCE     = 0x1,     /* service SHould be started once            */
   CINIT_ST_SH_RESPAWN  = 0x2,     /* service SHould respawn                    */
   CINIT_ST_ONCE_OK     = 0x4,     /* service was successfully started once     */
   CINIT_ST_ONCE_FAIL   = 0x8,     /* service failed to start                   */
   CINIT_ST_RESPAWNING  = 0x10,    /* service is respawning                     */
   CINIT_ST_NEED_FAILD  = 0x20,    /* this service is not started, need failed  */
   CINIT_ST_IN_LIST     = 0x40,    /* this service is being started (= in list) */
   CINIT_ST_BAD_ERR     = 0x80,    /* some kind of error that SHOULD NOT happen */
   CINIT_ST_ONCE_RUN    = 0x100,   /* the once process is currently running     */
   CINIT_ST_NOT_EXIST   = 0x200    /* there's no such service in our database   */
};

/***********************************************************************
 * Possibilities the needs of a service may have
 */
enum cinit_svc_needs_status {
   CINIT_SNS_NEEDS_STARTED = 1,  /* all needs are started. We may start, too  */
   CINIT_SNS_NEEDS_FAILED,       /* one ore more needs failed                 */
   CINIT_SNS_NEEDS_UNFINISHED    /* one ore more needs are not yet started    */
};
#endif   /* _CINIT_SVC_H */