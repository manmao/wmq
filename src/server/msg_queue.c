#include "msg_queue.h"


//初始化互斥锁
pthread_mutex_t msg_queue_mutex= PTHREAD_MUTEX_INITIALIZER;
//初始化条件变量
pthread_cond_t  msg_queue_cond= PTHREAD_COND_INITIALIZER;

msg_queue_t* init_meesage_queue(){
    
    msg_queue_t *mq_ptr;
    mq_ptr=(msg_queue_t *)malloc(sizeof(msg_queue_t)); 
    mq_ptr->msg_queue=(linked_list_queue_t *)llqueue_new();

    //mq_ptr->ht=create_fdtopic_hashtable();
    mq_ptr->ht=NULL;
    
    return mq_ptr;
}


void push_msg_tail(linked_list_queue_t *msg_queue,void *item)
{
	if(msg_queue == NULL){
        msg_queue=(linked_list_queue_t *)llqueue_new();
    }

    pthread_mutex_lock(&msg_queue_mutex); //未获取锁则一直等待,阻塞
    
    llqueue_offer(msg_queue,item); //添加消息
    
    pthread_cond_broadcast(&msg_queue_cond);   //广播通知所有线程

    pthread_mutex_unlock(&msg_queue_mutex);

}

void* pop_msg_head(linked_list_queue_t *msg_queue)
{
	void *item=NULL;
	if(msg_queue == NULL){
		msg_queue=(linked_list_queue_t *)llqueue_new();
    }

    pthread_mutex_lock(&msg_queue_mutex);

    
    while(llqueue_count(msg_queue)<=0){ //如果队列为空
		 pthread_cond_wait(&msg_queue_cond,&msg_queue_mutex);  //等待
	}

    item=llqueue_poll(msg_queue);
    pthread_mutex_unlock(&msg_queue_mutex);

    return item;
}

int count_queue(linked_list_queue_t *msg_queue){
    return llqueue_count(msg_queue);
}



