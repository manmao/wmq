#include <stdlib.h>
#include "msg_queue.h"



msg_queue_t* init_meesage_queue(struct rb_root *conn_root){
    msg_queue_t *mq_ptr;
    mq_ptr=(msg_queue_t *)malloc(sizeof(msg_queue_t)); 
    mq_ptr->list_queue=(linked_list_queue_t *)llqueue_new();

    pthread_mutex_init(&(mq_ptr->msg_queue_mutex),NULL);
    pthread_cond_init(&(mq_ptr->msg_queue_cond),NULL);
    //初始化互斥锁
    //mq_ptr->msg_queue_mutex = PTHREAD_MUTEX_INITIALIZER;
    //初始化条件变量
    //mq_ptr->msg_queue_cond = PTHREAD_COND_INITIALIZER;
    //mq_ptr->ht=create_fdtopic_hashtable();
    mq_ptr->ht=NULL;
    mq_ptr->conn_root=conn_root;

    return mq_ptr;
}



void push_msg_tail(msg_queue_t *msg_queue,void *item)
{
	if(msg_queue->list_queue == NULL){
        msg_queue->list_queue=(linked_list_queue_t *)llqueue_new();
    }

    pthread_mutex_lock(&(msg_queue->msg_queue_mutex)); //未获取锁则一直等待,阻塞
    
    llqueue_offer(msg_queue->list_queue,item); //添加消息
    
    pthread_cond_broadcast(&(msg_queue->msg_queue_cond));   //广播通知所有线程

    pthread_mutex_unlock(&(msg_queue->msg_queue_mutex));

}


void* pop_msg_head(msg_queue_t *msg_queue)
{
	void *item=NULL;
	if(msg_queue->list_queue == NULL){
		msg_queue->list_queue=(linked_list_queue_t *)llqueue_new();
    }

    pthread_mutex_lock(&(msg_queue->msg_queue_mutex));

    while(llqueue_count(msg_queue->list_queue)<=0){ //如果队列为空
		 pthread_cond_wait(&(msg_queue->msg_queue_cond),&(msg_queue->msg_queue_mutex));  //等待
	}

    item=llqueue_poll(msg_queue->list_queue);
    pthread_mutex_unlock(&(msg_queue->msg_queue_mutex));

    return item;
}


int count_queue(msg_queue_t *msg_queue){
     pthread_mutex_lock(&(msg_queue->msg_queue_mutex));
        int count=llqueue_count(msg_queue->list_queue);
     pthread_mutex_unlock(&(msg_queue->msg_queue_mutex));
     return count;
}



