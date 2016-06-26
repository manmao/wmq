#include <assert.h>
#include "server_dispatch.h"
#include "msg_queue.h"

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
		case MQ_OPEN:

			break;
		case MQ_CLOSE:

			break;
		case MQ_SUBTOPIC:	// 注册消息
			int fd=pkg->fd;
			struct conn_node conn;
			conn.conn_fd=fd;
			struct conn_type *type=conn_search(&(master_server->conn_root),&conn);
			struct conn_node* node=type->node;
			assert(node!=NULL);
			node->topic=pkg->msg->topic;

			break;
		case MQ_PUBMSG:
			message_t *msg=pkg->msg;
			push_msg_tail(msg_queue,msg); //插入消息队列
			break;
	}
}


void handle_socket_pkg(socket_pkg_t *pkg)
{
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

