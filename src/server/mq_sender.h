#ifndef _MQ_SENDER_H_
#define _MQ_SENDER_H_

#include "message.h"
#include "msg_queue.h"

static
void send_to_mq(msg_queue_t *msg_queue,message_t *msg){
	push_msg_tail(msg_queue,msg); //插入消息队列
}

void send_msg_mq(msg_queue_t *mq,message_t *msg){
	assert(mq != NULL);
	send_to_mq(mq,msg);
}

#endif