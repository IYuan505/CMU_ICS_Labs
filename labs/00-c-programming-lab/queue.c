/* 
 * Code for basic C skills diagnostic.
 * Developed for courses 15-213/18-213/15-513 by R. E. Bryant, 2017
 * Modified to store strings, 2018
 */

/*
 * This program implements a queue supporting both FIFO and LIFO
 * operations.
 *
 * It uses a singly-linked list to represent the set of queue elements
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
  Create empty queue.
  Return NULL if could not allocate space.
*/
queue_t *q_new()
{
    queue_t *q =  malloc(sizeof(queue_t));
    /* What if malloc returned NULL? */
    if (!q){
      return NULL;
    }
    
    q->head = NULL;
    q->tail = NULL;
    q->q_size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if(!q)
      return;
    /* How about freeing the list elements and the strings? */
    list_ele_t *cur = q->head;
    list_ele_t *next;
    while (cur){
      next = cur->next;
      free(cur->value);
      free(cur);
      cur = next;
    }
    /* Free queue structure */
    free(q);
}

/*
  Attempt to insert element at head of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh;
    /* What should you do if the q is NULL? */
    if (!q){
      return false;
    }

    newh = malloc(sizeof(list_ele_t));
    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    if (!newh){
      return false;
    }
    size_t len = strlen(s);
    newh->value = (char*) malloc(len+1);
    if (!newh->value){
      free(newh);
      return false;
    }
    memcpy(newh->value, s, len);
    newh->value[len]='\0';

    newh->next = q->head;
    q->head = newh;
    if (!q->tail)
      q->tail=q->head;
    q->q_size++;

    return true;
}


/*
  Attempt to insert element at tail of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    /* You need to write the complete code for this function */
    /* Remember: It should operate in O(1) time */
    list_ele_t *newt;
    /* What should you do if the q is NULL? */
    if (!q){
      return false;
    }

    newt = malloc(sizeof(list_ele_t));
    if (!newt){
      return false;
    }
    size_t len = strlen(s);
    newt->value = (char*) malloc(len+1);
    if (!newt->value){
      free(newt);
      return false;
    }
    memcpy(newt->value, s, len);
    newt->value[len]='\0';
    newt->next = NULL;

    q->tail->next = newt;
    q->tail = newt;
    if (!q->head)
      q->head = q->tail;
    q->q_size++;
    return true;
}

/*
  Attempt to remove element from head of queue.
  Return true if successful.
  Return false if queue is NULL or empty.
  If sp is non-NULL and an element is removed, copy the removed string to *sp
  (up to a maximum of bufsize-1 characters, plus a null terminator.)
  The space used by the list element and the string should be freed.
*/
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if(!q || q->q_size==0){
      return false;
    }
    list_ele_t *cur_head= q->head;
    // pointer s is not null
    if (sp){
      size_t len = strlen(cur_head->value);
      size_t min_len = bufsize-1<len? bufsize-1:len;
      memcpy(sp, cur_head->value, min_len);
      sp[min_len]='\0';
    }
    /* You need to fix up this code. */
    q->head = q->head->next;
    q->q_size--;
    free(cur_head->value);
    free(cur_head);
    return true;
}

/*
  Return number of elements in queue.
  Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q)
      return 0;
    /* You need to write the code for this function */
    /* Remember: It should operate in O(1) time */
    return q->q_size;
}

/*
  Reverse elements in queue
  No effect if q is NULL or empty
  This function should not allocate or free any list elements
  (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
  It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
   if (!q || q->q_size==0) return;
    /* You need to write the code for this function */
   list_ele_t *cur_head = q->head;
   list_ele_t *cur_tail = q->tail;
   list_ele_t *cur = q->head;
   list_ele_t *pre = NULL;
   list_ele_t *next = NULL;
   while (cur){
    next = cur->next;
    cur->next=pre;
    pre = cur;
    cur = next;
   }
   q->head = cur_tail;
   q->tail = cur_head;
}

