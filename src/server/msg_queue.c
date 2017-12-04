#include <stdlib.h>
#include <unistd.h>
#include "msg_queue.h"

#define INT_MAX 2147483647

static void signalNotFull(msg_queue_t *msg_queue);

static void signalNotEmpty(msg_queue_t *msg_queue);

msg_queue_t* init_meesage_queue(struct rb_root *conn_root, 
                        HashTable *ht,
                        pthread_rwlock_t *ht_lock, 
                        pthread_rwlock_t *rb_root_lock){

    msg_queue_t *mq_ptr;
    mq_ptr=(msg_queue_t *)malloc(sizeof(msg_queue_t)); 
    mq_ptr->list_queue=(linked_list_queue_t *)llqueue_new();
    
    //初始化消息队列 读取锁 和 等待读取条件变量
    pthread_mutex_init(&(mq_ptr->msg_queue_take_mutex),NULL);
    pthread_cond_init(&(mq_ptr->not_empty_cond),NULL);

    //初始化 消息队列锁 写入锁 和 等待写入条件变量
    pthread_mutex_init(&(mq_ptr->msg_queue_put_mutex),NULL);
    pthread_cond_init(&(mq_ptr->not_full_cond),NULL);
    
    mq_ptr->ht=ht;
    mq_ptr->ht_lock=ht_lock;

    mq_ptr->conn_root=conn_root;
    mq_ptr->rb_root_lock=rb_root_lock;

    mq_ptr->capacity=INT_MAX; //初始化 容量

    mq_ptr->count=0;          //计数 初始化

    return mq_ptr;
}

void push_msg_tail(msg_queue_t *msg_queue,void *item)
{

	if(msg_queue->list_queue == NULL){ return ; }
    int c=-1;

    pthread_mutex_lock(&(msg_queue->msg_queue_put_mutex)); //未获取锁则一直等待,阻塞
    
    //等待 not_full条件通知
    while (msg_queue->count == msg_queue->capacity) {
        pthread_cond_wait(&(msg_queue->not_full_cond),&(msg_queue->msg_queue_put_mutex));  //等待
    }

    llqueue_offer(msg_queue->list_queue,item); //添加消息
    
    c=msg_queue->count;
    __sync_fetch_and_add(&msg_queue->count, 1);  //加一

    if( c+1 < msg_queue->capacity){  //有足够空间
         pthread_cond_signal(&(msg_queue->not_full_cond));  //通知写入线程
    }

    pthread_mutex_unlock(&(msg_queue->msg_queue_put_mutex));

    /**
     * 处理边界情况下，及时通知其他等待线程
     */
    if(c == 0){  
        signalNotEmpty(msg_queue);  //有数据，通知读取线程读取
    }

}


void* pop_msg_head(msg_queue_t *msg_queue)
{
	void *item=NULL;
	if(msg_queue->list_queue == NULL){ return ; }

    int c = -1;

    pthread_mutex_lock(&(msg_queue->msg_queue_take_mutex));

    while(msg_queue->count <= 0){ //如果队列为空
		pthread_cond_wait(&(msg_queue->not_empty_cond),&(msg_queue->msg_queue_take_mutex));  //等待
	}

    item=llqueue_poll(msg_queue->list_queue);

    c = msg_queue->count;
    __sync_fetch_and_sub(&msg_queue->count, 1); //减一

    if (msg_queue->count > 1){ //有数据，通知读取线程读取
        pthread_cond_signal(&(msg_queue->not_empty_cond));   //
    }

    pthread_mutex_unlock(&(msg_queue->msg_queue_take_mutex));

    /**
     * 处理边界情况下，及时通知其他线程
     */
    if(c == msg_queue->capacity ){   //有足够空间，通知写入线程写入
        signalNotFull(msg_queue);
    }

    return item;
}


int count_queue(msg_queue_t *msg_queue){
    return msg_queue->count;
}



/**
* Signals a waiting put. Called only from take/poll.
*/
static void signalNotFull(msg_queue_t *msg_queue){

    pthread_mutex_lock(&(msg_queue->msg_queue_put_mutex));
    
    pthread_cond_signal(&(msg_queue->not_full_cond)); 

    pthread_mutex_unlock(&(msg_queue->msg_queue_take_mutex));
}


/**
* Signals a waiting take. Called only from put/offer (which do not
* otherwise ordinarily lock takeLock.)
*/
static void signalNotEmpty(msg_queue_t *msg_queue){

    pthread_mutex_lock(&(msg_queue->msg_queue_take_mutex));

    pthread_cond_signal(&(msg_queue->not_empty_cond));

    pthread_mutex_unlock(&(msg_queue->msg_queue_take_mutex));

}



