#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "mq_receiver.h"
#include "msg_queue.h"
#include "socket_pkg.h"
#include "lists.h"
#include "connect.h"


static 
int validate_conn(int fd,msg_queue_t *msgq){
	struct conn_node conn;
	conn.conn_fd=fd;
	struct conn_type * type=conn_search(msgq->conn_root,&conn);
	return type!=NULL?1:0;
}


/**
 * 发送消息给对应的客户端
 *
 * @param node [hash值指向地址]
 * @param msg  [消息]
 */
static void send_message_to_list(msg_queue_t *msgq,struct hash_node *node,socket_pkg_t* pkg){
	//delete fd
	struct list_entry *current;
	TGAP_LIST_TRAVERSE_SAFE_BEGIN( &(node->fd_list_head), current, field){
		
		//如果客户端断开连接，则移除连接的文件描述符号
		if(validate_conn(current->fd,msgq) == 0){
			printf("find client socket fd :%d deleted\n\n",current->fd);
			TGAP_LIST_LOCK(&(node->fd_list_head));
			TGAP_LIST_REMOVE_CURRENT(field);
			TGAP_LIST_UNLOCK(&(node->fd_list_head));
		}else{
			printf("find client socket fd :%d sendmsg\n\n",current->fd);
			int len=write(current->fd,pkg->msg,pkg->data_len);
		}

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
				printf("send message......\n");
				send_message_to_list(msgq,node,pkg);
			}
		}
   	}
}