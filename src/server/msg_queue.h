#ifndef __MSG_QUEUE_H_
#define __MSG_QUEUE_H_

#include <pthread.h>

#include "linked_list_queue.h"

//全局消息队列
extern linked_list_queue_t *msg_queue;

extern pthread_mutex_t msg_queue_mutex;
extern pthread_cond_t  msg_queue_cond;	

/**
 * 发送消息到消息队列中
 * @param msg_queue [description]
 * @param item      [description]
 */
extern void push_msg_tail(linked_list_queue_t *msg_queue,void *item);


/**
 * 从消息队列中取数据
 * @param  msg_queue [description]
 * @return           [description]
 */
extern void *pop_msg_head(linked_list_queue_t *msg_queue);


#endif