#ifndef __MSG_QUEUE_H_
#define __MSG_QUEUE_H_

#include <pthread.h>

#include "linked_list_queue.h"
#include "topic_fd_map.h"

typedef struct msg_queue{
	//消息队列
	linked_list_queue_t *msg_queue;
	//全局hash表，topic到fd list的映射
	HashTable *ht;
}msg_queue_t;

extern pthread_mutex_t msg_queue_mutex;
extern pthread_cond_t  msg_queue_cond;

extern msg_queue_t* init_meesage_queue();

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