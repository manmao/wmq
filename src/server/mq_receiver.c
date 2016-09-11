#include "mq_receiver.h"
#include "msg_queue.h"
#include "message.h"


/**
 * 发送消息给对应的客户端
 * @param node [hash值指向地址]
 * @param msg  [消息]
 */
static void send_message_to_list(struct hash_node *node,message_t *msg){
	//delete fd
	struct list_entry *current;
	TGAP_LIST_TRAVERSE_SAFE_BEGIN( &(node->fd_list_head), current, field){
		write(current->fd,msg->msg_buff,strlen(msg->msg_buff));		
	}
	TGAP_LIST_TRAVERSE_SAFE_END
}

void  msg_queue_receiver(void *arg){
	mq_t *msgq=(mq_t *)arg;
	while(1){
		message_t *msg=(message_t *)pop_msg_head(msgq->msg_queue);
		//msg->topic查找到对应的消费者列表，遍历列表，依次发送数据;
		if(msg->topic != NULL){
			printf("topic :%s ,reciver message:%s\n",msg->topic,msg->msg_buff);
			struct hash_node *node=find_topic_fdlist(msgq->ht,msg->topic);
			if(node!=NULL){
				printf("send message\n");
				send_message_to_list(node,msg);
			}
			
		}
		
   	}
}