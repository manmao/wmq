#ifndef __MSG_QUEUE_H_
#define __MSG_QUEUE_H_

#include <pthread.h>

#include "linked_list_queue.h"

extern linked_list_queue_t *msg_queue;


extern void push_msg_tail(linked_list_queue_t *msg_queue,void *item);

extern void *pop_msg_head(linked_list_queue_t *msg_queue);


#endif