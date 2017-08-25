#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "mq_receiver.h"
#include "msg_queue.h"
#include "socket_pkg.h"
#include "lists.h"


/**
 * 发送消息给对应的客户端
 * @param node [hash值指向地址]
 * @param msg  [消息]
 */
static void send_message_to_list(struct hash_node *node,socket_pkg_t* pkg){
	//delete fd
	struct list_entry *current;
	TGAP_LIST_TRAVERSE_SAFE_BEGIN( &(node->fd_list_head), current, field){

		int len=write(current->fd,pkg->msg,pkg->data_len);	
		if(pkg->data_len==len)
			free(pkg); //清空内存
	} 

	TGAP_LIST_TRAVERSE_SAFE_END;
}


/**
 * 消息监听线程执行单元
 * @param  arg [description]
 * @return     [description]
 */
void  msg_queue_receiver(void *arg){
	msg_queue_t *msgq=(msg_queue_t*)arg;
	while(1){
		socket_pkg_t *pkg=(socket_pkg_t *)pop_msg_head(msgq);
		//pkg->topic查找到对应的消费者列表，遍历列表，依次发送数据;
		if(pkg->topic != NULL){
			printf("topic :%s ,reciver message:%s\n",pkg->topic,pkg->msg);
			struct hash_node *node=find_topic_fdlist(msgq->ht,pkg->topic);
			if(node!=NULL){
				printf("send message\n");
				send_message_to_list(node,pkg);
			}
		}
   	}
}