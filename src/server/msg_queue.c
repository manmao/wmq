#include "msg_queue.h"

linked_list_queue_t *msg_queue;

static pthread_mutex_t mutex= PTHREAD_MUTEX_INITIALIZER;



void push_msg_tail(linked_list_queue_t *msg_queue,void *item)
{
	if(msg_queue == NULL)
		msg_queue=(linked_list_queue_t *)llqueue_new();

    pthread_mutex_lock(&mutex);
    llqueue_offer(msg_queue,item);
    pthread_mutex_unlock(&mutex);
}



void *pop_msg_head(linked_list_queue_t *msg_queue)
{
	void *item=NULL;
	if(msg_queue == NULL)
		msg_queue=(linked_list_queue_t *)llqueue_new();
	
    pthread_mutex_lock(&mutex);
    item=llqueue_poll(msg_queue);
    pthread_mutex_unlock(&mutex);
}




