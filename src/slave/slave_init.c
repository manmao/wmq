#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <errno.h>
#include <stdlib.h>

#include "config.h"
#include "common_define.h"
#include "error_hdr.h"

#include "server_util.h"

#include "slave_init.h"

static
int on_slave_handle(struct request *req_pkg_p)
{

}

//初始化slave服务器
int slave_server_init(int argnum,char *argv[])
{
	struct server_handler *handler=(struct server_handler *)malloc(sizeof(struct server_handler));
	handler->handle_readable=NULL;
    handler->handle_accept=NULL;
    handler->handle_unknown=NULL;
    handler->handle_sig=NULL;

	struct server *slave_server=NULL;
	init_server(&slave_server,CONF.this_slave.ip,CONF.this_slave.port,handler);
	start_listen(slave_server,0,0);  //启动服务器

	return 0;
}