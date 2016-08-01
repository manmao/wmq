#include "mq_receiver.h"
#include "msg_queue.h"

void  msg_queue_receiver(void *arg){

	message_t *msg=(message_t *)pop_msg_head(msg_queue);
	
	//msg->topic查找到对应的消费者列表，遍历列表，依次发送数据;
}