#include <assert.h>
#include "server_dispatch.h"

#include "topic_fd_map.h"
#include "mq_sender.h"

static
void handle_cmd_pkg(linked_list_queue_t *msg_queue,socket_pkg_t *pkg){
	send_msg_mq(msg_queue,&(pkg->msg));
}


static 
void handle_cmd_data_pkg(mq_t *mq,socket_pkg_t *pkg){
	switch(pkg->cmd){
		case MQ_OPEN:{
			break;
		}
		case MQ_CLOSE:{

			break;
		}
		case MQ_SUBTOPIC:{// 注册主题
			add_topic(mq->ht,pkg->msg.topic,pkg->fd);
			break;
		}		
		case MQ_PUBMSG:{//发送消息到消息队列
			send_msg_mq(mq->msg_queue,&(pkg->msg));
			break;
		}
		default:
			break;
	}
}

void handle_socket_pkg(server_t *master_server,socket_pkg_t *pkg)
{
	assert(mq->ht!=NULL);
	assert(pkg);
	switch(pkg->type){
		case TYPE_CMD:
			handle_cmd_pkg(master_server->mq[0]->msg_queue,pkg);
			break;
		case TYPE_CMD_DATA:
			handle_cmd_data_pkg(master_server->mq[0],pkg);
			break;
		default:
			break;
	}
}

