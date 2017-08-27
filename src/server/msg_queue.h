#ifndef __MSG_QUEUE_H_
#define __MSG_QUEUE_H_

#include <pthread.h>

#include "linked_list_queue.h"
#include "topic_fd_map.h"
#include "connect.h"


typedef struct msg_queue{

	//消息队列
	linked_list_queue_t *list_queue;

	//互斥锁
    pthread_mutex_t msg_queue_mutex;
    
    //条件变量
    pthread_cond_t  msg_queue_cond;


	//全局hash表，topic到fd list的映射
	HashTable *ht;

	//
	pthread_mutex_t *ht_lock;           //互斥锁


	struct rb_root *conn_root;  //存放客户端和服务端的连接
	
	//
    pthread_mutex_t *rb_root_lock;      //互斥锁


	

	

}msg_queue_t;



extern msg_queue_t* init_meesage_queue(struct rb_root *conn_root, 
                        HashTable *ht,
                        pthread_mutex_t *ht_lock, 
                        pthread_mutex_t *rb_root_lock);

/**
 * 发送消息到消息队列中
 * @param msg_queue [description]
 * @param item      [description]
 */
extern void push_msg_tail(msg_queue_t *msg_queue,void *item);


/**
 * 从消息队列中取数据
 * @param  msg_queue [description]
 * @return           [description]
 */
extern void *pop_msg_head(msg_queue_t *msg_queue);


extern int count_queue(msg_queue_t *msg_queue);

#endif