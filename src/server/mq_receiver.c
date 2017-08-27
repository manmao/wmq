#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "mq_receiver.h"
#include "msg_queue.h"
#include "socket_pkg.h"
#include "lists.h"
#include "connect.h"


/**
 *判断保存的和客户端的连接是否可用，如果客户端已经断开，则返回0，没断开则返回1
 * 
 */
static 
int validate_conn(int fd,msg_queue_t *msgq){
	
	struct conn_node conn;
	conn.conn_fd=fd;

	pthread_mutex_lock(msgq->rb_root_lock);              
	struct conn_type * type=conn_search(msgq->conn_root,&conn);
	pthread_mutex_unlock(msgq->rb_root_lock);

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
			printf("delete socket fd :%d \n\n",current->fd);
			TGAP_LIST_LOCK(&(node->fd_list_head)); //加锁
			TGAP_LIST_REMOVE_CURRENT(field);
			TGAP_LIST_UNLOCK(&(node->fd_list_head)); //解锁
		}else{
			printf("send msg to client socket fd :%d \n\n",current->fd);
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
			//find topic fd_list
			struct hash_node *node=find_topic_fdlist(msgq->ht,pkg->topic,msgq->ht_lock);
			if(node!=NULL){
				printf("send message......\n");
				send_message_to_list(msgq,node,pkg);
			}
		}
   	}
   	
}