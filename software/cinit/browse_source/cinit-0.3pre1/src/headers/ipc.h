/***********************************************************************
 *
 *    2006 Nico Schottelius (nico-linux-cinit //@\\ schottelius.org)
 *
 *    part of cLinux/cinit
 *
 *    Prototypes for ipc handler
 */

#ifndef CINIT_IPC_HEADER
#define CINIT_IPC_HEADER

/*****************************************************************************
 * Functions: in server
 */
int cinit_ipc_init(void);        /* init in init                     */
int cinit_ipc_listen(void);      /* listen in init                   */
int cinit_ipc_sclose(void);      /* fork of cinit come from outside! */
int cinit_ipc_ssend(void *data); /* send to a client from the server */
void cinit_ipc_destroy(void);    /* destroy ipc handler in cinit     */

/*****************************************************************************
 * Functions: in clients
 */
int cinit_ipc_logon(void);       /* logon to init  (client init)     */
int cinit_ipc_connect(void);     /* connect to init                  */
int cinit_ipc_csend(void *data); /* send to the server from a client */

/*****************************************************************************
 * Functions: shared
 */

#endif