#include <assert.h>
#include "server_dispatch.h"
#include "msg_queue.h"

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

