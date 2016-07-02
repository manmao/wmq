#include <assert.h>

#include "server_dispatch.h"
#include "msg_queue.h"
#include "topic_fd_map.h"

#include "connect.h"
#include "server.h"

static
void handle_cmd_pkg(socket_pkg_t *pkg){

}

static 
void handle_data_pkg(socket_pkg_t *pkg){
	message_t *msg=pkg->msg;
	push_msg_tail(msg_queue,msg);
}

static 
void handle_cmd_data_pkg(socket_pkg_t *pkg){
	switch(pkg->cmd){
		case MQ_OPEN:{
			
			break;
		}
		case MQ_CLOSE:{

			break;
		}
		case MQ_SUBTOPIC:{
			// 注册主题
			int fd=pkg->fd;			//推送消息的文件描述符
			char *topic=pkg->msg->topic; //主题 
			add_topic(ht,topic,fd);
			break;
		}		
		case MQ_PUBMSG:{
			message_t *msg=pkg->msg;
			push_msg_tail(msg_queue,msg); //插入消息队列
			break;
		}
		default:
			break;
	}
}


void handle_socket_pkg(socket_pkg_t *pkg)
{
	if(ht==NULL){
		ht=create_fdtopic_hashtable();
	}

	assert(ht!=NULL);
	assert(pkg);
	switch(pkg->type){
		case TYPE_CMD:
			handle_cmd_pkg(pkg);
			break;
		case TYPE_DATA:
			handle_data_pkg(pkg);
			break;
		case TYPE_CMD_DATA:
			handle_cmd_data_pkg(pkg);
			break;
		default:
			break;
	}
}

