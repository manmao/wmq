#include <assert.h>
#include "server_dispatch.h"


static
void handle_cmd_pkg(socket_pkg_t *pkg){

}

static 
void handle_data_pkg(socket_pkg_t *pkg){
	message_t *msg=pkg->msg;

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
		default:
			break;
	}
}

