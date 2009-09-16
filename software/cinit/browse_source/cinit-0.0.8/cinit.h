/* 
 * cinit
 * (c) 2005 Nico Schottelius (nico-linux at schottelius.org)
 * header of cinit
 */

/* includes */
#include <sys/types.h>  /* pid_t */
#include "config.h"     /* paths, socket options, etc. */

/* paths */
#define C_ONARG   C_ON  C_PARAMS
#define C_OFFARG  C_OFF C_PARAMS
#define C_ONENV   C_ON  C_ENV
#define C_OFFENV  C_OFF C_ENV

#define SLASH     "/"

#define CINIT_INIT CINIT_DIR SLASH C_INIT
#define CINIT_TMNT CINIT_DIR SLASH C_TMP
#define CINIT_SOCK CINIT_TMNT SLASH C_SOCK

/* commands (maximum: 2^8 = 256)*/
#define CMD_START_SVC   1
#define CMD_CHG_STATUS  2
#define CMD_STOP_SVC    3
#define CMD_RESCUE      4
#define CMD_HALT        5
#define CMD_REBOOT      6
#define CMD_POWEROFF    7

/* status of a service - DO NOT USE 0!  */
#define ST_TMP       1        /* currently working on it */
#define ST_ONCE      2        /* executed once */
#define ST_RESPAWN   3        /* running and respawning */
#define ST_FAIL      4        /* failed to start service */
#define ST_TMPNOW    5        /* now you are on it - only for clients */
#define ST_OFF       6        /* Switching service off */

/* actions for i/o handlers (see comm/do_*) */
#define ACT_SERV     0
#define ACT_CLIENT   1

/* Messages to the outside */
#define MSG_ERR_OPEN      "open"
#define MSG_ERR_READ      "read"
#define MSG_ERR_IO        "i/o"
#define MSG_ERR_PIPE      "pipe"
#define MSG_ERR_FORK      "fork"
#define MSG_BIND          "bind"
#define MSG_LISTEN        "listen"
#define MSG_SOCKET        "socket"
#define MSG_CONNECT       "connect"
#define MSG_FCNTL         "fcntl"
#define MSG_ERR_EXECVE    "execve"
#define MSG_ERR_ALLOC     "memory allocation"
#define MSG_ERR_UMOUNT    "umount"
#define MSG_ERR_MOUNT     "mount"
#define MSG_TERMKILL      "sigterm"
#define MSG_KILLBILL      "sigkill"
#define MSG_INIT_MISS     "initial service missing"
#define MSG_NOT_DIR       "not a directory"
#define MSG_CHDIR         "chdir"
#define MSG_READLINK      "readlink"
#define MSG_ERR_ADD_SVC   "Adding service failed"
#define MSG_CMD_UNKNOWN   "Unknown command"
#define MSG_ERR_DEPS      "Too many dependencies"
#define MSG_ERR_ACCEPT    "accept"
#define MSG_ERR_WAITPID   "waitpid"

struct listitem {
   char     *abs_path;           /* name of service */
   int      status;              /* current status */
   pid_t    pid;                 /* pid of service / respawn watcher */
   struct   listitem *before;    /* previous item */
   struct   listitem *after;     /* next item */
};

/* variables */
extern struct  listitem *list;   /* the process linked chain */
extern int     sock;             /* current writing/reading socket */
extern int     logfd;            /* log file deskriptor, currently unused */
extern pid_t   cpid;             /* used by main() and reused be respaw_svc() */

/* functions (used by server and client) */
int   run_svc(char *rpath);
void  mini_printf(char *str, int fd);
void  set_signals(int action);
#define LOG(s) mini_printf(s,logfd)

int   do_svc_name(int sock2, char *svc, int action);
int   do_change_status(char *svc, char *status, pid_t *pid, int sock2, int action);
char  do_result(int sock2, char *value);

/* server functions */
int      list_insert(char *path, int status);
struct   listitem *list_search(char *path);
int      list_delete(char *path);
int      run_init_svc(char *cinit_svc);
void     sigio(int signal);
void     sig_reboot(int signal);
void     panic(void);
void     sig_child(int signal);

/* client / message functions */
int      msg_svc_on_off(char *svc, int action);
int      msg_change_status(char *svc, char status, pid_t pid);
int      begin_msg(char cmd);

/* client functions */
int      run_run_svcs(char *abspath);
pid_t    exec_svc(char *abspath, int on);
pid_t    respawn_svc(char *abspath);
int      connect_sock(int socke);
char     **read_file(char *file);
void     sig_terminate(int signal);

/* listing functions (server only) */
int      list_insert(char *path, int status);
struct   listitem *list_search(char *path);
int      list_delete(char *path);
int      list_modify(char *path, int new_status, pid_t new_pid);

/* util */
int      msg_reboot(char cmd);

/* DEBUG */
#ifdef DEBUG
# define D_PRINTF(x)  ( printf("[%s:%d]: %s\n",__FILE__,__LINE__,x) )
#else 
# define D_PRINTF(x)  if(0)
#endif
