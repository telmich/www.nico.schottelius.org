/***********************************************************************
 * 
 *    2006 Nico Schottelius (nico-linux-cinit at schottelius.org)
 * 
 *    part of cLinux/cinit
 * 
 *    List handling: Add a new item to a (non-)empty list
 */

#include <stdio.h>         /* NULL     */
#include "svc.h"

/*
 * list: pointer to the list
 * new:  pointer to data to insert (already filled up)
 */
int dep_entry_add(struct dep **deplist, struct dep *new)
{
   if( *deplist == NULL ) {       /* new list          */
      *deplist          = new;
      (*deplist)->prev  = *deplist;
      (*deplist)->next  = *deplist;
   } else {                                        /* already existing  */
      new->next               = *deplist;          /* new-> first       */
      new->prev               = (*deplist)->prev;  /* last <- new       */
      (*deplist)->prev->next  = new;               /* last -> new       */
      (*deplist)->prev        = new;               /* new <- first      */
   }

   return 1;
}
